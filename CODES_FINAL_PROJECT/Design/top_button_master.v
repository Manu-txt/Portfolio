`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/31/2024 05:44:52 PM
// Design Name: 
// Module Name: top_button_master
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


module top_button_master(
    input clk,
    input rst,
    input timerEnd,
    input btn_up,
    input btn_dwn,
    input btn_start,
    output [1:0] mode,
    output start,
    output idle
    );
    
wire s;
wire i;    
assign start = s;
assign idle = i;    
   
wire up_debounced, down_debounced, start_debounced;
 
posedge_button_debounced buttonUp(
    .clk(clk),
    .rst(rst),
    .btn(btn_up),
    .debounced(),
    .p_edge(up_debounced),
    .n_edge(),
    ._edge()    
    );
    
posedge_button_debounced buttonDown(
    .clk(clk),
    .rst(rst),
    .btn(btn_dwn),
    .debounced(),
    .p_edge(down_debounced),
    .n_edge(),
    ._edge()    
    );
    
posedge_button_debounced buttonStart(
    .clk(clk),
    .rst(rst),
    .btn(btn_start),
    .debounced(),
    .p_edge(start_debounced),
    .n_edge(),
    ._edge()    
    );                
    
btn_start_button startButton(
    .clk(clk),              
    .rst(rst),              
    .btn_start(start_debounced),   
    .timerEnd(timerEnd),         
    .start(s),          
    .idle(i)            
    );
    
btn_mode_select modeSelect(
    .clk(clk),          
    .rst(rst),        
    .start(s),       
    .idle(i),         
    .btn_up(up_debounced),      
    .btn_dwn(down_debounced),     
    .mode(mode)   
    );        
    
endmodule
