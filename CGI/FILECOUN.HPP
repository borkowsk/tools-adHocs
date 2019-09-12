#ifndef __FILECOUNT_HPP__
#define __FILECOUNT_HPP__

class file_counter
{
void* f;
int ecode;
public:
file_counter(const char*name);
int is_ok(){ return ecode==0 && f!=0;}
int get_error(){int pom=ecode;ecode=0;return pom;}
~file_counter();
void operator ++ ();
void reset();
operator unsigned long ();
};

#endif