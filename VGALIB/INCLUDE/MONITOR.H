/* Wlaczanie i wylaczanie wyswietlania obrazu na monitorze */

static 
void near monitor_off()
{
outp(0x3c4,0x1);
outp(0x3c5,inp(0x3c5)|0x20); /* Ustawiony bit 5 */
}

static
void near monitor_on()
{
outp(0x3c4,0x1);
outp(0x3c5,inp(0x3c5)&0xDF); /* Wyzerowany bit 5 */
}
