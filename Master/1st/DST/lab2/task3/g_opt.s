	.file	"prog.c"
	.text
	.globl	foo
	.type	foo, @function
foo:
.LFB11:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	%rdi, %r14
	movq	%rsi, %r13
	movl	$0, %ebx
	jmp	.L2
.L3:
	movslq	%ebx, %rbp
	salq	$2, %rbp
	leaq	(%r14,%rbp), %r12
	call	rand
	cvtsi2ss	%eax, %xmm0
	mulss	.LC0(%rip), %xmm0
	movss	%xmm0, (%r12)
	addq	%r13, %rbp
	call	rand
	cvtsi2ss	%eax, %xmm0
	mulss	.LC0(%rip), %xmm0
	movss	%xmm0, 0(%rbp)
	addl	$1, %ebx
.L2:
	cmpl	$1023, %ebx
	jle	.L3
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
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
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	%rdi, %r14
	movq	%rsi, %r13
	movq	%rdx, %r12
	movl	$0, %ebx
	jmp	.L6
.L9:
	movslq	%ebx, %rax
	leaq	(%r12,%rax,4), %rbp
	movss	(%r14,%rax,4), %xmm2
	movss	0(%r13,%rax,4), %xmm1
	movaps	%xmm2, %xmm0
	mulss	%xmm1, %xmm0
	sqrtss	%xmm0, %xmm0
	ucomiss	%xmm0, %xmm0
	jnp	.L7
	mulss	%xmm1, %xmm2
	movaps	%xmm2, %xmm0
	call	sqrtf
.L7:
	movss	%xmm0, 0(%rbp)
	addl	$1, %ebx
.L6:
	cmpl	$1023, %ebx
	jle	.L9
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
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
