
for(i=0;i<LL->bufnum;i++)
	{
	for(j=0;j<20;j+=2)
		sprintf(wzor+j,"%2u",i);
	clreol();
	cprintf("For %2u ",i);
	x=wherex();
	s=p;
	while(_fmemcmp(s,(void far*)wzor,20)!=0)
		       {
		       s+=16;
		       gotoxy(x,wherey());
		       cprintf("%Fp",s);
		       if( FP_OFF(s)>=0xfff0 ) break;
		       }
	gotoxy(x,wherey());
	if(FP_OFF(s)>=0xfff0)
		cprintf("Bufer NOT detected .\r\n");
		else
		{
		cprintf("Bufer DETECTED at %Fp\r\n",s);
		cprintf("%512.512Fs\r\n",s);
		}
	}
