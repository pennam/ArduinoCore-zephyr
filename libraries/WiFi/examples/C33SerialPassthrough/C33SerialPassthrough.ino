/*
  C33SerialPassthrough - Bridge the USB CDC serial to the on-board ESP32-C3 of
  the Arduino Portenta C33 (Arduino Zephyr core) so its Wi-Fi/BLE firmware can be
  updated with esptool.

  Unlike the Renesas core, the Zephyr core does not expose SerialNina, NINA_GPIO0
  or NINA_RESETN, nor Serial.rts()/dtr()/baud(). This sketch therefore talks to
  the Zephyr peripherals directly:

    - ESP32-C3 UART0  -> sci8 / uart8            (shared with the BT HCI link)
    - ESP32-C3 EN     -> ioport8 pin 4 (P804)    (reset, active low)
    - ESP32-C3 IO0    -> ioport8 pin 3 (P803)    (download/boot select)
    - USB CDC control -> board_cdc_acm_uart      (DTR/RTS/baud via UART line ctrl)

  The USB RTS line drives EN to reset the ESP32-C3 for esptool. IO0 (P803) is
  held low the whole time: it is the esp_hosted Wi-Fi driver's data-ready line
  and driving it high makes that driver busy-spin and starve USB. The ESP32-C3
  therefore stays download-armed; power-cycle (or flash your normal sketch) after
  updating to run the new image. The USB baud rate is mirrored onto uart8.

  This variant opens uart8 directly (no loader handshake), relying on the
  uart_renesas_ra_sci ERI-ISR fix that keeps uart_configure(uart8) from hanging.

  Copyright (c) Arduino s.r.l. and/or its affiliated companies

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Arduino.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyrSerial.h>

/* ESP32-C3 UART0, shared with the BT HCI link on the Portenta C33. */
static const struct device *const esp_uart = DEVICE_DT_GET(DT_NODELABEL(uart8));

/* USB CDC ACM device backing 'Serial'; used to read the host DTR/RTS/baud. */
static const struct device *const cdc_uart = DEVICE_DT_GET(DT_NODELABEL(board_cdc_acm_uart));

/* ESP32-C3 control pins live on ioport8: EN (reset) = P804, IO0 (boot) = P803. */
static const struct device *const esp_gpio = DEVICE_DT_GET(DT_NODELABEL(ioport8));
#define ESP_RESET_PIN 4
#define ESP_GPIO0_PIN 3

arduino::ZephyrSerial SerialNina(esp_uart);

unsigned long baud = 115200;
int rts = -1;

uint8_t auc_buffer[256];

static int usb_line_ctrl(uint32_t ctrl, int fallback) {
  uint32_t value = 0;
  if (uart_line_ctrl_get(cdc_uart, ctrl, &value) != 0) {
    return fallback;
  }
  return (int)value;
}

void setup() {
  Serial.begin(baud, SERIAL_8N1);
  SerialNina.begin(baud);

  /* IO0 (P803) stays low: it is esp_hosted's data-ready line and driving it high
   * makes that driver busy-spin and starve USB. Release EN so the ESP32-C3 boots
   * download-armed. */
  gpio_pin_configure(esp_gpio, ESP_GPIO0_PIN, GPIO_OUTPUT_LOW);
  gpio_pin_configure(esp_gpio, ESP_RESET_PIN, GPIO_OUTPUT_HIGH);
  gpio_pin_set(esp_gpio, ESP_RESET_PIN, 0);
  gpio_pin_set(esp_gpio, ESP_RESET_PIN, 1);

  rts = usb_line_ctrl(UART_LINE_CTRL_RTS, rts);
}

void loop() {
  int _rts = usb_line_ctrl(UART_LINE_CTRL_RTS, rts);

  if (rts != _rts) {
    /* RTS -> EN (active low). IO0 stays low (esp_hosted data-ready). */
    gpio_pin_set(esp_gpio, ESP_RESET_PIN, _rts ? 0 : 1);
    rts = _rts;
  }

  int len = 0;
  while (Serial.available() && len < (int)sizeof(auc_buffer)) {
    auc_buffer[len++] = Serial.read();
  }
  if (len) {
    SerialNina.write(auc_buffer, len);
  }

  /* ESP -> USB. Cap to the CDC TX space so the loop never blocks when the host
   * is not reading, and call the base write to bypass SerialUSB_::write()'s DTR
   * gate (esptool deasserts DTR - its IO0 line - during sync, which would
   * otherwise drop the ESP's replies). */
  int space = Serial.availableForWrite();
  len = 0;
  while (SerialNina.available() && len < space && len < (int)sizeof(auc_buffer)) {
    auc_buffer[len++] = SerialNina.read();
  }
  if (len) {
    Serial.arduino::ZephyrSerial::write(auc_buffer, len);
  }
}
