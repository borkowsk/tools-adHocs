/*========================================================*/
/* 	        GOSUB IMPLEMENTATION	                  */
/*========================================================*/
/*   This code requiared inline or external assambler	  */
/* 	  and is only Borland C++ compatible.	          */
/*   For prevent multiple inclusion:                      */
#ifndef _GOSUB_H_
#define _GOSUB_H_ 1

/*NOTE:                                                   */
/*-You cant use local variable in nested blocs with GOSUB.*/
/*-You must check of a jumps optimizations.		  */
/* Default GOSUB is by its own "_gosub_stack[_GOSUB_STACK_]"*/
/* You may change len of this stack by define:            */
/*	#define _GOSUB_STACK_  size			  */
/*--------------------------------------------------------*/

//	 	   token-pasting macros from generic.h ;
// 		ANSI requires an extra level of indirection
/********************************************************************/
#define _paste( z , y )       _paste_x( z , y )
#define _paste_x( z , y )     z##y
#define MLAB  _paste(_Lab_sub_,__LINE__)

#	ifndef _GOSUB_STACK_
#	define _GOSUB_STACK_ 511
#	endif

static void near _stack_err()
	{onerror(RetOvr);}
static unsigned int  _gosub_stack[_GOSUB_STACK_+1]={(unsigned int)(&_stack_err)};
static unsigned int* _gosub_sp=_gosub_stack;

static void near cdecl _save_ip()
{
_gosub_sp++;	  //increment gosub stack pointer
#ifdef _TEST_GOSUB_STACK_
if((_gosub_sp)>(_gosub_stack+_GOSUB_STACK_))
		onerror(GosOvr);
#endif
asm mov ax,[bp+2];//get return adres from program stack
*_gosub_sp=_AX;	  //put this on gosub stack
asm clc;	  //clear carry flag - normal return
}

static void near cdecl _restore_ip()
{
_AX=*_gosub_sp;   //get return adres from gosub stack
_gosub_sp--;      //decrement gosub stack pointer
asm mov [bp+2],ax;//put return adres _i_n_  program stack
asm stc;	  //set carry flag - return from subrutine
}

#	define GOSUB(_LABEL_ )  _save_ip();asm jc MLAB;goto _LABEL_; MLAB:
#	define RET_SUB               _restore_ip();

#	define S_GSTACK   unsigned int *_old_gosub_sp=_gosub_sp
#	define R_GSTACK	  {_gosub_sp=_old_gosub_sp;}

#endif
