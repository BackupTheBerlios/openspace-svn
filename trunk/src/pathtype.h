#ifndef PATHTYPE
#define PATHTYPE

#include <string>
using namespace std;

//containg informations for remote filelist
class pathtype
{
public:
pathtype(string dir, string type="local", string server="", string user="", string password="", string port="");

string dir;

string type;
string server;
string user;
string password;
string port;
};

#endif
