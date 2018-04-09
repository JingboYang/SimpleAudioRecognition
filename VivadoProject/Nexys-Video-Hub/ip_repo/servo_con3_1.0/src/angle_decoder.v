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
//      This module takes in an angle value
//      and converts it into the PWM constant 
//      the servo needs to hold that angle.
// Dependencies
// 
// Revision:
// Revision 0.01 - File Created
// Revision 0.02 - Modified by Wenxuan Qiu
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module angle_decoder(
    input [8:0] angle,
    output reg [19:0] value
    );
    
    // Run when angle changes
    always @ (angle)
    begin
        // The angle gets converted to the 
        // constant value. This equation
        // depends on the servo motor you are 
        // using. To get this equation I used 
        // trial and error to get the 0
        // and 360 values and created an equation
        // based on those two points. 
        value = (10'd944)*(angle)+ 16'd60000;
    end
endmodule
