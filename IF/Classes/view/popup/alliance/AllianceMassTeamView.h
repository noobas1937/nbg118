//
//  AllianceMassTeamView.h
//  IF
//
//  Created by ganxiaohua on 14-8-7.
//
//

#ifndef __IF__AllianceMassTeamView__
#define __IF__AllianceMassTeamView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "QuestController.h"
#include "CCMultiColTableView.h"

class AllianceMassTeamView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
{
public:
    static AllianceMassTeamView *create(int pointId=0);
    AllianceMassTeamView(int pointId):m_pointId(pointId){};
    ~AllianceMassTeamView(){};
protected:
    // scrollview delegate
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void onClickConfirm(CCObject *pSender, CCControlEvent event);
    void onClickCancel(CCObject *pSender, CCControlEvent event);
    
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCLabelIF> m_infoTxt;
    CCSafeObject<CCLabelIF> m_txt1;
    CCSafeObject<CCLabelIF> m_txt2;
    CCSafeObject<CCLabelIF> m_txt3;
    CCSafeObject<CCLabelIF> m_txt4;
    CCSafeObject<CCLabelIF> m_descTxt;
    CCSafeObject<CCControlButton> m_btnCancel;
    CCSafeObject<CCControlButton> m_btnConfirm;
    
    CCSafeObject<CCNode> m_box1;
    CCSafeObject<CCNode> m_box2;
    CCSafeObject<CCNode> m_box3;
    CCSafeObject<CCNode> m_box4;
    CCSafeObject<CCNode> m_selectSpr;
    
    CCSafeObject<CCControlButton> m_btn1;
    CCSafeObject<CCControlButton> m_btn2;
    CCSafeObject<CCControlButton> m_btn3;
    CCSafeObject<CCControlButton> m_btn4;
    void onClickBtn1(CCObject *pSender, CCControlEvent event);
    void onClickBtn2(CCObject *pSender, CCControlEvent event);
    void onClickBtn3(CCObject *pSender, CCControlEvent event);
    void onClickBtn4(CCObject *pSender, CCControlEvent event);
    CCSafeObject<CCNode> m_hideNode;
    
    int m_pointId;
    int m_index;
};
#endif /* defined(__IF__AllianceMassTeamView__) */
