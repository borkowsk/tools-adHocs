bstring finps(FILE *file)
{
   char z;
   bstring pom(256);
   int i=0,j=0,k=0,l=0,t=0;

   for(i=0; i<256; i++)
   {
      if((z = fgetc(file)) == EOF) onerror(130);
      if(z == ',' || z == '\n')
	 break;
      else
      if(z == 32 && t > 0)
      {
	   l++;
      }
      else
      {
	if(z != 32 && l)
	 {
	    for(j=k; j<k+l; j++)
	       pom.text[j] = 32;
	    k+=l;
	    l=0;
	 }
	pom.text[k] = z;
	if(z != 32 || t == 1)
	{
	   t = 1;
	   k++;
	}
      }
   }
   pom.text[k] = NULL;
   pom.len = k;
   return pom;
}
bstring finps(FILE *file)
{
   char z;
   bstring pom(256);
   int i=0,j=0,k=0,l=0,t=0;

   for(i=0; i<256; i++)
   {
      if((z = fgetc(file)) == EOF) onerror(130);
      if(z == ',' || z == '\n')
	 break;
      else
      if(z == 32 && t > 0)
      {
	   l++;
      }
      else
      {
	if(z != 32 && l)
	 {
	    for(j=k; j<k+l; j++)
	       pom.text[j] = 32;
	    k+=l;
	    l=0;
	 }
	pom.text[k] = z;
	if(z != 32 || t == 1)
	{
	   t = 1;
	   k++;
	}
      }
   }
   pom.text[k] = NULL;
   pom.len = k;
   return pom;
}
bstring inps()
{
   char z=NULL, c[257];
   int k=0, x=wherex();

   while(1)
   {
      z=getchar();
      if(z == ',' || z == 10 || k == 256)
	   break;
      else
	  {
		if(!(z==32 && k == 0) && z > 31)
		{
		  c[k] = z; k++;
		}
	  }
   }
   c[k] = NULL;
   gotoxy(x+k,wherey()-1);
   return (bstring) c;
}
bstring linps()
{
   char z=NULL, c[257];
   int k=0;

   while(1)
   {
      z=getchar();
      if(z == 10 || k == 256)
	   break;
      else
	  {
		if(!(z==32 && k == 0) && z > 31)
		{
		  c[k] = z; k++;
		}
	  }
   }
   c[k] = NULL;
   return (bstring) c;
}
