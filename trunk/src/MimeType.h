
#ifndef __MimeType_h__
#define __MimeType_h__

class MimeType
{

  private:
    static map < string, string > mimeMap;
    static map < string, string > mimeCache;
    static bool isInitialized;

  private:
     MimeType ()
    {
    }
    ~MimeType ()
    {
    }

    static void __initialize (void);
    static bool _matches (string pattern, string fname);

  public:
    static string getMimeFromName (string name);

};

#endif
