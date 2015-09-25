//
//  FlyHint.h
//  IF
//
//  Created by 李锐奇 on 14-4-11.
//
//

#ifndef __IF__FlyHint__
#define __IF__FlyHint__

#include "CommonInclude.h"
#include "CCIFTouchNode.h"
#define REMOVE_ALL_HINT "removeAllHint"

class FlyHint : public CCIFTouchNode
, public CCBMemberVariableAssigner
{
public:
    static FlyHint *create(std::string icon, std::string titleText, std::string contentText, float time,float dy=0,bool useDefaultIcon=false);
    void show();
    void setMailId(string mailId);
    std::string m_content;
    void removeSelf(CCObject* p = NULL);
private:
    FlyHint(std::string icon, std::string titleText, std::string contentText, float time,float dy=0) :
    m_icon(icon)
    , m_title(titleText)
    , m_content(contentText)
    , m_time(time)
    ,m_dy(dy)
    {};
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool init(bool useDefaultIcon=false);
    virtual void onEnter();
    virtual void onExit();
    void removeAllSelf(CCObject* p = NULL);
    
    std::string m_icon;
    std::string m_title;
    
    bool m_isShowing;
    float m_time;
    float m_dy;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_contentText;
    CCSafeObject<CCNode> m_picContainer;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCSprite> m_flyHeadSpr;
    bool m_useDefaultIcon;
    void showInNativeUI(float time);
    
    string m_mailId;
};

#endif /* defined(__IF__FlyHint__) */
