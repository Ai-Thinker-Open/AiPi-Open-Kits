#include <nds32_intrinsic.h>
#include "irqn.h"
#include "type.h"

#ifndef NDS32_SR_PFT_CTL
#define NDS32_SR_PFT_CTL 0x210
#endif

inline void GIE_ENABLE()
{
	__nds32__setgie_en();
}

inline void GIE_DISABLE()
{
	__nds32__setgie_dis();
	__nds32__dsb();
}

inline bool GIE_STATE_GET()
{
	return ((__nds32__mfsr(NDS32_SR_PSW) & 0x01) == 0x01) ? TRUE : FALSE;
}

void InterruptLevelSet(uint8_t Level)
{
	uint32_t psw;
	psw = __nds32__mfsr(NDS32_SR_PSW)&(~(7<<16));
	__nds32__mtsr(psw|(Level<<16), NDS32_SR_PSW);
}

void InterruptLevelRestore(void)
{
	uint32_t psw;
	psw = __nds32__mfsr(NDS32_SR_PSW);
	__nds32__mtsr(psw|(7<<16), NDS32_SR_PSW);
}

void Throttle_Enable(void)
{
	uint32_t psw;
	psw = __nds32__mfsr(NDS32_SR_PSW);
	psw |= (1<<21);
	__nds32__mtsr(psw, NDS32_SR_PSW);
}

void Throttle_Disable(void)
{
	uint32_t psw;
	psw = __nds32__mfsr(NDS32_SR_PSW);
	psw &= ~(1<<21);
	__nds32__mtsr(psw, NDS32_SR_PSW);
}

void Throttle_Setup(uint32_t setting)
{
	uint32_t pft_ctl;
	pft_ctl = __nds32__mfsr(NDS32_SR_PFT_CTL);
	pft_ctl = (pft_ctl&~0x1f0)|setting;
	__nds32__mtsr(pft_ctl, NDS32_SR_PFT_CTL);
}


void NVIC_EnableIRQ(IRQn_Type IRQn)
{
	uint32_t int_mask;

	if((IRQn>=0)&&(IRQn<32))
	 {
	  int_mask = __nds32__mfsr(NDS32_SR_INT_MASK2);
	  int_mask |= 1<<IRQn;
	  __nds32__mtsr(int_mask, NDS32_SR_INT_MASK2);
	 }
}

void NVIC_DisableIRQ(IRQn_Type IRQn)
{
	uint32_t int_mask;
	if((IRQn>=0)&&(IRQn<32))
	 {
	  int_mask = __nds32__mfsr(NDS32_SR_INT_MASK2);
	  int_mask &= ~(1<<IRQn);
	  __nds32__mtsr(int_mask, NDS32_SR_INT_MASK2);
	 }
}

//priority=0,1,2,3 0=highest 3=lowest
void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
	uint32_t prio_mask;

	if((IRQn >= 0) && (IRQn < 16)){
      prio_mask = __nds32__mfsr(NDS32_SR_INT_PRI);
      prio_mask &= ~(3<<(IRQn*2));
      prio_mask |= (priority&3)<<(IRQn*2);
      __nds32__mtsr(prio_mask, NDS32_SR_INT_PRI);
	}
	else if((IRQn >= 16) && (IRQn < 32)){
	  prio_mask = __nds32__mfsr(NDS32_SR_INT_PRI2);
	  prio_mask &= ~(3<<((IRQn-16)*2));
	  prio_mask |= (priority&3)<<((IRQn-16)*2);
	  __nds32__mtsr(prio_mask, NDS32_SR_INT_PRI2);
	}
}

uint32_t NVIC_GetPriority(IRQn_Type IRQn)
{
	uint32_t prio_mask;

	if((IRQn >= 0) && (IRQn < 16)){
      prio_mask = __nds32__mfsr(NDS32_SR_INT_PRI);
      prio_mask >>= (IRQn*2);
      return (prio_mask&3);
	}
	else if((IRQn >= 16) && (IRQn < 32)){
	  prio_mask = __nds32__mfsr(NDS32_SR_INT_PRI2);
	  prio_mask >>= ((IRQn-16)*2);
      return (prio_mask&3);
	}

	return -1;
}

#if 0

 /* this function generates a s/w interrupt */
void gen_swi()
{
	unsigned int int_pend;

	int_pend = __nds32__mfsr(NDS32_SR_INT_PEND);
	int_pend |= 0x10000;
	__nds32__mtsr_dsb(int_pend, NDS32_SR_INT_PEND);
}

void initIntr_standby()
{

	/* Init GPIO */
	HAL_GPIO_INITIALIZE(GPIO_USED_MASK);

	/* Check IVIC numbers (IVB.NIVIC) */
	if ((__nds32__mfsr(NDS32_SR_IVB) & 0x0E) != 0) {	// 32IVIC
#if (defined(__NDS32_ISA_V3__) || defined(__NDS32_ISA_V3M__))
		/* enable HW (GPIO) */
		__nds32__mtsr(IC_GPIO, NDS32_SR_INT_MASK2);
		/* Interrupt pending register, write 1 to clear */
		__nds32__mtsr(0xFFFFFFFF, NDS32_SR_INT_PEND2);
#endif
	} else {
		/* Reset interrupt controller */
		intc_reset();
		/* GPIO source to be routed to HW0 */
                setIRQ_ivic(IRQ_GPIO, 0);
		/* GPIO is level-trigger and active-high */
		enableFiqIntr(IC_GPIO, 0, 0);
		/* enable HW0 */
		__nds32__mtsr(0x0001, NDS32_SR_INT_MASK);
	}
}

void clear_swi()
{
	/*
	 * Since INT_PEND is the RO register except bit:16 (SWI),
	 * set zero value to it directly to clear bit:16.
	 */
	__nds32__mtsr_dsb(0, NDS32_SR_INT_PEND);
}

void SW0_ISR()
{
	clear_swi();
	return;
}

void SWI_ISR(unsigned int num)
{
	__nds32__mtsr((1 << num), NDS32_SR_INT_PEND2);
	//SW0_ISR();
	while(1)
	printf("SoftWareInt\n");
}

void SWInterrupt(void)
{
	SWI_ISR(SWI_IRQn);
}

#endif
