/*========================================================*/
/* Default error handler - included only to one file      */
/*  (Main is the best)!   User can replace this function !*/
/*========================================================*/

#ifndef _ONERROR_HPP_
#define _ONERROR_HPP_  1
/*	 Assumed that previous include :
		stdio.h
		stdlib.h					       */

void cdecl onerror(int code)
{
//int flag=0;//for debuging only
fprintf(stderr,"Run time error %d\n",code);
if(errno!=0)
	perror("OS error");
if(code!=HpDisr)
	if(heapcheck()<0){fprintf(stderr," Heap checking failed !");abort();}
//if(flag) return;
fprintf(stderr,"Hit any key to return to system\n");
getch();
exit(code);
}

#endif