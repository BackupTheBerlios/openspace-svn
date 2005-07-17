#ifndef filetype
#define filetype

//icon and colors associated with filetype
class file_type {
	
	public:
		FXIcon *icon;
		FXColor color;
		FXColor backcolor;

		file_type::file_type(FXIcon*ic,FXColor col,FXColor backcol);
		file_type::file_type(){}

};

#endif
