#include "mouse.hpp"
#ifndef _INC_PRZYCISK_HPP 
#define _INC_PRZYCISK_HPP 
class pipek : Interface
{
int x,y,xd,yd;
int color,ramka;
int keychar,cliksens;
char* text;
public:
static void clear();
pipek();
int DoIt();
void InstallText(const char* t,int lkey,int lsens);
void set(int lx,int ly,int lxd, int lyd, int lcol , int lram);
void put();
void invers();
};
#endif