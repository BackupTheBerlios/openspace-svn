#include "fx.h"
using namespace FX;

#include "oslistitem.h"
#include "filelist.h"

//TMP
#include "sharedobjects.h"

#define SIDE_SPACING             4    // Left or right spacing between items
#define DETAIL_TEXT_SPACING      2    // Spacing between text and icon in detail icon mode
#define MINI_TEXT_SPACING        2    // Spacing between text and icon in mini icon mode
#define BIG_LINE_SPACING         0    // Line spacing in big icon mode
#define BIG_TEXT_SPACING         0    // Spacing between text and icon in big icon mode
#define ITEM_SPACE             128    // Default space for item



FXIMPLEMENT(os_ListItem,FXIconItem,NULL,0)

// Constructor

os_ListItem::os_ListItem(FXIconList *list,osfile osf, FXColor col, FXColor backcol,FXIcon* ic1,FXIcon* ic2):FXIconItem(osf.name.c_str(),ic1,ic2,NULL),osf(osf)
{

this->list=list;
color=col;
backcolor=backcol;
state|=FXIconItem::DRAGGABLE;
}


// Draw detail
void os_ListItem::drawDetails(const FXIconList* list,FXDC& dc,FXint x,FXint y,FXint,FXint h) const {

//=============CHANGES==========================================
	FXString label=osf.name.c_str();
	label.append("\t");
		for(int i=0;i<osf.attrib.size();i++)
		{				
		label.append(osf.attrib[i].c_str());
		label.append("\t");			
		}
//=============CHANGES==========================================



  register FXHeader *header=list->getHeader();
  register FXFont *font=list->getFont();
  register FXint iw=0,ih=0,tw=0,th=0,yt,beg,end,hi,drw,space,used,dw,xx;
  if(header->getNumItems()==0) return;
  if(isSelected()){
    dc.setForeground(list->getSelBackColor());
    dc.fillRectangle(x,y,header->getTotalSize(),h);
    }
  if(hasFocus()){
    dc.drawFocusRectangle(x+1,y+1,header->getTotalSize()-2,h-2);
    }
  xx=x+SIDE_SPACING/2;
  if(miniIcon){

    iw=miniIcon->getWidth();
    ih=miniIcon->getHeight();
    dc.setClipRectangle(x,y,header->getItemSize(0),h);
    dc.drawIcon(miniIcon,xx,y+(h-ih)/2);
    dc.clearClipRectangle();
    xx+=iw+DETAIL_TEXT_SPACING;
    }
  if(!label.empty()){
    th=font->getFontHeight();
    dw=font->getTextWidth("...",3);
    yt=y+(h-th-4)/2;
    if(!isEnabled())
      dc.setForeground(makeShadowColor(list->getBackColor()));
    else if(isSelected())
      dc.setForeground(list->getSelTextColor());
    else
      dc.setForeground(color); //CHANGES dc.setForeground(list->getTextColor());
    used=iw+DETAIL_TEXT_SPACING+SIDE_SPACING/2;
    for(hi=beg=0; beg<label.length() && hi<header->getNumItems(); hi++,beg=end+1){
      space=header->getItemSize(hi)-used;
      for(end=beg; end<label.length() && label[end]!='\t'; end++);
      if(end>beg){
        drw=end-beg;
        tw=font->getTextWidth(&label[beg],drw);
        if(tw>space-4){
          while((tw=font->getTextWidth(&label[beg],drw))+dw>space-4 && drw>1) drw--;
          dc.setClipRectangle(xx,y,space,h);
          dc.drawText(xx+2,yt+font->getFontAscent()+2,&label[beg],drw);
          dc.drawText(xx+tw+2,yt+font->getFontAscent()+2,"...",3);
          dc.clearClipRectangle();
          }
        else{
          dc.drawText(xx+2,yt+font->getFontAscent()+2,&label[beg],drw);
          }
        }
      xx+=space;
      used=0;
      }
    }
  }
  
 
 void os_ListItem::drawBigIcon(const FXIconList* list,FXDC& dc,FXint x,FXint y,FXint w,FXint h) const {
 
 
 //=============CHANGES==========================================
	FXString label=osf.name.c_str();
	
		
//=============CHANGES==========================================
   FXFont *font=((filelist*)list)->objmanager->captionfont;
   FXFont *font1=((filelist*)list)->objmanager->captionfont1; 
 
 
  register FXint len,dw,s,space,xt,yt,xi,yi;
  //register FXFont *font=list->getFont();
  register FXint iw=0,ih=0,tw=0,th=0,ss=0;
  space=w-SIDE_SPACING;
  if(!label.empty()){
    for(len=0; len<label.length() && label[len]!='\t'; len++);
    tw=4+font->getTextWidth(label.text(),len);
    
    
    
    
      if(bigIcon)
      {
  	  ih=bigIcon->getHeight();
	  iw=bigIcon->getWidth();
	   xi=x+(w-iw)/2;
      }
    
    
    
    
    th=4+font->getFontHeight();
     //=============CHANGES==========================================    
      for(int i=0;i<osf.attrib.size();i++)
		{		
		th=th+4+font1->getFontHeight();	   
		}
  //=============CHANGES==========================================
    
    
    //yt=y+h-th-BIG_LINE_SPACING/2;   
    yi=y+BIG_LINE_SPACING/2+(h-th-BIG_LINE_SPACING-ss-ih)/2;
    yt=y;
    
    dw=0;
    if(tw>space){
      dw=font->getTextWidth("...",3);
      s=space-dw;
      while((tw=4+font->getTextWidth(label.text(),len))>s && len>1) --len;
      if(tw>s) dw=0;
      }
    if(tw<=space){         // FIXME as below in drawDetails
      //xt=x+(w-tw-dw)/2;
      xt=x+iw;
      
      if(isSelected()){
       // dc.setForeground(list->getSelBackColor());
        //dc.fillRectangle(xt,yt,tw+dw,th);
        }
	dc.setForeground(backcolor);
	dc.fillRectangle(x,y,200,93);
	
	
      if(!isEnabled())
        dc.setForeground(makeShadowColor(list->getBackColor()));
      else if(isSelected())
        dc.setForeground(color); 
      else
	 dc.setForeground(color); 
	
	
      dc.drawText(xt+2-iw,yt+font->getFontAscent()+2,label.text(),len);
      
      
      if(dw) dc.drawText(xt-iw+tw-2,yt+font->getFontAscent()+2,"...",3);
   //=============CHANGES==========================================    
    dc.setFont(font1);
      for(int i=0;i<osf.attrib.size();i++)
		{				
		
		dc.drawText(xt+2,yt+(i+2)*font->getFontAscent()+2,osf.attrib[i].c_str(),osf.attrib[i].length());
			
		}
  //=============CHANGES==========================================
    dc.setFont(font);   
      /*if(hasFocus()){
        dc.drawFocusRectangle(xt+1,yt+1,tw+dw-2,th-2);
        }*/
      }
    ss=0;//BIG_TEXT_SPACING;    // Space between text and icon only added if we have both icon and text
    }
  if(bigIcon){
   
    //=============CHANGES==========================================    
    xi=x;
    //=============CHANGES==========================================    
    yi=y+BIG_LINE_SPACING/2+(h-th-BIG_LINE_SPACING-ss-ih)/2;
    if(isSelected()){
      dc.drawIconShaded(bigIcon,xi,y+4+font->getFontHeight());
      }
    else{
      dc.drawIcon(bigIcon,xi,y+4+font->getFontHeight());
      }
     if(osf.type&EXECUTABLE && !(osf.type&FOLDER))
     {       
     dc.drawIcon(((filelist*)list)->specialicons[4],xi,y+4+font->getFontHeight());
     }
     else if(osf.type&SYMLINK)
     {
       dc.drawIcon(((filelist*)list)->specialicons[5],xi,y+4+font->getFontHeight());
     }
     
     if(osf.content!="")
     {
     dc.setFont(font1);
     dc.drawText(xi,y+4+font->getFontHeight()+font->getFontAscent()+2,osf.content.c_str(),10);
     dc.setFont(font);     
     }
      
    }
  } 
  


FXint os_ListItem::getHeight(const FXIconList* list) const {
FXFont *font=((filelist*)list)->objmanager->captionfont;
FXFont *font1=((filelist*)list)->objmanager->captionfont1;

  register FXuint options=list->getListStyle();
  register FXint ih=0,th=0,h=0,firstth=0;
  if(options&ICONLIST_BIG_ICONS){
    if(bigIcon) ih=bigIcon->getHeight();
    if(!label.empty()) th=4+font->getFontHeight();
    if(ih && th) ih+=BIG_TEXT_SPACING;
    firstth=th;
     //=============CHANGES==========================================    
      for(int i=0;i<osf.attrib.size();i++)
		{		
	       th=th+4+font1->getFontHeight();           
		}
  //=============CHANGES==========================================
    
   	 if(ih>th)
   	 h=BIG_LINE_SPACING+ih+firstth+4;
	 else
	 h=BIG_LINE_SPACING+th;
	
    }
  else if(options&ICONLIST_MINI_ICONS){
    if(miniIcon) ih=miniIcon->getHeight();
    if(!label.empty()) th=4+list->getFont()->getFontHeight();
    h=FXMAX(ih,th);
    }
  else{
    if(miniIcon) ih=miniIcon->getHeight();
    if(!label.empty()) th=4+list->getFont()->getFontHeight();
    h=FXMAX(ih,th);
    }

  return h;
  }


FXint os_ListItem::hitItem(const FXIconList* list,FXint rx,FXint ry,FXint rw,FXint rh) const {

FXFont *font=((filelist*)list)->objmanager->captionfont;
FXFont *font1=((filelist*)list)->objmanager->captionfont1;

 register FXint iw=0,tw=0,ih=0,th=0,ss=0,ix,iy,tx,ty,w,h,sp,tlen;
  register FXuint options=list->getListStyle();
  //register FXFont *font=list->getFont();
  for(tlen=0; tlen<label.length() && label[tlen]!='\t'; tlen++);
  if(options&ICONLIST_BIG_ICONS){
    w=list->getItemSpace();
    h=list->getItemHeight();
    sp=w-SIDE_SPACING;
    if(!label.empty()){
      tw=4+font->getTextWidth(label.text(),tlen);
      th=4+font->getFontHeight();
      if(tw>sp) tw=sp;
      if(bigIcon) ss=BIG_TEXT_SPACING;
      }
    if(bigIcon){
      iw=bigIcon->getWidth();
      ih=bigIcon->getHeight();
      }
    ty=h-th-BIG_LINE_SPACING/2;
    iy=4+font->getFontHeight();
    ix=0;
    tx=(w-tw)/2;
    }
  else if(options&ICONLIST_MINI_ICONS){
    sp=list->getItemSpace()-SIDE_SPACING;
    ix=SIDE_SPACING/2;
    tx=SIDE_SPACING/2;
    if(miniIcon){
      iw=miniIcon->getWidth();
      ih=miniIcon->getHeight();
      tx+=iw+MINI_TEXT_SPACING;
      sp=sp-iw-MINI_TEXT_SPACING;
      }
    if(!label.empty()){
      tw=4+font->getTextWidth(label.text(),tlen);
      th=4+font->getFontHeight();
      if(tw>sp) tw=sp;
      }
    h=list->getItemHeight();
    iy=(h-ih)/2;
    ty=(h-th)/2;
    }
  else{
    ix=SIDE_SPACING/2;
    tx=SIDE_SPACING/2;
    if(miniIcon){
      iw=miniIcon->getWidth();
      ih=miniIcon->getHeight();
      tx+=iw+DETAIL_TEXT_SPACING;
      }
    if(!label.empty()){
      tw=10000000;
      th=4+font->getFontHeight();
      }
    h=list->getItemHeight();
    iy=(h-ih)/2;
    ty=(h-th)/2;
    }

  // In icon?
  if(ix<=rx+rw && iy<=ry+rh && rx<ix+iw && ry<iy+ih) return 1;

  // In text?
  if(tx<=rx+rw && ty<=ry+rh && rx<tx+tw && ry<ty+th) return 2;

  // Outside
  return 0;
  }
