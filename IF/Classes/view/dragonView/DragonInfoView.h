//
//  DragonInfoView.h
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

#ifndef __IF__DragonInfoView__
#define __IF__DragonInfoView__

#include "CommonInclude.h"
#include "ArcPopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCClipNode.h"

class DragonInfoView: public ArcPopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
{
public:
    static DragonInfoView* create();
    DragonInfoView(){};
    virtual ~DragonInfoView(){};
protected:
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onCloseclick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onOKClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCControlButton> m_OKBtn;
    CCSafeObject<CCTableView> m_tabView;
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCSprite> m_iconSpr;
    CCSafeObject<CCNode> m_starNode;
    CCSafeObject<CCSprite> m_starSpr;
    CCSafeObject<CCNode> m_attributeNode;
    CCSafeObject<CCSprite> m_attributeSpr;
    CCSafeObject<CCNode> m_outputNode;
    CCSafeObject<CCLabelIF> m_outputTxt;
    CCSafeObject<CCSprite> m_outputSpr;
    CCSafeObject<CCLabelIF> m_detailTxt;
    CCSafeObject<CCLabelIF> m_countTxt;
    CCSafeObject<CCLabelIF> m_haveTxt;
    
};

#endif /* defined(__IF__DragonInfoView__) */
