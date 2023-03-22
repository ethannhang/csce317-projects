# CSCE 317 - Computer Systems Engineering

## Lab 1
Design a Nios2-based System-on-Chip platform and develop associated software that will use Pulse Width Modulation to modulate the intensity of the 26 LEDs (18 red
and 8 green) that run across the bottom left side of the DE2-115 FPGA Board.

## Lab 2 -
Design a SystemVerilog based Avalon IP component for the platform you developed
as part of the previous lab, named “led_pwm”. led_pwm will provide an interface between Nios2
software and the 26 DE2-115 LEDs, and will allow the software, by way of writing a control register,
to illuminate any combination of LEDs corresponding to the pattern of 1-bits in the value written.
Immediating after writing a value, led_pwm will illumiate the LEDs corresponding to the 1-bits in the
written value, and then subsequently dim their brightness to zero after 1.3422 seconds (2ˆ26/50e6).
You will also write software that will illuminate a random LED every 90 ms until one of the buttons are
pressed, at which point the system will change the time between the illimination of a random LED
to 50 ms, 70 ms, or 90 ms, depending on which key was pressed.

## Lab 3 -
Design an open-loop controller for a motor. The controller will drive the motor’s
speed from 0 rpm to its maximum forward rpm, and then to its maximum reverse rpm, and then back
to 0 over the course of approximately 20 seconds. The controller will also monitor the motor’s speed,
record to an array, and then print a table of time, input value, and motor speed
