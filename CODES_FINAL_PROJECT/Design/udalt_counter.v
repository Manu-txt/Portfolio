`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/02/2024 05:10:16 PM
// Design Name: 
// Module Name: udalt_counter
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


module udalt_counter(
    input Clk,
    input reset,en,
    output [3:0]count
    );
    reg [3:0]temp;
    reg UD; //when UD = 0, up counts. when UD = 1, down counts.
    always @(posedge Clk)
    begin
    if(reset)
        begin
        UD <= 1'b0;
        temp <= 4'd0;
        end
    else
        begin
        if(en)
            begin
            if(UD)
                begin
                if(temp > 4'd0)
                temp <= temp - 1;
                else
                    begin
                    UD <= 1'b0;
                    temp <= temp + 1;
                    end
                end
            else
                begin
                if(temp < 4'd15)
                temp <= temp + 1;
                else
                    begin
                    UD <= 1'b1;
                    temp <= temp - 1;
                    end
                end
            end
        end
    end
    assign count = temp;
endmodule

