#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/counter.h>
#include <string.h>

static const uint8_t pins[][2] = {
    { 7, 3 }, // 0
    { 3, 7 },
    { 7, 4 },
    { 4, 7 },
    { 3, 4 },
    { 4, 3 },
    { 7, 8 },
    { 8, 7 },
    { 3, 8 },
    { 8, 3 },
    { 4, 8 }, // 10
    { 8, 4 },
    { 7, 0 },
    { 0, 7 },
    { 3, 0 },
    { 0, 3 },
    { 4, 0 },
    { 0, 4 },
    { 8, 0 },
    { 0, 8 },
    { 7, 6 }, // 20
    { 6, 7 },
    { 3, 6 },
    { 6, 3 },
    { 4, 6 },
    { 6, 4 },
    { 8, 6 },
    { 6, 8 },
    { 0, 6 },
    { 6, 0 },
    { 7, 5 }, // 30
    { 5, 7 },
    { 3, 5 },
    { 5, 3 },
    { 4, 5 },
    { 5, 4 },
    { 8, 5 },
    { 5, 8 },
    { 0, 5 },
    { 5, 0 },
    { 6, 5 }, // 40
    { 5, 6 },
    { 7, 1 },
    { 1, 7 },
    { 3, 1 },
    { 1, 3 },
    { 4, 1 },
    { 1, 4 },
    { 8, 1 },
    { 1, 8 },
    { 0, 1 }, // 50
    { 1, 0 },
    { 6, 1 },
    { 1, 6 },
    { 5, 1 },
    { 1, 5 },
    { 7, 2 },
    { 2, 7 },
    { 3, 2 },
    { 2, 3 },
    { 4, 2 }, // 60
    { 2, 4 },
    { 8, 2 },
    { 2, 8 },
    { 0, 2 },
    { 2, 0 },
    { 6, 2 },
    { 2, 6 },
    { 5, 2 },
    { 2, 5 },
    { 1, 2 }, // 70
    { 2, 1 },
    { 7, 10 },
    { 10, 7 },
    { 3, 10 },
    { 10, 3 },
    { 4, 10 },
    { 10, 4 },
    { 8, 10 },
    { 10, 8 },
    { 0, 10 }, // 80
    { 10, 0 },
    { 6, 10 },
    { 10, 6 },
    { 5, 10 },
    { 10, 5 },
    { 1, 10 },
    { 10, 1 },
    { 2, 10 },
    { 10, 2 },
    { 7, 9 }, // 90
    { 9, 7 },
    { 3, 9 },
    { 9, 3 },
    { 4, 9 },
    { 9, 4 },
    { 8, 9 },
    { 9, 8 },
    { 0, 9 },
    { 9, 0 },
    { 6, 9 }, // 100
    { 9, 6 },
    { 5, 9 },
    { 9, 5 },
  };


const int idxToPin(int idx) {
    switch (idx) {
        case 0: return 2;
        case 1: return 9;
        case 2: return 10;
        case 3: return 5;
        case 4: return 6;
        case 5: return 4;
        case 6: return 3;
        case 7: return 7;
        case 8: return 8;
        case 9: return 0;
        case 10: return 1;
    }
}

#define NUM_MATRIX_LEDS 104
static uint8_t __attribute__((aligned)) framebuffer[NUM_MATRIX_LEDS / 8];

static void turnLed(int idx, bool on) {
    GPIOF->MODER = 0;

    if (on) {
        GPIOF->MODER |= (1 << (idxToPin(pins[idx][0]) * 2) | 1 << (idxToPin(pins[idx][1]) * 2));
        GPIOF->BSRR |= (1 << (idxToPin(pins[idx][0])) | 1 << (idxToPin(pins[idx][1]) + 16));
    }
}

static uint32_t reverse(uint32_t x)
{
    x = ((x >> 1) & 0x55555555u) | ((x & 0x55555555u) << 1);
    x = ((x >> 2) & 0x33333333u) | ((x & 0x33333333u) << 2);
    x = ((x >> 4) & 0x0f0f0f0fu) | ((x & 0x0f0f0f0fu) << 4);
    x = ((x >> 8) & 0x00ff00ffu) | ((x & 0x00ff00ffu) << 8);
    x = ((x >> 16) & 0xffffu) | ((x & 0xffffu) << 16);
    return x;
}

static void timer_irq_handler_fn(const struct device *counter_dev, void *user_data)
{
    static volatile int i_isr = 0;
    turnLed(i_isr, ((framebuffer[i_isr >> 3] & (1 << (i_isr % 8))) != 0));
    i_isr = (i_isr + 1) % NUM_MATRIX_LEDS;
}

void matrixWrite(uint32_t* buf) {
    memcpy(framebuffer, (uint32_t*)buf, NUM_MATRIX_LEDS/8);
}

#define TIMER DT_INST(0, st_stm32_counter)

void matrixBegin() {
	const struct device *const counter_dev = DEVICE_DT_GET(TIMER);
    counter_start(counter_dev);

    struct counter_top_cfg top_cfg;
	top_cfg.ticks = counter_us_to_ticks(counter_dev, 50);
	top_cfg.callback = timer_irq_handler_fn;
	top_cfg.user_data = &top_cfg;
    top_cfg.flags = 0;

	int err = counter_set_top_value(counter_dev, &top_cfg);
    if (err) {
        printk("Failed to set counter_set_top_value");
    }

    //uint32_t buf[4] = {0x38E22, 0x8A09375D, 0x824A288E, 0x38000000};
    uint32_t buf[4] = {
        0b00000000000000011100011100010001,
        0b01000101000001001001101110101110,
        0b11000001001001010001010001000111,
        0b00011100000000000000000000000000};
    for (int i = 0; i < 4 ; i++) {
        buf[i] = reverse(buf[i]);
    }
    matrixWrite(buf);
}