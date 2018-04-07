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
//      This module creates the PWM signal needed to drive
//      one servo using the PmodCON3. To use the other 3 servo connectors,
//      you can instantiate this module 4 times, or send the same PWM sigal to 
//      four Pmod connector pins. This depends on whether you want the same servo signal, 
//      or different servo signals. 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Revision 0.02 - Modified by Wenxuan Qiu
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module Servo_interface (
    input [31:0] command,
    input clr,
    input clk,
    output PWM
    );
    
    wire [19:0] A_net;
    wire [19:0] value_net;
    wire [8:0] angle_net;
    
    // Convert the incoming switch value
    // to an angle.
    sw_to_angle converter(
        .sw(command),
        .angle(angle_net)
        );
    
    // Convert the angle value to 
    // the constant value needed for the PWM.
    angle_decoder decode(
        .angle(angle_net),
        .value(value_net)
        );
    
    // Compare the count value from the
    // counter, with the constant value set by
    // the switches.
    comparator compare(
        .A(A_net),
        .B(value_net),
        .PWM(PWM)
        );
      
    // Counts up to a certain value and then resets.
    // This module creates the refresh rate of 20ms.   
    counter count(
        .clr(clr),
        .clk(clk),
        .count(A_net)
        );
        
endmodule
