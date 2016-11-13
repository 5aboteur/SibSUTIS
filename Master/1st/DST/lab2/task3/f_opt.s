	.file	"prog.c"
	.text
	.p2align 4,,15
	.globl	foo
	.type	foo, @function
foo:
.LFB17:
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
.LFE17:
	.size	foo, .-foo
	.p2align 4,,15
	.globl	bar
	.type	bar, @function
bar:
.LFB18:
	.cfi_startproc
	leaq	16(%rdx), %rax
	leaq	16(%rsi), %rcx
	cmpq	%rax, %rsi
	setae	%r8b
	cmpq	%rcx, %rdx
	setae	%cl
	orb	%cl, %r8b
	je	.L7
	cmpq	%rax, %rdi
	leaq	16(%rdi), %rax
	setae	%cl
	cmpq	%rax, %rdx
	setae	%al
	orb	%al, %cl
	je	.L7
	xorps	%xmm2, %xmm2
	movaps	.LC1(%rip), %xmm5
	xorl	%eax, %eax
	movaps	.LC2(%rip), %xmm4
	.p2align 4,,10
	.p2align 3
.L9:
	movaps	%xmm2, %xmm0
	movaps	%xmm2, %xmm1
	movaps	%xmm2, %xmm3
	movlps	(%rsi,%rax), %xmm0
	movlps	(%rdi,%rax), %xmm1
	movhps	8(%rsi,%rax), %xmm0
	movhps	8(%rdi,%rax), %xmm1
	mulps	%xmm1, %xmm0
	cmpneqps	%xmm0, %xmm3
	rsqrtps	%xmm0, %xmm1
	andps	%xmm3, %xmm1
	mulps	%xmm1, %xmm0
	mulps	%xmm0, %xmm1
	mulps	%xmm4, %xmm0
	addps	%xmm5, %xmm1
	mulps	%xmm0, %xmm1
	movlps	%xmm1, (%rdx,%rax)
	movhps	%xmm1, 8(%rdx,%rax)
	addq	$16, %rax
	cmpq	$4096, %rax
	jne	.L9
	rep ret
	.p2align 4,,10
	.p2align 3
.L7:
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L10:
	movss	(%rsi,%rax), %xmm0
	mulss	(%rdi,%rax), %xmm0
	sqrtss	%xmm0, %xmm0
	movss	%xmm0, (%rdx,%rax)
	addq	$4, %rax
	cmpq	$4096, %rax
	jne	.L10
	rep ret
	.cfi_endproc
.LFE18:
	.size	bar, .-bar
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB19:
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
.LFE19:
	.size	main, .-main
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC0:
	.long	805306368
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC1:
	.long	3225419776
	.long	3225419776
	.long	3225419776
	.long	3225419776
	.align 16
.LC2:
	.long	3204448256
	.long	3204448256
	.long	3204448256
	.long	3204448256
	.ident	"GCC: (GNU) 4.8.3 20140911 (Red Hat 4.8.3-7)"
	.section	.note.GNU-stack,"",@progbits
