`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/01/2024 11:50:16 AM
// Design Name: 
// Module Name: display_7seg_timer
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
// For this modules, I have compile files display_7seg_words.v, old_display_7seg_timer.v, digit_separator.v and anode_controller.v together to help you check it easier. For testbench and simulation, we did separate each file so
// whenever we want to test cases to check if they are work well, we will enable those files above, with uncomment those files inside the top_7seg_master 

// the 7 seg display patterns for the minutes and seconds of the timer
module display_7seg_timer(clk_100MHz, sys_clk, sel, minutes, seconds, min_D1, min_D0, sec_D1, sec_D0, display_min_D1, display_min_D0, display_sec_D1, display_sec_D0, mode, display_words_3, display_words_0,
                        display_temp_D0, display_temp_D1, temp_data, AN, DP, display);

   input [1:0] mode;
   output reg [6:0] display_min_D1 = 0, display_min_D0 = 0, display_sec_D1 = 0, display_sec_D0 = 0, display_temp_D0 = 0, display_temp_D1 = 0;
   output reg [6:0] display_words_3, display_words_0 = 0;
   
   input clk_100MHz;               // Nexys A7 clock
   input [7:0] temp_data;          // Temp data from i2c master
//////////////////////////////////////DIGIT_SEPERATOR//////////////////////////////////////////////
//separates the digits the timer's minutes and seconds
   input [5:0] minutes, seconds;
   output reg [3:0] min_D1 = 0, min_D0 = 0, sec_D1 = 0, sec_D0 = 0;
   wire [3:0] temp_D0, temp_D1;

    assign temp_D1 = temp_data / 10;           // Tens value of temp data
    assign temp_D0 = temp_data % 10;           // Ones value of temp data
	
always@(minutes) // at every minute
    begin
        min_D1 = minutes / 10; // separate ten's place digit into one value
        min_D0 = minutes % 10; // separate one's place digit into another
    end
always@(seconds) // at every second
    begin
    sec_D1 = seconds / 10; // separate ten's place digit into one value
    sec_D0 = seconds % 10; // separate one's place digit into another
    end

///////////////////////////////////TEMP SET UP////////////////////////////////////////////////////
    reg [1:0] anode_select;         // 2 bit counter for selecting each of 4 digits
    reg [16:0] anode_timer;         // counter for digit refresh
    
    // Logic for controlling digit select and digit timer
    always @(posedge clk_100MHz) begin
        // 1ms x 4 displays = 4ms refresh period
        if(anode_timer == 99_999) begin         // The period of 100MHz clock is 10ns (1/100,000,000 seconds)
            anode_timer <= 0;                   // 10ns x 100,000 = 1ms
            anode_select <=  anode_select + 1;
        end
        else
            anode_timer <=  anode_timer + 1;
    end

///////////////////////////////////ANODE_CONTROLLER////////////////////////////////////////////////
// decides which 7 seg pattern to display and where based on the refresh counter (2-bit, 400 Hz clk counter)
   input sys_clk;
   input [2:0] sel;
   output reg [7:0] AN = 0;
   output reg DP = 1;
   output reg [6:0] display = 0;

reg [7:0] AN1, AN2, AN3, AN4, AN5, AN6, AN7, AN8;   // The eight segments that are to be lit up

always@(sel)
    begin
        AN1 = 8'b11111110; // sets a scenario for each individual 7 seg to be lit up
        AN2 = 8'b11111101;
        AN3 = 8'b11111011;
        AN4 = 8'b11110111;
        AN5 = 8'b11101111;
        AN6 = 8'b11011111;
        AN7 = 8'b10111111;
        AN8 = 8'b01111111;
            
        case(sel)
            3'b000: // if counter is at 0
                begin
                    display <= display_sec_D0; // select 1st pattern
                    AN <= AN1; // light up 1st 7 seg display
                end
            3'b001: // if counter is at 1
                begin
                    display <= display_sec_D1; // select 2nd pattern
                    AN <= AN2; // light up 2nd 7seg display
                end
            3'b010: // if counter is at 2
                begin
                    display <= display_min_D0; // select 3rd pattern
                    AN <= AN3; // light up 3rd 7seg display
                end
            3'b011: // if counter is at 3
                begin
                    display <= display_min_D1; // select 4th pattern
                    AN <= AN4; // light up 4th 7seg display
                end

            3'b100: // if counter is at 4
                begin
                    display <= display_words_0; // select 5th pattern
                    AN <= AN5; // light up 5th 7seg display
                end
            3'b101: // if counter is at 5
                begin
                    display <= display_temp_D0; // select 6th pattern
                    AN <= AN6; // light up 6th 7seg display
                end
            3'b110: // if counter is at 6
                begin
                    display <= display_temp_D1; // select 7th pattern
                    AN <= AN7; // light up 7th 7seg display
                end
            3'b111: // if counter is at 7
                begin
                    display <= display_words_3; // select 8th pattern
                    AN <= AN8; // light up 8th 7seg display
                end
            default:
                begin
                    display <= 7'b0111111; // otherwise display '-' and this
                    AN <= 8'b01010101; // flipping pattern
                end
        endcase
    end

///////////////////////////////////DISPLAY TIMER (MIN:SEC)////////////////////////////////////////
always @(min_D1) // Changes the 7seg display for every second (at the ones place)
    begin
        case(min_D1)
            4'b0000: display_min_D1 <= 7'b1000000; //0
            4'b0001: display_min_D1 <= 7'b1111001; //1
            4'b0010: display_min_D1 <= 7'b0100100; //2
            4'b0011: display_min_D1 <= 7'b0110000; //3
            4'b0100: display_min_D1 <= 7'b0011001; //4
            4'b0101: display_min_D1 <= 7'b0010010; //5
            4'b0110: display_min_D1 <= 7'b0000010; //6
            4'b0111: display_min_D1 <= 7'b1111000; //7
            4'b1000: display_min_D1 <= 7'b0000000; //8
            4'b1001: display_min_D1 <= 7'b0010000; //9
            default: display_min_D1 <= 7'dz;
        endcase        
    end
always @(min_D0) // Changes the 7seg display for every second (at the tens place)
    begin
        case(min_D0)
            4'b0000: display_min_D0 <= 7'b1000000; //0
            4'b0001: display_min_D0 <= 7'b1111001; //1
            4'b0010: display_min_D0 <= 7'b0100100; //2
            4'b0011: display_min_D0 <= 7'b0110000; //3
            4'b0100: display_min_D0 <= 7'b0011001; //4
            4'b0101: display_min_D0 <= 7'b0010010; //5
            4'b0110: display_min_D0 <= 7'b0000010; //6
            4'b0111: display_min_D0 <= 7'b1111000; //7
            4'b1000: display_min_D0 <= 7'b0000000; //8
            4'b1001: display_min_D0 <= 7'b0010000; //9
            default: display_min_D0 <= 7'dz;
        endcase        
    end
always @(sec_D1) // Changes the 7seg display for every minute (at the ones place)
    begin
        case(sec_D1)
            4'b0000: display_sec_D1 <= 7'b1000000; //0
            4'b0001: display_sec_D1 <= 7'b1111001; //1
            4'b0010: display_sec_D1 <= 7'b0100100; //2
            4'b0011: display_sec_D1 <= 7'b0110000; //3
            4'b0100: display_sec_D1 <= 7'b0011001; //4
            4'b0101: display_sec_D1 <= 7'b0010010; //5
            4'b0110: display_sec_D1 <= 7'b0000010; //6
            4'b0111: display_sec_D1 <= 7'b1111000; //7
            4'b1000: display_sec_D1 <= 7'b0000000; //8
            4'b1001: display_sec_D1 <= 7'b0010000; //9
            default: display_sec_D1 <= 7'dz;
        endcase        
    end

always @(sec_D0) // Changes the 7seg display for every minute (at the tens place)
    begin
        case(sec_D0)
            4'b0000: display_sec_D0 <= 7'b1000000; //0
            4'b0001: display_sec_D0 <= 7'b1111001; //1
            4'b0010: display_sec_D0 <= 7'b0100100; //2
            4'b0011: display_sec_D0 <= 7'b0110000; //3
            4'b0100: display_sec_D0 <= 7'b0011001; //4
            4'b0101: display_sec_D0 <= 7'b0010010; //5
            4'b0110: display_sec_D0 <= 7'b0000010; //6
            4'b0111: display_sec_D0 <= 7'b1111000; //7
            4'b1000: display_sec_D0 <= 7'b0000000; //8
            4'b1001: display_sec_D0 <= 7'b0010000; //9
            default: display_sec_D0 <= 7'dz;
        endcase        
    end

always @(temp_D0) // Changes the 7seg display for every tempurature (at the tens place)
    begin
        case(temp_D0)
            4'b0000: display_temp_D0 <= 7'b1000000; //0
            4'b0001: display_temp_D0 <= 7'b1111001; //1
            4'b0010: display_temp_D0 <= 7'b0100100; //2
            4'b0011: display_temp_D0 <= 7'b0110000; //3
            4'b0100: display_temp_D0 <= 7'b0011001; //4
            4'b0101: display_temp_D0 <= 7'b0010010; //5
            4'b0110: display_temp_D0 <= 7'b0000010; //6
            4'b0111: display_temp_D0 <= 7'b1111000; //7
            4'b1000: display_temp_D0 <= 7'b0000000; //8
            4'b1001: display_temp_D0 <= 7'b0010000; //9
            default: display_temp_D0 <= 7'dz;
        endcase        
    end

always @(temp_D1) // Changes the 7seg display for every tempurature (at the tens place)
    begin
        case(temp_D1)
            4'b0000: display_temp_D1 <= 7'b1000000; //0
            4'b0001: display_temp_D1 <= 7'b1111001; //1
            4'b0010: display_temp_D1 <= 7'b0100100; //2
            4'b0011: display_temp_D1 <= 7'b0110000; //3
            4'b0100: display_temp_D1 <= 7'b0011001; //4
            4'b0101: display_temp_D1 <= 7'b0010010; //5
            4'b0110: display_temp_D1 <= 7'b0000010; //6
            4'b0111: display_temp_D1 <= 7'b1111000; //7
            4'b1000: display_temp_D1 <= 7'b0000000; //8
            4'b1001: display_temp_D1 <= 7'b0010000; //9
            default: display_temp_D1 <= 7'dz;
        endcase        
    end


///////////////////////////////////DISPLAY POWER LEVEL////////////////////////////////////////

always @(mode)
    begin
        case(mode)
            2'b01 : display_words_3 = 7'b1000111; // low mode "L-Low"
            2'b10 : display_words_3 = 7'b0101011; // normal mode "n-normal"
            2'b11 : display_words_3 = 7'b0001001; // high mode "H-High"
            default : display_words_3 = 7'b0111111;
        endcase
    end

always @(mode)
    begin
        case(mode)
            2'b01 : display_words_0 = 7'b0011100; // degree sympol
            2'b10 : display_words_0 = 7'b0011100; // degree sympol
            2'b11 : display_words_0 = 7'b0011100; // degree sympol
            default : display_words_0 = 7'd0111111;
        endcase
    end
     
endmodule
