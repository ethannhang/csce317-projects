// led_pwm.v

// This file was auto-generated as a prototype implementation of a module
// created in component editor.  It ties off all outputs to ground and
// ignores all inputs.  It needs to be edited to make it do something
// useful.
// 
// This file will not be automatically regenerated.  You should check it in
// to your version control system if you want to keep it.

`timescale 1 ps / 1 ps
module led_pwm (
		input  wire        clk_clk,                //          clk.clk
		input  wire        avalon_slave_address,   // avalon_slave.address
		input  wire [31:0] avalon_slave_writedata, //             .writedata
		input  wire        avalon_slave_write,     //             .write
		input  wire        rst_reset,              //          rst.reset
		output wire [25:0] leds                    //     external.new_signal
	);

	reg [11:0] global_counter;
	reg [25:0] duty_cycle_counter [25:0];
	reg [25:0] dim_counter [25:0];
	reg [25:0] duty_progress;
	reg [25:0] dim_progress;
	// dim_period = 67110000;
	// pwm_period = 67108864;

	// global counter
	always @(posedge clk_clk) begin
		global_counter <= global_counter + 1'b1;
	end

	// generate loop
	genvar i;
	generate for (i=0;i<26;i=i+1) begin : gen_name
		// duty cycle
		always @(posedge clk_clk) begin
			if(avalon_slave_writedata[i] == 1 && avalon_slave_write)
				duty_cycle_counter[i] <= {26{1'b1}};
			if(duty_cycle_counter[i] > 26'd0)
				duty_cycle_counter[i] <= duty_cycle_counter[i] - 1'b1;
		end
		// assign leds value
		assign leds[i] = (global_counter < duty_cycle_counter[i][25:12]) ? 1'b1 : 1'b0;
	end
	endgenerate


endmodule
