/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯

          Modu’ obs’ugi pami‘ci XMS (eXtended Memory Specification)
                        wersja 1.1 dla Turbo C 2.0
                    Napisany przez Krzysztofa Klimczaka
                               1992.09

                          Program przykladowy (XMSCEX.C)

  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

#include <dos.h>
#include <stdio.h>
#include <string.h>

#include "xms.h"

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
  Program przyk’adowy, demonstruj†cy kilka operacji zwi†zanych z pamieci† XMS
  Zwaracam uwag‘ na u§ywanie bufor¢w typu "char far *"
                     Krzysztof Klimczak    Warszawa 1992
  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

void main(void)
{
  unsigned Handle;
  char far *Buffer="Test pamieci XMS";
  unsigned long Length=_fstrlen(Buffer)+1;

  printf("\n\nInicjacja: %d\n",XMSInit());
  if (XMSError)
    printf("\nXMSError no : %d - %s\n",XMSError,XMSGetError(XMSError));
  else
  {
    printf("\n\nDostepna pamiec XMS : %d kb.\n",XMSMemAvail());
    if (XMSError)
      printf("\nAvail: XMSError no : %d - %s\n",XMSError,XMSGetError(XMSError));
    Handle=XMSAllocate(2);
    if (XMSError)
      printf("\nAllocate: XMSError no : %d - %s\n",XMSError,XMSGetError(XMSError));
    else
    {
      printf("Dostepna pamiec XMS po allokacji: %d kb.\n",XMSMaxAvail());
      printf("\nBufor przed zapisem do XMS: %Fs\n",Buffer);
      MoveToXMS(Handle,Buffer,Length);
      Buffer="Bufor zmieniony";
      printf("\nPo zmianie zawartosci bufora: %Fs\n",Buffer);
      MoveFromXMS(Handle,Buffer,Length);
      printf("\nBufor po odczycie z XMS: %Fs\n",Buffer);
      printf("\nStatus\nXMSError no : %d - %s\n",XMSError,XMSGetError(XMSError));
      XMSFree(Handle);
      printf("\nDostepna pamiec XMS : %d kb.\n",XMSMaxAvail());
    }
  }
}

