/*
	Header biblioteki funkcji graficznych dla karty VGA
*/
#ifndef _INC_TOPVGA_H
#define _INC_TOPVGA_H

#ifdef __cplusplus
extern "C" {
#endif

void   far SetPixel13H(long int x , long int y ,unsigned int color);
void   far SetPixel320x400(long int x , long int y ,unsigned int color);
void   far SetPixel16C(long int x , long int y ,unsigned int color);
void   far SetPixelPRA(long int x , long int y ,unsigned int color);
void   far SetPixelVESA(long int x , long int y ,unsigned int color);

void  far Line13H(long int x1, long int y1,long int x2, long int y2);
void  far Line320x400(long int x1, long int y1,long int x2, long int y2);
void  far Line16C(long int x1, long int y1,long int x2, long int y2);
void  far LinePRA(long int x1, long int y1,long int x2, long int y2);
void  far LineVESA(long int x1, long int y1,long int x2, long int y2);

void  far Cleardevice13H();
void  far Cleardevice320x400();
void  far Cleardevice16C();
void  far CleardevicePRA();
void  far CleardeviceVESA();

void  far ClearBar13H(long int x1, long int y1,long int x2, long int y2);
void  far ClearBar320x400(long int x1, long int y1,long int x2, long int y2);
void  far ClearBar16C(long int x1, long int y1,long int x2, long int y2);
void  far ClearBarPRA(long int x1, long int y1,long int x2, long int y2);
void  far ClearBarVESA(long int x1, long int y1,long int x2, long int y2);

void  far FillBar13H(long int x1, long int y1,long int x2, long int y2);
void  far FillBar320x400(long int x1, long int y1,long int x2, long int y2);
void  far FillBar16C(long int x1, long int y1,long int x2, long int y2);
void  far FillBarPRA(long int x1, long int y1,long int x2, long int y2);
void  far FillBarVESA(long int x1, long int y1,long int x2, long int y2);

void  far GetMap13H(long int x1, long int y1,int n,int m,unsigned char far* bufor);
void  far GetMap320x400(long int x1, long int y1,int n,int m,unsigned char far* bufor);
void  far GetMap16C(long int x1, long int y1,int n,int m,unsigned char far* bufor);
void  far GetMapPROA(long int x1, long int y1,int n,int m,unsigned char far* bufor);
void  far GetMapVESA(long int x1, long int y1,int n,int m,unsigned char far* bufor);

void  far PutMap13H(long int x1, long int y1,int n,int m,unsigned char far* bufor);
void  far PutMap320x400(long int x1, long int y1,int n,int m,unsigned char far* bufor);
void  far PutMap16C(long int x1, long int y1,int n,int m,unsigned char far* bufor);
void  far PutMapPROA(long int x1, long int y1,int n,int m,unsigned char far* bufor);
void  far PutMapVESA(long int x1, long int y1,int n,int m,unsigned char far* bufor);

void  far Circle13H(long int xxs,long int yys,long int bb);
void  far Circle320x400(long int xxs,long int yys,long int bb);
void  far Circle16C(long int xxs,long int yys,long int bb);
void  far CirclePRA(long int xxs,long int yys,long int bb);
void  far CircleVESA(long int xxs,long int yys,long int bb);

void  far Ellipse13H(long int xxs,long int yys,long int aa,long int bb);
void  far Ellipse320x400(long int xxs,long int yys,long int aa,long int bb);
void  far Ellipse16C(long int xxs,long int yys,long int aa,long int bb);
void  far EllipsePRA(long int xxs,long int yys,long int aa,long int bb);
void  far EllipseVESA(long int xxs,long int yys,long int aa,long int bb);

void  far FillCircle13H(long int xxs,long int yys,long int bb);
void  far FillCircle320x400(long int xxs,long int yys,long int bb);
void  far FillCircle16C(long int xxs,long int yys,long int bb);
void  far FillCirclePRA(long int xxs,long int yys,long int bb);
void  far FillCircleVESA(long int xxs,long int yys,long int bb);

void  far FillEllipse13H(long int xxs,long int yys,long int aa,long int bb);
void  far FillEllipse320x400(long int xxs,long int yys,long int aa,long int bb);
void  far FillEllipse16C(long int xxs,long int yys,long int aa,long int bb);
void  far FillEllipsePRA(long int xxs,long int yys,long int aa,long int bb);
void  far FillEllipseVESA(long int xxs,long int yys,long int aa,long int bb);

#define RunBIOS 0
#define Run16C  1
#define Run13H  2
#define RunPRA  3
#define RunVESA 4
extern char  GRSERVICE;

void  far graphicsBIOS();
void  far graphics16C();
void  far graphics13H();
void  far graphics320x400();
void  far graphicsPRA();
void  far graphicsVESA();
void  far ManualSetService();

extern void  far (*setpixel)(long int x , long int y ,unsigned int color);
extern void  far (*line)(long int x1, long int y1,long int x2, long int y2);
extern void  far (*cleardevice)(void);
extern void  far (*clearbar)(long int x1, long int y1,long int x2, long int y2);
extern void  far (*fillbar)(long int x1, long int y1,long int x2, long int y2);
extern void  far (*getmap)(long int x1, long int y1,int n,int m,unsigned char far* bufor);
extern void  far (*putmap)(long int x1, long int y1,int n,int m,unsigned char far* bufor);
extern void  far (*circle)(long int xxs,long int yys,long int bb);
extern void  far (*ellipse)(long int xxs,long int yys,long int aa,long int bb);
extern void  far (*fillcircle)(long int xxs,long int yys,long int bb);
extern void  far (*fillellipse)(long int xxs,long int yys,long int aa,long int bb);

#ifdef __cplusplus
};//extern "C" {
#endif

#endif

