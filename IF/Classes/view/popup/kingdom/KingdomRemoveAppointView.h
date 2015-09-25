//
//  KingdomRemoveAppointView.h
//  IF
//
//  Created by ganxiaohua on 14-12-10.
//
//

#ifndef __IF__KingdomRemoveAppointView__
#define __IF__KingdomRemoveAppointView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCMultiColTableView.h"
#include "KingdomExecCommand.h"
#include "OfficeInfo.h"

class KingdomRemoveAppointView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
{
public:
    static KingdomRemoveAppointView *create(OfficeInfo* info,bool manager=false);
    KingdomRemoveAppointView(OfficeInfo* info,bool manager):m_info(info),m_manager(manager){};
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
    
    void onGoWorldClick(CCObject *pSender, CCControlEvent event);
    void btnClick1(CCObject *pSender, CCControlEvent event);
    void btnClick2(CCObject *pSender, CCControlEvent event);
    void btnClick3(CCObject *pSender, CCControlEvent event);
    void removeAppointedFun();
    void removeSuccess(CCObject* obj);
    void freshTime(float _time);
    
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIFTTF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_txt1;
    CCSafeObject<CCLabelIF> m_txt1Value;
    CCSafeObject<CCLabelIF> m_txt2;
    CCSafeObject<CCLabelIF> m_txt2Value;
    CCSafeObject<CCLabelIF> m_txt3;
    CCSafeObject<CCLabelIF> m_txt3Value;
    CCSafeObject<CCControlButton> m_goBtn;
    CCSafeObject<CCNode> m_tipNode1;
    CCSafeObject<CCNode> m_tipNode2;
    CCSafeObject<CCNode> m_tipNode3;
    CCSafeObject<CCLabelIF> m_introTxt;
    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCNode> m_btnNode1;
    CCSafeObject<CCNode> m_btnNode2;
    CCSafeObject<CCNode> m_btnNode3;
    CCSafeObject<CCLabelIF> m_btnTxt1;
    CCSafeObject<CCLabelIF> m_btnTxt2;
    CCSafeObject<CCLabelIF> m_btnTxt3;
    OfficeInfo* m_info;
    bool m_manager;
};

#endif /* defined(__IF__KingdomRemoveAppointView__) */
