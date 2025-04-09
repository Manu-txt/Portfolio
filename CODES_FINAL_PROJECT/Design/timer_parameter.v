`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/31/2024 05:57:26 PM
// Design Name: 
// Module Name: timer_parameter
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


module timer_parameter #(parameter FINAL_VALUE = 1999999)(
    input clk,
    input rst,
    input en,
    output done
    );
    
    localparam BITS = $clog2(FINAL_VALUE);

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


