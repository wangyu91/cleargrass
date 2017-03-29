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
#include "sensirion_common.h"
#include "sgpc1x_featureset.h"


static void do_div(s64 *n, u64 base) {
    *n = (*n) / base;
}

/**
 * sgp_adc_to_resistance() - Calculates the pixel resistance from the ADC value
 *                           (fixed point calculation). Original formula is:
 *                           1e6 * e^(25.5 * ((adc_out / 8191.0) - 0.5))
 * @adc_out:    Unsigned 16Bit ADC value
 *
 * Return:      Signed 64Bit fixed point value (40.24).
 */
s64 sgp_adc_to_resistance(u16 adc_out) {
    /* int(round(25.5 / log(2) * (1 << 24))) */
    static u32 EXPONENT_FACTOR = 617212361;
    static u32 ROOTS_OF_2[] = {23726566,
                               19951584,
                               18295683,
                               17520006,
                               17144589,
                               16959907,
                               16868314,
                               16822703,
                               16799944,
                               16788576,
                               16782895,
                               16780055,
                               16778635,
                               16777925,
                               16777570,
                               16777393};
    const u32 RESISTANCE_SCALING = 0xfff8;
    s64 exponent, result, dividend, inverse;
    u8 negative, i;

    /* do 64bit division -> do_div will do it in place */
    dividend = (u64) adc_out << 24;
    do_div(&dividend, RESISTANCE_SCALING);

    /* calculate the exponent: 25.5 / log(2) * (adc_out / 8191. - 0.5),
     * for exponentiation with base 2 */
    exponent = (EXPONENT_FACTOR * (dividend - (1 << (24 - 1)))) >> 24;

    negative = exponent < 0;
    /* make exponent positive to simplify calculation */
    if (negative)
        exponent = -exponent;
    /* initialize result to 1.0 */
    result = 1 << 24;
    /* multiply up the 16 fractional bits of the exponent */
    for (i = 0; i < 16; ++i) {
        if (exponent & (1 << (24 - 1 - i)))
            result = (result * ROOTS_OF_2[i]) >> 24;
    }
    /* use left shift for the integer part of the exponent */
    result <<= exponent >> 24;
    /* negative exponent -> 1.0 / result */
    if (negative) {
        inverse = (u64) 1 << 48;
        do_div(&inverse, result);
        result = inverse;
    }
    /* multiply by 1e6 and round result to integer value */
    return (result * 1000000 + (1 << (24 - 1)));
}


static const struct sgp_signal PIXEL1 = {
    .conversion_function = sgp_adc_to_resistance,
    .name = "pixel1",
};

static const struct sgp_signal PIXEL3 = {
    .conversion_function = sgp_adc_to_resistance,
    .name = "pixel3",
};

static const struct sgp_signal TVOC_PPB = {
    .conversion_function = NULL,
    .name = "tVOC",
};

static const struct sgp_signal CO2_EQ_PPM = {
    .conversion_function = NULL,
    .name = "co2_eq",
};

static const struct sgp_signal BASELINE_WORD1 = {
    .conversion_function = NULL,
    .name = "baseline1",
};

static const struct sgp_signal BASELINE_WORD2 = {
    .conversion_function = NULL,
    .name = "baseline2",
};

static const struct sgp_signal *SGP_PROFILE_IAQ_MEASURE_SIGNALS[] =
    { &TVOC_PPB, &CO2_EQ_PPM };

static const struct sgp_signal *SGP_PROFILE_IAQ_GET_BASELINE_SIGNALS[] =
    { &BASELINE_WORD1, &BASELINE_WORD2 };

static const struct sgp_signal *SGP_PROFILE_MEASURE_RAW_SIGNALS[] =
    { &PIXEL1, &PIXEL3 };


static const struct sgp_profile SGP_PROFILE_IAQ_INIT = {
    .number            = PROFILE_NUMBER_IAQ_INIT,
    .duration_us       = 1000,
    .signals           = NULL,
    .number_of_signals = 0,
    .command           = { {0x20, 0x03} },
    .name              = "iaq_init",
};

static const struct sgp_profile SGP_PROFILE_IAQ_MEASURE = {
    .number            = PROFILE_NUMBER_IAQ_MEASURE,
    .duration_us       = 50000,
    .signals           = SGP_PROFILE_IAQ_MEASURE_SIGNALS,
    .number_of_signals = ARRAY_SIZE(SGP_PROFILE_IAQ_MEASURE_SIGNALS),
    .command           = { {0x20, 0x08} },
    .name              = "iaq_measure",
};

static const struct sgp_profile SGP_PROFILE_IAQ_GET_BASELINE = {
    .number            = PROFILE_NUMBER_IAQ_GET_BASELINE,
    .duration_us       = 10000,
    .signals           = SGP_PROFILE_IAQ_GET_BASELINE_SIGNALS,
    .number_of_signals = ARRAY_SIZE(SGP_PROFILE_IAQ_GET_BASELINE_SIGNALS),
    .command           = { {0x20, 0x15} },
    .name              = "iaq_get_baseline",
};

static const struct sgp_profile SGP_PROFILE_IAQ_SET_BASELINE = {
    .number            = PROFILE_NUMBER_IAQ_SET_BASELINE,
    .duration_us       = 1000,
    .signals           = NULL,
    .number_of_signals = 0,
    .command           = { {0x20, 0x1e} },
    .name              = "iaq_set_baseline",
};


static const struct sgp_profile SGP_PROFILE_MEASURE_RAW = {
    .number            = PROFILE_NUMBER_MEASURE_RAW,
    .duration_us       = 200000,
    .signals           = SGP_PROFILE_MEASURE_RAW_SIGNALS,
    .number_of_signals = ARRAY_SIZE(SGP_PROFILE_MEASURE_RAW_SIGNALS),
    .command           = { {0x20, 0x50} },
    .name              = "pixel_resistances",
};


static const struct sgp_profile *sgp_profiles[] = {
    &SGP_PROFILE_IAQ_INIT,
    &SGP_PROFILE_IAQ_MEASURE,
    &SGP_PROFILE_IAQ_GET_BASELINE,
    &SGP_PROFILE_IAQ_SET_BASELINE,
    &SGP_PROFILE_MEASURE_RAW,
};


const struct sgp_otp_featureset sgp_featureset = {
    .profiles           = sgp_profiles,
    .number_of_profiles = ARRAY_SIZE(sgp_profiles)
};

