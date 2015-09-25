//
//  GeneralBodySprite.h
//  IF
//
//  Created by ganxiaohua on 14-5-8.
//
//

#ifndef __IF__GeneralBodySprite__
#define __IF__GeneralBodySprite__

#include "CommonInclude.h"

class GeneralBodySprite :
public CCNode
{
public:
    /**
     * @ id 为generalId
     * @ h 不写为原来大小
     * @ gray 是否变灰
     */
    static GeneralBodySprite* create(std::string id,int h=-1,bool gray=false);
    GeneralBodySprite(std::string id,int h,bool gray):m_id(id),m_height(h),m_gray(gray){};
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    void delayFunc(float t);
    std::string m_id;
    int m_height;
    bool m_gray;
};
#endif /* defined(__IF__GeneralBodySprite__) */
