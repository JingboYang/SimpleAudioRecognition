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
//      This module compares two contant values, A and B. 
//      If B is bigger than A then the PWM output will be 1,
//      otherwise it will be zero. This creates the waveform
//      that drives the servos.
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Revision 0.02 - Modified by Wenxuan Qiu
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module comparator (
	input [19:0] A,
	input [19:0] B,
	output reg PWM
);

    // Run when A or B change
	always @ (A,B)
	begin
	// If A is less than B
	// output is 1.
	if (A < B)
		begin
		PWM <= 1'b1;
		end
	// If A is greater than B
	// output is 0.
	else 
		begin
		PWM <= 1'b0;
		end
	end
endmodule