/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/counter.h>
#include <string.h>

#if defined(CONFIG_BOARD_ARDUINO_UNO_Q)

static const uint8_t pins[][2] = {
    { 0, 1 }, // 0
    { 1, 0 },
    { 0, 2 },
    { 2, 0 },
    { 1, 2 },
    { 2, 1 },
    { 0, 3 },
    { 3, 0 },
    { 1, 3 },
    { 3, 1 },
    { 2, 3 }, // 10
    { 3, 2 },
    { 0, 4 },
    { 4, 0 },
    { 1, 4 },
    { 4, 1 },
    { 2, 4 },
    { 4, 2 },
    { 3, 4 },
    { 4, 3 },
    { 0, 5 }, // 20
    { 5, 0 },
    { 1, 5 },
    { 5, 1 },
    { 2, 5 },
    { 5, 2 },
    { 3, 5 },
    { 5, 3 },
    { 4, 5 },
    { 5, 4 },
    { 0, 6 }, // 30
    { 6, 0 },
    { 1, 6 },
    { 6, 1 },
    { 2, 6 },
    { 6, 2 },
    { 3, 6 },
    { 6, 3 },
    { 4, 6 },
    { 6, 4 },
    { 5, 6 }, // 40
    { 6, 5 },
    { 0, 7 },
    { 7, 0 },
    { 1, 7 },
    { 7, 1 },
    { 2, 7 },
    { 7, 2 },
    { 3, 7 },
    { 7, 3 },
    { 4, 7 }, // 50
    { 7, 4 },
    { 5, 7 },
    { 7, 5 },
    { 6, 7 },
    { 7, 6 },
    { 0, 8 },
    { 8, 0 },
    { 1, 8 },
    { 8, 1 },
    { 2, 8 }, // 60
    { 8, 2 },
    { 3, 8 },
    { 8, 3 },
    { 4, 8 },
    { 8, 4 },
    { 5, 8 },
    { 8, 5 },
    { 6, 8 },
    { 8, 6 },
    { 7, 8 }, // 70
    { 8, 7 },
    { 0, 9 },
    { 9, 0 },
    { 1, 9 },
    { 9, 1 },
    { 2, 9 },
    { 9, 2 },
    { 3, 9 },
    { 9, 3 },
    { 4, 9 }, // 80
    { 9, 4 },
    { 5, 9 },
    { 9, 5 },
    { 6, 9 },
    { 9, 6 },
    { 7, 9 },
    { 9, 7 },
    { 8, 9 },
    { 9, 8 },
    { 0, 10 }, // 90
    { 10, 0 },
    { 1, 10 },
    { 10, 1 },
    { 2, 10 },
    { 10, 2 },
    { 3, 10 },
    { 10, 3 },
    { 4, 10 },
    { 10, 4 },
    { 5, 10 }, // 100
    { 10, 5 },
    { 6, 10 },
    { 10, 6 },
  };

static void turnLed(int idx, bool on) {
    GPIOF->MODER &= 0xF0000000U;
    if (on) {
        uint8_t pin0 = pins[idx][0];
        uint8_t pin1 = pins[idx][1];
        
        GPIOF->BSRR |= (1U << pin0) | (1U << (pin1 + 16));
        GPIOF->MODER |= (1U << (pin0 << 1)) | (1U << (pin1 << 1));
    }
}

#endif

#if defined(CONFIG_BOARD_ARDUINO_VENTUNO_Q)

static const uint8_t pins[][2] = {
    { 2, 3 }, // 0
    { 3, 2 },
    { 2, 4 },
    { 4, 2 },
    { 3, 4 },
    { 4, 3 },
    { 2, 5 },
    { 5, 2 },
    { 3, 5 },
    { 5, 3 },
    { 4, 5 }, // 10
    { 5, 4 },
    { 2, 6 },
    { 6, 2 },
    { 3, 6 },
    { 6, 3 },
    { 4, 6 },
    { 6, 4 },
    { 5, 6 },
    { 6, 5 },
    { 2, 7 }, // 20
    { 7, 2 },
    { 3, 7 },
    { 7, 3 },
    { 4, 7 },
    { 7, 4 },
    { 5, 7 },
    { 7, 5 },
    { 6, 7 },
    { 7, 6 },
    { 2, 9 }, // 30
    { 9, 2 },
    { 3, 9 },
    { 9, 3 },
    { 4, 9 },
    { 9, 4 },
    { 5, 9 },
    { 9, 5 },
    { 6, 9 },
    { 9, 6 },
    { 7, 9 }, // 40
    { 9, 7 },
    { 2, 10 },
    { 10, 2 },
    { 3, 10 },
    { 10, 3 },
    { 4, 10 },
    { 10, 4 },
    { 5, 10 },
    { 10, 5 },
    { 6, 10 }, // 50
    { 10, 6 },
    { 7, 10 },
    { 10, 7 },
    { 9, 10 },
    { 10, 9 },
    { 2, 13 },
    { 13, 2 },
    { 3, 13 },
    { 13, 3 },
    { 4, 13 }, // 60
    { 13, 4 },
    { 5, 13 },
    { 13, 5 },
    { 6, 13 },
    { 13, 6 },
    { 7, 13 },
    { 13, 7 },
    { 9, 13 },
    { 13, 9 },
    { 10, 13 }, // 70
    { 13, 10 },
    { 2, 14 },
    { 14, 2 },
    { 3, 14 },
    { 14, 3 },
    { 4, 14 },
    { 14, 4 },
    { 5, 14 },
    { 14, 5 },
    { 6, 14 }, // 80
    { 14, 6 },
    { 7, 14 },
    { 14, 7 },
    { 9, 14 },
    { 14, 9 },
    { 10, 14 },
    { 14, 10 },
    { 13, 14 },
    { 14, 13 },
    { 2, 15 }, // 90
    { 15, 2 },
    { 3, 15 },
    { 15, 3 },
    { 4, 15 },
    { 15, 4 },
    { 5, 15 },
    { 15, 5 },
    { 6, 15 },
    { 15, 6 },
    { 7, 15 }, // 100
    { 15, 7 },
    { 9, 15 },
    { 15, 9 },
  };

static void turnLed(int idx, bool on) {
    GPIOH->MODER &= 0x03C3000FU;               // don't touch pins 0, 1, 8, 11, 12
    if (on) {
        uint8_t pin0 = pins[idx][0];
        uint8_t pin1 = pins[idx][1];

        GPIOH->BSRR |= (1U << pin0) | (1U << (pin1 + 16));
        GPIOH->MODER |= (1U << (pin0 << 1)) | (1U << (pin1 << 1));
    }
}

#endif

#if defined(CONFIG_BOARD_ARDUINO_UNO_Q) || defined(CONFIG_BOARD_ARDUINO_VENTUNO_Q)

#define NUM_MATRIX_LEDS 104
static uint8_t __attribute__((aligned)) framebuffer[NUM_MATRIX_LEDS / 8];
static uint8_t __attribute__((aligned)) framebuffer_color[NUM_MATRIX_LEDS];

static bool color = false;
static uint8_t _max_grayscale_bits = 3;

static void timer_irq_handler_fn(const struct device *counter_dev, void *user_data) {
    static volatile int i_isr = 0;
    if (color) {
        static volatile int counter = 0;
        switch ((framebuffer_color[i_isr] * 8 / (1 << _max_grayscale_bits))) {
            case 0:
                turnLed(i_isr, false);
                break;
            case 1:
                turnLed(i_isr, counter % 23 == 0);
                break;
            case 2:
                turnLed(i_isr, counter % 15 == 0);
                break;
            case 3:
                turnLed(i_isr, counter % 5 == 0);
                break;
            case 4:
                turnLed(i_isr, counter % 3 == 0);
                break;
            case 5:
            case 6:
            case 7:
                turnLed(i_isr, true);
                break;
        }
        counter++;
    } else {
        turnLed(i_isr, ((framebuffer[i_isr >> 3] & (1 << (i_isr % 8))) != 0));
    }
    i_isr = (i_isr + 1) % NUM_MATRIX_LEDS;
}

void matrixWrite(const uint32_t* buf) {
    memcpy(framebuffer, buf, NUM_MATRIX_LEDS/8);
    color = false;
}

void matrixGrayscaleWrite(const uint8_t* buf) {
    memcpy(framebuffer_color, buf, NUM_MATRIX_LEDS);
    color = true;
}

void matrixSetGrayscaleBits(uint8_t _max) {
    _max_grayscale_bits = _max;
}

#define TIMER DT_NODELABEL(counter_matrix)

void matrixBegin() {
	const struct device *const counter_dev = DEVICE_DT_GET(TIMER);
    counter_start(counter_dev);

    struct counter_top_cfg top_cfg;
	top_cfg.ticks = counter_us_to_ticks(counter_dev, 10);
	top_cfg.callback = timer_irq_handler_fn;
	top_cfg.user_data = &top_cfg;
    top_cfg.flags = 0;

	int err = counter_set_top_value(counter_dev, &top_cfg);
    if (err) {
        printk("Failed to set counter_set_top_value");
    }
}

void matrixEnd() {
    const struct device *const counter_dev = DEVICE_DT_GET(TIMER);
    counter_stop(counter_dev);
}


void matrixPlay(const uint8_t* buf, uint32_t len) {
    int i = 0;
    while (i < (len / 104)) {
		matrixGrayscaleWrite(&buf[i*104]);
		i++;
		k_msleep(16);
    }
}

#endif /* CONFIG_BOARD_ARDUINO_UNO_Q || CONFIG_BOARD_ARDUINO_VENTUNO_Q */

#if defined(CONFIG_BOARD_ARDUINO_UNO_R4) && DT_NODE_HAS_STATUS(DT_NODELABEL(agt1), okay)

/*
 * UNO R4 WiFi: 12x8 charlieplexed matrix on 11 lines of ports 0 and 2,
 * lit one LED per 100 us slot.
 *
 * The timer is AGT1, programmed here directly: the Zephyr counter driver
 * does not fit in the loader partition.
 *
 * Brightness: each LED gets an on-time inside its slot. Only one ICU slot is
 * free on this board, so instead of a compare interrupt the AGT is run as a
 * chain of variable-length phases. Writing the AGT register while it counts
 * sets both the running counter and the reload value (measured on the
 * board), so at each underflow the handler starts the next phase (light an
 * LED or turn everything off) and sets its length, minus the ticks already
 * counted since the underflow. A dimmed LED takes an "on" and an "off" phase,
 * an LED fully on or off one slot-long phase, so binary frames cost one
 * interrupt per slot.
 */

#include <soc.h>

#define NUM_MATRIX_LEDS 96

/* matrix lines 0..10: P003 P004 P011 P012 P013 P015 P204 P205 P206 P212 P213 */
static const uint8_t line_port[11] = {0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2};
static const uint8_t line_bit[11] = {3, 4, 11, 12, 13, 15, 4, 5, 6, 12, 13};

#define LED_MATRIX_PORT0_MASK ((1 << 3) | (1 << 4) | (1 << 11) | (1 << 12) | (1 << 13) | (1 << 15))
#define LED_MATRIX_PORT2_MASK ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 12) | (1 << 13))

/* {anode, cathode} line of each LED, row-major (as in the Renesas core) */
static const uint8_t pins[][2] = {
	{7, 3},  {3, 7},  {7, 4},  {4, 7},  {3, 4},  {4, 3},  {7, 8},  {8, 7},  {3, 8},  {8, 3},
	{4, 8},  {8, 4},  {7, 0},  {0, 7},  {3, 0},  {0, 3},  {4, 0},  {0, 4},  {8, 0},  {0, 8},
	{7, 6},  {6, 7},  {3, 6},  {6, 3},  {4, 6},  {6, 4},  {8, 6},  {6, 8},  {0, 6},  {6, 0},
	{7, 5},  {5, 7},  {3, 5},  {5, 3},  {4, 5},  {5, 4},  {8, 5},  {5, 8},  {0, 5},  {5, 0},
	{6, 5},  {5, 6},  {7, 1},  {1, 7},  {3, 1},  {1, 3},  {4, 1},  {1, 4},  {8, 1},  {1, 8},
	{0, 1},  {1, 0},  {6, 1},  {1, 6},  {5, 1},  {1, 5},  {7, 2},  {2, 7},  {3, 2},  {2, 3},
	{4, 2},  {2, 4},  {8, 2},  {2, 8},  {0, 2},  {2, 0},  {6, 2},  {2, 6},  {5, 2},  {2, 5},
	{1, 2},  {2, 1},  {7, 10}, {10, 7}, {3, 10}, {10, 3}, {4, 10}, {10, 4}, {8, 10}, {10, 8},
	{0, 10}, {10, 0}, {6, 10}, {10, 6}, {5, 10}, {10, 5}, {1, 10}, {10, 1}, {2, 10}, {10, 2},
	{7, 9},  {9, 7},  {3, 9},  {9, 3},  {4, 9},  {9, 4},
};

#define MATRIX_AGT_NODE DT_NODELABEL(agt1)
#define MATRIX_AGT      ((R_AGTX0_Type *)DT_REG_ADDR(MATRIX_AGT_NODE))
#define MATRIX_AGT_IRQ  DT_IRQ_BY_NAME(MATRIX_AGT_NODE, agti, irq)
#define MATRIX_AGT_PRIO DT_IRQ_BY_NAME(MATRIX_AGT_NODE, agti, priority)

#define ON_FULL 0xFFFF
/* on-time of each LED in timer ticks: 0 off, ON_FULL the whole slot */
static volatile uint16_t on_ticks[NUM_MATRIX_LEDS];
static uint8_t _max_grayscale_bits = 3;
static uint32_t slot_ticks, min_on_ticks, max_on_ticks;
static bool running;

static inline R_PORT0_Type *line_reg(uint8_t line) {
	return line_port[line] ? R_PORT2 : R_PORT0;
}

static inline void all_off(void) {
	R_PORT0->PCNTR1 &= ~((uint32_t)LED_MATRIX_PORT0_MASK);
	R_PORT2->PCNTR1 &= ~((uint32_t)LED_MATRIX_PORT2_MASK);
}

static inline void led_on(int idx) {
	uint8_t a = pins[idx][0], c = pins[idx][1];
	R_PORT0_Type *pa = line_reg(a), *pc = line_reg(c);

	all_off();
	pa->PCNTR3 = BIT(line_bit[a]);      /* POSR: anode high */
	pc->PCNTR3 = BIT(line_bit[c] + 16); /* PORR: cathode low */
	pa->PCNTR1 |= BIT(line_bit[a]);     /* PDR: outputs */
	pc->PCNTR1 |= BIT(line_bit[c]);
}

static int cur_led;          /* LED of the current slot */
static bool cur_dim_on;      /* in the "on" phase of a dimmed LED: "off" follows */
static uint16_t last_reload; /* the counter restarted from this at the underflow */

static void matrix_agt_isr(const void *arg) {
	ARG_UNUSED(arg);

	/* clear the underflow flag (keep counting) and the ICU request; read the
	 * request back so the write is done before returning, or the NVIC sees
	 * it still pending and runs this handler a second time (see FSP's
	 * R_BSP_IrqStatusClear())
	 */
	MATRIX_AGT->AGT16.CTRL.AGTCR = BIT(0);
	R_ICU->IELSR_b[MATRIX_AGT_IRQ].IR = 0;
	(void)R_ICU->IELSR[MATRIX_AGT_IRQ];

	uint16_t now = MATRIX_AGT->AGT16.AGT;
	uint32_t elapsed = last_reload >= now ? last_reload - now : 0;
	uint32_t len;

	if (cur_dim_on) {
		/* the "off" phase of a dimmed LED: rest of its slot */
		all_off();
		len = slot_ticks - on_ticks[cur_led];
		cur_dim_on = false;
	} else {
		cur_led = (cur_led + 1) % NUM_MATRIX_LEDS;
		uint16_t t = on_ticks[cur_led];

		if (t == 0) {
			all_off();
			len = slot_ticks;
		} else {
			led_on(cur_led);
			if (t == ON_FULL) {
				len = slot_ticks;
			} else {
				len = t;
				cur_dim_on = true;
			}
		}
	}

	uint32_t reload = (len - 1 > elapsed) ? len - 1 - elapsed : 1;
	MATRIX_AGT->AGT16.AGT = reload;
	last_reload = reload;
}


void matrixWrite(const uint32_t *buf) {
	for (int i = 0; i < NUM_MATRIX_LEDS; i++) {
		on_ticks[i] = (buf[i / 32] & BIT(i % 32)) ? ON_FULL : 0;
	}
}

void matrixGrayscaleWrite(const uint8_t *buf) {
	uint32_t max = BIT(_max_grayscale_bits) - 1;

	for (int i = 0; i < NUM_MATRIX_LEDS; i++) {
		uint32_t level = MIN(buf[i], max);
		uint16_t t;

		if (level == 0) {
			t = 0;
		} else if (level == max) {
			t = ON_FULL;
		} else {
			/* quadratic curve between the shortest on-time that outlasts
			 * the interrupt and the longest that leaves room for an off
			 * phase, as the Renesas core library does
			 */
			uint32_t v = level * 255 / max;
			t = min_on_ticks + (max_on_ticks - min_on_ticks) * v * v / (255 * 255);
		}
		on_ticks[i] = t;
	}
}

void matrixSetGrayscaleBits(uint8_t _max) {
	_max_grayscale_bits = CLAMP(_max, 1, 8);
}

void matrixBegin() {
	if (running) {
		return;
	}

	/* the matrix lines as plain GPIO inputs: off */
	R_BSP_PinAccessEnable();
	for (int l = 0; l < 11; l++) {
		R_PFS->PORT[line_port[l]].PIN[line_bit[l]].PmnPFS = 0;
	}
	R_BSP_PinAccessDisable();
	all_off();

	/* AGT counts PCLKB: ICLK * 2^ICK / 2^PCKB */
	uint32_t pclkb = (sys_clock_hw_cycles_per_sec() << R_SYSTEM->SCKDIVCR_b.ICK) >>
			 R_SYSTEM->SCKDIVCR_b.PCKB;
	slot_ticks = pclkb / 10000;
	min_on_ticks = slot_ticks / 12;
	max_on_ticks = slot_ticks - slot_ticks / 16;

	cur_led = NUM_MATRIX_LEDS - 1;
	cur_dim_on = false;
	last_reload = slot_ticks - 1;

	static bool irq_connected;
	if (!irq_connected) {
		R_ICU->IELSR[MATRIX_AGT_IRQ] = ELC_EVENT_AGT1_INT;
		IRQ_CONNECT(MATRIX_AGT_IRQ, MATRIX_AGT_PRIO, matrix_agt_isr, NULL, 0);
		irq_connected = true;
	}

	R_BSP_MODULE_START(FSP_IP_AGT, 1);
	MATRIX_AGT->AGT16.CTRL.AGTCR = BIT(2); /* TSTOP: forced stop */
	MATRIX_AGT->AGT16.CTRL.AGTMR1 = 0;     /* timer mode, PCLKB */
	MATRIX_AGT->AGT16.CTRL.AGTMR2 = 0;
	MATRIX_AGT->AGT16.CTRL.AGTIOC = 0;
	MATRIX_AGT->AGT16.CTRL.AGTISR = 0;
	MATRIX_AGT->AGT16.CTRL.AGTCMSR = 0;
	MATRIX_AGT->AGT16.AGT = slot_ticks - 1;
	R_ICU->IELSR_b[MATRIX_AGT_IRQ].IR = 0;
	irq_enable(MATRIX_AGT_IRQ);
	MATRIX_AGT->AGT16.CTRL.AGTCR = BIT(0); /* TSTART */
	running = true;
}

void matrixEnd() {
	irq_disable(MATRIX_AGT_IRQ);
	MATRIX_AGT->AGT16.CTRL.AGTCR = BIT(2); /* TSTOP */
	running = false;
	all_off();
}

void matrixPlay(const uint8_t *buf, uint32_t len) {
	for (uint32_t i = 0; i < len / NUM_MATRIX_LEDS; i++) {
		matrixGrayscaleWrite(&buf[i * NUM_MATRIX_LEDS]);
		k_msleep(16);
	}
}

#endif /* CONFIG_BOARD_ARDUINO_UNO_R4 && agt1 */
