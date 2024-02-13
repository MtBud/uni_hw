module tb();
    reg [31:0] srcA, srcB;
	reg [3:0] ALUControl;
    wire Zero, LesserThan;
    wire [31:0] ALUOut;

    ALU alu1( srcA, srcB, ALUControl, Zero, LesserThan, ALUOut );

    initial begin
        $dumpfile ("alu_test");
        $dumpvars;
        srcA = 6;
        srcB = 3;
        ALUControl = 0;
        #10;

        ALUControl = 1;
        #5;
        srcA = 0;
        srcB = 0;
        #5;
        srcA = 1;
        srcB = 3;
        #10;

        ALUControl = 2;
        srcA = 6;
        srcA = 3;
        #10;

        ALUControl = 3;
        #5;
        srcA = 3;
        srcB = 6;
        #5;
        srcA = 6;
        #10;

        ALUControl = 4;
        srcA = 6;
        srcB = 3;
        #10;

        ALUControl = 5;
        #5;
        srcA = 5;
        srcB = 3;
        #10;

        ALUControl = 6;
        srcA = 4;
        srcB = 1;
        #5;
        srcB = 2;
        #10;

        ALUControl = 7;
        srcA = 10;
        srcB = 1;
        #5;
        srcB = 2;
        #5;
        srcA = -7;
        srcB = 1;
        #10;

        ALUControl = 8;
        srcA = 10;
        srcB = 1;
        #5;
        srcB = 1;
        #5;
        srcA = -7;
        srcB = 1;
        #10;

        ALUControl = 9;
        srcA = 24;
        srcB = 7;
        #10;

    end

endmodule