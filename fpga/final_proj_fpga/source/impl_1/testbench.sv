`timescale 1ns/1ns

module testbench();
    logic clk;

	// declare inputs and outputs
	logic reset, done;
	logic[3:0] cols, rows, leds;
	logic crct_pwd, incrct_pwd;
	
    
    // device under test
    main_fsm dut(clk, reset, done, cols, rows, leds, crct_pwd, incrct_pwd);
    
    always begin
      clk = 1'b0; #5;
      clk = 1'b1; #5;
    end



    initial begin
      // Delay and then apply inputs
		#6
		reset = 0;
		#103
		reset = 1;

		#103;
		cols = 4'b1111;
		#103;
		cols = 4'b0111;

		#103;
		cols = 4'b1111;
		#103;
		cols = 4'b1011;

		#103;
		cols = 4'b1111;
		#103;
		cols = 4'b1101;

		#103;
		cols = 4'b1111;
		#103;
		cols = 4'b1110;
    end

endmodule