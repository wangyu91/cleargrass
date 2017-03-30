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
#ifndef SGP_SGPC1X_FEATURESET_H
#define SGP_SGPC1X_FEATURESET_H

#include "sensirion_configuration.h"

#define SGP_WORD_LEN                    2
#define SGP_COMMAND_LEN                 SGP_WORD_LEN

/* maximal size of signal and profile names */
#define NAME_SIZE 32

#define PROFILE_NUMBER_IAQ_INIT 0
#define PROFILE_NUMBER_IAQ_MEASURE 1
#define PROFILE_NUMBER_IAQ_GET_BASELINE 2
#define PROFILE_NUMBER_IAQ_SET_BASELINE 3
#define PROFILE_NUMBER_MEASURE_RAW 10

struct sgp_command {
    u8 value[SGP_COMMAND_LEN];
};

struct sgp_signal {
    s64(*conversion_function)(u16);
    char name[NAME_SIZE];
};

struct sgp_profile {
    /* expected duration of measurement, i.e., when to return for data */
    u32 duration_us;
    /* signals */
    const struct sgp_signal **signals;
    u16 number_of_signals;
    u8 number;
    const struct sgp_command command;
    char name[NAME_SIZE];
};

struct sgp_otp_featureset {
    const struct sgp_profile **profiles;
    u16 number_of_profiles;
};

extern const struct sgp_otp_featureset sgp_featureset;

#endif /* SGP_SGPC1X_FEATURESET_H */

