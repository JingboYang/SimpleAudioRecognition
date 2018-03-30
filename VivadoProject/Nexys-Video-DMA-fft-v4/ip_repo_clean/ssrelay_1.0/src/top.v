`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 01/25/2017 09:53:39 AM
// Design Name: 
// Module Name: top
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
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
