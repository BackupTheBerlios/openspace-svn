#ifndef OSFILE
#define OSFILE

#include <string>
#include <vector>
using namespace std;





enum
{
    FOLDER = 64,		// Directory item
    EXECUTABLE = 128,		// Executable item
    SYMLINK = 256,		// Symbolic linked item
    CHARDEV = 512,		// Character special item
    BLOCKDEV = 1024,		// Block special item
    FIFO = 2048,		// FIFO item
    SOCK = 4096			// Socket item
};


//class associated with file
class osfile
{
  public:
    string name;
    unsigned int type;
    unsigned long size;
    vector <string> attrib;
    string content;

string mode;
string user;
string group;
string date;


};

#endif
