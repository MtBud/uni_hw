.data
.align 2

size:
.word 22

array:
.word 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 990, 991, 992, 993, 994, 995, 996, 997, 998, 999, 1000

.text
#---------------------------------------------------------------------------------------------
start:
	
	

# function checks if number at a0 is prime
# return 1 if yes, 0 if no
prime:
	addi t0, zero, 2	# t0 is an iterator 
	blt a0, t0, prime_0	# if the number is smaller than 2, it is automatically not prime
	prime_loop:
		rem t1, a0, t0
		beq t1, zero, prime_1	# if modulo of a0 and t0 is 0, number is prime
		beq a0, t1, prime_0	# end the cycle -- number is not prime
		addi t0, t0, 1		# increment one
		j prime_loop
	
	
	prime_0:
		addi a0, zero, 0
		ret
		
	prime_1:
		addi a0, zero, 1
		ret