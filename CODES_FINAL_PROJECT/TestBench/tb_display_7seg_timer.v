`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12/16/2022 10:50:26 AM
// Design Name: 
// Module Name: TB_display_7seg_timer
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

/*
module tb_display_7seg_timer;
    // DUT inputs and outputs
    reg clk_100MHz;
    reg [5:0] minutes, seconds;
    reg [7:0] temp_data;
    reg [1:0] mode;
    reg [2:0] sel;

    wire [6:0] display_min_D1, display_min_D0, display_sec_D1, display_sec_D0;
    wire [6:0] display_temp_D0, display_temp_D1;
    wire [7:0] AN;
    wire DP;
    wire [6:0] display;

    // DUT instantiation
    display_7seg_timer uut (
        .clk_100MHz(clk_100MHz),
        .minutes(minutes),
        .seconds(seconds),
        .temp_data(temp_data),
        .mode(mode),
        .sel(sel),
        .display_min_D1(display_min_D1),
        .display_min_D0(display_min_D0),
        .display_sec_D1(display_sec_D1),
        .display_sec_D0(display_sec_D0),
        .display_temp_D0(display_temp_D0),
        .display_temp_D1(display_temp_D1),
        .AN(AN),
        .DP(DP),
        .display(display)
    );

    // Clock generation
    initial begin
        clk_100MHz = 0;
        forever #5 clk_100MHz = ~clk_100MHz; // 100MHz clock period = 10ns
    end

    // Task to check outputs and report pass/fail
    task check_output;
        input [6:0] expected_display_min_D1;
        input [6:0] expected_display_min_D0;
        input [6:0] expected_display_sec_D1;
        input [6:0] expected_display_sec_D0;

        begin
            $display("Test Case: minutes=%0d, seconds=%0d", minutes, seconds);
            $display("Expected: min_D1=%b, min_D0=%b, sec_D1=%b, sec_D0=%b",
                     expected_display_min_D1, expected_display_min_D0,
                     expected_display_sec_D1, expected_display_sec_D0);
            $display("Actual:   min_D1=%b, min_D0=%b, sec_D1=%b, sec_D0=%b",
                     display_min_D1, display_min_D0, display_sec_D1, display_sec_D0);

            if (display_min_D1 === expected_display_min_D1 &&
                display_min_D0 === expected_display_min_D0 &&
                display_sec_D1 === expected_display_sec_D1 &&
                display_sec_D0 === expected_display_sec_D0) begin
                $display("Result: PASS\n");
            end else begin
                $display("Result: FAIL\n");
            end
        end
    endtask

    // Stimulus
    initial begin
        // Initialize inputs
        minutes = 0; seconds = 0; temp_data = 0; mode = 2'b00; sel = 3'b000;

        // Wait for reset period
        #10;

        // Test case 1: minutes = 12, seconds = 34
        minutes = 6'd12; seconds = 6'd34; #10;
        check_output(7'b1111001, 7'b0100100, 7'b0110000, 7'b0011001); // Expected 12:34

        // Test case 2: minutes = 45, seconds = 59
        minutes = 6'd45; seconds = 6'd59; #10;
        check_output(7'b0011001, 7'b0010010, 7'b0010010, 7'b0010000); // Expected 45:59

        // Test case 3: edge case minutes = 0, seconds = 0
        minutes = 6'd0; seconds = 6'd0; #10;
        check_output(7'b1000000, 7'b1000000, 7'b1000000, 7'b1000000); // Expected 00:00

        $display("All test cases completed");
        $stop;
    end
endmodule
*/

module tb_display_7seg_timer(
    );
    
reg [3:0] min_D1, min_D0, sec_D1, sec_D0;
wire [6:0] display_min_D1, display_min_D0, display_sec_D1, display_sec_D0;

display_7seg_timer GEN(min_D1, min_D0, sec_D1, sec_D0, display_min_D1, display_min_D0, display_sec_D1, display_sec_D0);

initial begin
min_D1 = 4'b0000;
min_D0 = 4'b0001;
sec_D1 = 4'b0001;
sec_D0 = 4'b0000;
#20
min_D1 = 4'b0000;
min_D0 = 4'b0001;
sec_D1 = 4'b0000;
sec_D0 = 4'b1001;
#20
min_D1 = 4'b0000;
min_D0 = 4'b0001;
sec_D1 = 4'b0000;
sec_D0 = 4'b1000;
#20
min_D1 = 4'b0000;
min_D0 = 4'b0001;
sec_D1 = 4'b0000;
sec_D0 = 4'b0111;
#20
min_D1 = 4'b0000;
min_D0 = 4'b0001;
sec_D1 = 4'b0000;
sec_D0 = 4'b0110;
#20
min_D1 = 4'b0000;
min_D0 = 4'b0001;
sec_D1 = 4'b0000;
sec_D0 = 4'b0101;
#20
min_D1 = 4'b0000;
min_D0 = 4'b0001;
sec_D1 = 4'b0000;
sec_D0 = 4'b0100;
#20
min_D1 = 4'b0000;
min_D0 = 4'b0001;
sec_D1 = 4'b0000;
sec_D0 = 4'b0011;
#20
min_D1 = 4'b0000;
min_D0 = 4'b0001;
sec_D1 = 4'b0000;
sec_D0 = 4'b0010;
#20
min_D1 = 4'b0000;
min_D0 = 4'b0001;
sec_D1 = 4'b0000;
sec_D0 = 4'b0001;
#20
min_D1 = 4'b0000;
min_D0 = 4'b0001;
sec_D1 = 4'b0000;
sec_D0 = 4'b0000;
#20
min_D1 = 4'b0000;
min_D0 = 4'b0000;
sec_D1 = 4'b0101;
sec_D0 = 4'b1001;
#20
min_D1 = 4'b0000;
min_D0 = 4'b0000;
sec_D1 = 4'b0101;
sec_D0 = 4'b1000;
#20
$finish;
end    

endmodule



/*{min_D1_tb, min_D0_tb, sec_D1_tb, sec_D0_tb} = 0;
    for (i=0; i<32; i = i+1) begin
        {min_D1_tb, min_D0_tb, sec_D1_tb, sec_D0_tb} = i;
        display_min_D1_tb = 0;
        display_min_D0_tb = 0;
        display_sec_D1_tb = 0; 
        display_sec_D0_tb = 0;
        #period;
        
        if(display_min_D1 = expected_display_min_D1) begin
            $display(min_D0, min_D1, sec_D1, sec_D0, "passed.");
        end
        else begin
            $display(min_D0, min_D1, sec_D1, sec_D0, "failed.");
        end
    end*/




