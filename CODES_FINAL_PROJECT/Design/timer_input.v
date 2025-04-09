`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/02/2024 05:34:10 PM
// Design Name: 
// Module Name: timer_input
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


module timer_input #(parameter BITS = 4)(
    input clk,
    input rst,
    input en,
    input [BITS - 1:0] FINAL_VALUE,
    output done
    );
    
reg [BITS - 1:0] Q_reg, Q_next;

always @ (posedge clk, posedge rst)
begin
    if (rst)
        Q_reg <= 'b0;
    else if(en)
        Q_reg <= Q_next;
    else
        Q_reg <= Q_reg;
end    

assign done = Q_reg == FINAL_VALUE;

always @(clk or en or rst)
    Q_next = done? 'b0: Q_reg + 1;
      
    
endmodule

