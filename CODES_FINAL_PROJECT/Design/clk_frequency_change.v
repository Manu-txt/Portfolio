`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/29/2024 02:16:54 AM
// Design Name: 
// Module Name: clk_frequency_change
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


// slows down the system clock to the three frequencues (1 Hz, 400 Hz, and 5 Hz)
module clk_frequency_change(clk, clk_1Hz, clk_400Hz, clk_5Hz);
input clk;
output  clk_1Hz, clk_400Hz, clk_5Hz;
reg clk_1Hz = 0, clk_400Hz = 0, clk_5Hz = 0;
reg [26:0] counter1 = 0;
reg [26:0] counter2 = 0;
reg [26:0] counter3 = 0;

always@(posedge clk) // at positive edge of system clock
    begin
        counter1 <= counter1 + 1;
        counter2 <= counter2 + 1;
        counter3 <= counter3 + 1;
    	//1hz clock equal 50MHz    
        if (counter1 == 50_000_000) // 50,000,000 is amount for 1 Hz clock
            begin
                clk_1Hz <= ~clk_1Hz; // flip signal every period
                counter1 <= 0;  // restart counter for 1 Hz clock
            end
        if (counter2 == 125_000) // 125,000 is amount for 400 Hz clock
            begin
                clk_400Hz <= ~clk_400Hz; // flip signal every period
                counter2 <= 0;  // restart other counter for 400 Hz clock
            end
        if (counter3 == 10_000_000) // 10,000,000 is amount for 5 Hz clock
            begin
                clk_5Hz <= ~clk_5Hz; // flip signal every period
                counter3 <= 0;  // restart other counter for 5 Hz clock
        end
    end
endmodule
