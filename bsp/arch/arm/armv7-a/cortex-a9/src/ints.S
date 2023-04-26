#include "arm_mode_s_file.h"
	.cpu cortex-a9
	.arch armv7-a
	.arch_extension mp
	.arch_extension sec
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 2
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"ints.S"
	.text
	.align	2
	.global	panic
	.syntax unified
	.arm
	.type	panic, %function
panic:
	@ Volatile: function does not return.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
.L2:
	b	.L2
	.size	panic, .-panic
	.ident	"GCC: (GNU) 12.2.0"

