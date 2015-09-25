//
//  TipsWithPicView.h
//  IF
//
//  Created by 〜TIAN~ on 15/4/2.
//
//

#ifndef __IF__TipsWithPicView__
#define __IF__TipsWithPicView__

#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CommonInclude.h"
#include "CCLabelIF.h"

class TipsWithPicView : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
{
public:
    static TipsWithPicView* create(std::string type);
    TipsWithPicView(std::string type):m_typestr(type){};
    virtual ~TipsWithPicView(){};
    
private:
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onCloseClick(CCObject *pSender, CCControlEvent event);
    
    CCNode* addPic(std::string resourceIcon, std::string icon1, std::string icon2);
    CCSafeObject<CCLabelIF> m_infoLabel1;
    CCSafeObject<CCLabelIF> m_infoLabel2;
    CCSafeObject<CCLabelIF> m_infoLabel3;
    CCSafeObject<CCLabelIF> m_infoLabel4;
    CCSafeObject<CCLabelIF> m_infoLabel5;
    CCSafeObject<CCLabelIF> m_infoLabel6;
    CCSafeObject<CCLabelIF> m_infoLabel7;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCNode> m_spriteNode1;
    CCSafeObject<CCNode> m_spriteNode2;
    CCSafeObject<CCNode> m_spriteNode3;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_infoNode1;
    CCSafeObject<CCNode> m_infoNode2;
    CCSafeObject<CCNode> m_infoNode3;
    CCSafeObject<CCControlButton> m_closeBtn;
    
    std::string m_typestr;
};

#endif /* defined(__IF__TipsWithPicView__) */
