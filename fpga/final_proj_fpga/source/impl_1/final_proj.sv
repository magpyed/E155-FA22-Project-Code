///MICRO Ps final project
// Limnanthes Serafini & Noah Limpert
// 

module fin_proj_top(
	input logic reset,
	input logic done,
	input logic[3:0] cols,
	output logic[3:0] rows, 
	output logic[3:0] leds,
	output logic crct_pwd,
	output logic incrct_pwd
);
	logic int_osc;
	HSOSC #(.CLKHF_DIV(2'b10))
		hf_osc (.CLKHFPU(1'b1), .CLKHFEN(1'b1), .CLKHF(int_osc));

	main_fsm fsm(int_osc, reset, done, cols, rows, leds, crct_pwd, incrct_pwd);
endmodule


module main_fsm(
	input logic clk,
	input logic reset,
	input logic done,
	input logic[3:0] cols,
	output logic[3:0] rows,
	output logic[3:0] leds,
	output logic crct_pwd,
	output logic incrct_pwd
	);
	//we're reading cols and writing to rows.
	
	typedef enum logic [2:0] {scan, w1, pressed, stillpressed, w2} statetype; 
	statetype state, nextstate;
	logic [3:0] num1, num2, num3, num4; //pressed buttons
	logic [3:0] nextnum; 
	logic [3:0] synched_cols;
	logic [3:0] synched_rows; 
	fourbit_synchronizer synch(clk, cols, synched_cols);
	fourbit_synchronizer synchmatch(clk, rows, synched_rows);
	// no switch bounce on those, but need it to line up with the cols for decoding
	assign buttonpress = !(&synched_cols);
	// the buttons are active-low
	logic [31:0] counter;
	logic [31:0] threshold;
	assign threshold = 1000000; // changed t0 5 from 1000000
	
	//password stored here
	logic [3:0] pwd1, pwd2, pwd3, pwd4;
	assign pwd1 = 4'd1;
	assign pwd2 = 4'd2;
	assign pwd3 = 4'd3;
	assign pwd4 = 4'd4;
	
	logic [3:0] numpressed; // changed from 2:0. for some reason it was acting like a signed number so 3 + 1 = -4
	
	always_ff @(posedge clk) begin
		if (reset == 0 || done == 0) begin
			state <= scan;
			num1 <= 4'b0000;
			num2 <= 4'b0000;
			num3 <= 4'b0000;
			num4 <= 4'b0000;
			rows <= 4'b1110;
			counter <= 0;
			numpressed <= 0;
			if (reset == 0) leds <= 4'b0000;
		end
		else begin
			state <= nextstate;
			if (state == scan) begin
				rows <= {rows[2:0], rows[3]};
			end
			if (state == pressed && buttonpress) begin
				num1 <= num2;
				num2 <= num3;
				num3 <= num4;
				num4 <= nextnum;
				numpressed <= numpressed + 1'b1;
			end
			if (state == w1 || state == w2) begin
				if (counter != threshold) counter <= counter + 1;
			end else counter <= 0;
			if (state == stillpressed) rows <= 4'b0000;
			if (state == w2) rows <= 4'b1110;
			
			if (numpressed == 4) begin
				leds[0] <= (pwd1 != num1);
				leds[1] <= (pwd2 != num2);
				leds[2] <= (pwd3 != num3);
				leds[3] <= (pwd4 != num4);
			end
		end
	end
	

	
	always_comb
		case(state)
			scan: 
				if (buttonpress && (numpressed < 4)) nextstate = w1;
				else nextstate = scan;
			w1: 
				if (counter == threshold) nextstate = pressed;
				else nextstate = w1;
			pressed: 
				if (buttonpress) nextstate = stillpressed;
				else nextstate = w2;
			stillpressed: 
				if (buttonpress) nextstate = stillpressed;
				else nextstate = w2;
			w2: 
				if (counter == threshold) nextstate = scan;
				else nextstate = w2;
			default: nextstate = state;
		endcase
		
	matrix_decoder newnum({synched_rows, synched_cols}, nextnum);
	
	assign crct_pwd = ((numpressed == 4'd4) && ({pwd1,pwd2,pwd3,pwd4} == {num1,num2,num3,num4}));
	assign incrct_pwd = ((numpressed == 4'd4) && !crct_pwd); 
			
endmodule


module fourbit_synchronizer(
	input logic clk,
	input logic [3:0] unsynch,
	output logic [3:0] synch
);
	logic [3:0] mid1, mid2;
	always_ff @(posedge clk) begin
		mid1 <= unsynch;
		mid2 <= mid1;
		synch <= mid2;
	end
endmodule

module matrix_decoder(
	input logic [7:0] rowscols,
	output logic [3:0] num
);
	always_comb 
		casez(rowscols)
			8'b1110_0???: num = 4'd14;
			8'b1110_?0??: num = 4'd0;
			8'b1110_??0?: num = 4'd15;
			8'b1110_???0: num = 4'd13;
			8'b1101_0???: num = 4'd7;
			8'b1101_?0??: num = 4'd8;
			8'b1101_??0?: num = 4'd9;
			8'b1101_???0: num = 4'd12;
			8'b1011_0???: num = 4'd4;
			8'b1011_?0??: num = 4'd5;
			8'b1011_??0?: num = 4'd6;
			8'b1011_???0: num = 4'd11;
			8'b0111_0???: num = 4'd1;
			8'b0111_?0??: num = 4'd2;
			8'b0111_??0?: num = 4'd3;
			8'b0111_???0: num = 4'd10;
			8'b????_1111: num = 4'd3;
			default: num = 4'd15;
		endcase

endmodule


