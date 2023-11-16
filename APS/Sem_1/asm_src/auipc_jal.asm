.align 2
.text

addi t1, x0, 6
addi t2, x0, 7
jal plus
jalr x1, x1, 16
addi t4, x0, 10
sw t4, 8(x0)
beq x0, x0, end

 
substract:
	sub t3, t2, t1
	sw t3, 4(x0)
	ret 


plus:
	add t3, t1, t2
	sw t3, 0(x0)
	ret
	
end:
auipc t5, 5
sw t5, 12(x0)