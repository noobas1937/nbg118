//
//  GeneralHeadPic.h
//  IF
//
//  Created by 李锐奇 on 14-3-7.
//
//

#ifndef __IF__GeneralHeadPic__
#define __IF__GeneralHeadPic__

#include "CommonInclude.h"

class GeneralHeadPic : public CCNode{
public:
    static GeneralHeadPic *create(std::string generalId, int color = -1,int type=-1);
    void setGray();
private:
    virtual bool init(std::string generalId, int color,int type);
    CCSafeObject<CCSprite> m_head;
    CCSafeObject<CCSprite> m_cover;
    CCSafeObject<CCSprite> m_bgColor;
};

#endif /* defined(__IF__GeneralHeadPic__) */
