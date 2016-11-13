	.file	"prog.c"
	.text
	.p2align 4,,15
	.globl	foo
	.type	foo, @function
foo:
.LFB11:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movq	%rdi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rsi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L3:
	call	rand
	cvtsi2ss	%eax, %xmm0
	mulss	.LC0(%rip), %xmm0
	movss	%xmm0, (%r12,%rbx)
	call	rand
	cvtsi2ss	%eax, %xmm0
	mulss	.LC0(%rip), %xmm0
	movss	%xmm0, 0(%rbp,%rbx)
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
.LFE11:
	.size	foo, .-foo
	.p2align 4,,15
	.globl	bar
	.type	bar, @function
bar:
.LFB12:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	xorl	%ebx, %ebx
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	.p2align 4,,10
	.p2align 3
.L10:
	movss	(%rdi,%rbx), %xmm0
	mulss	(%rsi,%rbx), %xmm0
	sqrtss	%xmm0, %xmm1
	ucomiss	%xmm1, %xmm1
	jp	.L12
.L7:
	movss	%xmm1, (%rdx,%rbx)
	addq	$4, %rbx
	cmpq	$4096, %rbx
	jne	.L10
	addq	$32, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L12:
	.cfi_restore_state
	movq	%rdx, 24(%rsp)
	movq	%rsi, 16(%rsp)
	movq	%rdi, 8(%rsp)
	call	sqrtf
	movq	24(%rsp), %rdx
	movaps	%xmm0, %xmm1
	movq	16(%rsp), %rsi
	movq	8(%rsp), %rdi
	jmp	.L7
	.cfi_endproc
.LFE12:
	.size	bar, .-bar
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB13:
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
.LFE13:
	.size	main, .-main
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC0:
	.long	805306368
	.ident	"GCC: (GNU) 4.8.3 20140911 (Red Hat 4.8.3-7)"
	.section	.note.GNU-stack,"",@progbits
