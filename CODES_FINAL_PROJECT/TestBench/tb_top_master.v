`timescale 1ns / 1ps

module tb_top_master(
    );
    
reg sys_clk;
reg sys_rst;
reg btn_up;
reg btn_dwn;
reg btn_start;
//reg pizza;
//reg potato;
reg [5:0] load_minutes;
reg [5:0] load_seconds;
wire [7:0] AN;
wire [6:0] display;
wire DP;
wire [15:0] LED;
wire start;
wire idle;
//wire buzzer;
//wire temp_SCL;

   
top_master GEN(
    .sys_clk(sys_clk),
    .sys_rst(sys_rst),
    .btn_up(btn_up),
    .btn_dwn(btn_dwn),
    .btn_start(btn_start),
    //.potato(potato),
    //.pizza(pizza),
    .load_minutes(load_minutes), 
    .load_seconds(load_seconds),   
    .AN(AN),
    .display(display),
    .DP(DP),    
    .LED(LED),
    .start(start),
    .idle(idle),
    //.buzzer(buzzer)
    //.temp_SCL(temp_SCL)
    );    
    
    
initial begin
sys_clk = 0;
sys_rst = 1;
btn_up = 0;
btn_dwn = 0;
btn_start = 0;
//pizza = 0;
//potato = 0;
load_minutes = 6'd0;
load_seconds = 6'd2;
end    
    
always    
    #5 sys_clk = ~sys_clk;
    
initial begin
#5
sys_rst = 0;
#1000000
btn_up = 1;
#30000000
btn_up = 0;
#1000000
btn_up = 1;
#30000000
btn_up = 0;
#1000000
btn_up = 1;
#30000000
btn_up = 0;
#1000000
btn_dwn = 1;
#30000000
btn_dwn = 0;
#1000000
btn_dwn = 1;
#30000000
btn_dwn = 0;
#1000000
btn_dwn = 1;
#30000000
btn_dwn = 0;
#1000000
btn_dwn = 1;
#30000000
btn_dwn = 0;
#1000000
btn_dwn = 1;
#30000000
btn_dwn = 0;
#30000000
btn_start = 1;
#30000000
btn_start = 0;

end    
endmodule
