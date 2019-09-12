/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯

          Modu’ obs’ugi pami‘ci XMS (eXtended Memory Specification)
                        wersja 1.1 dla Turbo C 2.0
                    Napisany przez Krzysztofa Klimczaka
                               1992.09

                Plik definiujacy wszystkie procedury (XMSC.C)

  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

#include <dos.h>
#include <stdio.h>
#include <string.h>

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
           Deklaracje i definicje zmiennych i struktur danych
 ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

void far (*XMSSub)(void);

typedef struct
{
  long unsigned Length;
       unsigned SourceHandle;
  void far *    SourceOffset;
       unsigned DestinHandle;
  void far *    DestinOffset;
} XMSParamBlock;

static int Present;
       int XMSError=0;

static unsigned char XMSErrorsCode[]={
0x01,0x80,0x81,0x82,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,0xa0,0xa1,0xa2,
0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xb0,0xb1,0xb2};

static char *XMSErrors[]={
"No error",
"Unknown error",
"No XMM installed",
"Function no implemented",
"VDISK device driver was detected",
"A20 error occured",
"General driver errror",
"Unrecoverable driver error",
"High memory area does not exist",
"High memory area is already in use",
"DX is less than the ninimum of KB that program may use",
"High memory area not allocated",
"A20 line still enabled",
"All extended memory is allocated",
"Extended memory handles exhausted",
"Invalid handle",
"Invalid source handle",
"Invalid source offset",
"Invalid destination handle",
"Invalid destination offset",
"Invalid length",
"Invalid overlap in move request",
"Parity error detected",
"Block is not locked",
"Block is locked",
"Lock count overflowed",
"Lock failed",
"Smaller UMB is available",
"No UMBs are available",
"Inavlid UMB segment number"};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
                  Implementacja w’ažciwych funkcji
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int  XMMPresent( void )
{
  _AX=0x4300;
  geninterrupt(0x2f);
  _AH=0;
  return( _AX );
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

unsigned GetXMSVersion( void )
{
  unsigned version;

  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    _AH=0;
    XMSSub();
    _CX=_AX;
    version=_CH*100 + _CL;
    XMSError= version ? 0 : _BL;
    return( version );
  }
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

unsigned GetXMMVersion( void )
{
  unsigned version;

  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    _AH=0;
    XMSSub();
    _CX=_AX;
    version=_BH*100 + _BL;
    XMSError= _CX ? 0 : _BL;
    return( !XMSError ? version : 0 );
  }
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

unsigned XMSMemAvail( void )
{
  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    _AH=8;
    XMSSub();
    _CX=_AX;
    XMSError= _CX ? 0 : _BL;
    return( _DX );
  }
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

unsigned XMSMaxAvail( void )
{

  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    _AL=0;
    _AH=8;
    XMSSub();
    _CX=_AX;
    XMSError= _CX ? 0 : _BL;
    return( _CX );
  }
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

unsigned XMSAllocate(unsigned BlockSize)
{
  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    _AH=9;
    _DX=BlockSize;
    XMSSub();
    _CX=_AX;
    XMSError= _CX ? 0 : _BL;
    return( _CX ? _DX : 0 );
  }

};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int XMSFree(unsigned EMBHandle)
{
  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    _AH=0xa;
    _DX=EMBHandle;
    XMSSub();
    _CX=_AX;
    XMSError= _CX ? 0 : _BL;
    return( XMSError );
  }

};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int MoveToXMS(unsigned EMBHandle, void far *Source, unsigned long BlockLength)
{
  XMSParamBlock Parameters;
  void far *XMSPBPointer=&Parameters;

  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    Parameters.Length=(BlockLength % 2) ? (BlockLength+1) : BlockLength;
    Parameters.SourceHandle=0;
    Parameters.SourceOffset=Source;
    Parameters.DestinHandle=EMBHandle;
    Parameters.DestinOffset=0;
    _SI=*(unsigned *)(&XMSPBPointer);
    _DX=*((unsigned *)(&XMSPBPointer) + 1);
    asm push DS;
    asm push ES;
    _AX=_DS;
    _ES=_AX;
    _DS=_DX;
    _AH=0xb;
    asm call dword ptr ES:XMSSub;
    _CX=_AX;
    asm pop  ES;
    asm pop  DS;
    XMSError= _CX ? 0 : _BL;
    return( XMSError );
  }
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int MoveFromXMS(unsigned EMBHandle, void far *Destin, unsigned long BlockLength)
{
  XMSParamBlock Parameters;
  void far *XMSPBPointer=&Parameters;

  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    Parameters.Length=(BlockLength % 2) ? (BlockLength+1) : BlockLength;
    Parameters.SourceHandle=EMBHandle;
    Parameters.SourceOffset=0;
    Parameters.DestinHandle=0;
    Parameters.DestinOffset=Destin;
    _SI=*(unsigned *)(&XMSPBPointer);
    _DX=*((unsigned *)(&XMSPBPointer) + 1);
    asm push DS;
    asm push ES;
    _AX=_DS;
    _ES=_AX;
    _DS=_DX;
    _AH=0xb;
    asm call dword ptr ES:XMSSub;
    _CX=_AX;
    asm pop  ES;
    asm pop  DS;
    XMSError= _CX ? 0 : _BL;
    return( XMSError );
  }
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int XMSResize(unsigned EMBHandle, unsigned NewBlockSize)
{
  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    _AH=0xf;
    _DX=EMBHandle;
    _BX=NewBlockSize;
    XMSSub();
    _CX=_AX;
    XMSError= _CX ? 0 : _BL;
    return( XMSError );
  }

};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

unsigned long LockEMB( unsigned EMBHandle )
{
  unsigned long Address;

  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    _AH=0xc;
    _DX=EMBHandle;
    XMSSub();
    _CX=_AX;
    *((unsigned *)(&Address))=_BX;
    *((unsigned *)(&Address) + 1)=_DX;
    XMSError= _CX ? 0 : _BL;
    return( XMSError ? 0 : Address);
  }
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int UnlockEMB( unsigned EMBHandle )
{
  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    _AH=0xd;
    _DX=EMBHandle;
    XMSSub();
    _CX=_AX;
    XMSError= _CX ? 0 : _BL;
    return( XMSError );
  }
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

unsigned EMBGetSize( unsigned EMBHandle )
{
  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    _AH=0xe;
    _DX=EMBHandle;
    XMSSub();
    _CX=_AX;
    XMSError= _CX ? 0 : _BL;
    return( XMSError ? 0 : _DX );
  }
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int EMBGetLock( unsigned EMBHandle )
{
  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    _AH=0xe;
    _DX=EMBHandle;
    XMSSub();
    _CX=_AX;
    XMSError= _CX ? 0 : _BL;
    _BL=_BH;
    _BH=0;
    return( XMSError ? 0 : _BX );
  }
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int EMBGetAvailHands( unsigned EMBHandle )
{
  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    _AH=0xe;
    _DX=EMBHandle;
    XMSSub();
    _CX=_AX;
    XMSError= _CX ? 0 : _BL;
    _BH=0;
    return( XMSError ? 0 : _BX );
  }
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int HMAStatus( void )
{

  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    _AH=0;
    XMSSub();
    _CX=_AX;
    XMSError= _CX ? 0 : _BL;
    return( _DX );
  }
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int HMAAllocate( unsigned Size )
{
  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    _AH=1;
    _DX=Size;
    XMSSub();
    _CX=_AX;
    XMSError= _CX ? 0 : _BL;
    return( XMSError );
  }
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int XMSFunction( char FunctionNo )
{
  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    _AH=FunctionNo;
    XMSSub();
    _CX=_AX;
    XMSError= _CX ? 0 : _BL;
    return( XMSError );
  }
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int HMAFree( void )
{
  return (XMSFunction(2));
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int A20GlobalEnable( void )
{
  return (XMSFunction(3));
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int A20GlobalDisable( void )
{
  return (XMSFunction(4));
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int A20LocalEnable( void )
{
  return (XMSFunction(5));
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int A20LocalDisable( void )
{
  return (XMSFunction(6));
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int A20GetStatus( void )
{
  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    _AH=7;
    XMSSub();
    _CX=_AX;
    XMSError= _CX ? 0 : _BL;
    _BH=0;
    if ( !_CX && _BX )
      return(XMSError);
    else
      return( _CX );
  }
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/


unsigned UMBAllocate( unsigned ParSize, unsigned *UMBSegment)
{
  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    _AH=0x10;
    _DX=ParSize;
    XMSSub();
    _CX=_AX;
    *UMBSegment=_BX;
    XMSError= _CX ? 0 : _BL;
    return( _DX );
  }
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int UMBFree( unsigned UMBSegment)
{
  if (!Present)
  {
    XMSError=1;
    return(0);
  }
  else
  {
    _AH=0x11;
    _DX=UMBSegment;
    XMSSub();
    _CX=_AX;
    XMSError= _CX ? 0 : _BL;
    return( XMSError );
  }
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

char *XMSGetError( int Error )
{
  char *Temp;

  if (Error)
    if ((Temp=strchr(XMSErrorsCode,Error)) != NULL)
      return (XMSErrors[(int)(Temp-XMSErrorsCode)+2]);
    else
      return (XMSErrors[1]);
  else
    return (XMSErrors[0]);
}

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int XMSInit( void )
{
  union  REGS  Reg;
  struct SREGS Sreg;

  if (!XMMPresent())
    return(Present=0);
  else
  {
  void far* pom;
    Reg.x.ax=0x4310;
    int86x(0x2f,&Reg,&Reg,&Sreg);
    pom=MK_FP(Sreg.es,Reg.x.bx);
    XMSSub=(void (far *)())pom;
    return(Present=1);
  }
};

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
                   Koniec implementacji funkcji zwi†zanych z XMS
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/
