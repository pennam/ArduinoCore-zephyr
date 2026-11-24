/*
 * Copyright (c) 2026 Seeed Technology Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// SPI and I2C (Wire) pins; indices match digital-pin-gpios
#define MOSI    D10 /* P1.15 */
#define MISO    D9  /* P1.14 */
#define SCK     D8  /* P1.13 */
#define SS      D3  /* P0.29, no dedicated CS on the XIAO connector */
#define SDA     D4  /* P0.04 */
#define SCL     D5  /* P0.05 */

/* Pin naming matches the Seeed nRF52 core so that existing libraries and
 * wiki examples recognize the board. The board identity macro
 * ARDUINO_SEEED_XIAO_NRF52840 is injected by the build system from
 * boards.txt (build.board), so it is not redefined here. LED_BUILTIN is
 * derived from the overlay (builtin-led-gpios), not defined here. */

/* RGB LED indexes, same values as the Seeed nRF52 core (D12 is blue and
 * D13 is green there -- not physical R,G,B order). */
#define LED_RED   11
#define LED_GREEN 13
#define LED_BLUE  12
