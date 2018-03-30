`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Digilent
// Engineer: Kaitlyn Franz
// 
// Create Date: 02/01/2016 01:32:45 PM
// Design Name: Servo control with the CON3
// Module Name: sw_to_angle
// Project Name: The Claw
// Target Devices: Basys 3 with PmodCON3
// Tool Versions: 2015.4
// Description: 
//      This module takes the switch values as an input
//      and converts them to a degree value. 
//
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

// Convert from switch value to angle
// Each switch provides a different angle in degrees, starting
// at 0, incrementing by 24 degrees each time. 
module sw_to_angle(
    input [31:0] sw,
    output reg [8:0] angle
    );
    
    // Run when the value of the switches
    // changes
    always @ (sw)
    begin
        case (sw)
        1:
        angle = 9'd10;
        2:
        angle = 9'd20;
        3:
        angle = 9'd30;
        4:
        angle = 9'd40;
        5:
        angle = 9'd50;
        6:
        angle = 9'd60;
        7:
        angle = 9'd70;
        8:
        angle = 9'd80;
        9:
        angle = 9'd90;
        10:
        angle = 9'd100;
        11:
        angle = 9'd110;
        12:
        angle = 9'd120;
        13:
        angle = 9'd130;
        14:
        angle = 9'd140;
        15:
        angle = 9'd150;
        16:
        angle = 9'd160;
        17:
        angle = 9'd170;
        18:
        angle = 9'd180;
        default:
        angle = 9'd0;
        endcase                 
    end
endmodule
