#define USES_FSTREAM
#define USES_key_iterator
#define USES_cgi_config
#define USES_sstring
#include "uses_wb.hpp"

typedef  ostream html_output;
static const  char* html_begin="Content type: text/html\n\n<HTML>\n";
static const  char* html_end="</HTML>\n";
static const  char* html_ln="<br>\n";
static const  char* html_hr="<hr>\n";
class html_object;
class html_title;
class html_pre;
class html_text;
class html_italic;
class html_bold;
class html_head;
class html_anchor;
class html_image;
class html_graphics_anchor;

class html_object
{
};

class html_title:public  html_object
{
sstring ptext;
public:
html_title(const sstring& itext)    {ptext="<title>";ptext+=itext;ptext+="</title>";}
operator const sstring& () const  {return ptext; }
friend html_output& operator << (html_output& o,const html_title& h)
				{o<<(const char*)(h.ptext);return o;}
};

class html_pre:public  html_object
{
sstring ptext;
public:
html_pre(const sstring& itext)    {ptext="<pre>";ptext+=itext;ptext+="</pre>";}
operator const sstring& () const  {return ptext; }
friend html_output& operator << (html_output& o,const html_pre& h)
				{o<<(const char*)(h.ptext);return o;}
};

class html_text:public  html_object
{
sstring ptext;
public:
html_text(const sstring& itext) {ptext=itext;}
operator const sstring& () const {return ptext; }
friend html_output& operator << (html_output& o,const html_text& h)
				{o<<(const char*)(h.ptext);return o;}
};


class html_italic:public  html_object
{
sstring ptext;
public:
html_italic(const sstring& itext) {ptext="<i>";ptext+=itext;ptext+="</i>";}
operator const sstring& () const {return ptext; }
friend html_output& operator << (html_output& o,const html_italic& h)
				{o<<(const char*)(h.ptext);return o;}
};

class html_bold:public  html_object
{
sstring ptext;
public:
html_bold(const sstring& itext)   {ptext="<b>";ptext+=itext;ptext+="</b>";}
operator const sstring& () const {return ptext; }
friend html_output& operator << (html_output& o,const html_bold& h)
				{o<<(const char*)(h.ptext);return o;}
};

class html_head:public  html_object
{
sstring ptext;
int size;
public:
html_head(int isize,const sstring& itext)
{
char HH[12];
size=isize;
sprintf(HH,"h%d",size);
ptext ='<';ptext+=HH;ptext+='>';
ptext+=itext;
ptext+="</";ptext+=HH;ptext+='>';
}
operator const sstring& () const {return ptext; }
friend html_output& operator << (html_output& o,const html_head& h)
				{o<<(const char*)(h.ptext);return o;}
};

class html_anchor:public  html_object
{
sstring ptext;
public:
html_anchor(const sstring url,const sstring& name="")
{
ptext="<a href=\"";ptext+=url;ptext+="\">";
if(name!="")
       ptext+=name;
       else
       ptext+=url;
ptext+="</a>";
}
operator const sstring& () const {return ptext; }
friend html_output& operator << (html_output& o,const html_anchor& h)
				{o<<(const char*)(h.ptext);return o;}
};

class html_image:public html_object
{
sstring ptext;
public:
html_image(const sstring url,const sstring& attribs="")
{
ptext="<img src=\"";ptext+=url;ptext+="\" ";
ptext+=attribs;
ptext+=" >";
}
operator const sstring& ()  const {return ptext; }
friend html_output& operator << (html_output& o,const html_image& h)
				{o<<(const char*)(h.ptext);return o;}

};

class html_graphics_anchor:public  html_object
{
sstring ptext;
public:
html_graphics_anchor(const sstring url,const sstring image,const sstring& name="")
{
ptext="<a href=\"";ptext+=url;ptext+="\">";
ptext+="<img src=\"";ptext+=image;ptext+="\" ismap >";
if(name!="")
       ptext+=name;
ptext+="</a>";
}
html_graphics_anchor(const sstring url,const html_image& image,const sstring& name="")
{
ptext="<a href=\"";ptext+=url;ptext+="\">";
ptext+=image;
if(name!="")
       ptext+=name;
ptext+="</a>";
}
operator const sstring& () const {return ptext; }
friend html_output& operator << (html_output& o,const html_graphics_anchor& h)
				{o<<(const char*)(h.ptext);return o;}
};

/* Declaration for cgi_main */
int cgi_main(cgi_config&,html_output&);
