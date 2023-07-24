##==============================================================================
##
##  crt0.S
##
##  startup code
##
##==============================================================================

#include <nds32_init.inc>
#include "interrupt.h"

##------------------------------------------------------------------------------
	.macro vector name
	.align 2
	j OS_Trap_\name
	.endm

	.macro ENABLE_IT_DT
	.align 2
		pushm   $r0, $r1
		mfsr    $r1, $IPSW
		andi	$r1, $r1, #0x20c0
		mfsr    $r0, $PSW
		add   	$r0, $r0, $r1
		mtsr    $r0, $PSW
		isb
		popm    $r0, $r1
	.endm

	.section .vector, "ax"
!========================================================================
! Vector table
!========================================================================
	.align 2
exception_vector:
_start:
	j ___start                    !  (-9) Trap Reset
	vector TLB_Fill             !  (-8) Trap TLB fill
	vector PTE_Not_Present      !  (-7) Trap PTE not present
	vector TLB_Misc             !  (-6) Trap TLB misc
	vector TLB_VLPT_Miss        !  (-5) Trap TLB VLPT miss
	vector Machine_Error        !  (-4) Trap Machine error
	vector Debug_Related        !  (-3) Trap Debug related
	vector General_Exception    !  (-2) Trap General exception
	vector Syscall              !  (-1) Syscall

	/* HW interrupt vector layout */
	hal_hw_vectors
exception_vector_end:

.section .int_stack_op, "ax"
	.align  2
ISR_TABLE:
	.long SystickInterrupt //0
	.long WakeupInterrupt  //1
	.long GpioInterrupt    //2
	.long RtcInterrupt     //3
	.long SpdifInterrupt   //4
	.long SWInterrupt      //5
	.long I2C_Interrupt    //6
	.long UART0_Interrupt  //7
	.long Timer2Interrupt  //8
	.long DMA0_Interrupt   //9
	.long DMA1_Interrupt   //10
	.long DMA2_Interrupt   //11
	.long DMA3_Interrupt   //12
	.long DMA4_Interrupt   //13
	.long DMA5_Interrupt   //14
	.long DMA6_Interrupt   //15
	.long DMA7_Interrupt   //16
	.long DMA8_Interrupt   //17
	.long UART1_Interrupt  //18
	.long USI_Interrupt    //19
	.long I2sInterrupt     //20
	.long Timer3Interrupt  //21
	.long Timer4Interrupt  //22
	.long Timer5Interrupt  //23
	.long Timer6Interrupt  //24
	.long SDIO0_Interrupt  //25
	.long SDIO1_Interrupt  //26
	.long UsbInterrupt     //27
	.long SPIM_Interrupt   //28
	.long PSR_Interrupt    //29
	.long SPIS_Interrupt   //30
	.long FFTInterrupt     //31
/*
 * exception handlers
 */

/*----------------------------------------------------------------------
   Since N903 doesn't have MMU, make them share the common error handler.
----------------------------------------------------------------------*/
OS_Trap_TLB_Fill:
//1:  b   1b
	pushm $lp,$sp
	pushm $r0,$r20
	mov55 $r0, $sp
	li	$r1, 0x1
	b   ExceptionCommHandler


OS_Trap_PTE_Not_Present:
//1:  b   1b
	pushm $lp,$sp
	pushm $r0,$r20
	mov55 $r0, $sp
	li	$r1, 0x2
	b   ExceptionCommHandler

OS_Trap_TLB_Misc:
//1:  b   1b
	pushm $lp,$sp
	pushm $r0,$r20
	mov55 $r0, $sp
	li	$r1, 0x3
	b   ExceptionCommHandler


OS_Trap_TLB_VLPT_Miss:
//1:  b   1b
	pushm $lp,$sp
	pushm $r0,$r20
	mov55 $r0, $sp
	li	$r1, 0x4
	b   ExceptionCommHandler

OS_Trap_Machine_Error:
//1:  b   1b
	pushm $lp,$sp
	pushm $r0,$r20
	mov55 $r0, $sp
	li	$r1, 0x5
	b   ExceptionCommHandler

OS_Trap_Debug_Related:
//1:  b   1b
	pushm $lp,$sp
	pushm $r0,$r20
	mov55 $r0, $sp
	li	$r1, 0x6
	b   ExceptionCommHandler

OS_Trap_General_Exception:
//1:  b   1b
	pushm $lp,$sp
	pushm $r0,$r20
	mov55 $r0, $sp
	li	$r1, 0x7
	b   ExceptionCommHandler

OS_Trap_Syscall:
//1:  b   1b
	pushm $lp,$sp
	pushm $r0,$r20
	mov55 $r0, $sp
	li	$r1, 0x8
	b   ExceptionCommHandler

OS_Trap_Int_Comm:
	ENABLE_IT_DT
	SAVE_ALL_HW
	la  $r1, ISR_TABLE
	lw  $r1, [$r1+$r0<<2]
	jral $r1
	RESTORE_ALL_HW
	iret

! Interrupt wrapper function
OS_Trap_Interrupt_HW0:
	push	$r0
	li	$r0, 0x0
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW1:
	push	$r0
	li	$r0, 0x01
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW2:
	push	$r0
	li	$r0, 0x02
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW3:
	push	$r0
	li	$r0, 0x03
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW4:
	push	$r0
	li	$r0, 0x04
	j	OS_Trap_Int_Comm
/*
OS_Trap_Interrupt_HW5:
	push	$r0
	li	$r0, 0x05
	j	OS_Trap_Int_Comm
*/

OS_Trap_Interrupt_HW5:
	pushm	$r0, $r1
	ENABLE_IT_DT
	j	OS_Trap_Interrupt_SWI

OS_Trap_Interrupt_HW6:
	push	$r0
	li	$r0, 0x06
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW7:
	push	$r0
	li	$r0, 0x07
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW8:
	push	$r0
	li	$r0, 0x08
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW9:
	push	$r0
	li	$r0, 0x09
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW10:
	push	$r0
	li	$r0, 0x0a
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW11:
	push	$r0
	li	$r0, 0x0b
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW12:
	push	$r0
	li	$r0, 0x0c
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW13:
	push	$r0
	li	$r0, 0x0d
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW14:
	push	$r0
	li	$r0, 0x0e
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW15:
	push	$r0
	li	$r0, 0x0f
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW16:
	push	$r0
	li	$r0, 0x10
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW17:
	push	$r0
	li	$r0, 0x11
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW18:
	push	$r0
	li	$r0, 0x12
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW19:
	push	$r0
	li	$r0, 0x13
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW20:
	push	$r0
	li	$r0, 0x14
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW21:
	push	$r0
	li	$r0, 0x15
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW22:
	push	$r0
	li	$r0, 0x16
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW23:
	push	$r0
	li	$r0, 0x17
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW24:
	push	$r0
	li	$r0, 0x18
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW25:
	push	$r0
	li	$r0, 0x19
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW26:
	push	$r0
	li	$r0, 0x1a
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW27:
	push	$r0
	li	$r0, 0x1b
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW28:
	push	$r0
	li	$r0, 0x1c
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW29:
	push	$r0
	li	$r0, 0x1d
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW30:
	push	$r0
	li	$r0, 0x1e
	j	OS_Trap_Int_Comm

OS_Trap_Interrupt_HW31:
	push	$r0
	li	$r0, 0x1f
	j	OS_Trap_Int_Comm

##------------------------------------------------------------------------------
## Startup code

	.section .text
	.global _start
	.weak _call_exit
	.weak _SDA_BASE_
	.func _start
	.type _start, @function

	.align 2
___start:
	!************************** Begin of do-not-modify **************************
	! Please don't modify this code
	! Initialize the registers used by the compiler

	nds32_init		! NDS32 startup initial macro in <nds32_init.inc>
	!*************************** End of do-not-modify ***************************
	#bal _nds32_init_mem

	movi55 $r0,#0x0
 	mtsr $r0,$misc_ctl

	bal __init

	bal main
1:  b   1b

	.size _start, .-___start

	.end
