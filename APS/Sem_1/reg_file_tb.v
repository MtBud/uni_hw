module tb();
	reg [4:0] a1, a2, a3;
	reg clk, we3;
	reg [31:0] wd3;
	wire [31:0] rd1, rd2;
	
	register_file regFile( a1, a2, a3, clk, we3, wd3, rd1, rd2);
	
	initial begin
		$dumpfile ( " test " ); // soubor pro zapis vysledku
		$dumpvars ; // zapis vsech hodnot
		a1 =4; // pocatecni hodnota x
		a2 =0; // pocatecni hodnota y
		a3 =4; // pocatecni hodnota c_in
		clk =0;
		we3 =1;
		wd3 =5;
		# 10 $finish ; // pozdrzeni 1024 jednotek a konec
	end
	
	always@(clk) begin
        	clk = 1;
        	#1;
        	clk = 0;
        	#1;
    	end
    	
    	always #10 a1 = 4;
    	always #10 a3 = 4;
    	always #20 we3 = 1;
    	always #30 a2 = 4;
	
endmodule
