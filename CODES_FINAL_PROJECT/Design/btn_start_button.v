`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/31/2024 05:53:38 PM
// Design Name: 
// Module Name: btn_start_button
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


module btn_start_button(
    input clk,              //input system clock
    input rst,              //input reset
    input btn_start,        //button to start microwave
    input timerEnd,         //input signal to detect when timer ends
    output start,           //output signal to know if microwave started and is heating
    output idle             //output signal to know if microwave is in idle
    );
    
reg startreg = 'b0;          //start register
reg idlereg = 'b1;           //idle register


always @ (posedge clk)          //for every clock pulse...
begin
    if (rst)                        //check for reset
        begin
            startreg <= 'b0;           //set default idle state
            idlereg <= 'b1;
        end
    else if (btn_start)         //if start button pressed...
        begin        
            startreg <= ~startreg;      //inverse start and idle registers (button is a pause/unpause mechanism)
            idlereg <= ~idlereg;
        end  
    if (timerEnd)               //if timer ends...
        begin
            startreg <= 'b0;       //set idle
            idlereg <= 'b1;
        end    
end 

assign start = startreg;
assign idle = idlereg;


endmodule

