`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/01/2024 11:46:48 AM
// Design Name: 
// Module Name: top_7seg_master
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


module top_7seg_master(
    input sys_clk,
    input [1:0] mode,
    input rst,
    input [5:0] load_minutes, //= 6'b000010; // using load_minutes = 2 and load_seconds = 30 for testing
    input [5:0] load_seconds, //= 6'b011110; // using load_minutes = 2 and load_seconds = 30 for testing
    input start,
    input idle,
    input clk_100MHz,               // Nexys A7 clock
    input [7:0] temp_data,          // Temp data from i2c master
    output pizza,
    output potato,
    output [7:0] AN,
    output [6:0] display,
    output DP,
    //output buzzer,
    output timerEnd
    );
    
  
wire clk_1Hz, clk_400Hz, clk_5Hz;
wire [2:0] sel;
wire [5:0] minutes, seconds;
wire [3:0] min_D1, min_D0, sec_D1, sec_D0, temp_D0, temp_D1;
wire [6:0] display_min_D1, display_min_D0, display_sec_D1, display_sec_D0, display_temp_D0, diaplay_temp_D1;
wire [6:0] display_words_3, display_words_0;
    
// slows down the system clock to the three frequencues (1 Hz, 400 Hz, and 5 Hz)
clk_frequency_change slowerClock(sys_clk, clk_1Hz, clk_400Hz, clk_5Hz);

// is a select/refresh counter for the anode controller                                                        
counter_refresh refreshCounter(clk_400Hz, sel); // counts up from 0-3 using the 400 Hz clock

// counts down the minutes and seconds of the timer using the 1 Hz clock
min_sec_down_counter timerCounter(potato, pizza, start, rst, clk_1Hz, load_minutes, load_seconds, minutes, seconds, timerEnd); // begins counting down once start button is pushed

// the 7 seg display patterns for the minutes and seconds of the timer                                                   
display_7seg_timer timerDisplay(clk_100MHz, sys_clk, sel, minutes, seconds, min_D1, min_D0, sec_D1, sec_D0, display_min_D1, display_min_D0, display_sec_D1, display_sec_D0, mode, display_words_3, display_words_0,
                        display_temp_D0, display_temp_D1, temp_data, AN, DP, display);

// FOR TESTBENCH AND SIMULATION EACH SECTION WE WANT
/*
// separates the digits the timer's minutes and seconds
digit_separator timerDigits(minutes, seconds, min_D1, min_D0, sec_D1, sec_D0);

// the 7 seg display patterns for the minutes and seconds of the timer                                                   
old_display_7seg_timer old_timerDisplay(min_D1, min_D0, sec_D1, sec_D0, display_min_D1, display_min_D0, display_sec_D1, display_sec_D0);

// the 7 seg display patterns for the modes
display_7seg_words wordsDisplay(mode, display_words_3, display_words_2, display_words_1, display_words_0);

// decides which 7 seg pattern to display and where based on the refresh counter (2-bit, 400 Hz clk counter)
anode_controller timerAnode(sys_clk, sel, display_min_D1, display_min_D0, display_sec_D1, display_sec_D0,
         display_words_3, display_words_2, display_words_1, display_words_0, AN, DP, display);
*/

endmodule

