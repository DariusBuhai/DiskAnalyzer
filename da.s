	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 11, 0	sdk_version 11, 1
	.globl	_get_literal_priority   ## -- Begin function get_literal_priority
	.p2align	4, 0x90
_get_literal_priority:                  ## @get_literal_priority
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	%edi, -12(%rbp)
	cmpl	$1, -12(%rbp)
	jne	LBB0_2
## %bb.1:
	leaq	L_.str(%rip), %rax
	movq	%rax, -8(%rbp)
	jmp	LBB0_5
LBB0_2:
	cmpl	$2, -12(%rbp)
	jne	LBB0_4
## %bb.3:
	leaq	L_.str.1(%rip), %rax
	movq	%rax, -8(%rbp)
	jmp	LBB0_5
LBB0_4:
	leaq	L_.str.2(%rip), %rax
	movq	%rax, -8(%rbp)
LBB0_5:
	movq	-8(%rbp), %rax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_is_option              ## -- Begin function is_option
	.p2align	4, 0x90
_is_option:                             ## @is_option
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	_strcmp
	cmpl	$0, %eax
	movb	$1, %cl
	movb	%cl, -25(%rbp)          ## 1-byte Spill
	je	LBB1_2
## %bb.1:
	movq	-8(%rbp), %rdi
	movq	-24(%rbp), %rsi
	callq	_strcmp
	cmpl	$1, %eax
	sete	%cl
	movb	%cl, -25(%rbp)          ## 1-byte Spill
LBB1_2:
	movb	-25(%rbp), %al          ## 1-byte Reload
	andb	$1, %al
	movzbl	%al, %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_itoa                   ## -- Begin function itoa
	.p2align	4, 0x90
_itoa:                                  ## @itoa
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movslq	-8(%rbp), %rax
	shlq	$0, %rax
	movq	%rax, %rdi
	callq	_malloc
	xorl	%esi, %esi
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rdi
	movl	-4(%rbp), %r8d
	movq	$-1, %rdx
	leaq	L_.str.3(%rip), %rcx
	movb	$0, %al
	callq	___sprintf_chk
	movq	-16(%rbp), %rcx
	movl	%eax, -20(%rbp)         ## 4-byte Spill
	movq	%rcx, %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_write_to_file          ## -- Begin function write_to_file
	.p2align	4, 0x90
_write_to_file:                         ## @write_to_file
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$176, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	leaq	-160(%rbp), %rsi
	callq	_stat$INODE64
	cmpl	$0, %eax
	jl	LBB3_2
## %bb.1:
	movq	-8(%rbp), %rdi
	callq	_remove
LBB3_2:
	movq	-8(%rbp), %rdi
	movl	$513, %esi              ## imm = 0x201
	movl	$448, %edx              ## imm = 0x1C0
	movb	$0, %al
	callq	_open
	movl	%eax, -164(%rbp)
	movl	-164(%rbp), %edi
	movq	-16(%rbp), %rsi
	movq	-16(%rbp), %rcx
	movl	%edi, -168(%rbp)        ## 4-byte Spill
	movq	%rcx, %rdi
	movq	%rsi, -176(%rbp)        ## 8-byte Spill
	callq	_strlen
	shlq	$0, %rax
	movl	-168(%rbp), %edi        ## 4-byte Reload
	movq	-176(%rbp), %rsi        ## 8-byte Reload
	movq	%rax, %rdx
	callq	_write
	cmpq	$0, %rax
	jg	LBB3_4
## %bb.3:
	xorl	%eax, %eax
	movl	%eax, %edi
	callq	_perror
LBB3_4:
	movl	-164(%rbp), %edi
	callq	_close
	addq	$176, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_get_daemon_pid         ## -- Begin function get_daemon_pid
	.p2align	4, 0x90
_get_daemon_pid:                        ## @get_daemon_pid
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$224, %rsp
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	leaq	L_.str.4(%rip), %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rdi
	leaq	-168(%rbp), %rsi
	callq	_stat$INODE64
	cmpl	$0, %eax
	jge	LBB4_2
## %bb.1:
	leaq	L_.str.5(%rip), %rdi
	movb	$0, %al
	callq	_printf
	movl	$0, -12(%rbp)
	jmp	LBB4_3
LBB4_2:
	movq	-24(%rbp), %rdi
	leaq	L_.str.6(%rip), %rsi
	callq	_fopen
	movq	%rax, -176(%rbp)
	movq	-72(%rbp), %rax
	movq	%rsp, %rcx
	movq	%rcx, -184(%rbp)
	movq	%rax, %rcx
	addq	$15, %rcx
	andq	$-16, %rcx
	movq	%rax, -200(%rbp)        ## 8-byte Spill
	movq	%rcx, %rax
	callq	____chkstk_darwin
	subq	%rax, %rsp
	movq	%rsp, %rax
	movq	-200(%rbp), %rcx        ## 8-byte Reload
	movq	%rcx, -192(%rbp)
	movq	-176(%rbp), %rdi
	leaq	L_.str.7(%rip), %rsi
	movq	%rax, %rdx
	movq	%rax, -208(%rbp)        ## 8-byte Spill
	movb	$0, %al
	callq	_fscanf
	movq	-208(%rbp), %rdi        ## 8-byte Reload
	movl	%eax, -212(%rbp)        ## 4-byte Spill
	callq	_atoi
	movl	%eax, -12(%rbp)
	movq	-184(%rbp), %rcx
	movq	%rcx, %rsp
LBB4_3:
	movl	-12(%rbp), %eax
	movq	___stack_chk_guard@GOTPCREL(%rip), %rcx
	movq	(%rcx), %rcx
	movq	-8(%rbp), %rdx
	cmpq	%rdx, %rcx
	movl	%eax, -216(%rbp)        ## 4-byte Spill
	jne	LBB4_5
## %bb.4:
	movl	-216(%rbp), %eax        ## 4-byte Reload
	movq	%rbp, %rsp
	popq	%rbp
	retq
LBB4_5:
	callq	___stack_chk_fail
	ud2
	.cfi_endproc
                                        ## -- End function
	.globl	_initialize_signal      ## -- Begin function initialize_signal
	.p2align	4, 0x90
_initialize_signal:                     ## @initialize_signal
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	xorl	%eax, %eax
	movl	%eax, %esi
	movl	$31, %edi
	callq	_signal
	xorl	%ecx, %ecx
	movq	%rax, -8(%rbp)          ## 8-byte Spill
	movl	%ecx, %eax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_send_signal            ## -- Begin function send_signal
	.p2align	4, 0x90
_send_signal:                           ## @send_signal
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	callq	_get_daemon_pid
	movl	%eax, -8(%rbp)
	cmpl	$0, -8(%rbp)
	jne	LBB6_2
## %bb.1:
	movl	$0, -4(%rbp)
	jmp	LBB6_3
LBB6_2:
	movl	-8(%rbp), %edi
	movl	$2, %esi
	callq	_kill
	movl	-8(%rbp), %esi
	leaq	L_.str.8(%rip), %rdi
	movl	%eax, -12(%rbp)         ## 4-byte Spill
	movb	$0, %al
	callq	_printf
	movl	$0, -4(%rbp)
LBB6_3:
	movl	-4(%rbp), %eax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$272, %rsp              ## imm = 0x110
	movl	$0, -4(%rbp)
	movl	%edi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	cmpl	$1, -8(%rbp)
	jne	LBB7_2
## %bb.1:
	movl	$0, -4(%rbp)
	jmp	LBB7_35
LBB7_2:
	callq	_initialize_signal
	movq	-16(%rbp), %rcx
	movq	8(%rcx), %rdi
	leaq	L_.str.9(%rip), %rsi
	leaq	L_.str.10(%rip), %rdx
	movl	%eax, -124(%rbp)        ## 4-byte Spill
	callq	_is_option
	cmpl	$0, %eax
	je	LBB7_12
## %bb.3:
	cmpl	$3, -8(%rbp)
	jge	LBB7_5
## %bb.4:
	movl	$-1, -4(%rbp)
	jmp	LBB7_35
LBB7_5:
	movl	$1, -20(%rbp)
	movq	-16(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, -32(%rbp)
	cmpl	$5, -8(%rbp)
	jne	LBB7_11
## %bb.6:
	movq	-16(%rbp), %rax
	movq	24(%rax), %rdi
	leaq	L_.str.11(%rip), %rsi
	leaq	L_.str.12(%rip), %rdx
	callq	_is_option
	cmpl	$0, %eax
	je	LBB7_11
## %bb.7:
	movq	-16(%rbp), %rax
	movq	32(%rax), %rdi
	callq	_atoi
	movl	%eax, -20(%rbp)
	cmpl	$1, -20(%rbp)
	jl	LBB7_9
## %bb.8:
	cmpl	$3, -20(%rbp)
	jle	LBB7_10
LBB7_9:
	leaq	L_.str.13(%rip), %rdi
	movb	$0, %al
	callq	_printf
	movl	$1, -20(%rbp)
LBB7_10:
	jmp	LBB7_11
LBB7_11:
	movl	$1024, %edi             ## imm = 0x400
	callq	_malloc
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rdi
	movl	-20(%rbp), %r9d
	movq	-32(%rbp), %rax
	movq	%rdi, -136(%rbp)        ## 8-byte Spill
	movl	%r9d, -140(%rbp)        ## 4-byte Spill
	movq	%rax, -152(%rbp)        ## 8-byte Spill
	callq	_getpid
	xorl	%esi, %esi
	movq	-136(%rbp), %rdi        ## 8-byte Reload
	movq	$-1, %rdx
	leaq	L_.str.14(%rip), %rcx
	movl	$1, %r8d
	movl	-140(%rbp), %r9d        ## 4-byte Reload
	movq	-152(%rbp), %r10        ## 8-byte Reload
	movq	%r10, (%rsp)
	movl	%eax, 8(%rsp)
	movb	$0, %al
	callq	___sprintf_chk
	movq	-40(%rbp), %rsi
	leaq	L_.str.15(%rip), %rdi
	movl	%eax, -156(%rbp)        ## 4-byte Spill
	callq	_write_to_file
	callq	_send_signal
	movq	-32(%rbp), %rdx
	movl	-20(%rbp), %edi
	movl	%eax, -160(%rbp)        ## 4-byte Spill
	movq	%rdx, -168(%rbp)        ## 8-byte Spill
	callq	_get_literal_priority
	leaq	L_.str.16(%rip), %rdi
	movl	$2, %esi
	movq	-168(%rbp), %rdx        ## 8-byte Reload
	movq	%rax, %rcx
	movb	$0, %al
	callq	_printf
	movl	$0, -4(%rbp)
	jmp	LBB7_35
LBB7_12:
	movq	-16(%rbp), %rax
	movq	8(%rax), %rdi
	leaq	L_.str.17(%rip), %rsi
	leaq	L_.str.18(%rip), %rdx
	callq	_is_option
	cmpl	$0, %eax
	je	LBB7_16
## %bb.13:
	cmpl	$3, -8(%rbp)
	jge	LBB7_15
## %bb.14:
	movl	$-1, -4(%rbp)
	jmp	LBB7_35
LBB7_15:
	movq	-16(%rbp), %rax
	movq	16(%rax), %rdi
	callq	_atoi
	movl	%eax, -44(%rbp)
	leaq	L_.str.19(%rip), %rdi
	movb	$0, %al
	callq	_printf
	movl	$1024, %edi             ## imm = 0x400
	movl	%eax, -172(%rbp)        ## 4-byte Spill
	callq	_malloc
	movq	%rax, -56(%rbp)
	movq	-56(%rbp), %rdi
	movl	-44(%rbp), %r9d
	movq	%rdi, -184(%rbp)        ## 8-byte Spill
	movl	%r9d, -188(%rbp)        ## 4-byte Spill
	callq	_getpid
	xorl	%esi, %esi
	movq	-184(%rbp), %rdi        ## 8-byte Reload
	movq	$-1, %rdx
	leaq	L_.str.20(%rip), %rcx
	movl	$2, %r8d
	movl	-188(%rbp), %r9d        ## 4-byte Reload
	movl	%eax, (%rsp)
	movb	$0, %al
	callq	___sprintf_chk
	movq	-56(%rbp), %rsi
	leaq	L_.str.15(%rip), %rdi
	movl	%eax, -192(%rbp)        ## 4-byte Spill
	callq	_write_to_file
	callq	_send_signal
	movl	-44(%rbp), %esi
	leaq	L_.str.21(%rip), %rdi
	movl	%eax, -196(%rbp)        ## 4-byte Spill
	movb	$0, %al
	callq	_printf
LBB7_16:
	movq	-16(%rbp), %rax
	movq	8(%rax), %rdi
	leaq	L_.str.22(%rip), %rsi
	leaq	L_.str.23(%rip), %rdx
	callq	_is_option
	cmpl	$0, %eax
	je	LBB7_20
## %bb.17:
	cmpl	$3, -8(%rbp)
	jge	LBB7_19
## %bb.18:
	movl	$-1, -4(%rbp)
	jmp	LBB7_35
LBB7_19:
	movq	-16(%rbp), %rax
	movq	16(%rax), %rdi
	callq	_atoi
	movl	%eax, -60(%rbp)
	movl	$1024, %edi             ## imm = 0x400
	callq	_malloc
	movq	%rax, -72(%rbp)
	movq	-72(%rbp), %rdi
	movl	-60(%rbp), %r9d
	movq	%rdi, -208(%rbp)        ## 8-byte Spill
	movl	%r9d, -212(%rbp)        ## 4-byte Spill
	callq	_getpid
	xorl	%esi, %esi
	movq	-208(%rbp), %rdi        ## 8-byte Reload
	movq	$-1, %rdx
	leaq	L_.str.20(%rip), %rcx
	movl	$3, %r8d
	movl	-212(%rbp), %r9d        ## 4-byte Reload
	movl	%eax, (%rsp)
	movb	$0, %al
	callq	___sprintf_chk
	movq	-72(%rbp), %rsi
	leaq	L_.str.15(%rip), %rdi
	movl	%eax, -216(%rbp)        ## 4-byte Spill
	callq	_write_to_file
	callq	_send_signal
	movl	-60(%rbp), %esi
	leaq	L_.str.24(%rip), %rdi
	movl	%eax, -220(%rbp)        ## 4-byte Spill
	movb	$0, %al
	callq	_printf
LBB7_20:
	movq	-16(%rbp), %rax
	movq	8(%rax), %rdi
	leaq	L_.str.25(%rip), %rsi
	leaq	L_.str.26(%rip), %rdx
	callq	_is_option
	cmpl	$0, %eax
	je	LBB7_24
## %bb.21:
	cmpl	$3, -8(%rbp)
	jge	LBB7_23
## %bb.22:
	movl	$-1, -4(%rbp)
	jmp	LBB7_35
LBB7_23:
	movq	-16(%rbp), %rax
	movq	16(%rax), %rdi
	callq	_atoi
	movl	%eax, -76(%rbp)
	leaq	L_.str.27(%rip), %rcx
	movq	%rcx, -88(%rbp)
	leaq	L_.str.28(%rip), %rcx
	movq	%rcx, -96(%rbp)
	movl	$1024, %edi             ## imm = 0x400
	callq	_malloc
	movq	%rax, -104(%rbp)
	movq	-104(%rbp), %rdi
	movl	-76(%rbp), %r9d
	movq	%rdi, -232(%rbp)        ## 8-byte Spill
	movl	%r9d, -236(%rbp)        ## 4-byte Spill
	callq	_getpid
	xorl	%esi, %esi
	movq	-232(%rbp), %rdi        ## 8-byte Reload
	movq	$-1, %rdx
	leaq	L_.str.20(%rip), %rcx
	movl	$4, %r8d
	movl	-236(%rbp), %r9d        ## 4-byte Reload
	movl	%eax, (%rsp)
	movb	$0, %al
	callq	___sprintf_chk
	movq	-104(%rbp), %rsi
	leaq	L_.str.15(%rip), %rdi
	movl	%eax, -240(%rbp)        ## 4-byte Spill
	callq	_write_to_file
	callq	_send_signal
	movl	-76(%rbp), %esi
	movq	-88(%rbp), %rdx
	movq	-96(%rbp), %rcx
	leaq	L_.str.29(%rip), %rdi
	movl	%eax, -244(%rbp)        ## 4-byte Spill
	movb	$0, %al
	callq	_printf
LBB7_24:
	movq	-16(%rbp), %rax
	movq	8(%rax), %rdi
	leaq	L_.str.30(%rip), %rsi
	leaq	L_.str.31(%rip), %rdx
	callq	_is_option
	cmpl	$0, %eax
	je	LBB7_28
## %bb.25:
	cmpl	$3, -8(%rbp)
	jge	LBB7_27
## %bb.26:
	movl	$-1, -4(%rbp)
	jmp	LBB7_35
LBB7_27:
	movq	-16(%rbp), %rax
	movq	16(%rax), %rdi
	callq	_atoi
	movl	%eax, -108(%rbp)
	leaq	L_.str.27(%rip), %rcx
	movq	%rcx, -120(%rbp)
	movl	-108(%rbp), %esi
	movq	-120(%rbp), %rdx
	leaq	L_.str.32(%rip), %rdi
	movb	$0, %al
	callq	_printf
LBB7_28:
	movq	-16(%rbp), %rax
	movq	8(%rax), %rdi
	leaq	L_.str.33(%rip), %rsi
	leaq	L_.str.34(%rip), %rdx
	callq	_is_option
	cmpl	$0, %eax
	je	LBB7_30
## %bb.29:
	leaq	L_.str.35(%rip), %rdi
	movb	$0, %al
	callq	_printf
LBB7_30:
	movq	-16(%rbp), %rax
	movq	8(%rax), %rdi
	leaq	L_.str.11(%rip), %rsi
	leaq	L_.str.36(%rip), %rdx
	callq	_is_option
	cmpl	$0, %eax
	je	LBB7_34
## %bb.31:
	cmpl	$3, -8(%rbp)
	jge	LBB7_33
## %bb.32:
	movl	$-1, -4(%rbp)
	jmp	LBB7_35
LBB7_33:
	leaq	L_.str.37(%rip), %rdi
	movb	$0, %al
	callq	_printf
LBB7_34:
	movl	$0, -4(%rbp)
LBB7_35:
	movl	-4(%rbp), %eax
	addq	$272, %rsp              ## imm = 0x110
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"low"

L_.str.1:                               ## @.str.1
	.asciz	"normal"

L_.str.2:                               ## @.str.2
	.asciz	"high"

L_.str.3:                               ## @.str.3
	.asciz	"%d"

L_.str.4:                               ## @.str.4
	.asciz	"TempData/daemon.pid"

L_.str.5:                               ## @.str.5
	.asciz	"No pid available!\n"

L_.str.6:                               ## @.str.6
	.asciz	"r"

L_.str.7:                               ## @.str.7
	.asciz	"%s"

L_.str.8:                               ## @.str.8
	.asciz	"Signal send to pid: %d\n"

L_.str.9:                               ## @.str.9
	.asciz	"-a"

L_.str.10:                              ## @.str.10
	.asciz	"--add"

L_.str.11:                              ## @.str.11
	.asciz	"-p"

L_.str.12:                              ## @.str.12
	.asciz	"--priority"

L_.str.13:                              ## @.str.13
	.asciz	"Priority should have one of the values: 1-low, 2-normal, 3-high\n"

L_.str.14:                              ## @.str.14
	.asciz	"TYPE %d\nPRIORITY %d\nPATH %s\nPPID %d"

L_.str.15:                              ## @.str.15
	.asciz	"TempData/daemon_instruction.txt"

L_.str.16:                              ## @.str.16
	.asciz	"Created analysis task with ID `%d` for `%s` and priority `%s`.\n"

L_.str.17:                              ## @.str.17
	.asciz	"-S"

L_.str.18:                              ## @.str.18
	.asciz	"--suspend"

L_.str.19:                              ## @.str.19
	.asciz	"Hereeee\n"

L_.str.20:                              ## @.str.20
	.asciz	"TYPE %d\nPID %d\nPPID %d"

L_.str.21:                              ## @.str.21
	.asciz	"Task with ID `%d` suspended.\n"

L_.str.22:                              ## @.str.22
	.asciz	"-R"

L_.str.23:                              ## @.str.23
	.asciz	"--resume"

L_.str.24:                              ## @.str.24
	.asciz	"Task with ID `%d` resumed.\n"

L_.str.25:                              ## @.str.25
	.asciz	"-r"

L_.str.26:                              ## @.str.26
	.asciz	"--remove"

L_.str.27:                              ## @.str.27
	.asciz	"done"

L_.str.28:                              ## @.str.28
	.asciz	"/home/user"

L_.str.29:                              ## @.str.29
	.asciz	"Removed analysis task with ID `%d`, status `%s` for `%s`\n"

L_.str.30:                              ## @.str.30
	.asciz	"-i"

L_.str.31:                              ## @.str.31
	.asciz	"--info"

L_.str.32:                              ## @.str.32
	.asciz	"The analysis of task with ID `%d` has the status `%s`\n"

L_.str.33:                              ## @.str.33
	.asciz	"-l"

L_.str.34:                              ## @.str.34
	.asciz	"--list"

L_.str.35:                              ## @.str.35
	.asciz	"ID  PRI  Path  Done  Status  Details\n"

L_.str.36:                              ## @.str.36
	.asciz	"--print"

L_.str.37:                              ## @.str.37
	.asciz	"Path  Usage  Size  Amount\n"

.subsections_via_symbols
