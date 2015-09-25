//
//  DragonBuildUpgradeView.h
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

#ifndef __IF__DragonBuildUpgradeView__
#define __IF__DragonBuildUpgradeView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "ArcPopupBaseView.h"
#include "CCLabelIF.h"
#include "FunBuildInfo.h"

#include "BuildUpgradeView.h"

class DragonBuildUpgradeView: public ArcPopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static DragonBuildUpgradeView* create(int buildId, int pos=0);
    DragonBuildUpgradeView():m_buildId(0),m_pos(0),_tmpGold(0),_resGold(0),_tmpTime(0),m_openNum(0),m_qid(QID_MAX),m_rentTime(0){};
    virtual ~DragonBuildUpgradeView(){};
    CCNode *getGuideNode(int itemId);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(int buildId, int pos=0);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onCreateOrUpClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onInstantClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onOkUp();
    void onLastUp();
    void showWarningInstantUp();
    void onOkInstantUp();
    void onGotoSpdView();
    
    void updateInfo(CCObject* obj);
    void updateParasInfo();
    
    void onYesRent();
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCControlButton> m_instantBtn;
    CCSafeObject<CCControlButton> m_upBtn;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCLabelIF> m_demandLabel;
    
    CCSafeObject<CCNode> m_inBtnNode;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCLabelIF> m_btnMsgLabel;
    CCSafeObject<CCLabelIF> m_inBtnGoldNum;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    
    CCSafeObject<CCLabelIF> m_lvLabel;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_nextLabel;
    CCSafeObject<CCLabelIF> m_nextLvLabel;
    CCSafeObject<CCLabelIF> m_nextMsgLabel;
    CCSafeObject<CCLabelIF> m_upBtnMsgLabel;
    
    CCSafeObject<CCNode> m_para1Node;
    CCSafeObject<CCLabelIF> m_para1Name;
    CCSafeObject<CCLabelIF> m_para1Num;
    CCSafeObject<CCLabelIF> m_next1Num;
    
    CCSafeObject<CCNode> m_para2Node;
    CCSafeObject<CCLabelIF> m_para2Name;
    CCSafeObject<CCLabelIF> m_para2Num;
    CCSafeObject<CCLabelIF> m_next2Num;
    
    CCSafeObject<CCNode> m_para3Node;
    CCSafeObject<CCLabelIF> m_para3Name;
    CCSafeObject<CCLabelIF> m_para3Num;
    CCSafeObject<CCLabelIF> m_next3Num;
    
    CCSafeObject<CCNode> m_para4Node;
    CCSafeObject<CCLabelIF> m_para4Name;
    CCSafeObject<CCLabelIF> m_para4Num;
    CCSafeObject<CCLabelIF> m_next4Num;
    
    int m_buildId;
    int m_pos;
    bool m_isReturn;
    int _tmpGold;
    int _resGold;
    int _tmpTime;
    CCSafeObject<CCScrollView> m_scrollView;
    int m_openNum;
    int m_qid;
    int m_rentTime;
    
    int m_lfood;
    int m_lwood;
    int m_liron;
    int m_lstone;
};

#endif /* defined(__IF__DragonBuildUpgradeView__) */
