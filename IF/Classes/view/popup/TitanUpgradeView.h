//
//  TitanUpgradeView.h
//  IF
//
//  Created by fusheng on 15-9-17.
//
//

#ifndef __IF__TitanUpgradeView__
#define __IF__TitanUpgradeView__

#include <stdio.h>
#include "BuildUpgradeView.h"

class TitanUpgradeView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static TitanUpgradeView* create(int buildId,int titanId=0, int pos=0);
    TitanUpgradeView():m_buildId(0),m_pos(0),_tmpGold(0),_resGold(0),_tmpTime(0),m_openNum(0),m_qid(QID_MAX),m_rentTime(0),m_titanId(0){};
    virtual ~TitanUpgradeView()
    {
        if(m_itemScrollView)
            m_itemScrollView->getContainer()->removeAllChildren();
    };
    CCNode *getGuideNode(int itemId);
private:
    virtual void onEnter();
    virtual void onExit();
    int m_titanId;
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchMoved (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
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
    
    void onShowNextUnlockItem();
    
    void onYesRent();
    
    CCSafeObject<CCScale9Sprite> m_BackGroud;
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_titanPosInView;
    CCSafeObject<CCControlButton> m_instantBtn;
    CCSafeObject<CCControlButton> m_upBtn;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCLabelIF> m_demandLabel;
    
    CCSafeObject<CCNode> m_inBtnNode;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCLabelIF> m_btnMsgLabel;
    CCSafeObject<CCLabelIF> m_inBtnGoldNum;
    CCSafeObject<CCLabelIF> m_titanCurrentZDL;
    CCSafeObject<CCLabelIF> m_titanCurrentZDLInNext;
    CCSafeObject<CCLabelIF> m_titanZDLAddValue;
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
    
    CCSafeObject<CCNode> m_iconList;
    CCSafeObject<CCLabelIF> m_unlockLable;
    CCSafeObject<CCNode> m_arrowNode;
    CCSafeObject<CCSprite> m_rArrow;
    CCSafeObject<CCSprite> m_lArrow;
    
    int m_buildId;
    int m_pos;
    bool m_isReturn;
    int _tmpGold;
    int _resGold;
    int _tmpTime;
    CCSafeObject<CCScrollView> m_scrollView;
    CCScrollView* m_itemScrollView;
    int m_openNum;
    int m_qid;
    int m_rentTime;
    
    int m_lfood;
    int m_lwood;
    int m_liron;
    int m_lstone;
    
    int m_unlockItemWidth;
    int m_sumItemWidth;
    int m_x;
    
    vector<string> willUnlockItems;
};


#endif /* defined(__IF__TitanUpgradeView__) */
