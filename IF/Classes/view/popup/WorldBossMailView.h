//
//  WorldBossMailView.h
//  IF
//
//  Created by zhangjun on 2015-04-27 10:21:14.
//
		
#ifndef __LastEmpire__WorldBossMailView__
#define __LastEmpire__WorldBossMailView__
		
#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "HFHeadImgNode.h"

class WorldBossMailView : public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
//, public CCTouchDelegate

{
public:
    WorldBossMailView(MailInfo& info):m_info(&info){};
    virtual ~WorldBossMailView(){};
    virtual bool init();
    virtual void onExit();
    virtual void onEnter();
    static WorldBossMailView* create(MailInfo& info);
		
    void refreshView(CCObject* p = NULL);
    void refreshRewardView(CCObject* p = NULL);
protected:
    CCSafeObject<MailInfo> m_info;
    vector<std::string> rewardVector;
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    bool onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);
    
	void onAddSaveClick(CCObject * pSender, CCControlEvent pCCControlEvent);
	void onDeleteClick(CCObject * pSender, CCControlEvent pCCControlEvent);
	void onShareClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onReturnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onWriteClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onOkDeleteMail();
    
    CCSafeObject<CCNode> m_battlePicNode;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_failNode;
    CCSafeObject<CCNode> m_firstNode;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_totalNode;
//    CCSafeObject<CCNode> m_upNode;
//    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCScale9Sprite> m_hintBG;
    CCSafeObject<CCLabelIF> m_attackText;
    CCSafeObject<CCLabelIF> m_hintText;
    CCSafeObject<CCLabelIF> m_mailTitle;
    CCSafeObject<CCLabelIF> m_timeText;
//    CCSafeObject<CCLabelIF> m_upTitle;
    CCSafeObject<CCControlButton> m_addSaveBtn;
    CCSafeObject<CCControlButton> m_returnBtn;
    CCSafeObject<CCSprite> m_returnSpr;
    CCSafeObject<CCControlButton> m_writeBtn;
    CCSafeObject<CCControlButton> m_deleteBtn;
    CCSafeObject<CCControlButton> m_shareBtn;
//    CCSafeObject<CCControlButton> m_unSaveBtn;
    CCSafeObject<CCSprite> m_hintLine;

    CCSafeObject<CCLabelIF> m_monsterNameText;
    CCSafeObject<CCNode> m_monsterPicNode;
    CCSafeObject<CCScale9Sprite> m_bar;
    CCSafeObject<CCLabelIF> m_monsterLifeText;
    
//    CCSafeObject<CCNode> m_mainCellNode;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_rwdNode1;
    CCSafeObject<CCNode> m_rwdNode2;
    CCSafeObject<CCNode> m_rwdNode3;
    CCSafeObject<CCNode> m_rwdPicNode[3];
    CCSafeObject<CCLayerColor> m_rwdCover[3];
    CCSafeObject<CCLabelIF> m_rwdNum[3];
//    CCSafeObject<CCNode> m_rwdPicNode2;
//    CCSafeObject<CCNode> m_rwdPicNode3;
//    CCSafeObject<CCNode> m_upCellNode;
    CCSafeObject<CCControlButton> m_iconBtn;
    CCSafeObject<CCControlButton> m_rwd1Btn;
    CCSafeObject<CCControlButton> m_rwd2Btn;
    CCSafeObject<CCControlButton> m_rwd3Btn;
    CCSafeObject<CCLabelIF> m_deadCountTxt;
    CCSafeObject<CCLabelIF> m_deadTxt;
    CCSafeObject<CCLabelIF> m_killCountTxt;
    CCSafeObject<CCLabelIF> m_killTxt;
    CCSafeObject<Label> m_nameTxt;
    CCSafeObject<CCLabelIF> m_rewardTxt;
//    CCSafeObject<CCLabelIF> m_upNodeText;
    CCSafeObject<CCLabelIF> m_woundCountTxt;
    CCSafeObject<CCLabelIF> m_woundTxt;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    CCSafeObject<CCRenderTexture> m_selfModelLayer;
    CCSafeObject<CCLabelIF> m_posTxt;
    CCSafeObject<Node> m_posBG;
    CCSafeObject<CCScale9Sprite> m_underlineSpr;
    void onRwd1BtnPressedClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onRwd2BtnPressedClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onRwd3BtnPressedClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onplayerHeadClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    CCSafeObject<CCControlButton> m_getRewardBtn;
    void onGetRewardBtnPressed(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    CCSafeObject<CCScrollView> m_scrollView;
};

#define PLayerCellH 268.0
#define PLayerCellHeadH 30.0
#define PLayerCellW 306.0

class WorldBossMailPlayerCellView :
public cocos2d::CCNode,
public CCBSelectorResolver,
public CCBMemberVariableAssigner
{
public:
    WorldBossMailPlayerCellView();
    virtual ~WorldBossMailPlayerCellView();
    virtual bool init();
    virtual void onExit();
    virtual void onEnter();
    static WorldBossMailPlayerCellView* create();
    
    void setData(int index,bool isShowTitle=false);
public:
    
    bool onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);
    
protected:
    virtual void onRwd1BtnPressedClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    virtual void onRwd2BtnPressedClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    virtual void onRwd3BtnPressedClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    virtual void onplayerHeadClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    
protected:
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_rwdNode1;
    CCSafeObject<CCNode> m_rwdNode2;
    CCSafeObject<CCNode> m_rwdNode3;
    CCSafeObject<CCNode> m_rwdPicNode1;
    CCSafeObject<CCNode> m_rwdPicNode2;
    CCSafeObject<CCNode> m_rwdPicNode3;
    CCSafeObject<CCNode> m_upNode;
    CCSafeObject<CCControlButton> m_iconBtn;
    CCSafeObject<CCControlButton> m_rwd1Btn;
    CCSafeObject<CCControlButton> m_rwd2Btn;
    CCSafeObject<CCControlButton> m_rwd3Btn;
    CCSafeObject<CCLabelIF> m_deadCountTxt;
    CCSafeObject<CCLabelIF> m_deadTxt;
    CCSafeObject<CCLabelIF> m_killCountTxt;
    CCSafeObject<CCLabelIF> m_killTxt;
    CCSafeObject<Label> m_nameTxt;
    CCSafeObject<CCLabelIF> m_rewardTxt;
    CCSafeObject<CCLabelIF> m_upNodeText;
    CCSafeObject<CCLabelIF> m_woundCountTxt;
    CCSafeObject<CCLabelIF> m_woundTxt;
};

#endif /* defined(__LastEmpire__WorldBossMailView__) */
		
