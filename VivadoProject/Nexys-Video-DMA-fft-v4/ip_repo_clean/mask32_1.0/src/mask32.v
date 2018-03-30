`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/05/2018 02:16:51 PM
// Design Name: 
// Module Name: mask32
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


module mask32(
    input [31:0]word_in,
    output [31:0]word_out
    );
    
    parameter mask = 32'h0000ffff;
    assign word_out = word_in & mask;
    
endmodule
