`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/31/2024 05:50:47 PM
// Design Name: 
// Module Name: edge_detector
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

module edge_detector(
input clk,
    input rst,
    input level,
    output p_edge, n_edge, _edge
    );
    
reg state;
reg next_state;
parameter s0 = 0;
parameter s1 = 1;

always @(posedge clk, posedge rst)
begin
    if(rst)
        state <= s0;
    else
        state <= next_state;
end    
    
always @(level or state)
begin
    case(state)
        s0:
            begin
                if(level)
                    next_state = s1;
                else
                    next_state = s0;
            end
        s1:
            begin
                if(level)
                    next_state = s1;
                else
                    next_state = s0;
            end
      default: next_state = s0;
    endcase
end    
    
assign p_edge = (state == 0) & level;
assign n_edge = (state == s1) & ~level;
assign _edge = p_edge | n_edge; 
    
    
endmodule
