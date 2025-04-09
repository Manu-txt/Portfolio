`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/31/2024 06:00:22 PM
// Design Name: 
// Module Name: tb_btn_mode_select
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


module tb_btn_mode_select(

    );
    
reg clk;
reg rst;
reg start;
reg idle;
reg btn_up;
reg btn_dwn;
wire [1:0] mode;

btn_mode_select GEN(
    .clk(clk),
    .rst(rst),
    .start(start),
    .idle(idle),
    .btn_up(btn_up),
    .btn_dwn(btn_dwn),
    .mode(mode)
    );
    
initial begin
clk = 0;
rst = 1;
start = 0;
idle = 0;
btn_up = 0;
btn_dwn = 0;
end    

always
    #5 clk = ~clk;
    

initial begin
rst = 0;
start = 0;
idle = 1;
#25
btn_up = 1;
#10
btn_up = 0;
#25
btn_up = 1;
#10
btn_up = 0;
#25
btn_up = 1;
#10
btn_up = 0;
#25
btn_dwn = 1;
#10
btn_dwn = 0;
#25
btn_dwn = 1;
#10
btn_dwn = 0;
#25
btn_dwn = 1;
#10
btn_dwn = 0;
#25
btn_dwn = 1;
#10
btn_dwn = 0;
#25
btn_dwn = 1;
#10
btn_dwn = 0;
#10
start = 0;
idle = 1;
#25
btn_up = 1;
#10
btn_up = 0;
#25
btn_up = 1;
#10
btn_up = 0;
#25
btn_up = 1;
#10
btn_up = 0;
#25
btn_dwn = 1;
#10
btn_dwn = 0;
#25
btn_dwn = 1;
#10
btn_dwn = 0;
#25
btn_dwn = 1;
#10
btn_dwn = 0;
#25
btn_dwn = 1;
#10
btn_dwn = 0;
#25
btn_dwn = 1;
#10
btn_dwn = 0;
#25
btn_up = 1;
btn_dwn = 1;
#10
btn_up = 0;
btn_dwn = 0;
end

endmodule

