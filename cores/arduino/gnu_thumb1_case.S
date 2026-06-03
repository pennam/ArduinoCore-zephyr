/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Thumb-1 switch dispatch helpers (__gnu_thumb1_case_*) for ARMv6-M.
 *
 * GCC compiles dense switch statements on Thumb-1 targets into
 *
 *     bl   __gnu_thumb1_case_<type>
 *     <inline jump table>
 *
 * The helper locates the jump table THROUGH THE RETURN ADDRESS (LR)
 * and dispatches by adding 2 * table[index] to LR. Because of that,
 * the BL must reach the real implementation directly: the helper can
 * not be reached through the LLEXT loader exports, a long-call
 * wrapper or any other call-through stub, since the intermediate call
 * repoints LR away from the table and the helper then dispatches on
 * the stub's own instruction bytes (hard fault on RP2040). Providing
 * the implementations here keeps the BL in-module and the ABI intact.
 *
 * ABI: r0 = case index, jump table immediately follows the BL; all
 * registers are preserved. Same algorithm as libgcc's lib1funcs.S.
 */

#if defined(__ARM_ARCH_6M__)

	.syntax unified
	.thumb

	.macro CASE_HELPER name
	.section .text.\name, "ax", %progbits
	.global \name
	.type \name, %function
	.thumb_func
\name:
	.endm

/* unsigned byte table */
CASE_HELPER __gnu_thumb1_case_uqi
	push	{r1}
	mov	r1, lr
	lsrs	r1, r1, #1
	lsls	r1, r1, #1	/* clear Thumb bit: LR -> table address */
	ldrb	r1, [r1, r0]
	lsls	r1, r1, #1
	add	lr, lr, r1
	pop	{r1}
	bx	lr
	.size __gnu_thumb1_case_uqi, . - __gnu_thumb1_case_uqi

/* signed byte table */
CASE_HELPER __gnu_thumb1_case_sqi
	push	{r1}
	mov	r1, lr
	lsrs	r1, r1, #1
	lsls	r1, r1, #1
	ldrsb	r1, [r1, r0]
	lsls	r1, r1, #1
	add	lr, lr, r1
	pop	{r1}
	bx	lr
	.size __gnu_thumb1_case_sqi, . - __gnu_thumb1_case_sqi

/* unsigned halfword table */
CASE_HELPER __gnu_thumb1_case_uhi
	push	{r0, r1}
	mov	r1, lr
	lsrs	r1, r1, #1
	lsls	r0, r0, #1
	lsls	r1, r1, #1
	ldrh	r1, [r1, r0]
	lsls	r1, r1, #1
	add	lr, lr, r1
	pop	{r0, r1}
	bx	lr
	.size __gnu_thumb1_case_uhi, . - __gnu_thumb1_case_uhi

/* signed halfword table */
CASE_HELPER __gnu_thumb1_case_shi
	push	{r0, r1}
	mov	r1, lr
	lsrs	r1, r1, #1
	lsls	r0, r0, #1
	lsls	r1, r1, #1
	ldrsh	r1, [r1, r0]
	lsls	r1, r1, #1
	add	lr, lr, r1
	pop	{r0, r1}
	bx	lr
	.size __gnu_thumb1_case_shi, . - __gnu_thumb1_case_shi

/* word table: entries are offsets relative to the (aligned) table base */
CASE_HELPER __gnu_thumb1_case_si
	push	{r0, r1}
	mov	r1, lr
	adds	r1, r1, #2	/* LR is odd: +2 then truncating rounds up */
	lsrs	r1, r1, #2
	lsls	r1, r1, #2	/* word-aligned table address */
	lsls	r0, r0, #2
	ldr	r0, [r1, r0]
	adds	r0, r0, r1
	mov	lr, r0
	pop	{r0, r1}
	mov	pc, lr		/* MOV PC does not interwork: stays Thumb */
	.size __gnu_thumb1_case_si, . - __gnu_thumb1_case_si

#endif /* __ARM_ARCH_6M__ */
