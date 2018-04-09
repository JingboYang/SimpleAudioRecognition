`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: University of Toronto
// Engineer: Wenxuan Qiu (University of Toronto)
// 
// Create Date: 03/11/2018 03:34:12 PM
// Module Name: top
// Project Name: SimpleAudioRecognition
// Target Devices: Nexys 4 Video board with SSRelay
// Tool Versions: 2017.4
// Description: 
//      This module assign the least significant bit to the SSRelay. 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Revision 0.02 - Modified by Wenxuan Qiu
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module top(
    input [31:0]command,
    input clr,
    output [7:0] jc
);
    reg ssrON;

    assign jc[7:1] = 7'bzzzzzzz;
    assign jc[0] = command[0]&(clr) | 0&(~clr);
endmodule
