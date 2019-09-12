#include "wb_cgi.hpp"

int main(int Argc,char** Argv)
{
ifstream config_input("wb_cgi.rc");
cgi_config rc(config_input,Argc,Argv);
config_input.close();

cout<<html_begin<<'\n';
cout<<rc("HTML_SITE_HEADER")<<html_ln;

int pom=cgi_main(rc,cout);

if(pom!=0)
  {
  cout<<"<pre>\n";
  cout<<"\n\nZAWARTOSC ZMIENNEJ CGI_CONFIG:\n";
  for(key_iterator<const char*,const char*> K(rc);0!=K;++K)
	cout<<K.CopyOfKey()<<"=\""<<*K<<"\"\n";
  cout<<"</pre>\n";
  }

cout<<rc("HTML_SITE_FOOTER")<<'\n';
cout<<html_end;
return 0;
}
