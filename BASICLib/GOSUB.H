/*========================================================*/
/*           GOSUB macros for BASIC->C translator         */
/*========================================================*/
/*   This code requiared inline or external assambler	  */
/* 	  and is only Borland C++ compatible.	          */
/*   For prevent multiple inclusion:                      */
#ifndef _GOSUB_H_
#define _GOSUB_H_ 1
/*NOTE:                                                   */
/*-You cant use local variable in nested blocs with GOSUB.*/
/*-You must check of a jumps optimizations.		  */
/* Default GOSUB is by its own "_gosub_stack[GOSUB_STACK]"*/
/* You may change len of this stack by define:            */
/*	#define GOSUB_STACK  size			  */
/* You may use program stack for GOSUB by define:	  */
/*	#define _GOSUB_BY_STACK_                          */
/* But this metod make probably impredictable results if  */
/* you use math package.                                  */
/* If you dont use this and you have MASM or BASM ,you may*/
/* swith to compile inline very fast code GOSUB	by define:*/
/*	#define _GOSUB_BY_STACK_                          */
/*	#define _MASM_INLINE_	                          */
/*	#pragma inline					  */
/*--------------------------------------------------------*/
// token-pasting macros; ANSI requires an extra level of indirection
/********************************************************************/
#define _paste(z, y)       _paste_x(z, y)
#define _paste_x(z, y)     z##y
#define MLAB  _paste(_Lab_sub_,__LINE__)

#ifdef _GOSUB_BY_STACK_
/* Is faster but mayby incompatible */

#	ifdef _MASM_INLINE_
#	define GOSUB(_LABEL_)  asm{clc;push OFFSET $+3;jc MLAB;jmp _LABEL_;} MLAB:
#	define RET_SUB               asm{stc;retn;}
#	else
static void near _push_ip()
	{
	asm{pop bp;
	    pop ax;	//get return adress
	    push ax;	//unget this
	    clc;	//clear cary flag
	    jmp ax;	//jump tu return adress
	    }
	}
#	define GOSUB(_LABEL_ )  _push_ip();asm jc MLAB;goto _LABEL_;MLAB:
#	define RET_SUB               asm{stc;retn;}
#       endif

#else
#	ifndef GOSUB_STACK
#	define GOSUB_STACK 1024
#	endif
static void near _stack_err()
	{onerror(RetOvr);}
static unsigned int  _gosub_stack[GOSUB_STACK]={(unsigned int)(&_stack_err)};
static unsigned int* _gosub_sp=_gosub_stack;
static void near _save_ip()
{
_gosub_sp++;	  //increment gosub stack pointer
asm mov ax,[bp+2];//get return adres from program stack
*_gosub_sp=_AX;	  //put this on gosub stack
asm clc;	  //clear carry flag - normal return
}
static void near _restore_ip()
{
_AX=*_gosub_sp;   //get return adres from gosub stack
_gosub_sp--;      //decrement gosub stack pointer
asm mov [bp+2],ax;//put return adres _i_n_  program stack
asm stc;	  //set carry flag - return from subrutine
}
#	define GOSUB(_LABEL_ )  _save_ip();asm jc MLAB;goto _LABEL_; MLAB:
#	define RET_SUB               _restore_ip();
#endif

#endif
