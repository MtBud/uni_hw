module tb();
    reg [24:0] instruction;
    reg [3:0] immControl;
    wire [31:0] out;
    imm_control immCtrl( instruction, immControl, out);

    initial begin
        $dumpfile ("immCtrl_test");
        $dumpvars;

        immControl = 0;
        instruction = 
    end
endmodule