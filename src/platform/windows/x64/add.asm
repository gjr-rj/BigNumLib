PUBLIC	bnAddArrayOfBytes_
_TEXT	SEGMENT
i$1 = 0
overflow$ = 4
al$2 = 8
bl$3 = 16
sl$4 = 24
s$ = 48
a$ = 56
b$ = 64
maxSize$ = 72
sizeBiggerNum$ = 80
bnAddArrayOfBytes_ PROC
	mov	DWORD PTR [rsp+32], r9d
	mov	QWORD PTR [rsp+24], r8
	mov	QWORD PTR [rsp+16], rdx
	mov	QWORD PTR [rsp+8], rcx
	sub	rsp, 40					; 00000028H

	mov	DWORD PTR overflow$[rsp], 0

	mov	DWORD PTR i$1[rsp], 0
	jmp	$InitForLoop
$IncIForLoop:
	mov	eax, DWORD PTR i$1[rsp]
	add	eax, 8
	mov	DWORD PTR i$1[rsp], eax
$InitForLoop:
	mov	eax, DWORD PTR maxSize$[rsp]
	cmp	DWORD PTR i$1[rsp], eax
	jae $ExitForLoop

	mov	eax, DWORD PTR i$1[rsp]
	mov	rcx, QWORD PTR s$[rsp]
	add	rax, rcx
	mov	QWORD PTR sl$4[rsp], rax

	mov	eax, DWORD PTR i$1[rsp]
	mov	rcx, QWORD PTR a$[rsp]
	add	rax, rcx
	mov	QWORD PTR al$2[rsp], rax

	mov	eax, DWORD PTR i$1[rsp]
	mov	rcx, QWORD PTR b$[rsp]
	add	rax, rcx
	mov	QWORD PTR bl$3[rsp], rax

	mov	eax, DWORD PTR sizeBiggerNum$[rsp]
	cmp	DWORD PTR i$1[rsp], eax
	jb	SHORT $NoBreakFor
	cmp	DWORD PTR overflow$[rsp], 0
	jne	SHORT $NoBreakFor

	jmp	SHORT $ExitForLoop
$NoBreakFor:

	cmp	DWORD PTR overflow$[rsp], 0
	je	SHORT $SetCarry0
	stc
	jmp	SHORT $CarrySeted
$SetCarry0:
	clc
$CarrySeted:

	mov	rax, QWORD PTR al$2[rsp]
	mov	rax, QWORD PTR [rax]
	mov	rcx, QWORD PTR bl$3[rsp]
	adc	rax, QWORD PTR [rcx]
	mov	rcx, QWORD PTR sl$4[rsp]
	mov	QWORD PTR [rcx], rax
	
	setc al
	cmp al, 0
	je	SHORT $StoreCarry0
	mov	DWORD PTR overflow$[rsp], 1
	jmp	SHORT $CarryStored
$StoreCarry0:
	mov	DWORD PTR overflow$[rsp], 0
$CarryStored:

	jmp $IncIForLoop
$ExitForLoop:

	mov	eax, DWORD PTR overflow$[rsp]

	add	rsp, 40					; 00000028H
	ret	0
bnAddArrayOfBytes_ ENDP
_TEXT	ENDS
END

