/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

// Copyright 2022 Ethan Hang

// library to use altera i/o functions
#include "sys/alt_stdio.h"

// library to use altera variable types
#include "alt_types.h"

// library to get all hardware definitions
#include "system.h"

// library that has the definition of pio functions
#include "altera_avalon_pio_regs.h"

// C standard libraries
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <io.h>

#define LEDS_BASE 0x10001040

int main() {

  	// PWM variables
	float pwm_freq = 10e3f;  // switching in Hz
	float dim_period = 2.0f;  // 2 seconds

	// multiplying by ALT_CPU_FREQ (50,000,000 Hz) value in cycles (one complete wave)
	float pwm_period = 1/pwm_freq * ALT_CPU_FREQ;  // inverse of pwm_freq is pwm period in seconds
	alt_u64 dim_period_in_cycles = dim_period * ALT_CPU_FREQ;  // dim period in cycles

	// start counter
	IOWR(PERFORMANCE_COUNTER_0_BASE,1,0); // writing 0 to register 1

  	while (1) {
		// reading upper 32 bits
		alt_u64 cycle_counter = (alt_u64)IORD(PERFORMANCE_COUNTER_0_BASE,1) << (alt_u64) 32;
		// reading lower 32 bits
		cycle_counter = cycle_counter | (alt_u64)IORD(PERFORMANCE_COUNTER_0_BASE,0);
		
		// time through dim period
		float progress_through_dim_period = (float)(cycle_counter % (alt_u64)dim_period_in_cycles) / (float)dim_period_in_cycles;
		// time through pwm period
		float progress_through_pwm_period = (float)(cycle_counter % (alt_u64)pwm_period) / pwm_period;
		// duty cycle, led brightness 0% to 100%
		float brightness = progress_through_dim_period < 0.5f ? progress_through_dim_period / 0.5f : 1.f - progress_through_dim_period;
		
		// measures pwm period progress against duty cycle, true 0x3FFFFFF = ON, false 0 = OFF
		alt_u64 led_on_or_off = progress_through_pwm_period < brightness? 0x3ffffff : 0;
		// light LEDs, writing led_on_or_off value to LEDS_BASE address
		IOWR(LEDS_BASE,0,led_on_or_off);
    	}
  return 0;
}
