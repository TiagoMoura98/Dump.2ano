.section .data

.section .note.GNU-stack,"",@progbits

.section .text
	.global extract_data

# %rdi = char* str
# %rsi = char* token
# %rdx = char* unit
# %rcx = int* value

extract_data:      
	movl $0, (%rcx) 							# set value to 0
	movb $0, (%rdx)								# set unit to empty string
	movq %rcx, %r9								# store int pointer in r9 to use ecx in loop
	movl $2, %ecx								# setup loop for extraction of 2 digits
	movl $0, %r8d								# setup temp variable for storing value
	movq %rsi, %r10								# store beginning of token
	
compare_tokens:
	movb (%rsi), %al 							# extract char from token string
	cmpb $0, %al 								# when token end is reached check if there is a match
	je check_match
	movb (%rdi), %r11b 							# extract char from str
	cmpb $0, %r11b								# check if end of str
	je error
	cmpb %al, %r11b 								# compare the 2 chars
	jne restart_pointer 		
	incq %rsi									# increment pointer to next char from token
	incq %rdi									# increment pointer to next char from str
	jmp compare_tokens

find_next_token:	
	incq %rdi 									# increment pointer to next char from str
	movb (%rdi), %al							# copy char from str (pointed by rdi) to al
	cmpb $0, %al	 							# check if end of input string is reached
	je error 
	cmpb $'#', %al 								# check if it is "#"
	je compare_next_token
	jmp find_next_token

compare_next_token:
    incq %rdi									# skip "#"
    jmp compare_tokens

restart_pointer:
	movq %r10, %rsi								# restart token pointer
	jmp find_next_token
    
check_match:
	movb (%rdi), %r11b							# extract char from str
	cmpb $'&', %r11b								# check if it is "&"
	jne restart_pointer
	
find_unit:
	incq %rdi									# increment pointer to next char from str
	movb (%rdi), %al							# extract char from str
	cmpb $0, %al								# check if end of string
	je error
	cmpb $':', %al 								# check if it is ":"	
    je store_unit    
    jmp find_unit
    
store_unit:
	incq %rdi									# increment pointer to next char from str
	movb (%rdi), %al                            # extract char from str
	cmpb $0, %al								# check if end of str
	je error
	cmpb $'&', %al								# check if it is "&"
	je end_string	
	movb %al, (%rdx)							# copy char from input string to output string
	incq %rdx									# increment pointer to next char from output string
	jmp store_unit

end_string:
	movb $0, (%rdx)								# end unit string
		
find_value:
	incq %rdi									# increment pointer to next char from str
	movb (%rdi), %al							# extract char from str
	cmpb $0, %al								# check if end of str
	je error		
	cmpb $':', %al 								# check if it is ":"	
    je extract_value            
	jmp find_value

extract_value:
	incq %rdi									# skip current char
	movb (%rdi), %r11b							# extract first digit
	cmpb $0, %r11b								# check if end of string
	je error	
	sub $'0', %r11b                             # convert from ascii to int
	movsbl %r11b, %r11d							# extend sign and store in 32 bits
	imull $10, %r8d								# multiply current value by 10
	addl %r11d, %r8d								# add found digit to value
	loop extract_value
	
store_value:
	movl %r8d, (%r9)                           	# store value in memory
	movl $1, %eax								# update return flag 
	jmp end_function						
	
error:
	movl $0, (%r9) 								# set value to 0
	movb $0, (%rdx)								# set unit to empty string
	movl $0, %eax                               # update return flag
	
end_function:
	ret
