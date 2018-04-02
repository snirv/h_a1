section .data                    	; data section, read-write
        result:    DQ 0              	; this is a temporary var

section .text                    	; our code is always in the .text section
        global add_func  			; makes the function appear in global scope
		global sub_func
		global mul_func
		global div_func
		
		
		
add_func:							; functions are defined as labels
        mov rax,0					; init rax to zero
		push    rbp              	; save Base Pointer (bp) original value
        mov     rbp, rsp         	; use base pointer to access stack contents
		
		add rdi, rsi
		add rdi, rdx
		


        mov     rax,rdi		; return an (returned values are in rax)
        mov     rsp, rbp
        pop     rbp
        ret 
		 
 sub_func:							; functions are defined as labels
        mov rax,0					; init rax to zero
		push    rbp              	; save Base Pointer (bp) original value
        mov     rbp, rsp         	; use base pointer to access stack contents
		
		sub rdi, rsi
		sub rdi, rdx
		


        mov     rax,rdi		; return an (returned values are in rax)
        mov     rsp, rbp
        pop     rbp
        ret 

		
		
mul_func:							; functions are defined as labels
        mov rax,0					; init rax to zero
		push    rbp              	; save Base Pointer (bp) original value
        mov     rbp, rsp         	; use base pointer to access stack contents
		
		;mul rdi, rsi
		
        mov     rax,rdi		; return an (returned values are in rax)
        mov     rsp, rbp
        pop     rbp
        ret 

		
		
				
div_func:							; functions are defined as labels
        mov rax,0					; init rax to zero
		push    rbp              	; save Base Pointer (bp) original value
        mov     rbp, rsp         	; use base pointer to access stack contents
		
		;div rdi, rsi
		
        mov     rax,rdi		; return an (returned values are in rax)
        mov     rsp, rbp
        pop     rbp
        ret 

		
