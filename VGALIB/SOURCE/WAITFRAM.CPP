#include <dos.h>
#include "biosgraf.h"
void wait_for_frame()
{
unsigned adr=peek(0,0x463)+6;
asm mov dx,adr;

PET1:
asm{
in al,dx
test al,8
jnz PET1
}

PET2:
asm{
in al,dx
test al,8
jnz PET2
}

}