#include <iostream.h>
#include <values.h>

#define    Pi        3.141593
#define  _2Pi        (2*Pi)
#define  _05Pi       (Pi*0.5)

inline double rad(double g)//Konwersja ze stopni na radiany
	{ return g*_2Pi/360. ;}

template<class baseuint>
class angle
{
public:
static baseuint  MAX() //Max angle = 2 * Pi
       { return (baseuint)((unsigned long)MAXLONG|HIBITL);   }

static double  MAXdiv2Pi()   // do konwersji z radianow
       { return  MAX()/_2Pi; }

static double _2PidivMAX()   // do konwersji na radiany
       { return  _2Pi/MAX(); }

private:
baseuint val;
public:
	angle()
	{ val=0; }

	angle(double R)   // konwersja z radianow
	{ val= MAXdiv2Pi()*R;}

operator double()         // konwersja na radiany
	{ return _2PidivMAX()*val; }

angle& operator += (angle& b)// powiekszanie katow
	{ val+=b.val;
	  return *this; }

angle& operator -= (angle& b)// pomiejszanie katow
	{ val+=b.val;
	  return *this; }

angle& operator *= (baseuint m)// pomnazanie katow
	{ val*=m;
	  return *this; }

friend angle operator + (angle a,angle& b)// sumowanie katow
	{ a.val+=b.val;
	  return a; }

friend angle operator - (angle a,angle& b)// roznica katow
	{ a.val-=b.val;
	  return a; }

friend angle operator * (angle a,baseuint m)// mnozenie kata i liczby
	{ a.val*=m;
	  return a; }

friend angle operator * (baseuint m,angle a)// mnozenie liczby i kata
	{ a.val*=m;
	  return a; }

friend double grad(angle& aa)// przeksztalcenie na stopnie
	{ return aa.val*360./MAX(); }

angle& grad(double vv)  // przeksztalcenie ze stopni
	{ val=vv*MAX()/360.;
	  return *this;}

friend ostream& operator << (ostream& s,angle& a)// wyprowadzenie
	{ s<<(double)a;
	  return s;   }

friend istream& operator >> (istream& s,angle& a)// wprowadzanie
	{
          double pom;
          s>>pom;
          a=pom;
	  return s;   }

};




