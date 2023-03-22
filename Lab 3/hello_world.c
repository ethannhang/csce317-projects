//Copyright 2022 Ethan Hang

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

#define ENCODER_IN_BASE 0x10001050
#define PWM_OUT_BASE 0x10001040

int main() {
	// variables
	alt_u8 pwm_out = IORD(PWM_OUT_BASE, 0);		// GPIO[1:0]
	alt_u8 encoder_in = IORD(ENCODER_IN_BASE, 0);  	// GPIO[3:2] c1 and c2

	float dim = 12.0f;

	float val_a = 0;
	float pwm_freq = 100.0f;  // 100 HZ sampling rate
	float speed;	// motor speed in RPM
	// mult by cpu_freq to convert to cycles
	float pwm_period = 1/pwm_freq * ALT_CPU_FREQ;  // 500,000 cycles
	alt_u64 dim_period = dim * ALT_CPU_FREQ;

	alt_u64 on_off; // switch
	
	// find period b/t pulses for C1 and C2
	// array variables to hold values for display
	// counter variables
	int m_period;
	float new_period;
	int n_count[3];
	float sec_counter;
	int array_count = 1;

	// start counter
	IOWR(PERFORMANCE_COUNTER_0_BASE,1,0); // writing 0 to register 1

  	while (1) {
		// reading upper 32 bits
		alt_u64 cycle_counter = (alt_u64)IORD(PERFORMANCE_COUNTER_0_BASE,1) << (alt_u64)32;
		// reading lower 32 bits
		cycle_counter = cycle_counter | (alt_u64)IORD(PERFORMANCE_COUNTER_0_BASE,0);

		float progress_d = (float)(cycle_counter%(alt_u64)dim_period) / (float)(dim_period);
		float progress_a = (float)(cycle_counter%(alt_u64)pwm_period) / (pwm_period);

		// new counter in seconds to keep track of time
		sec_counter = (float)(cycle_counter/ALT_CPU_FREQ);

		// read encoder 0x01, 0x10, 0x11
		encoder_in = IORD(ENCODER_IN_BASE, 0);
		if (encoder_in == 2 || encoder_in == 1) {
			n_count[array_count] = cycle_counter;
		// new time - old time = time b/t pulses
			if (array_count == 1) {
				array_count = 2;
			}else if (array_count == 2) {
				array_count = 1;
				m_period = (n_count[array_count] - n_count[array_count-1]);
				new_period = (float)(m_period/(int)ALT_CPU_FREQ);	
			}
			
		}

		// go forward, duty cycle in IA
		if (sec_counter <= 11.0f) {
			val_a = progress_d < .5f? progress_d/0.5f : 1.0f-progress_d;
			on_off = progress_a < val_a ? 1: 0;
			// write 1 to pwm_out
			IOWR(PWM_OUT_BASE, 0, on_off);
		
			pwm_out = IORD(PWM_OUT_BASE, 0);
			encoder_in = IORD(ENCODER_IN_BASE, 0);
			
			// speed RPM calculation
			speed = (1.0/new_period)/50.0/12.0*60.0;
			// display
			printf("Time: %f\nMotor Input: %f\nMotor Speed: %f\n=-=-=-=-=-=\n", sec_counter, val_a, speed);
		
		// go backward, duty cycle in IB
		} else if (sec_counter >= 11.0f && sec_counter < 24.0f) {

			val_a = progress_d < .5f? progress_d/0.5f : 1.0f-progress_d;
			// write 2 to pwm_out
			on_off = progress_a < val_a ? 2: 0;
			IOWR(PWM_OUT_BASE, 0, on_off);

			pwm_out = IORD(PWM_OUT_BASE, 0);
			encoder_in = IORD(ENCODER_IN_BASE, 0);
			
			// speed calculation
			speed = (1.0/new_period)/50.0/12.0*60.0;
			
			// display 
			printf("Time: %f\nMotor Input: -%f\nMotor Speed: -%f\n=-=-=-=-=-=\n", sec_counter, val_a, speed);
		}

	}
	return 0;
}








