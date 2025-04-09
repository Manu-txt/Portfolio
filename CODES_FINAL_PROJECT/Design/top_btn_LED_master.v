`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/02/2024 04:34:59 PM
// Design Name: 
// Module Name: top_btn_LED_master
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

module top_btn_LED_master(
    input sys_clk, sys_rst,
    input mini_rst,
    input timerEnd,
    input btn_up,
    input btn_dwn,
    input btn_start,
    output [15:0] LED,
    output [1:0] mode,
    output start,
    output idle
    );
    
wire s;
wire i;    
wire [1:0] modes;
assign start = s;
assign idle = i;
assign mode = modes;
    
top_button_master btnMaster(
    .clk(sys_clk),
    .rst(1'b0),
    .timerEnd(timerEnd),
    .btn_up(btn_up),
    .btn_dwn(btn_dwn),
    .btn_start(btn_start),
    .mode(modes),
    .start(s),
    .idle(i)
    );
    
top_LED_master LEDMaster(
    .sys_clk(sys_clk),
    .sys_rst(sys_rst),
    .mini_rst(mini_rst),
    .start(s),
    .mode(modes),
    .LED(LED)
    );
    
    
       
endmodule
