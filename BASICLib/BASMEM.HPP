/* DEFINICJA MAKR UMOZLIWIAJACYCH SZYBKIE PRZESLANIA PAMIEC-PAMIEC */

#ifndef _BASMEM_HPP_
#define _BASMEM_HPP_

#define MOV2W(_T_,_S_)  {*(long*)(&(_T_))=*(long*)(&(_S_));}

#if defined(__TURBOC__)
#	if defined(__TINY_)||defined(__SMALL__)||defined(_MEDIUM_)
/* DANE W JEDNYM SEGMENCIE */

#define MOV4W(_T_,_S_) {                      	     			  \
			asm{push si;push di;push es;}  /* save regs */    \
			    (void near*)_SI=&(_S_);    /* calculate and */\
			    (void near*)_DI=&(_T_);    /* load adress   */\
			asm{push ds;pop  es;           /* load ds to es */\
			    cld;                       /* set direction */\
			    movsw;movsw;movsw;movsw;   /* transfer */     \
			    pop es;pop di;pop si;}     /* restore regs */ \
			}

#define  MOVSW(_T_,_S_) {				          	  \
			asm{push si;push di;push es;}  /* save regs */    \
			    (void near*)_SI=&(_S_);    /* calculate and */\
			    (void near*)_DI=&(_T_);    /* load adress   */\
			    _CX=sizeof(_T_)/2;	       /* loop num calc.*/\			  \
			asm{push ds;pop  es;           /* load ds to es */\
			    cld;                       /* set direction */\
			    rep movsw;		       /* Transfer      */\
			    pop es;pop di;pop si;}     /* Restore regs  */\
			}
#	else
/* DANE W WIELU SEGMENTACH */
#define MOV4W(_T_,_S_) 	 {					       \
			 void far* t=&(_T_), far* s=&(_S_);	       \
			 asm{push si;push di;push es;push ds;/* save */\
			     lds si,s;                /* load source */\
			     les di,t;                /* load target */\
			     cld;                   /* set direction */\
			     movsw;movsw;movsw;movsw;   /* transfer  */\
			     pop ds;pop es;pop di;pop si;}/* restore */\
			 }

#define  MOVSW(_T_,_S_)	 {                                             \
			 void far* t=&(_T_), far* s=&(_S_);	       \
			     _CX=sizeof(_T_)/2;			       \
			 asm{push si;push di;push es;push ds;/* save */\
			     lds si,s;                /* load source */\
			     les di,t;                /* load target */\
			     cld;                     /* set direction */\
			     rep movsw;               /* Transfer    */\
			     pop ds;pop es;pop di;pop si;}/* restore */\
			 }
#	endif
#else
/* Full ANSII compatible definition */
#define  MOVSW(_T_,_S_)\
			 memcpy(&(_T_),&(_S_),sizeof(_T_));

#define  MOV4W(_T_,_S_) {   						\
			 typedef struct { char ll[8]; } L;	        \
			 *(L*)(&(_T_))=*(L*)(&(_S_));			\
			 }
#endif

#define COPYDOUBLE(_TT_,_SS_)		MOV4W(_TT_,_SS_)
#define COPYFLOAT(_TT_,_SS_)		MOV2W(_TT_,_SS_)

#endif
