struct {unsigned char R;
	unsigned char G;
	unsigned char B;} RGBarray[257],RGB;

for(k=0;k<256;k++)
   {RGB.R=0;
    RGB.G=0;
    RGB.B=0;
    RGBarray[k]=RGB;
    }
for(k=0;k<64;k++)
    {
    RGBarray[k].R=k;
    RGBarray[k+192].R=k;
    RGBarray[k+64].G=k;
    RGBarray[k+128].B=k;
    }
for(k=0;k<64;k++)
    {
    RGBarray[k+64].R=63-k;
    RGBarray[k+128].G=63-k;
    RGBarray[k+192].B=63;
    }
