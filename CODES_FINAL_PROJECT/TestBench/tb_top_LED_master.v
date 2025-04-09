`timescale 1ns / 1ps

module tb_top_LED_master;

    // Inputs
    reg sys_clk, sys_rst;
    reg mini_rst, start, idle;
    reg [1:0] mode;

    // Outputs
    wire [15:0] LED;

    // Instantiate the Device Under Test (DUT)
    top_LED_master uut (
        .sys_clk(sys_clk),
        .sys_rst(sys_rst),
        .mini_rst(mini_rst),
        .start(start),
        .idle(idle),
        .mode(mode),
        .LED(LED)
    );

    // Clock generation
    initial sys_clk = 0;
    always #5 sys_clk = ~sys_clk;  // 10ns clock period (100MHz)

    // Testbench logic
    initial begin
        // Initialize inputs
        sys_rst = 1;
        mini_rst = 1;
        start = 0;
        idle = 0;
        mode = 2'b00;

        // Hold reset for a few cycles
        #20 sys_rst = 0;
        mini_rst = 0;

        // Test case 1: Low mode, start is active
        start = 1;
        mode = 2'b01; // Low mode
        #50;  // Wait for the LEDs to update
        check_output(LED, 16'b0000000000000000, "Test case 1: Low mode, start active");

        // Test case 2: Normal mode
        mode = 2'b10; // Normal mode
        #50;
        check_output(LED, 16'b0000000000000000, "Test case 2: Normal mode");

        // Test case 3: High mode
        mode = 2'b11; // High mode
        #50;
        check_output(LED, 16'b0000000000000000, "Test case 3: High mode");

        // Test case 4: Idle
        start = 0;
        idle = 1;
        #50;
        check_output(LED, 16'b0000000000000111, "Test case 4: Idle");

        // Test case 5: Counter-driven decoding
        start = 1;
        idle = 0;
        mode = 2'b00; // Mode shouldn't affect counter decoding
        #500;  // Let the counter go through a few cycles
        // Manually verify a subset of counter results if needed

        // End simulation
        $finish;
    end

    // Task to check output
    task check_output(input [15:0] actual, expected, input [127:0] test_case);
        if (actual !== expected)
            $display("FAIL: %s. Expected: %b, Got: %b", test_case, expected, actual);
        else
            $display("PASS: %s", test_case);
    endtask

endmodule
