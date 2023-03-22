#include "sys/alt_stdio.h"

// library to use altera variable types
#include "alt_types.h"

// library to get all hardware definitions
#include "system.h"

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <io.h>
// Copyright 2022 Ethan Hang
int main(){	
    // variables
    int sleep = 90000;
    alt_u8 key;
    int max = 25;
    int min = 0;
    int random;

	while(1){
	    // led value variable
	    alt_u32 led_val = 1;

	    // read key base value
	    alt_u8 key_val = IORD(KEY_BASE,0);
	    printf("kv: %d\n", key_val);

	    // if key is pressed, key_val AND 0
	    if(key_val < 15){
	    	key = key_val;
	    }
	    printf("key: %d\n", key);

	    // monitor key value and change write frequency
	    if(key == 13){
	      sleep = 90000;  // 90,000 microseconds = 90 milliseconds
	    }
	    if(key == 11){
	      sleep = 70000;
	    }
	    if(key == 7){
	      sleep = 50000;
	    }

	    // random number 1-26
	    random = (rand() % (max+1-min)) + min;
	    // shift led value by number
	    led_val = led_val << random;

	    // write random led value to pwm base
	    IOWR(LED_PWM_0_BASE, 0, led_val);
	    // sleep in microseconds
	    usleep(sleep);
	}
return 0;
}
