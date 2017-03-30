/*
 * Copyright (c) 2016, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "i2c.h"
#include "sensirion_configuration.h"
#include "sensirion_common.h"
#include "sgpc1x.h"
#include "sgpc1x_featureset.h"


#define SGP_RAM_WORDS                   512
#define SGP_BUFFER_SIZE                 ((SGP_RAM_WORDS + 2) * (SGP_WORD_LEN + CRC8_LEN))
#define SGP_SERIAL_ID_LEN               6
#define SGP_VERSIONS_LEN                (2 * SGP_WORD_LEN)
#define SGP_FEATURESET_VERSION_OFFSET   (0 * SGP_WORD_LEN)
#define SGP_READ_CHUNK_SIZE             240 /* 240b is divisible by 3 */
#define SGP_MAX_PROFILE_RET_LEN         1024
#define SGP_VALID_IAQ_BASELINE(b)       ((b) != 0)


#ifdef SGP_ADDRESS
static const uint8_t SGPC1X_ADDRESS = SGP_ADDRESS;
#else
static const uint8_t SGPC1X_ADDRESS = 0x58;
#endif

union sgp_u16 {
    u8 buf[2];
    u16 val; /* only for comparsion -> Could have wrong byte order! */
};

/* command and constants for reading the serial ID */
static const struct sgp_command sgp_cmd_get_serial_id = {
    {0x36, 0x82}
};

/* command and constants for reading the status */
static const struct sgp_command sgp_cmd_get_status = {
    {0xf3, 0x2d}
};

/* command and constants for reading the version data */
static const struct sgp_command sgp_cmd_get_versions = {
    {0x36, 0x24}
};

/* command and constants for on-chip self-test */
#define SGP_CMD_MEASURE_TEST_DURATION_US 220000
#define SGP_CMD_MEASURE_TEST_LEN SGP_WORD_LEN
static const struct sgp_command sgp_cmd_measure_test = {
    {0x20, 0x32}
};

static const struct sgp_otp_featureset sgp_features_unknown = {
    .profiles = NULL,
    .number_of_profiles = 0,
};

enum sgp_state_code {
    WAIT_STATE,
    MEASURING_PROFILE_STATE
};

struct sgp_info {
    u64 serial_id;
    u16 feature_set_version;
    u16 status;
};

struct sgp_command_desc {
    u16 result_len;
    u8 command[SGP_COMMAND_LEN];
};

struct sgp_data {
    enum sgp_state_code current_state;
    struct sgp_info info;
    struct sgp_command_desc current_command;
    const struct sgp_otp_featureset *otp_features;
    u8 buf[SGP_BUFFER_SIZE];
} client_data;


/**
 * sgp_i2c_read() - reads data from SGP sensor
 * @data:       Allocated buffer to store the read data
 * @data_len:   Data length (without CRC)
 *
 * Return:      0 on success.
 */
static s16 sgp_i2c_read(u8 *data, u16 data_len) {

    s16 ret;
    s16 i, y;
    u16 size;
    u8 buf[SGP_MAX_PROFILE_RET_LEN];

    size = data_len / 2 * 3;

    ret = sensirion_i2c_read(SGPC1X_ADDRESS, buf, size);

    if (ret != 0)
        return STATUS_FAIL;

    /* check the CRC for each word */
    for (i = 0, y = 0; i < size;
         i += SGP_WORD_LEN + CRC8_LEN, y += SGP_WORD_LEN) {
        if (sensirion_common_check_crc(&buf[i], SGP_WORD_LEN,
                                       buf[i + SGP_WORD_LEN]) == STATUS_FAIL) {
            return STATUS_FAIL;
        }
        *((u16 *)&data[y]) = *((u16 *)&buf[i]);
    }

    return STATUS_OK;
}


/**
 * sgp_i2c_write() - writes to the SGP sensor
 * @command:     Command
 *
 * Return:      STATUS_OK on success.
 */
static s16 sgp_i2c_write(const struct sgp_command *command) {
    s8 ret;

    ret = sensirion_i2c_write(SGPC1X_ADDRESS, command->value, SGP_COMMAND_LEN);
    if (ret != 0)
        return STATUS_FAIL;

    return STATUS_OK;
}


/**
 * unpack_signals() - unpack signals which are stored in client_data.buf
 * @profile:    The profile
 */
static void unpack_signals(const struct sgp_profile *profile) {
    u16 i, j;
    const struct sgp_signal *signal;
    u16 data_len = profile->number_of_signals * SGP_WORD_LEN;
    u8 buf[128];
    u16 value;

    /* copy buffer */
    for(i = 0; i < data_len; i++)
        buf[i] = client_data.buf[i];

    for(i = 0, j = 0; i < profile->number_of_signals; i += 1) {
        signal = profile->signals[i];
        value = be16_to_cpu(*((u16 *)&buf[i * SGP_WORD_LEN]));

        if(signal->conversion_function != NULL) {
            *((s64*)&client_data.buf[j]) = signal->conversion_function(value);
            j += 4 * SGP_WORD_LEN;
        } else {
            *((u16*)&client_data.buf[j]) = value;
            j += SGP_WORD_LEN;
        }
    }
}

/**
 * read_measurement() - reads the result of a profile measurement
 *
 * Return:  Length of the written data to the buffer. Negative if it fails.
 */
static s16 read_measurement(const struct sgp_profile *profile) {

    s16 ret;

    switch (client_data.current_state) {

        case MEASURING_PROFILE_STATE:
            ret = sgp_i2c_read(client_data.buf,
                               profile->number_of_signals * SGP_WORD_LEN);

            if (ret)
                /* Measurement in progress */
                return STATUS_FAIL;

            unpack_signals(profile);
            client_data.current_state = WAIT_STATE;

            return STATUS_OK;

        default:
            /* No command issued */
            return STATUS_FAIL;
    }
}

/**
 * sgp_i2c_read_from_cmd() - reads data from SGP sensor after a command has
 *                           been issued
 * @cmd:        Command
 * @data:       Allocated buffer to store the read data
 * @data_len:   Data length (without CRC)
 *
 * Return:      STATUS_OK on success, else STATUS_FAIL
 */
static s16 sgp_i2c_read_from_cmd(const struct sgp_command *cmd,
                                 u32 duration_us,
                                 u8 *data,
                                 const u16 data_len) {

    if (sgp_i2c_write(cmd) == STATUS_FAIL)
        return STATUS_FAIL;

    /* the chip needs some time to write the data into the RAM */
    sensirion_sleep_usec(duration_us);
    return sgp_i2c_read(data, data_len);
}


/**
 * sgp_run_profile() - run a profile and read write its return to client_data
 * @profile     A pointer to the profile
 *
 * Return:      STATUS_OK on success, else STATUS_FAIL
 */
static s16 sgp_run_profile(const struct sgp_profile *profile) {
    const struct sgp_command cmd = profile->command;
    u32 duration_us = profile->duration_us + 5;

    if (sgp_i2c_write(&cmd) == STATUS_FAIL)
        return STATUS_FAIL;

    sensirion_sleep_usec(duration_us);

    if (profile->number_of_signals > 0) {
        client_data.current_state = MEASURING_PROFILE_STATE;
        return read_measurement(profile);
    }

    return STATUS_OK;
}


/**
 * sgp_get_profile_by_number() - get a profile by its identifier number
 * @number      The number that identifies the profile
 *
 * Return:      A pointer to the profile or NULL if the profile does not exists
 */
static const struct sgp_profile *sgp_get_profile_by_number(u16 number) {
    u8 i;
    const struct sgp_profile *profile = NULL;

    for (i = 0; i < client_data.otp_features->number_of_profiles; i++) {
        profile = client_data.otp_features->profiles[i];
        if (number == profile->number)
            break;
    }

    if (i == client_data.otp_features->number_of_profiles) {
        return NULL;
    }

    return profile;
}


/**
 * sgp_run_profile_by_number() - run a profile by its identifier number
 * @number:     The number that identifies the profile
 *
 * Return:      STATUS_OK on success, else STATUS_FAIL
 */
static s16 sgp_run_profile_by_number(u16 number) {
    const struct sgp_profile *profile;

    profile = sgp_get_profile_by_number(number);
    if (profile == NULL)
        return STATUS_FAIL;

    if (sgp_run_profile(profile) == STATUS_FAIL)
        return STATUS_FAIL;

    return STATUS_OK;
}


/**
 * sgp_measure_test() - Run the on-chip self-test
 *
 * This method is executed synchronously and blocks for the duration of the
 * measurement (slightly over 200ms)
 *
 * Return: 0xd400 on success. Error code or STATUS_FAIL otherwise.
 */
u16 sgp_measure_test() {
    u8 measure_test_buf[SGP_CMD_MEASURE_TEST_LEN];

    if (sgp_i2c_write(&sgp_cmd_measure_test) != STATUS_OK)
        return STATUS_FAIL;

    sensirion_sleep_usec(SGP_CMD_MEASURE_TEST_DURATION_US);

    if (sgp_i2c_read(measure_test_buf, SGP_CMD_MEASURE_TEST_LEN) != STATUS_OK)
        return STATUS_FAIL;

    return be16_to_cpu(*(u16 *)measure_test_buf);
}


/**
 * sgp_measure_iaq() - Measure IAQ values async
 *
 * The profile is executed asynchronously. Use sgp_read_iaq to get the
 * values.
 *
 * Return:  STATUS_OK on success, else STATUS_FAIL
 */
s16 sgp_measure_iaq() {
    const struct sgp_profile *profile;

    profile = sgp_get_profile_by_number(PROFILE_NUMBER_IAQ_MEASURE);
    if (profile == NULL) {
        return STATUS_FAIL;
    }

    if (sgp_i2c_write(&(profile->command)) == STATUS_FAIL)
        return STATUS_FAIL;
    client_data.current_state = MEASURING_PROFILE_STATE;

    return STATUS_OK;
}


/**
 * sgp_read_iaq() - Read IAQ values async
 *
 * Read the IAQ values. This command can only be exectued after a measurement
 * has started with sgp_measure_iaq and is finished.
 *
 * @tvoc_ppb:   The tVOC ppb value will be written to this location
 * @tvoc_ppm:   The CO2-Equivalent ppm value will be written to this location
 *
 * Return:      STATUS_OK on success, else STATUS_FAIL
 */
s16 sgp_read_iaq(u16 *tvoc_ppb, u16 *co2_eq_ppm) {
    const struct sgp_profile *profile;

    profile = sgp_get_profile_by_number(PROFILE_NUMBER_IAQ_MEASURE);
    if (profile == NULL)
        return STATUS_FAIL;

    if (read_measurement(profile) == STATUS_FAIL)
        return STATUS_FAIL;

    *tvoc_ppb   = *((u16 *)(client_data.buf + SGP_WORD_LEN));
    *co2_eq_ppm = *((u16 *)client_data.buf);

    return STATUS_OK;
}


/**
 * sgp_measure_tvoc_blocking_read() - Measure tVOC concentration
 *
 * The profile is executed synchronously.
 *
 * Return:  tVOC concentration in ppb. Negative if it fails.
 */
s16 sgp_measure_iaq_blocking_read(u16 *tvoc_ppb, u16 *co2_eq_ppm) {
    if (sgp_run_profile_by_number(PROFILE_NUMBER_IAQ_MEASURE) == STATUS_FAIL)
        return STATUS_FAIL;

    *tvoc_ppb   = *((u16 *)(client_data.buf + SGP_WORD_LEN));
    *co2_eq_ppm = *((u16 *)client_data.buf);

    return STATUS_OK;
}


/**
 * sgp_measure_tvoc() - Measure tVOC concentration async
 *
 * The profile is executed asynchronously. Use sgp_read_tvoc to get the
 * ppb value.
 *
 * Return:  STATUS_OK on success, else STATUS_FAIL
 */
s16 sgp_measure_tvoc() {
    return sgp_measure_iaq();
}


/**
 * sgp_read_tvoc() - Read tVOC concentration async
 *
 * Read the tVOC value. This command can only be exectued after a measurement
 * has started with sgp_measure_tvoc and is finished.
 *
 * @tvoc_ppb:   The tVOC ppb value will be written to this location
 *
 * Return:      STATUS_OK on success, else STATUS_FAIL
 */
s16 sgp_read_tvoc(u16 *tvoc_ppb) {
    u16 co2_eq_ppm;
    return sgp_read_iaq(tvoc_ppb, &co2_eq_ppm);
}


/**
 * sgp_measure_tvoc_blocking_read() - Measure tVOC concentration
 *
 * The profile is executed synchronously.
 *
 * Return:  tVOC concentration in ppb. Negative if it fails.
 */
s16 sgp_measure_tvoc_blocking_read(u16 *tvoc_ppb) {
    u16 co2_eq_ppm;
    return sgp_measure_iaq_blocking_read(tvoc_ppb, &co2_eq_ppm);
}


/**
 * sgp_measure_co2_eq() - Measure tVOC concentration async
 *
 * The profile is executed asynchronously. Use sgp_read_co2_eq to get the
 * ppm value.
 *
 * Return:  STATUS_OK on success, else STATUS_FAIL
 */
s16 sgp_measure_co2_eq() {
    return sgp_measure_iaq();
}


/**
 * sgp_read_co2_eq() - Read CO2-Equivalent concentration async
 *
 * Read the CO2-Equivalent value. This command can only be exectued after a
 * measurement was started with sgp_measure_co2_eq() and is finished.
 *
 * @co2_eq_ppm: The CO2-Equivalent ppm value will be written to this location
 *
 * Return:      STATUS_OK on success, else STATUS_FAIL
 */
s16 sgp_read_co2_eq(u16 *co2_eq_ppm) {
    u16 tvoc_ppb;
    return sgp_read_iaq(&tvoc_ppb, co2_eq_ppm);
}


/**
 * sgp_measure_co2_eq_blocking_read() - Measure CO2-Equivalent concentration
 *
 * The profile is executed synchronously.
 *
 * Return:  CO2-Equivalent concentration in ppm. Negative if it fails.
 */
s16 sgp_measure_co2_eq_blocking_read(u16 *co2_eq_ppm) {
    u16 tvoc_ppb;
    return sgp_measure_iaq_blocking_read(&tvoc_ppb, co2_eq_ppm);
}


/**
 * sgp_measure_pixel_resistance_blocking_read() - Measure pixel resistance
 * The profile is executed synchronously.
 * @pixel_resistance1: Output variable for pixel1 resistance value
 * @pixel_resistance3: Output variable for pixel3 resistance value
 *
 * Return:      STATUS_OK on success, else STATUS_FAIL
 */
s16 sgp_measure_pixel_resistance_blocking_read(s64 *pixel_resistance1, s64 *pixel_resistance3) {
    if (sgp_run_profile_by_number(PROFILE_NUMBER_MEASURE_RAW) == STATUS_FAIL)
        return STATUS_FAIL;

    *pixel_resistance1 = *((s64*)(client_data.buf + 4 * SGP_WORD_LEN));
    *pixel_resistance3 = *((s64*)client_data.buf);

    return STATUS_OK;
}

/**
 * sgp_measure_pixel_resistance() - Measure raw pixel value async
 *
 * The profile is executed asynchronously. Use sgp_read_pixel_resistance to get the
 * pixel resistance values
 *
 * Return:  STATUS_OK on success, else STATUS_FAIL
 */
s16 sgp_measure_pixel_resistance(void) {
    const struct sgp_profile *profile;

    profile = sgp_get_profile_by_number(PROFILE_NUMBER_MEASURE_RAW);
    if (profile == NULL) {
        return STATUS_FAIL;
    }

    if (sgp_i2c_write(&(profile->command)) == STATUS_FAIL)
        return STATUS_FAIL;
    client_data.current_state = MEASURING_PROFILE_STATE;

    return STATUS_OK;
}


/**
 * sgp_read_pixel_resistance() - Read pixel resistance async
 * This command can only be exectued after a measurement started with
 * sgp_measure_pixel_resistance and has finished.
 *
 * @pixel_resistance1: Output variable for pixel1 resistance value
 * @pixel_resistance3: Output variable for pixel3 resistance value
 *
 * Return:      STATUS_OK on success, else STATUS_FAIL
 */

s16 sgp_read_pixel_resistance(s64 *pixel_resistance1, s64 *pixel_resistance3) {
    const struct sgp_profile *profile;

    profile = sgp_get_profile_by_number(PROFILE_NUMBER_MEASURE_RAW);
    if (profile == NULL)
        return STATUS_FAIL;

    if (read_measurement(profile) == STATUS_FAIL)
        return STATUS_FAIL;

    *pixel_resistance1 = *((s64*)(client_data.buf + 4 * SGP_WORD_LEN));
    *pixel_resistance3 = *((s64*)client_data.buf);

    return STATUS_OK;
}


/**
 * sgp_get_iaq_baseline() - read out the baseline from the chip
 *
 * The IAQ baseline should be retrieved and persisted for a faster sensor
 * startup. See sgp_set_iaq_baseline() for further documentation.
 *
 * A valid baseline value is only returned approx. 60min after a call to
 * sgp_iaq_init() that is not followed by a call to sgp_set_iaq_baseline() with
 * a valid baseline.
 * This functions returns STATUS_FAIL if the baseline value is not valid.
 *
 * @baseline:   Pointer to raw u32 where to store the baseline
 *              If the method returns STATUS_FAIL, the baseline value must be
 *              discarded and must not be passed to sgp_set_iaq_baseline().
 *
 * Return:      STATUS_OK on success, else STATUS_FAIL
 */
s16 sgp_get_iaq_baseline(u32 *baseline) {
    s16 ret = sgp_run_profile_by_number(PROFILE_NUMBER_IAQ_GET_BASELINE);
    if (ret == STATUS_FAIL)
        return STATUS_FAIL;

    *baseline = *((u32*)client_data.buf);

    if (!SGP_VALID_IAQ_BASELINE(*baseline))
        return STATUS_FAIL;

    return STATUS_OK;
}


/**
 * sgp_set_iaq_baseline() - set the on-chip baseline
 * @baseline:   A raw u32 baseline
 *              This value must be unmodified from what was retrieved by a
 *              successful call to sgp_get_iaq_baseline() with return value
 *              STATUS_OK. A persisted baseline should not be set if it is
 *              older than one week.
 *
 * Return:      STATUS_OK on success, else STATUS_FAIL
 */
s16 sgp_set_iaq_baseline(u32 baseline) {
    if (!SGP_VALID_IAQ_BASELINE(baseline))
        return STATUS_FAIL;

    u16 buf_size = SGP_COMMAND_LEN + 2 * (SGP_WORD_LEN + CRC8_LEN);
    u8 buf[32];
    u8 buf_idx = 0;
    const struct sgp_profile *profile;
    profile = sgp_get_profile_by_number(PROFILE_NUMBER_IAQ_SET_BASELINE);

    /* assemble buffer */
    *((u16 *)&buf[buf_idx]) = *((u16 *)profile->command.value);
    buf_idx += SGP_COMMAND_LEN;

    *((u16 *)&buf[buf_idx]) = be16_to_cpu((baseline >> 16) & 0xFFFF);
    buf_idx += SGP_WORD_LEN;
    buf[buf_idx] = sensirion_common_generate_crc(&buf[buf_idx - SGP_WORD_LEN],
                                             SGP_WORD_LEN);
    buf_idx += CRC8_LEN;

    *((u16 *)&buf[buf_idx]) = be16_to_cpu(baseline & 0xFFFF);
    buf_idx += SGP_WORD_LEN;
    buf[buf_idx] = sensirion_common_generate_crc(&buf[buf_idx - SGP_WORD_LEN],
                                             SGP_WORD_LEN);

    if (sensirion_i2c_write(SGPC1X_ADDRESS, buf, buf_size) != 0)
        return STATUS_FAIL;

    return STATUS_OK;
}


/**
 * sgp_get_configured_address() - returns the configured I2C address
 *
 * Return:      u8 I2C address
 */
u8 sgp_get_configured_address() {
    return SGPC1X_ADDRESS;
}


/**
 * sgp_get_serial_id() - get the SGP serial id
 * @serial_id:  pointer to u64 where the serial id will be written to
 */
void sgp_get_serial_id(u64 *serial_id) {
    *serial_id = client_data.info.serial_id;
}


/**
 * sgp_update_status() - reads the chip status and updates it in the client data
 * @return      STATUS_OK on success
 */
static s16 sgp_update_status() {
    s16 ret;
    union sgp_u16 network_status;
    u16 status_length = sizeof(network_status);
    ret = sgp_i2c_read_from_cmd(&sgp_cmd_get_status, 100000,
                                network_status.buf, status_length);

    if (ret == STATUS_FAIL)
        return STATUS_FAIL;

    client_data.info.status = network_status.val;
    return STATUS_OK;
}


/**
 * sgp_detect_featureset_version() - extracts the featureset and initializes
 * client_data.
 * @version_data    STATUS_OK on success
 */
s16 sgp_detect_featureset_version(u8 *version_data) {
    int ret = STATUS_OK;
    u16 feature_set_version = be16_to_cpu(*((u16 *)version_data));

    client_data.info.feature_set_version = feature_set_version;
    switch (feature_set_version) {
        case 7: /* feature set 7 */
        case 8: /* feature set 8 */
            client_data.otp_features = &sgp_featureset;
            break;
        default:
            ret = STATUS_FAIL;
            client_data.otp_features = &sgp_features_unknown;
            break;
    }
    return ret;
}

/**
 * sgp_iaq_init() - reset the SGPC1x's internal IAQ baselines
 *
 * Return:  STATUS_OK on success.
 */
s16 sgp_iaq_init() {
    return sgp_run_profile_by_number(PROFILE_NUMBER_IAQ_INIT);
}

/**
 * sgp_probe() - check if SGPC1x is available and initialize it
 *
 * This call aleady initializes the IAQ baselines (sgp_iaq_init())
 *
 * Return:  STATUS_OK on success.
 */
s16 sgp_probe() {
    s16 err;

    client_data.current_state = WAIT_STATE;

    /* Initialize I2C */
    sensirion_i2c_init();

    /* try to read the serial ID */
    err = sgp_i2c_read_from_cmd(&sgp_cmd_get_serial_id, 10000,
                                client_data.buf, SGP_SERIAL_ID_LEN);
    printf("\r\n Succeed = %d !\r\n", err);
                    
    if (err == STATUS_FAIL)
        return err;
    client_data.info.serial_id = be64_to_cpu(*((u64*)client_data.buf)) >> 16;

    /* read the versions */
    err = sgp_i2c_read_from_cmd(&sgp_cmd_get_versions, 10000,
                                client_data.buf, SGP_VERSIONS_LEN);
    if (err == STATUS_FAIL)
        return STATUS_FAIL;

    err = sgp_detect_featureset_version(client_data.buf);
    if (err == STATUS_FAIL)
        return STATUS_FAIL;

    /* read the status */
    err = sgp_update_status();
    if (err == STATUS_FAIL)
        return STATUS_FAIL;

    return sgp_iaq_init();
}

