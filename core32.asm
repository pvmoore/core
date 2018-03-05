; scratch registers: eax, ecx, edx
; caller tidies the stack
;
	.686P
	.XMM
	.model	flat

extrn 	_printf:proc
extrn 	_putchar:proc

_TEXT	SEGMENT

;--------------------------------------------------------------------------------------------------
; void core_memcpy(char* const dest, const char* const src, uint length)
; Note: This needs some optimisation
_core_memcpy proc
	OPTION PROLOGUE:NONE, EPILOGUE:NONE
@localpushes equ 8
@dest 	equ [esp + 4  + @localpushes]	
@src	equ [esp + 8  + @localpushes]
@length	equ [esp + 12 + @localpushes]
	push esi
	push edi
	
	mov esi,@src
	mov edi,@dest
	mov ecx,@length
	
	test ecx,ecx
	jz @end	; nothing to do
	
	mov edx,ecx
	shr ecx,2
	jz @bytes
	
	rep movsd
	
@bytes:
	mov ecx,edx
	and ecx,3
	rep movsb
	
@end:
	pop edi
	pop esi
	ret
_core_memcpy endp
;--------------------------------------------------------------------------------------------------
; bool core_strcmp(const char* str1, const char* str2, uint strlen)
; returns true if str1 and str2 are equal up to strlen chars
; Note: This needs some optimisation
_core_strcmp proc
	OPTION PROLOGUE:NONE, EPILOGUE:NONE
@localpushes equ 8
str1 	equ [esp + 4  + @localpushes]	
str2	equ [esp + 8  + @localpushes]
strlen	equ [esp + 12 + @localpushes]
	push esi
	push edi
	
	mov esi,str1
	mov edi,str2
	mov ecx,strlen
	
	test ecx,ecx
	je end@_core_strcmp
	
	xor eax,eax
	cmp ecx,4
	jb bytes@_core_strcmp
	
	mov edx,ecx
	shr ecx,2
	repz cmpsd
	jnz end@_core_strcmp
	
	mov ecx,edx
	and ecx,3
	
bytes@_core_strcmp:
	repz cmpsb
	
end@_core_strcmp:
	cmovz eax, dd_true

	pop edi
	pop esi
	ret
_core_strcmp endp

;--------------------------------------------------------------------------------------------------
; uint core_parse_uint(char* buffer, uint value)
; buffer is expected to be 30 chars
; returns offset of start of string in buffer
_core_parse_uint proc
	OPTION PROLOGUE:NONE, EPILOGUE:NONE
buffer 	equ [esp + 4]	
value	equ [esp + 8]
	mov ecx, buffer
	mov eax, value
	
	push ebx
	push edi
	push esi
	mov esi, 30
	
	align 16
lp@_core_parse_uint:
	mov ebx, eax	
	mov	edi, -858993459	
	mul	edi
	shr	edx, 3
	mov eax, edx ; eax = value/10

	lea	edi, DWORD PTR [edx+edx*4]
	lea	edx, DWORD PTR [edi+edi]
	sub	ebx, edx ; ebx = value%10
	
	dec esi
	add bl, '0'
	mov [ecx+esi], bl
	
	test eax, eax
	jnz lp@_core_parse_uint
	
	; return pos
	mov eax, esi

	pop esi
	pop edi
	pop ebx
	ret	
_core_parse_uint endp	

;--------------------------------------------------------------------------------------------------
; uint core_div_10(uint value)
_core_div_10 proc
	OPTION PROLOGUE:NONE, EPILOGUE:NONE
value equ [esp + 4]

	mov	ecx, value
	mov	eax, -858993459				
	mul	ecx
	shr	edx, 3
	mov eax, edx
	ret
_core_div_10 endp	

;--------------------------------------------------------------------------------------------------
; void core_div_mod_10(uint* divresult, uint* modresult, uint value)
_core_div_mod_10 proc
	OPTION PROLOGUE:NONE, EPILOGUE:NONE
localpushes equ 4
divresult 	equ [esp + 4  + localpushes]
modresult 	equ [esp + 8  + localpushes]
value 		equ [esp + 12 + localpushes]

	push edi
	mov edi, divresult

	mov	ecx, value
	mov	eax, -858993459				
	mul	ecx
	shr	edx, 3
	mov [edi], edx ; divresult
	
	mov edi, modresult
	lea	eax, DWORD PTR [edx+edx*4]
	lea	edx, DWORD PTR [eax+eax]
	mov	eax, ecx
	sub	eax, edx
	mov [edi], eax ; modresult
	
	pop edi
	ret
_core_div_mod_10 endp

_TEXT	ENDS

CONST	SEGMENT
dd_false	dd 0
dd_true		dd 1			
CONST	ENDS

END
