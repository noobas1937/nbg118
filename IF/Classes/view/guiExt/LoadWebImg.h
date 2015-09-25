//
//  LoadWebImg.h
//  IF
//
//  Created by ganxiaohua on 14-10-15.
//
//

#ifndef __IF__LoadWebImg__
#define __IF__LoadWebImg__

#include "CommonInclude.h"
#include "HFUrlImage.h"

class LoadWebImg : public CCSprite
{
public:
    LoadWebImg(void){};
    ~LoadWebImg(void){};
   	CREATE_FUNC(LoadWebImg);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    void callBack(CCObject* p);
    void setLoadURL(string imgURL);//设置要加载的图片的url
protected:
    CCSafeObject<HFUrlImage> m_urlImg;
};

#endif /* defined(__IF__LoadWebImg__) */
