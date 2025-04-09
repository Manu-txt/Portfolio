`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/31/2024 05:49:52 PM
// Design Name: 
// Module Name: posedge_button_debounced
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


module posedge_button_debounced(
    input clk, rst,
    input btn,
    output debounced,
    output p_edge, n_edge, _edge
    );
    
debouncer_delayed DD(
    .clk(clk),
    .rst(rst),
    .noisy(btn),
    .debounced(debounced)
    );
    
edge_detector ED(
    .clk(clk),
    .rst(rst),
    .level(debounced),
    .p_edge(p_edge),
    .n_edge(n_edge),
    ._edge(_edge)
    );        
    
    
    
    
    
endmodule