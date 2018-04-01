section .data                    	; data section, read-write
        an:    DQ 0              	; this is a temporary var

section .text                    	; our code is always in the .text section
        global add_func          	    ; makes the function appear in global scope
        extern printf            	; tell linker that printf is defined elsewhere 							; (not used in the program)

add_func:                        	    ; functions are defined as labels
        push    rbp              	; save Base Pointer (bp) original value
        mov     rbp, rsp         	; use base pointer to access stack contents
        mov rcx, rdi			    ; get function argument

;;;;;;;;;;;;;;;; FUNCTION EFFECTIVE CODE STARTS HERE ;;;;;;;;;;;;;;;; 

	mov	qword [an], 0		; initialize answer
	label_here:
                
            
                
                
   		end:

		inc rcx      	    ; increment pointer
		cmp byte [rcx], 0   ; check if byte pointed to is zero
		jnz label_here      ; keep looping until it is null terminated


;;;;;;;;;;;;;;;; FUNCTION EFFECTIVE CODE ENDS HERE ;;;;;;;;;;;;;;;; 

         mov     rax,[an]         ; return an (returned values are in rax)
         mov     rsp, rbp
         pop     rbp
         ret 
		 
   
