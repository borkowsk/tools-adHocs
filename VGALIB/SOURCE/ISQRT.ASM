	ifndef	??version
?debug	macro
	endm
publicdll macro	name
	public	name
	endm
$comm	macro	name,dist,size,count
	comm	dist name:BYTE:count*size
	endm
	else
$comm	macro	name,dist,size,count
	comm	dist name[size]:BYTE:count
	endm
	endif
	?debug	S "..\scc\vgatest\isqrt.c"
	?debug	C E911A3561B162E2E5C7363635C766761746573745C69737172742E+
	?debug	C 63
	?debug	C E90010971614443A5C42435C494E434C5544455C6D6174682E68
_TEXT	segment byte public 'CODE'
_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
_TEXT	segment byte public 'CODE'
   ;	
   ;	unsigned int isqrt(unsigned long int par)
   ;	
	assume	cs:_TEXT
_isqrt	proc	near
	push	bp
	mov	bp,sp
	dec	sp
	dec	sp
	push	si
	push	di
   ;	
   ;	{
   ;	unsigned int Max=0xFFFF,Min=0x0,Bis;
   ;	
	mov	word ptr [bp-2],65535
	xor	si,si
@1@50:
   ;	
   ;	do
   ;	{
   ;	Bis=(Max+Min)>>1;
   ;	
	mov	ax,word ptr [bp-2]
	add	ax,si
	shr	ax,1
	mov	di,ax
   ;	
   ;	if( (unsigned long)Bis*Bis<=par ) Min=Bis;
   ;	
	mov	bx,di
	xor	cx,cx
	mov	ax,di
	xor	dx,dx
	call	near ptr N_LXMUL@
	cmp	dx,word ptr [bp+6]
	ja	short @1@146
	jne	short @1@122
	cmp	ax,word ptr [bp+4]
	ja	short @1@146
@1@122:
	mov	si,di
	jmp	short @1@170
@1@146:
   ;	
   ;		else	  Max=Bis;
   ;	
	mov	word ptr [bp-2],di
@1@170:
   ;	
   ;	}
   ;	while( Min!=Max-1 );
   ;	
	mov	ax,word ptr [bp-2]
	dec	ax
	cmp	ax,si
	jne	short @1@50
   ;	
   ;	return Min;
   ;	
	mov	ax,si
	jmp	short @1@218
@1@218:
   ;	
   ;	
   ;	/*if( par-(unsigned int)Min*Min < (unsigned int)Max*Max-par )
   ;		return Min;
   ;		else
   ;		return Max; */
   ;	}
   ;	
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_isqrt	endp
   ;	
   ;	unsigned int _c_isqrt(unsigned long int par)
   ;	
	assume	cs:_TEXT
__c_isqrt	proc	near
	push	bp
	mov	bp,sp
	sub	sp,8
   ;	
   ;	{
   ;	return sqrt(par);/* Coprocesor */
   ;	
	mov	ax,word ptr [bp+6]
	mov	dx,word ptr [bp+4]
	mov	word ptr [bp-8],dx
	mov	word ptr [bp-6],ax
	mov	word ptr [bp-4],0
	mov	word ptr [bp-2],0
	fild	qword ptr [bp-8]
	add	sp,65528
	fstp	qword ptr [bp-16]
	fwait	
	call	near ptr _sqrt
	add	sp,8
	call	near ptr N_FTOL@
	jmp	short @2@50
@2@50:
   ;	
   ;	}
   ;	
	mov	sp,bp
	pop	bp
	ret	
__c_isqrt	endp
	?debug	C E9
_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	extrn	_sqrt:near
	public	__c_isqrt
	extrn	N_LXMUL@:far
	extrn	N_FTOL@:far
	public	_isqrt
	end
