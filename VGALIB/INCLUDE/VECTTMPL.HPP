//       Typ szablonowy vector
// ( Kompilator: conajmniej Borland C++ 3.0 )

#ifndef _inc_vector_hpp_
#define	_inc_vector_hpp_

#include <iostream.h>
#include <math.h>

template<class _TYPE_>
class vector
{
protected:

_TYPE_ x,y,z;

public:

vector()
// konstruktor bezparametrowy
	{
	x=0; y=0; z=0;
	}

vector(const _TYPE_ lx,const _TYPE_ ly,const _TYPE_ lz)
// konstruktor inicjujacy
	{
	x=lx; y=ly; z=lz;
	}

void set(const _TYPE_ lx,const _TYPE_ ly,const _TYPE_ lz)
// ustawianie skladowych
	 {
	 x=lx; y=ly; z=lz;
	 }

void erase()
// zerowanie wektora
	{
	x=0;y=0;z=0;
	}

_TYPE_ getX() const
	{
	return x;
	}

_TYPE_ getY() const
	{
	return y;
	}

_TYPE_ getZ() const
	{
	return z;
	}

vector& operator++()
// inkrementacja wszystkich skladowych
	{
	x++;  y++; z++;
	return *this;
	}

vector& operator--()
// dekrementacja wszystkich skladowych
	{
	x--;  y--; z--;
	return *this;
	}

vector& operator = (const vector& par)
// przypisanie
	{
	x=par.x; y=par.y; z=par.z;
	return *this;
	}

vector& operator - ()
// zmiana zwrotu wektora
	{
	x=-x; y=-y; z=-z;
	return *this;
	}

vector& operator += (const vector& par)
// przypisanie z dodawaniem
	{
	x+=par.x; y+=par.y; z+=par.z;
	return *this;
	}

vector& operator -= (const vector& par)
// przypisanie z odejmowaniem
	{
	x-=par.x; y-=par.y; z-=par.z;
	return *this;
	}

vector& operator *= (const _TYPE_& par)
// przypisanie z mnozeniem przez skalar
	{
	x*=par; y*=par; z*=par;
	return *this;
	}

vector& operator ^= (const vector& par)
// przypisanie iloczynu wektorowego
	{
	set(	y*par.z-z*par.y,
		z*par.x-x*par.z,
		x*par.y-y*par.x);
	return *this;
	}

vector& operator /= (const _TYPE_& par)
// przypisanie z dzieleniem przez skalar
	{
	  x/=par; y/=par; z/=par;
	  return *this;
	}

vector operator ^ (vector par)
// iloczyn wektorowy
	{
	par.set(y*par.z-z*par.y,
		z*par.x-x*par.z,
		x*par.y-y*par.x);
	return par;
	}

vector operator + (vector par)
// suma wektorow
	{
	  par.x=x+par.x; par.y=y+par.y; par.z=z+par.z;
	  return par;
	}

vector operator - (vector p)
// roznica wektorow
	{
	  p.x=x-p.x; p.y=y-p.y; p.z=z-p.z;
	  return p;
	}

friend
vector   operator * (vector v,const _TYPE_& par)
// iloczyn wektora i skalara  (wektor*skalar)
	{
	v.x*=par; v.y*=par; v.z*=par;
	return v;
	}

friend
vector   operator * (const _TYPE_& par,vector v)
// iloczyn skalara i wektora   (skalar*wektor)
	{
	v.x*=par; v.y*=par; v.z*=par;
	return v;
	}

friend
vector   operator / (vector v,const _TYPE_& par)
// iloraz wektora i skalara  (wektor/skalar)
	{
	  v.x/=par; v.y/=par; v.z/=par;
	  return v;
	}

friend
vector   operator / (const _TYPE_& p,vector v)
// iloraz skalara i wektora   (skalar/wektor)
	{
	v.x=p/v.x; v.y=p/v.y; v.z=p/v.z;
	return v;
	}

_TYPE_ operator * (const vector& par)
// iloczyn skalarny dwu wektorow
	{
	return x*par.x + y*par.y + z*par.z;
	}

_TYPE_ d()   const
// dlugosc wektora
	{
	_TYPE_ p=x*x+y*y+z*z;
	return sqrt(p);
	}

_TYPE_ d2()  const
// kwadrat dlugosci wektora
	{
	_TYPE_ p=x*x+y*y+z*z;
	return p;
	}

_TYPE_ d3()  const
// szescian dlugosci wektora
	{
	_TYPE_ p=x*x+y*y+z*z;
	return pow( p , 3.0/2.0 );
	}

friend
ostream& operator << (ostream& out,vector& par)
// operator wyprowadzania na dowolny strumien wyjsciowy
	{
	  out <<"{x:"<<par.x<<" y:"<<par.y<<" z:"<<par.z<<" }";
	  return out;
	}

void   dump()
//alternatywna metoda wyprowadzania na strumien standartowego wyjscia
	 {
	 cout <<" x="<<x<<" y="<<y<<" z="<<z<<'\n';
	 }

};

#endif