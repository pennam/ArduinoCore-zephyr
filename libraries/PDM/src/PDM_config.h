/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ARDUINO_ZEPHYR_PDM_CONFIG_H
#define ARDUINO_ZEPHYR_PDM_CONFIG_H

/* The number of samples the user receive
 * For performance reason the user is strongly suggested to use this
 * dimension for its application buffer that take the audio samples */
#define PDM_NUMBER_OF_SAMPLES 512

/* size in bit of an audio sample */
/* NANO 33 BLE will work only if this value is 16
 * GIGA can work up with 24 */
#define SAMPLE_BIT_WIDTH 16

#endif
