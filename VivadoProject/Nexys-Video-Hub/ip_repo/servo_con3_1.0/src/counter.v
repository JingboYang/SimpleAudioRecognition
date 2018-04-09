`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Digilent, University of Toronto
// Engineer: Kaitlyn Franz (Digilent), Wenxuan Qiu (University of Toronto)
// 
// Create Date: 03/11/2018 03:04:42 PM
// Module Name: Servo_interface
// Project Name: SimpleAudioRecognition
// Target Devices: Nexys 4 Video board with PmodCON3
// Tool Versions: 2017.4
// Description: 
//      This module counts to a constant value,
//      and then resets. This constant value is 
//      the value necessary to have the PWM
//      signal refresh every 10ms. 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Revision 0.02 - Modified by Wenxuan Qiu
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module counter (
	input clr,
	input clk,
	output reg [19:0]count
);

    // Run on the positive edge of the clock
	always @ (posedge clk)
	begin
	    // If the clear button is being pressed or the count
	    // value has been reached, set count to 0.
	    // This constant depends on the refresh rate required by the
	    // servo motor you are using. This creates a refresh rate
	    // of 10ms. 100MHz/(1/10ms) or (system clock)/(1/(Refresh Rate)).
		if (clr == 1'b0 || count == 20'd1000000)
			begin
			count <= 20'b0;
			end
		// If clear is not being pressed and the 
		// count value is not reached, continue to increment
		// count. 
		else
			begin
			count <= count + 1'b1;
			end
	end
endmodule