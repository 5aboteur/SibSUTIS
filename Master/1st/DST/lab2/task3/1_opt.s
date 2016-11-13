	.file	"prog.c"
	.text
	.globl	foo
	.type	foo, @function
foo:
.LFB11:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdi, %r12
	movq	%rsi, %rbp
	movl	$0, %ebx
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
	.globl	bar
	.type	bar, @function
bar:
.LFB12:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	movq	%rdi, %r12
	movq	%rsi, %rbp
	movq	%rdx, %r13
	movl	$0, %ebx
.L9:
	movss	(%r12,%rbx), %xmm0
	mulss	0(%rbp,%rbx), %xmm0
	sqrtss	%xmm0, %xmm0
	ucomiss	%xmm0, %xmm0
	jnp	.L6
	movss	(%r12,%rbx), %xmm0
	mulss	0(%rbp,%rbx), %xmm0
	call	sqrtf
.L6:
	movss	%xmm0, 0(%r13,%rbx)
	addq	$4, %rbx
	cmpq	$4096, %rbx
	jne	.L9
	addq	$8, %rsp
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
.LFE12:
	.size	bar, .-bar
	.globl	main
	.type	main, @function
main:
.LFB13:
	.cfi_startproc
	subq	$12296, %rsp
	.cfi_def_cfa_offset 12304
	movl	$0, %edi
	call	time
	movl	%eax, %edi
	call	srand
	leaq	4096(%rsp), %rsi
	leaq	8192(%rsp), %rdi
	call	foo
	movq	%rsp, %rdx
	leaq	4096(%rsp), %rsi
	leaq	8192(%rsp), %rdi
	call	bar
	movl	$0, %eax
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
