#ifndef __OSFILELISTCONTROLLER_H__
#define __OSFILELISTCONTROLLER_H__

class OSFileListController
{
public:
	virtual void dirChange(long id)=0;
	virtual void getFocus(long id)=0;
	virtual void splitFileList(long id)=0;
	virtual void openVfs(long id,string file)=0;

};


#endif
