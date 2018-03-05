; http://wiki.freepascal.org/Win64/AMD64_API
;
; MSVC 64bit calling convention:
;	Parameter passing:
;
;   arg1 arg2 arg3 arg4
;----------------------------------------
;	ecx	 rdx  r8   r9		 	for int
;	xmm0 xmm1 xmm2 xmm3			for float/double
;	
; Parameters less than 64 bits long are not zero extended; the high bits contain garbage.
; rax or xmm0 hold return value
;
; Callee scratch registers:
;  	rax, rcx, rdx, r8, r9, r10, r11
;	xmm0 - xmm5
;
; NOTES:
; 	1) always use ret 0 instead of ret

; compile with "\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin\amd64\ml64.exe" /c core64.asm
; or run build_asm.bat

extrn malloc:proc
extrn free:proc

_TEXT	SEGMENT

; void core_memset(char* const dest, ubyte value, uint length)
; void core_memcpy(char* const dest, const char* const src, uint length)
; uint core_div_10(uint value)
; int core_indexof(const char* const haystack, const char* const needle, uint haystackLength, uint needleLength)


;-----------------------------------------------------------------------------------------------------------------------
; void core_memset(char* const dest, ubyte value, uint length)
core_memset proc
@destptr	equ rcx
@value8		equ dl
@length32	equ r8d

	push rdi
	
	mov rdi, @destptr
	mov al, @value8
	mov ecx, @length32
	
	test ecx, ecx
	je @end
	cmp ecx,8
	jb @bytes
	
	; duplicate al accross eax
	mov ah, al
	mov edx, eax
	shl eax, 16
	mov ax, dx
	
	shr ecx, 6
	jz @qwords
	
	; duplicate eax across xmm0
	movd xmm0, eax
	movsldup xmm0, xmm0		; sse3
	movddup xmm0, xmm0		; sse3
	
	align 16
@loop:
	movaps [rdi], 	 xmm0
	movaps [rdi+16], xmm0
	movaps [rdi+32], xmm0
	movaps [rdi+48], xmm0
	add rdi, 64
	dec ecx
	jne @loop
	
@qwords:
	mov ecx, @length32
	and ecx, 63
	shr ecx, 3
	jz @bytes
	
	; duplicate eax across rax
	mov edx, eax
	shl rax, 32
	mov eax, edx
@qwords_lp:
	stosq
	dec ecx
	jne @qwords_lp

@bytes:
	mov ecx, @length32
	and ecx, 7
	rep stosb
	
@end:
	pop rdi
	ret 0
core_memset endp




;-----------------------------------------------------------------------------------------------------------------------
; void core_memcpy(char* const dest, const char* const src, uint length)
core_memcpy proc
@destptr	equ rcx
@srcptr 	equ rdx
@length32	equ r8d

	push rsi
	push rdi
	
	mov rdi, @destptr
	mov rsi, @srcptr
	mov ecx, @length32
	
	test ecx, ecx
	jz @end	; nothing to do
	
	shr ecx, 6
	jz @qwords
	
	align 16
@loop:
	movaps xmm0, [rsi]
	movaps [rdi], xmm0
	movaps xmm0, [rsi+16]
	movaps [rdi+16], xmm0 
	movaps xmm0, [rsi+32]
	movaps [rdi+32], xmm0
	movaps xmm0, [rsi+48]
	movaps [rdi+48], xmm0
	add rsi, 64
	add rdi, 64
	dec ecx
	jne @loop
	
@qwords:
	mov ecx,@length32
	and ecx, 63
	shr ecx, 3
	jz @bytes
@qwordsloop:
	movsq
	dec ecx
	jne @qwordsloop
	
@bytes:
	mov ecx, @length32
	and ecx, 7
	jz @end
@bytesloop:
	movsb
	dec ecx
	jne @bytesloop
	
@end:
	pop rdi
	pop rsi
	ret 0
core_memcpy endp




;-----------------------------------------------------------------------------------------------------------------------
; uint core_div_10(uint value)
core_div_10 proc
	;ecx = value
	
	mov	eax, -858993459				
	mul	ecx
	shr	edx, 3
	mov eax, edx
	ret 0
core_div_10 endp	




; int core_indexof(const char* const haystack, const char* const needle, uint haystackLength, uint needleLength)
; (Assumes all arguments are valid and have been sanitised)
core_indexof proc
@haystackptr	textequ <rcx>
@needleptr		textequ <rdx>	
@haystacklength textequ <r8d>
@needlelength   textequ <r9d>

	sub @haystacklength, @needlelength
	sub r10, r10	; needle index
	mov r11, @haystackptr
	align 4	
@lp:
	test @haystacklength, @haystacklength
	jz @notfound

	mov al, [rcx]
	cmp al, [rdx+r10]
	jne @F
		inc r10d
		cmp r10d, @needlelength
		je @found
		inc ecx
		dec @haystacklength
		jmp @lp
@@:	sub r10d, r10d
	inc ecx
	dec @haystacklength
	jmp @lp
@found:
	mov eax, ecx
	sub rax, r11
	ret 0
@notfound:
	mov eax, -1
	ret 0
core_indexof endp


_TEXT	ENDS



CONST	SEGMENT
dd_false	dd 0
dd_true		dd 1			
CONST	ENDS



DATA 	segment
monitor_mem dq 0
DATA	ends

END