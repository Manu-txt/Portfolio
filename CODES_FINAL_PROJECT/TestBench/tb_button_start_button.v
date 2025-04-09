`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/31/2024 06:01:05 PM
// Design Name: 
// Module Name: tb_button_start_button
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


module tb_btn_start_button(

    );
reg clk;
reg rst;    
reg btn_start;
reg timerEnd;
wire start;
wire idle;    

btn_start_button GEN(
    .clk(clk),
    .rst(rst),
    .btn_start(btn_start),
    .timerEnd(timerEnd),
    .start(start),
    .idle(idle)
    );
    
initial begin
clk = 0;
rst = 1;
btn_start = 0;
timerEnd = 0;
end

always
    #5 clk = ~clk;
    
initial begin    
rst = 0;
#50
btn_start = 1;
#5
btn_start = 0;
#50
btn_start = 1;
#5
btn_start = 0;
#50
btn_start = 1;
#5
btn_start = 0;
#100
timerEnd = 1;
#50
btn_start = 1;
#5
btn_start = 0;
end    

endmodule
