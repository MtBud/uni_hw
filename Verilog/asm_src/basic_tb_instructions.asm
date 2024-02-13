.data
.align 2
.text

addi x1, x0, 8
addi x2, x0, 6
addi x5, x0, 8
addi x6, x0, 2
addi x7, x0, 1

add x3, x1, x2
sw x3, 4(x0)

sub x3, x1, x2
sw x3, 8(x0)
sub x3, x1, x5
sw x3, 12(x0)

and x3, x1, x2
sw x3, 16(x0)

slt x4, x1, x2
sw x4, 20(x0)
slt x4, x2, x1
sw x4, 24(x0)
slt x4, x1, x5
sw x4, 28(x0)

div x3, x1, x6
sw x3, 32(x0)

rem x3, x1, x6
sw x3, 36(x0)
rem x3, x1, x2
sw x3, 40(x0)

sll x3, x1, x7
sw x3, 44(x0)
srl x3, x1, x7
sw x3, 48(x0)
sra x3, x1, x7
sw x3, 52(x0)

lui x3, 1
sw x3, 56(x0)