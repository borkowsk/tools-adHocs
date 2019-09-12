#define USES_IO
#define USES_gd_image_handle
#define USES_directory_iterator
#define USES_config_resources
#define USES_regular_expression
#include "wb_cgi.hpp"
#include "filecoun.hpp"

const HARD_PREVIEW_LIMIT=20;
const HARD_FULL_LIMIT=5;

#ifdef __MSDOS__
static const  char* EXTENDED_ATR_FILE="~names.ext";
#else
static const  char* EXTENDED_ATR_FILE=".names.ext";
#endif

enum do_action { TEXT_ONLY=1, TEXT_LNK=2, PREV_LNK=3, FULL_LNK=4 };
int do_subdir(
	const char**		pom,
	directory_iterator& 	currdir,
	cgi_config& 		rc,
	html_output& 		hout
	)
{
const char* pom2=(pom==NULL?*currdir:*pom);
hout<<html_anchor(
	sstring(rc("SELF_URL_PATH"))+rc("PATH_INFO")+'/'+sstring(*currdir)+"?NAME_PATTERN=*",
	pom2)<<html_ln;
return TEXT_LNK;
}

int do_item(
	const char**		pom,
	directory_iterator& 	currdir,
	cgi_config& 		rc,
	html_output& 		hout,
	do_action		act
	)
{
const char* pom2=(pom==NULL?*currdir:*pom);
const sstring Source=sstring(currdir.get_name())+'/'+(*currdir);
const sstring FullExport=rc("SERVER_URL_PATH")+
			sstring(rc["FULL_GIF_DIRECTORY"]);
const sstring ViewExport=rc("SERVER_URL_PATH")+
			sstring(rc["PREVIEW_GIF_DIRECTORY"]);

//cerr<<"ITEM:"<<currdir.get_name()<<'/'<<*currdir<<'\n';//DEBUG!

switch(act){
case TEXT_ONLY:{
	if(pom==NULL)
		hout<<rc("PATH_INFO")<<'/'<<html_text(pom2)<<html_ln;
		else
		hout<<rc("PATH_INFO")<<'/'<<html_bold(*currdir)<<':'<<html_text(pom2)<<html_ln;
	return  TEXT_ONLY;
	}
case TEXT_LNK:{
	hout<<html_anchor(
		sstring(rc("SELF_URL_PATH"))+rc("PATH_INFO")+"?NAME_PATTERN="+sstring(*currdir),
		pom2)<<html_ln;
	return TEXT_LNK;}
case PREV_LNK:{
	const sstring dest=sstring(rc("WWW_DOCS_PATH"))+rc("PREVIEW_GIF_DIRECTORY")+rc("PATH_INFO")+'/'+(*currdir);
	if(access(dest,0))// Not exist
		{
		cerr<<"PREVIEW:"<<Source<<"-->"<<dest<<'\n';
		gd_image_handle::ShouldRaiseException(0);// Nie ma wylatywac na zlym pliku
		gd_image_handle  hnd(Source,READ);
		if(!hnd.Is_OK())
			{hnd.GetSavedException()->PrintTo(hout);return 0;}
		gd_image_handle  hnd2(dest,int(hnd.GetWidth()*0.2),int(hnd.GetHeight()*0.2));
		if(!hnd2.Is_OK())
			{hnd2.GetSavedException()->PrintTo(hout);return 0;}
		hnd2.CopyFrom(hnd,0,0,0,0,hnd2.GetWidth(),hnd2.GetHeight(),hnd.GetWidth(),hnd.GetHeight());
		}
	hout<<html_graphics_anchor(
		sstring(rc("SELF_URL_PATH"))+rc("PATH_INFO")+"?NAME_PATTERN="+sstring(*currdir)+"&FULL=Y",
		sstring(rc("SERVER_URL_PATH"))+
			rc("PREVIEW_GIF_DIRECTORY")+rc("PATH_INFO")+'/'+sstring(*currdir),
		pom2)<<html_ln;
	return PREV_LNK;}
case FULL_LNK:{
	const sstring dest=sstring(rc("WWW_DOCS_PATH"))+rc("FULL_GIF_DIRECTORY")+rc("PATH_INFO")+'/'+(*currdir);
	//cerr<<"FULL DEST:"<<dest<<'\n';
	if(access(dest,0))// Not exist
		{
		cerr<<"FULL:"<<Source<<"-->"<<dest<<'\n';
		gd_image_handle::ShouldRaiseException(0);// Nie ma wylatywac na zlym pliku
		gd_image_handle  hnd(Source,READ);
		if(!hnd.Is_OK())
			{hnd.GetSavedException()->PrintTo(hout);return 0;}
		gd_color col=hnd.ColorSmart(128,128,128);
		hnd.OutText(gdFontSmall,0,0,rc["COPYRIGHT"],col);
		gd_image_handle::ShouldRaiseException(1);
		hnd.SaveAs(dest);
		if(!hnd.Is_OK())
			{hnd.GetSavedException()->PrintTo(hout);return 0;}
		}
	hout<<html_image(sstring(rc("SERVER_URL_PATH"))+rc("FULL_GIF_DIRECTORY")+rc("PATH_INFO")+'/'+sstring(*currdir),
			 "align=center");
	hout<<html_ln<<html_head(2,pom2)<<html_ln;
	return FULL_LNK;}
default:
	return 0;
}

}



int do_directory(cgi_config& rc,html_output& hout)
{
const sstring& dirname=	sstring(rc["PATH_TO_SOURCE_GIF"])+rc("PATH_INFO");
      directory_iterator currdir(dirname,rc("NAME_PATTERN"));
const is_full=*rc("FULL")=='Y';// Czy pelne obrazki czy prewiew
long  preview_limit=atol(rc["PREVIEW_LIMIT"]);
if(preview_limit>HARD_PREVIEW_LIMIT) preview_limit=HARD_PREVIEW_LIMIT;
long  full_limit=atol(rc["FULL_LIMIT"]);
if(full_limit>HARD_FULL_LIMIT) preview_limit=HARD_FULL_LIMIT;
ifstream ext_input((const char*)(dirname+'/'+EXTENDED_ATR_FILE));
config_resources infos(ext_input);//Prowizorka - zawiera tez srodowisko!
ext_input.close();

if(!currdir.IsOK())
		goto ERROR;

{
regular_expression  Keyword=rc("KEYWORD");
long licznik=0;
if(is_full)
	hout<<html_title(sstring("FULL:")+rc("NAME_PATTERN")+sstring(" ")+rc("KEYWORD"))<<html_ln;
	else
	hout<<html_title(sstring("PREVIEW:")+rc("NAME_PATTERN")+sstring(" ")+rc("KEYWORD"))<<html_ln;

for(;currdir!=0;currdir++)
      {
      if((*currdir)[0]=='.' || (*currdir)[0]=='~')// Maja byc ukryte!
			continue;
      if(!currdir.is_subdir())
	{
#ifdef __MSDOS__
	const char** pom=infos.Search( lower(*currdir),0/*DON'T MAKE*/);
#else
	const char** pom=infos.Search(*currdir,0/*DON'T MAKE*/);
#endif
	if( Keyword.match(pom!=NULL?*pom:"") ){
		if(is_full){
		   if(do_item(pom,currdir,rc,hout,(licznik<full_limit?FULL_LNK:TEXT_LNK)
			)==0) goto ERROR1;
		   }
		   else {
		   if(do_item(pom,currdir,rc,hout,(licznik<preview_limit?PREV_LNK:TEXT_LNK)
		    )==0) goto ERROR1;
		   }
		licznik++;
		}
	 } /* end for item */
      } /* end for */

if(!is_full)
	hout<<licznik<<html_bold("  items")<<html_ln;

if(!currdir.IsOK())
		goto ERROR;

//directory_list
currdir.change_pattern("*");
currdir.reset();
if(!currdir.IsOK())
		goto ERROR;
for(int first=1;currdir!=0;currdir++)
      {
      if((*currdir)[0]=='.' || (*currdir)[0]=='~')// Maja byc ukryte!
			continue;
      if(currdir.is_subdir())
	{
	if(first)
		{
		hout<<html_hr;first=0;
		}
#ifdef __MSDOS__
      const char** pom=infos.Search( lower(*currdir),0/*DON'T MAKE*/);
#else
      const char** pom=infos.Search(*currdir,0/*DON'T MAKE*/);
#endif
	if( do_subdir(pom,currdir,rc,hout)
		==0) goto ERROR;
	 }
      }
// Prawidlowy koniec funkcji
return 0;
}

ERROR1:
hout<<html_ln<<html_bold("Current item:")<<*currdir;
ERROR:
hout<<html_ln<<html_bold("ERROR:");
hout<<"Name pattern "<<dirname<<"/"<<rc("NAME_PATTERN");
{int pom=currdir.GetError();
if(pom)
	hout<<':'<<strerror(pom);
hout<<html_ln;}
return 2;
}


int cgi_main(cgi_config& rc,html_output& hout)
{
file_counter count("gifbrows.cou"/*nter"*/);
ifstream config_input("gifbrows.rc");
rc.Preset(config_input);
config_input.close();

if(*rc("GET_FORM")=='Y')
	{
//	hout<<"SORRY - FORM NOT IMPLEMENTED"<<html_ln;
	if(!count.is_ok())
		hout<<html_text(sstring("ERROR IN COUNTER:")+strerror(count.get_error()))<<html_ln<<html_hr;
	hout<<"<P><FORM ACTION="
"\"http://www.bot.astrouw.edu.pl/cgi-bin/borkowsk/gifbrowser\" "
"METHOD=\"GET\">\n"
"<P>\n"
"Select file names:\n"
"<INPUT TYPE=\"text\" NAME=\"NAME_PATTERN\" SIZE=20 VALUE=\"*.gif\">\n"
"<P>\n"
"Regular expression for searching comments:\n"
"<INPUT TYPE=\"text\" NAME=\"KEYWORD\" SIZE=20 MAXLENGTH=40 VALUE=\"\">\n"
"<P>\n"
"<INPUT TYPE=\"submit\" VALUE=\"Submit Form\">\n"
"<P>\n"
"<INPUT TYPE=\"reset\" VALUE=\"Clear Values\">\n"
"</FORM> <br>\n"<<"<B> Current usage counter: "<<count<<"</B><br>";
if(!count.is_ok())
	hout<<html_hr<<html_text(sstring("ERROR IN COUNTER:")+strerror(count.get_error()))<<html_ln;
	return 0;
	}

if(rc("NAME_PATTERN")==String(""))
	{
	hout<<html_pre(
	"\nUSAGE: Use as CGI. Set parameters:\n"
	"NAME_PATTERN=wild_name - for filtering names\n"
	"optionally:\n"
	"KEYWORD=reg-expression - for filtering comments.\n"
	"/subdirectory - for change directory for searching.\n"
	"GET_FORM=YES - for apropriate form.\n"
		       );
	return 0;
	}

int pom=do_directory(rc,hout);

if(pom!=0)
	return pom;
// Ecounting of usage
++count;
return 0;
}
