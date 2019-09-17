//            Typ rodzajowy vector
//  Testowane dla kompilatora Borland C++ 2.0 

#include <iostream.h>
#include <math.h>

//  Makra do tworzenia identyfikatorow napisane wg pliku generic.h ;
//         Konieczne sa dwa poziomy odniesien
/******************************************************************/
#define _paste( z , y )       _paste_x( z , y )
#define _paste_x( z , y )     z##y


#if !defined(_TYPE_)
#define _TYPE_ double
#else
#define vector _paste(_TYPE_,_vector)
#endif

class vector
{
protected:
_TYPE_ x,y,z;

public:

vector();
// konstruktor bezparametrowy

vector(const _TYPE_ lx,const _TYPE_ ly,const _TYPE_ lz);
// konstruktor inicjujacy

void     erase();
// zerowanie

void     set(const _TYPE_ lx,const _TYPE_ ly,const _TYPE_ lz);
// ustawianie

_TYPE_   getX() const;		//odczyt skladowej X
_TYPE_   getY() const;		//odczyt skladowej Y
_TYPE_   getZ() const;          //odczyt skladowej Z

vector&  operator ++ ();               // inkrementacja
vector&  operator -- ();               // dekrementacja
vector&  operator = (const vector& p); // przypisanie
vector&  operator - ();	               // operator zmiany zwrotu

vector&  operator += (const vector& par) ;
// przypisanie z dodawaniem

vector&  operator -= (const vector& par) ;
// przypisanie roznicy wektorow

vector&  operator *= (const _TYPE_& par) ;
// przypisanie z mnozeniem przez skalar

vector&  operator ^= (const vector& par) ;
// przypisanie iloczynu wektorowego

vector&  operator /= (const _TYPE_& par) ;
// przypisanie z dzieleniem przez skalar

vector   operator ^ (vector  p);
// iloczyn wektorowy --//--

vector   operator + (vector  p);
// suma wpisywana na nowy wektor

vector   operator - (vector  p);
// roznica wpisywana na nowy wektor

friend
vector   operator * (vector v,const _TYPE_& p);
// iloczyn wektora i skalaru na nowy wektor

friend
vector   operator * (const _TYPE_& p,vector v);
//iloczyn skalaru i wectora na nowy wektor

friend
vector   operator / (vector v,const _TYPE_& p);
// iloraz wektora i skalaru na nowy wektor

friend
vector   operator / (const _TYPE_& p,vector v);
// iloraz skalaru i wectora na nowy wektor


_TYPE_   operator * (const vector& p);  // iloczyn skalarny
_TYPE_   d() const ;			// dlugosc vektora
_TYPE_   d2() const ;                   // kwadrat dlugosci vektora
_TYPE_   d3() const ;			// szescian dlugosci vectora

friend
ostream& operator << (ostream& out,vector& p);
// wyjscie na strumien

void     dump();
// wyjscie alternatywne
};

inline vector::vector()
	{
	x=0; y=0; z=0;
	}

inline vector::vector(const _TYPE_ lx,const _TYPE_ ly,const _TYPE_ lz)
	{
	x=lx; y=ly; z=lz;
	}

inline void vector::set
	(const _TYPE_ lx,const _TYPE_ ly,const _TYPE_ lz)
	{
	x=lx; y=ly; z=lz;
	}

inline void vector::erase()
	{
	x=0;y=0;z=0;
	}

inline vector& vector::operator = (const vector& p)
	{
	x=p.x; y=p.y; z=p.z;
	return *this;
	}

inline vector& vector::operator - ()
	{
	x=-x; y=-y; z=-z;
	return *this;
	}

inline vector& vector::operator++()
	{
	x++;  y++; z++;
	return *this;
	}

inline vector& vector::operator--()
	{
	x--;  y--; z--;
	return *this;
	}

inline vector& vector::operator += (const vector& par)
	{
	x+=par.x; y+=par.y; z+=par.z;
	return *this;
	}

inline vector& vector::operator -= (const vector& par)
	{
	x-=par.x; y-=par.y; z-=par.z;
	return *this;
	}

inline vector& vector::operator *= (const _TYPE_& par)
	{
	x*=par; y*=par; z*=par;
	return *this;
	}

inline vector& vector::operator ^= (const vector& par)
	{
	set(	y*par.z-z*par.y,
		z*par.x-x*par.z,
		x*par.y-y*par.x);
	return *this;
	}

inline vector& vector::operator /= (const _TYPE_& par)
	{
	  x/=par; y/=par; z/=par;
	  return *this;
	}

inline vector vector::operator ^ (vector par)
	{
	par.set(y*par.z-z*par.y,
		z*par.x-x*par.z,
		x*par.y-y*par.x);
	return par;
	}


inline vector   operator * (vector v,const _TYPE_& p)
	{
	v.x*=p; v.y*=p; v.z*=p;
	return v;
	}

inline vector   operator * (const _TYPE_& p,vector v)
	{
	v.x*=p; v.y*=p; v.z*=p;
	return v;
	}

inline vector vector::operator + (vector p)
	{
	  p.x=x+p.x; p.y=y+p.y; p.z=z+p.z;
	  return p;
	}

inline vector vector::operator - (vector p)
	{
	  p.x=x-p.x; p.y=y-p.y; p.z=z-p.z;
	  return p;
	}


inline vector   operator / (vector v,const _TYPE_& p)
	{
	  v.x/=p; v.y/=p; v.z/=p;
	  return v;
	}

inline vector   operator / (const _TYPE_& p,vector v)
	{
	v.x=p/v.x; v.y=p/v.y; v.z=p/v.z;
	return v;
	}

inline _TYPE_ vector::operator * (const vector& par)
	{
	return x*par.x + y*par.y + z*par.z;
	}

#if __BCPLUSPLUS__ <= 0x0200
#define Storage
#else
#define Storage inline
#endif

/*
Metody klopotliwe dla Borland C++ 2.0 -
- rozwijane jako inline powoduja bledy w optymalizacji kodu
*/

#if _VECTOR_DEFINITION_==1 || __BCPLUSPLUS__ > 0x0200

Storage	void vector::set
	(const _TYPE_ lx,const _TYPE_ ly,const _TYPE_ lz)
	 {
	 x=lx; y=ly; z=lz;
	 }

Storage _TYPE_ vector::getX() const
	{
	return x;
	}

Storage _TYPE_ vector::getY() const
	{
	return y;
	}

Storage _TYPE_ vector::getZ() const
	{
	return z;
	}
#endif

inline _TYPE_ vector::d()   const
	{
	_TYPE_ p=x*x+y*y+z*z;
	return sqrt(p);
	}

inline _TYPE_ vector::d2()  const
	{
	_TYPE_ p=x*x+y*y+z*z;
	return (p);
	}

inline _TYPE_ vector::d3()  const
	{
	_TYPE_ p=x*x+y*y+z*z;
	return pow( p , 3.0/2.0 );
	}

inline ostream& operator << (ostream& out,vector& p)
	{
	  out <<"{x:"<<p.x<<" y:"<<p.y<<" z:"<<p.z<<'}';
	  return out;
	}

inline void   vector::dump()
{
cout <<" x="<<x<<" y="<<y<<" z="<<z<<'\n';
}

#undef _TYPE_

#ifdef vector
#undef vector
#endif