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
//      This module takes the switch values as an input
//      and converts them to a degree value. 
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Revision 0.02 - Modified by Wenxuan Qiu
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

// Convert from input value to angle
// angle in degress = input value * 10 
module sw_to_angle(
    input [31:0] sw,
    output reg [8:0] angle
    );
    
    // Run when the value of the input
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
