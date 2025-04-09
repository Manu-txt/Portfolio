module top_master(
    input sys_clk,
    input sys_rst,
    input btn_up,
    input btn_dwn,
    input btn_start,
    input pizza,
    input potato,
    input [5:0] load_minutes,    // Input for preset minutes
    input [5:0] load_seconds,    // Input for preset seconds
    output [7:0] AN,             // 7-segment anodes
    output [6:0] display,        // 7-segment segments
    output DP,                   // Decimal point
    output [15:0] LED,           // LED output
    output start,
    output idle,
    inout temp_SDA,          // i2c sda on temp sensor - bidirectional
    output temp_SCL,          // i2c scl on temp sensor
    output wire buzzer             // Beep signal
);

    // Internal Signals
    wire timer_done;
    wire [1:0] modes;
    wire timerEnd;
    wire s;
    wire i;
    wire [5:0] minutes, seconds;
    wire sda_dir;                   // direction of SDA signal - to or from master
    wire w_200kHz;                  // 200kHz SCL
    wire [7:0] temp_data;           // 8 bits of temperature data

    assign start = s;
    assign idle = i;
    assign timer_done = (timerEnd == 1 && minutes == 6'd0 && seconds == 6'd0);  //when timer is done, the sound will be activated too notice that heating is complete
    
    // Top Button and LED Master
    top_btn_LED_master btnLEDMaster (
        .sys_clk(sys_clk), 
        .sys_rst(sys_rst),
        .mini_rst(sys_rst),
        .timerEnd(timerEnd),
        .btn_up(btn_up),
        .btn_dwn(btn_dwn),
        .btn_start(btn_start),
        .LED(LED),
        .mode(modes),
        .start(s),
        .idle(i)
    );

    // Top 7-Segment Display Master
    top_7seg_master ssegMaster (
        .sys_clk(sys_clk),
        .mode(modes),
        .rst(sys_rst),
        .load_minutes(load_minutes), 
        .load_seconds(load_seconds),
        .start(s),
        .idle(i),
        .AN(AN),
        .display(display),
        .DP(DP),
        .timerEnd(timerEnd),
        .pizza(pizza),
        .potato(potato),
        .clk_100MHz(sys_clk),
        .temp_data(temp_data)
    );

    // Beep Sound Generator
    sound beep_generator (
        .clk(sys_clk),
        .enable(timer_done),   // Enable beep when timer reaches 0:00
        .buzzer(buzzer)
    );
    
    
    // Instantiate i2c master
    Temperature temp(
        .clk_200kHz(w_200kHz),
        .reset(reset),
        .temp(temp_data),
        .SDA(temp_SDA),
        .SDA_dir(sda_dir),
        .SCL(temp_SCL)
    );
    
    // Instantiate 200kHz clock generator
    Clk200kHzGenerator cgen(
        .clk_100MHz(sys_clk),
        .clk_200kHz(w_200kHz)
    );
    
endmodule