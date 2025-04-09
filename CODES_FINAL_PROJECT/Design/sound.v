//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/18/2024 12:25:26 AM
// Design Name: 
// Module Name: MicrowaveBuzzer
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

module sound(
    input clk,           // 100 MHz clock input
    input enable,        // Enable signal to start the buzzer
    output reg buzzer    // Output signal to external buzzer
);

    reg [26:0] counter;  // Counter for clock division
    reg [26:0] timer;    // Timer for 0.5-second intervals
    reg clk_2hz;         // 2 Hz clock signal
    
    integer state = 0;   // State machine variable
    integer count = 0;   // Cycle counter

    // Clock divider to generate a 2 Hz clock from the 100 MHz clock
    always @(posedge clk) begin
        if (counter == 100_000_000 / 4 - 1) begin
            counter <= 0;
            clk_2hz <= ~clk_2hz;
        end else begin
            counter <= counter + 1;
        end
    end

    // State machine to control the buzzer
    always @(posedge clk_2hz) begin
        if (enable) begin
            case (state)
                0: begin
                    // Initial state, buzzer off
                    buzzer <= 0;
                    timer <= 0;
                    count <= 0;
                    state <= 1;
                end
                1: begin
                    // Buzzer on for 0.5 seconds
                    if (timer < 1) begin
                        buzzer <= 1;
                        timer <= timer + 1;
                    end else begin
                        timer <= 0;
                        buzzer <= 0;
                        state <= 2;
                        count <= count + 1;
                    end
                end
                2: begin
                    // Buzzer off for 0.5 seconds
                    if (timer < 1) begin
                        timer <= timer + 1;
                    end else if (count == 3) begin
                        timer <= 0;
                        state <= 3;
                    end else begin
                        timer <= 0;
                        state <= 1;
                    end
                end
                3: begin
                    // After 3 cycles, buzzer stays off
                    buzzer <= 0;
                    state <= 4;
                end
                default: begin
                    state <= 3;
                end
            endcase
        end else begin
            // If not enabled, keep the buzzer off
            buzzer <= 0;
            state <= 0;
            timer <= 0;
            count <= 0;
        end
    end
endmodule


/*module sound(
    input clk,            // 100 MHz clock input
    input enable,         // Enable signal to start the buzzer
    output reg buzzer     // Output signal to external passive buzzer
);

    reg [16:0] tone_counter; // Counter for square wave generation
    reg [23:0] duration_counter; // Counter for buzzer duration
    reg tone;                // Square wave output for sound
    reg active;              // Internal flag to indicate buzzer activity

    // Parameters for sound configuration
    localparam TONE_DIVIDER = 50_000;  // Tone frequency (2 kHz)
    localparam DURATION = 100_000_000; // Duration for sound (~1 second)

    always @(posedge clk) begin
        if (enable && !active) begin
            // Start the buzzer when enabled
            active <= 1;
            duration_counter <= DURATION;
        end else if (active) begin
            if (duration_counter > 0) begin
                duration_counter <= duration_counter - 1;

                // Generate square wave for the buzzer
                if (tone_counter == TONE_DIVIDER - 1) begin
                    tone_counter <= 0;
                    tone <= ~tone;
                end else begin
                    tone_counter <= tone_counter + 1;
                end

                buzzer <= tone; // Drive the buzzer
            end else begin
                // Stop the buzzer when the duration ends
                active <= 0;
                buzzer <= 0;
            end
        end else begin
            // Default state when inactive
            tone_counter <= 0;
            buzzer <= 0;
        end
    end
endmodule
*/


