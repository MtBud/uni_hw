.data
.align 2

size:
.word 22

array:
.word 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 990, 991, 992, 993, 994, 995, 996, 997, 998, 999, 1000

.text
#---------------------------------------------------------------------------------------------
# main
lw s0, 8(x0)		# s0 points and iterates through the array
lw s1, 4(x0)		# s1 holds the array size
addi s2, zero, 0	# s2 is the array iterator

main_loop:
	beq s1, s2, end		# break the loop if the array has been iterated through
	lw a0, 0(s0)
	jal prime
	sw a0, 0(s0)
	addi s2, s2, 1		# increment one
	addi s0, s0, 4
	j main_loop
	

# function checks if number at a0 is prime
# return 1 if yes, 0 if no
prime:
	addi t0, zero, 2	# t0 is an iterator 
	blt a0, t0, prime_0	# if the number is smaller than 2, it is automatically not prime
	beq a0, t0, prime_1	# if the number is 2, it is prime
	prime_loop:
		rem t1, a0, t0
		beq t1, zero, prime_0	# if modulo of a0 and t0 is 0, number is not prime
		addi t0, t0, 1		# increment one
		beq a0, t0, prime_1	# end the cycle -- number is prime
		j prime_loop
	
	
	prime_0:
		addi a0, zero, 0
		ret
		
	prime_1:
		addi a0, zero, 1
		ret

end:
beq x0, x0, end