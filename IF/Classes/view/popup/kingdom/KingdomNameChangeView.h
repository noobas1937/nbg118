//
//  KingdomNameChangeView.h
//  IF
//
//  Created by ganxiaohua on 14-12-29.
//
//

#ifndef __IF__KingdomNameChangeView__
#define __IF__KingdomNameChangeView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCMultiColTableView.h"
#include "KingdomExecCommand.h"
#include "OfficeInfo.h"

class KingdomNameChangeView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
{
public:
    static KingdomNameChangeView *create(OfficeInfo* info);
    KingdomNameChangeView(OfficeInfo* info):m_info(info){};
protected:
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onChangeNameClick(CCObject *pSender, CCControlEvent event);
    void checkName(float time);
    void checkNameCallBack(CCObject* data);
    void confirmFun1();
    void confirmFun2();
    void modifyNameSuccess(CCObject* data);
    
    CCSafeObject<CCControlButton> m_nameBtn;
    CCSafeObject<CCNode> m_nameOkNode;
    CCSafeObject<CCNode> m_inputNode;
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCLabelIFTTF> m_kingTxt;
    CCSafeObject<CCLabelIF> m_tip1;
    CCSafeObject<CCLabelIF> m_tip2;
    CCSafeObject<CCEditBox> m_inputName;
    OfficeInfo* m_info;
    string m_chenckName;
};
#endif /* defined(__IF__KingdomNameChangeView__) */
