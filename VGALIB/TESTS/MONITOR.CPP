#include <dos.h>
#include <stdio.h>
#include <conio.h>

void monitor_off();
void monitor_on();

int main()
{
while(kbhit())
	getch();
delay(500);
monitor_off();
do{
}while(!kbhit());
getch();
monitor_on();
}

void monitor_off()
{
outp(0x3c4,0x1);
outp(0x3c5,inp(0x3c5)|0x20); /* Ustawiony bit 5 */
}

void monitor_on()
{
outp(0x3c4,0x1);
outp(0x3c5,inp(0x3c5)&0xDF); /* Wyzerowany bit 5 */
}



