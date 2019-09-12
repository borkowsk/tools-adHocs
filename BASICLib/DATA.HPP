/*========================================================*/
/*  		  DATA externals and function		  */
/*========================================================*/

#ifndef __DATA_HPP__
#define	__DATA_HPP__  1

extern char  _data_table[];
extern char* _data_pointer;
#define RESTORE  {_data_pointer=_data_table;}
#define READDATA read_data_table

inline
void read_data_table(double& d)
{
char* end;
d=strtod(_data_pointer,&end);
_data_pointer=end+1;
}

inline
void read_data_table(float& d)
{
char* end;
d=strtod(_data_pointer,&end);
_data_pointer=end+1;
}

inline
void read_data_table(long int& d)
{
char* end;
d=strtol(_data_pointer,&end,0);
_data_pointer=end+1;
}

inline
void read_data_table(int& d)
{
char* end;
d=strtol(_data_pointer,&end,0);
_data_pointer=end+1;
}

#ifdef __BSTRING_HPP__
inline
void read_data_table(bstring& b)
{
b.clear();
b.stat=1;
b=_data_pointer;// statycznie - bez kopiowania
_data_pointer+=b.len+1;
}
#endif

#endif