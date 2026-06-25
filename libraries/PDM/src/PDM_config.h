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

/* receiving thread configuration */
#define PDM_THREAD_STACK_SIZE 1024
#define PDM_THREAD_PRIORITY   7

/* TODO: to enable ARDUINO GIGA when supported by zephyr just remove this */
#ifdef ARDUINO_GIGA
#undef ARDUINO_GIGA
#endif

/* memory slab configuration */
#if defined(ARDUINO_NANO33BLE)
#define SLAB_BLOCK_NUM 4
#define SLAB_ALIGN     4
#if SAMPLE_BIT_WIDTH != 16
#error "Compilation runtime check: SAMPLE_BIT_WITDH must be set to 16 for ARDUINO_NANO33BLE"
#endif
#define SLAB_BLOCK_SIZE (PDM_NUMBER_OF_SAMPLES * 2)
#elif defined(ARDUINO_GIGA)
#define SLAB_BLOCK_NUM 4
#define SLAB_ALIGN     32
#if SAMPLE_BIT_WIDTH == 16
#define SLAB_BLOCK_SIZE (PDM_NUMBER_OF_SAMPLES * 2)
#elif SAMPLE_BIT_WIDTH == 24
#define SLAB_BLOCK_SIZE (PDM_NUMBER_OF_SAMPLES * 4)
#else
#error "Compilation runtime check: SAMPLE_BIT_WIDTH must be 16 or 24 for ARDUINO_GIGA"
#endif
#endif

#endif // ARDUINO_ZEPHYR_PDM_CONFIG_H
