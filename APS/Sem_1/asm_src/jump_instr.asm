.align 2
.text

addi x1, x0, 5
addi x2, x0, 5
beq x1, x2, ifeq
	addi x10, x0, 10
	
	
ifeq:
addi x10, x0, 5

sw x10, 0(x0)

addi x2, x0, 6
addi x3, x0, 1
blt x2, x1, ifblt
addi x3, x0, 2
blt x1, x2, ifblt
addi x3, x0, 3

ifblt:
sw x3, 4(x0)

lw x4, 0(x0)
lw x5, 4(x0)

add x6, x4, x5
sw x6, 8(x0)