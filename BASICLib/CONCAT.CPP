
 /*
 << CONCAT.BAS >> converted to C++ by
 BASIC_TO_C TRANSLATOR, (Beta version),
 written by I. Moraczewski & W. Borkowski, 1993
 Dept. of Plant Systematics, Warsaw University
 Al. Ujazdowskie 4, 00-478 Warsaw, Poland; tel 628 25 94
 */
 #include <stdio.h>
 #include <stdlib.h>
 #include <conio.h>
 #include "basic.hpp"
 #include "onerror.hpp"
 #include "basstrin.hpp"
 /************* MAIN *************/

 void main()
 {
    bstring As;
    bstring Bs;
    bstring Cs;
    bstring Ds;
    bstring Es;

    _setcursortype(_NOCURSOR);
    printf("Program translated into C++ by BAS_TO_C, (c) I.M. & W.B. 1993. Press any key.\n");
    getch();
    clrscr();
    As = (bstring) "ala ";
    Bs = (bstring) "bebe ";
    Cs = (bstring) "hop ";
    Ds = (bstring) "sasa";
    Es = (bstring) " bumbum";
    As = concat(7,addr(As),addr(Bs),addr(Cs),addr(right(Es,4)),addr(Ds), addr(left(Es,4)), addr((bstring)"####!"));
    cprintf("%s", (char*) (As));
    if(wherex() > 1) printf("\n");
    getch();
 }
