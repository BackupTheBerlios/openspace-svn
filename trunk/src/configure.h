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
    
    bool initialized(void);
    string readonestring (string path);
    bool configure::saveonestring (string path, string value);
    bool configure::addstring (string path, string node, string value);
    int configure::openxpath (string path);
    string configure::getnextstring (void);
    string configure::getnextnode (void);
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
