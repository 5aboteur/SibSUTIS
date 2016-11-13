	.file	"prog.c"
	.text
	.globl	foo
	.type	foo, @function
foo:
.LFB0:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	%rsi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	xorl	%ebx, %ebx
.L3:
	call	rand
	cvtsi2ss	%eax, %xmm0
	mulss	.LC0(%rip), %xmm0
	movss	%xmm0, 0(%rbp,%rbx)
	call	rand
	cvtsi2ss	%eax, %xmm0
	mulss	.LC0(%rip), %xmm0
	movss	%xmm0, (%r12,%rbx)
	addq	$4, %rbx
	cmpq	$4096, %rbx
	jne	.L3
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE0:
	.size	foo, .-foo
	.globl	bar
	.type	bar, @function
bar:
.LFB1:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	movq	%rdx, %r13
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	movq	%rsi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	xorl	%ebx, %ebx
	pushq	%rcx
	.cfi_def_cfa_offset 48
.L8:
	movss	0(%rbp,%rbx), %xmm0
	mulss	(%r12,%rbx), %xmm0
	call	sqrtf
	movss	%xmm0, 0(%r13,%rbx)
	addq	$4, %rbx
	cmpq	$4096, %rbx
	jne	.L8
	popq	%rax
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1:
	.size	bar, .-bar
	.section	.text.startup,"ax",@progbits
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	subq	$12296, %rsp
	.cfi_def_cfa_offset 12304
	xorl	%edi, %edi
	call	time
	movl	%eax, %edi
	call	srand
	leaq	4096(%rsp), %rsi
	movq	%rsp, %rdi
	call	foo
	leaq	8192(%rsp), %rdx
	leaq	4096(%rsp), %rsi
	movq	%rsp, %rdi
	call	bar
	xorl	%eax, %eax
	addq	$12296, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC0:
	.long	805306368
	.ident	"GCC: (GNU) 4.8.3 20140911 (Red Hat 4.8.3-7)"
	.section	.note.GNU-stack,"",@progbits
