addi t0, zero, 0
addi t1, zero, 5
addi t2, zero, -5
sw t1, 0(t0)
addi t0, t0, 4
sw t2, 0(t0)


# slt
slt t3, t1, t2
addi t0, t0, 4
sw t3, 0(t0)
slt t3, t2, t1
addi t0, t0, 4
sw t3, 0(t0)
slt t3, t1, t1
addi t0, t0, 4
sw t3, 0(t0)

# div
addi t1, zero, 8
addi t2, zero, 2
addi t4, zero, -2
div t3, t1, t2
addi t0, t0, 4
sw t3, 0(t0)
div t3, t1, t4
addi t0, t0, 4
sw t3, 0(t0)

# rem
addi t4, zero, 3
rem t3, t1, t2
addi t0, t0, 4
sw t3, 0(t0)
rem t3, t1, t4
addi t0, t0, 4
sw t3, 0(t0)

# lui
lui t3, 4
addi t0, t0, 4
sw t3, 0(t0)
lui t3, -4
addi t0, t0, 4
sw t3, 0(t0)

# auipc
auipc t3, 4
addi t0, t0, 4
sw t3, 0(t0)
auipc t3, -4
addi t0, t0, 4
sw t3, 0(t0)

# sll
addi t2, zero, -8
addi t4, zero, 1
sll t3, t1, t4
addi t0, t0, 4
sw t3, 0(t0)
sll t3, t2, t4
addi t0, t0, 4
sw t3, 0(t0)

# srl
srl t3, t1, t4
addi t0, t0, 4
sw t3, 0(t0)
srl t3, t2, t4
addi t0, t0, 4
sw t3, 0(t0)

# sra
sra t3, t1, t4
addi t0, t0, 4
sw t3, 0(t0)
sra t3, t2, t4
addi t0, t0, 4
sw t3, 0(t0)

end:
beq zero, zero, end