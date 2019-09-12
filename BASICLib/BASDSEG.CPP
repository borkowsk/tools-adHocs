#include "basic.hpp"

unsigned _def_seg_;

void _init_def_seg(void)
{
 _def_seg_=FP_SEG(&_def_seg_);//DGROUP
 return ;
 __emit__((char)('B'^93),(char)('A'^93),(char)('S'^93),(char)('2'^93),(char)('C'^93));
}

#pragma startup   _init_def_seg



