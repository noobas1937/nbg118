//
//  DragonBornView.h
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

#ifndef __IF__DragonBornView__
#define __IF__DragonBornView__

#include "CommonInclude.h"
#include "ArcPopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCClipNode.h"

class DragonBornView: public ArcPopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static DragonBornView* create();
    DragonBornView(){};
    virtual ~DragonBornView(){};
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onOKClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCControlButton> m_OKBtn;
    CCSafeObject<CCTableView> m_tabView;
    
    CCSafeObject<CCLabelIF> m_attributeTxt;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_levelTxt;
    CCSafeObject<CCLabelIF> m_outputTxt;
    CCSafeObject<CCLabelIF> m_outputValueTxt;
    CCSafeObject<CCNode> m_starNode;
    CCSafeObject<CCNode> m_dragonNode;
    CCSafeObject<CCNode> m_btnNode1;
    CCSafeObject<CCSprite> m_starSpr;
    CCSafeObject<CCControlButton> m_sellBtn;
    CCSafeObject<CCControlButton> m_moveBtn;
    void onSellClick(cocos2d::CCObject * pSender, CCControlEvent pCCControlEvent);
    void onMoveClick(cocos2d::CCObject * pSender, CCControlEvent pCCControlEvent);
};

#endif /* defined(__IF__DragonBornView__) */
