#ifndef CONFIGURE
#define CONFIGURE


#include <libxml/parser.h>
#include <libxml/xpath.h>

#include <string>
using namespace std;

//read and save to XML config file
class configure
{
  public:
    configure::configure ();
    configure::configure (configure & conf);
    configure::~configure ();
    

    bool loadconfig(bool restore=false);
    string readonestring (string path);
    bool configure::readonestring (string path,string &ret);
    bool configure::saveonestring (string path, string value);
    bool configure::addstring (string path, string node, string value);
    int configure::openxpath (string path);
    bool configure::getnextstring (string &ret);
    bool configure::getnextnode (string &ret);
    int configure::countxpath (string path);
    bool configure::removestring (string path);
    
  private:  
    string file;
    xmlDocPtr doc;

    bool copy;

    int pos;
    xmlXPathObjectPtr result;
    xmlNodeSetPtr nodeset;
    xmlNodePtr cur;
};
#endif
