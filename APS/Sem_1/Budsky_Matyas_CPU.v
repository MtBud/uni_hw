`default_nettype none
module processor( input         clk, reset,
                  output [31:0] PC,
                  input  [31:0] instruction,
                  output        WE,
                  output [31:0] address_to_mem,
                  output [31:0] data_to_mem,
                  input  [31:0] data_from_mem
                );
	// declare wiring			
    wire [31:0] rs1, rs2, ALUOut, ImmOp, srcB, BranchTarget, BrTargetSrc, PCn, PCPlus4, res, BrJalxMuxOut;
	wire BranchBeq, BranchBlt, BranchJal, BranchJalr, BranchAuiPC,
		 RegWrite, MemWrite,
		 Zero, Lst,
		 BranchOutcome, ALUSrc, BrTargetSelect, BranchJalx, MemToReg;
	wire [3:0] immControl, ALUControl;

	assign WE = MemWrite;
	assign data_to_mem = rs2;
	assign address_to_mem = ALUOut; 	 
	

	// declare main cpu components
	PC_reg PCReg1 (PCn, clk, PC);
	register_file registerFile1(instruction[19:15], instruction[24:20], instruction[11:7],
								 clk, RegWrite, res, rs1, rs2 );

	imm_control immDecoder1(instruction[31:7], immControl, ImmOp);

	ALU ALU1(rs1, srcB, ALUControl, Zero, Lst, ALUOut);

	control_unit controlUnit1( 
					instruction,
					BranchBeq,
		     	    BranchBlt,
		     	    BranchJal,
		     	    BranchJalr,
					BranchAuiPC,
		     	    RegWrite,
		     	    MemToReg,
		     	    MemWrite,
					ALUControl,
					ALUSrc,
					immControl);

	// declare multiplexors
	mux_2_1 mux_BrOutcome (PCPlus4, BranchTarget, BranchOutcome, PCn);
	mux_2_1 mux_ALUSrc (rs2, ImmOp, ALUSrc, srcB);
	mux_2_1 mux_BrTarget (BrTargetSrc, ALUOut, BrTargetSelect, BranchTarget);
	mux_2_1 mux_BrJalx (ALUOut, PCPlus4, BranchJalx, BrJalxMuxOut);
	mux_2_1 mux_MemToReg (BrJalxMuxOut, data_from_mem, MemToReg, res);

	// declare adders
	adder_32bit adder_PC4 (4, PC, PCPlus4);
	adder_32bit adder_PCImm (ImmOp, PC, BrTargetSrc);

	// adittional logic gates for resolving branching
	assign BranchJalx = BranchJal || BranchJalr;
	assign BrTargetSelect = BranchJalr || BranchAuiPC;
	assign BranchOutcome = BranchJalx || BrTargetSelect || (BranchBeq && Zero) || (BranchBlt && Lst);
	

endmodule

module PC_reg(  input [31:0] PCn,
				input clk,
			    output [31:0] PC
			);
	reg [31:0] PCregister;

	assign PC = PCregister;
	always @ (posedge clk) PCregister = PCn;
	
endmodule

module adder_32bit( input [31:0] num1, num2,
	      	    output [31:0] sum
	    	  ); 	  
	assign sum = num1 + num2;
endmodule



module mux_2_1( input [31:0] in0, in1,
		input select,
		output [31:0] out
	      );
	assign out = select?in1:in0;
endmodule
	 

module register_file( input [4:0] a1, a2, a3,
		      input clk, we3,
		      input [31:0] wd3,
		      output reg [31:0] rd1, rd2
		    );  
	reg [31:0] rf[31:0];
	always @(*) begin
		rd1 = rf[a1];
		rd2 = rf[a2];
	end
	always @ (posedge clk) begin
		if(we3) rf[a3] = wd3;
	end		
endmodule

// I - 0, S - 1, B - 2, U - 3 (upper immediate), J - 4
module imm_control( input [24:0] instruction,
		    input [3:0] immControl,
		    output reg [31:0] out
		  );
	always @(*) begin
		case(immControl)
			// I-type
			0:begin
				out[10:0] = instruction[23:12];
				out[31:11] = instruction[24];
			  end
			// S-type  
			1:begin
				out[4:0] = instruction[4:0];
				out[10:5] = instruction[23:18];
				out[31:11] = instruction[24];
			  end  
			// B-type
			2:begin
				out[0] = 0;
				out[4:1] = instruction[4:1];
				out[10:5] = instruction[23:18];
				out[11] = instruction[0];
				out[31:12] = instruction[24];
			  end   
			// U-type
			3:begin
				out[11:0] = 0;
				out[31:12] = instruction[24:5] * 4096;
			  end
			// J-type
			4:begin
				out[0] = 0;
				out[10:1] = instruction[23:12];
				out[11] = instruction[11];
				out[19:12] = instruction[10:5];
				out[31:20] = instruction[24];

			end

		endcase
	end
		  
endmodule

// add - 0, and - 1, sub - 2, slt - 3, div - 4, rem - 5, sll - 6, srl - 7, sra - 8, lui - 9
module ALU( input [31:0] srcA, srcB,
	    input [3:0] ALUControl,
	    output reg Zero, LesserThan,
	    output reg [31:0] ALUOut
	  );
	always @(*) begin
		case(ALUControl)
			0: ALUOut = srcA + srcB;
			1: ALUOut = srcA & srcB;
			2: ALUOut = srcA - srcB;
			3:begin
				ALUOut[0] = srcA < srcB;
				ALUOut[31:1] = 0;
				if( ALUOut[0] )
					LesserThan = 1;
				else
					LesserThan = 0;	
			  end	
			4: ALUOut = srcA / srcB;
			5: ALUOut = srcA % srcB;
			6: ALUOut = srcA << srcB;
			7: ALUOut = srcA >> srcB;
			8: ALUOut = srcA >>> srcB;
			9: ALUOut = srcB;
		endcase
	end
	
	always @(ALUOut) begin
		if(ALUOut == 0) Zero = 1;
		else Zero = 0;
	end

endmodule


module control_unit( input [31:0] instruction,
		     output reg BranchBeq,
		     	    BranchBlt,
		     	    BranchJal,
		     	    BranchJalr,
					BranchAuiPC,
		     	    RegWrite,
		     	    MemToReg,
		     	    MemWrite,
		     output reg [3:0] ALUControl,
		     output reg ALUSrc,
		     output reg [3:0] immControl
		   );
	always @(*) begin
		case(instruction[6:0])
			// all ALU reliant operations without immediate
			51:begin
				BranchBeq = 0;
				BranchBlt = 0;
				BranchJal = 0;
				BranchJalr = 0;
				BranchAuiPC = 0;
				RegWrite = 1;
				MemToReg = 0;
				MemWrite = 0;
				ALUSrc = 0;
				case(instruction[14:12])
					0:begin
						case(instruction[31:25])
							// add
							0: ALUControl = 0;
							// sub
							32: ALUControl = 2;
						endcase
					end
					// and
					7: ALUControl = 1;
					// slt
					2: ALUControl = 3;
					// div
					4: ALUControl = 4;
					// rem
					6: ALUControl = 5;
					// sll
					1: ALUControl = 6;
					
					// right shifts
					5:begin
						case(instruction[31:25])
							// srl
							0: ALUControl = 7;
							// sra
							32: ALUControl = 8;
						endcase
					end
				endcase
			end
			
			// addi
			19:begin
				BranchBeq = 0;
				BranchBlt = 0;
				BranchAuiPC = 0;
     	    	BranchJal = 0;
     	    	BranchJalr = 0;
     	    	RegWrite = 1;
		     	MemToReg = 0;
		     	MemWrite = 0;
				ALUControl = 0;
				immControl = 0;
				ALUSrc = 1;
			end
			
			// branch operations
			99:begin
				BranchJal = 0;
				BranchJalr = 0;
				BranchAuiPC = 0;				
				RegWrite = 0;
				MemToReg = 0;
				MemWrite = 0;
				immControl = 2;
				ALUSrc = 0;
				case(instruction[14:12])
					// beq
					0:begin
						BranchBeq = 1;
						BranchBlt = 0;
						ALUControl = 2;
					end
					
					// blt
					4:begin
						BranchBeq = 0;
						BranchBlt = 1;
						ALUControl = 3;
					end
				endcase	
			end
			
			// lw
			3:begin
				BranchBeq = 0;
				BranchBlt = 0;
				BranchJal = 0;
				BranchJalr = 0;
				BranchAuiPC = 0;
				RegWrite = 1;
				MemToReg = 1;
				MemWrite = 0;
				ALUControl = 0;
				immControl = 0;
				ALUSrc = 1;
			end
			
			// sw
			35:begin
				BranchBeq = 0;
				BranchBlt = 0;
				BranchJal = 0;
				BranchJalr = 0;
				BranchAuiPC = 0;
				RegWrite = 0;
				MemToReg = 0;
				MemWrite = 1;
				ALUControl = 0;
				immControl = 1;
				ALUSrc = 1;
			end
			
			// lui
			55:begin
				BranchBeq = 0;
				BranchBlt = 0;
				BranchJal = 0;
				BranchJalr = 0;
				BranchAuiPC = 0;
				RegWrite = 1;
				MemToReg = 0;
				MemWrite = 0;
				ALUControl = 9;
				immControl = 3;
				ALUSrc = 1;
			end
			
			// jal
			111:begin
				BranchBeq = 0;
				BranchBlt = 0;
				BranchJal = 1;
				BranchJalr = 0;
				BranchAuiPC = 0;
				RegWrite = 1;
				MemToReg = 0;
				MemWrite = 0;
				immControl = 4;
			end
			
			// jalr
			103:begin
				BranchBeq = 0;
				BranchBlt = 0;
				BranchJal = 0;
				BranchJalr = 1;
				BranchAuiPC = 0;
				RegWrite = 1;
				MemToReg = 0;
				MemWrite = 0;
				ALUControl = 0;
				immControl = 0;
				ALUSrc = 1;
			end
			
			// auipc
			23:begin
				BranchBeq = 0;
				BranchBlt = 0;
				BranchJal = 0;
				BranchJalr = 0;
				BranchAuiPC = 1;
				RegWrite = 0;
				MemToReg = 0;
				MemWrite = 0;
				immControl = 3;
				ALUSrc = 1;
			end
			    
		endcase
	end
endmodule
`default_nettype wire














