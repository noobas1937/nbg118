//
//  TrainingView.h
//  IF
//
//  Created by xxrdsg on 15-7-16.
//
//

#ifndef __IF__Test1__
#define __IF__Test1__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCClipNode.h"
#include "CCAniNode.h"

#define MSG_CELL_TOUCH_BEGAN "msg.cell.touched.began"
#define MSG_CELL_TOUCH_MOVED "msg.cell.touched.moved"
#define MSG_CELL_TOUCH_ENDED "msg.cell.touched.ended"

#define MSG_TRAINING_CELL_ADDED "msg.training.cell.added"
#define MSG_TRAINING_CELL_REMOVED "msg.training.cell.removed"
#define MSG_TRAINING_CELL_ALL_REMOVED "msg.training.cell.all.removed"

#define MSG_TRAINING_CELL_START_LINING "msg.training.cell.start.lining"
#define MSG_START_USE_BUF "msg.start.use.buf"
#define MSG_START_ATK "msg.start.atk"
#define MSG_CELL_BOOM "msg.cell.boom"

class Training1View: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    Training1View(): m_type(){};
    static Training1View* create();
    bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onOKClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void onGetMsgTrialStartSuc(CCObject*);
    void onGetMsgTrialFininshSuc(CCObject*);
    
    void showCountDown();
    void setButtonTitle();
    void setCountLabel(CCObject* obj);
    void openTraining2View();
    void setMyType();
    void refreshInterface();
protected:
private:
    
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCSprite> m_buildBg;
    
    CCSafeObject<CCNode> m_middleNode;
    CCSafeObject<CCLabelIF> m_infoLabel;
    
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCControlButton> m_OKBtn;
    
    CCSafeObject<CCNode> m_countNode;
    CCSafeObject<CCSprite> m_woman;
    
    int m_type;//1 开始新的训练 2继续当前训练 3 兑换奖励
};

class TrainingConfirmView: public PopupBaseView
,public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    TrainingConfirmView(int type): m_type(type){};
    static TrainingConfirmView* create(int type);
    bool init();
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName){return NULL;};
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    
    virtual void onEnter();
    virtual void onExit();
    
    void onClickLeftBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickRightBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
protected:
private:
    CCSafeObject<CCLabelIF> m_infoLabel;
    CCSafeObject<CCLabelIF> m_leftLabel;
    CCSafeObject<CCLabelIF> m_rightLabel;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCSprite> m_leftIcon;
    CCSafeObject<CCLabelIF> m_leftNumLabel;
    
    CCSafeObject<CCControlButton> m_leftBtn;
    CCSafeObject<CCControlButton> m_rightBtn;
    
    CCSafeObject<CCNode> m_touchNode;
    
    int m_type;
};

class Training2View: public PopupBaseView
,public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    CREATE_FUNC(Training2View);
    bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName){return NULL;};
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    
    void onClickAtkBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickBufBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void onGetMsgCellTouchBegan(CCObject* obj);
    void onGetMsgCellTouchMoved(CCObject* obj);
    void onGetMsgCellTouchEnded(CCObject* obj);
    void onGetMsgGetAtkData(CCObject* obj);
    void onGetMsgTrialBuySuc(CCObject* obj);
    void onGetMsgCellBoom(CCObject* obj);
    
    void onEnterFrame(float dt);
    vector<int> getTwoLegalNeighbor();
    void playHandAction();
    void hideHandAction();
    void refreshFixedWire();
    void removeAllFixedWire();
    void addOneFixedWire();
    void removeFixedWireToIdx(int idx);
    void refreshCells();
    void refreshBtnState();
    void refreshMonster();
    void refreshBlood();
    void refreshInterface();
    bool isLegalNeighbor(int tarIdx, int myIdx);
    bool isHasTouched(int idx);
    bool isIdsValid();
    void showAtkParByType(int type, bool isLastKilled);
    void removeAtkPar();
    void sendAtkCmd(CCObject* obj);
    void makeOnceEffectSpr(CCObject* obj);
protected:
private:
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCSprite> m_proBar;
    CCSafeObject<CCLabelIF> m_bloodLabel;
    CCSafeObject<CCLabelIF> m_leftCntLabel;
    
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCScale9Sprite> m_rightBg1;
    CCSafeObject<CCScale9Sprite> m_rightBg2;
    CCSafeObject<CCScale9Sprite> m_leftBg1;
    CCSafeObject<CCScale9Sprite> m_leftBg2;
    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCControlButton> m_atkBtn;
    CCSafeObject<CCControlButton> m_bufBtn;
    
    CCSafeObject<CCNode> m_bNode;
    CCSafeObject<CCNode> m_tNode;
    CCSafeObject<CCNode> m_parNode;
    CCSafeObject<CCAniNode> m_parAniNode;
    CCSafeObject<CCNode> m_fixedWireNode;
    CCSafeObject<CCClipNode> m_clipNode;
    CCSafeObject<CCNode> m_monsterNode;
    
    CCSafeObject<CCLabelIF> m_leftLabel;
    CCSafeObject<CCLabelIF> m_rightLabel;
    CCSafeObject<CCLabelIF> m_rightNumLabel;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_midNode1;
    CCSafeObject<CCNode> m_midNode2;
    CCSafeObject<CCSprite> m_monsterBg;
    CCSafeObject<CCNode> m_handNode;
    
    CCSafeObject<CCClipNode> m_clipNode2;
    CCSafeObject<CCSprite> m_handSpr;
    
    vector<CCScale9Sprite*> m_addedSpr;
    vector<int> m_touchedIdxes;
    map<int, CCPoint> m_iToPMap;
    
    bool m_isInTouching;
    bool m_isInCellWiring;
    bool m_isUseBuf;
    bool m_isInAtk;
    time_t m_actionTime;
};

class TrainingCell: public CCAniNode
{
public:
    TrainingCell(int idx): m_idx(idx), m_bCanClick(true), m_isInBuf(false), m_addedPos(0){};
    static TrainingCell* create(int idx);
    bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onGetMsgCellAllRemoved(CCObject* obj);
    void onGetMsgCellAdded(CCObject* obj);
    void onGetMsgCellRemoved(CCObject* obj);
    void onGetMsgCellStartLining(CCObject* obj);
    void onGetMsgStartUseBuf(CCObject* obj);
    void onGetMsgStartAtk(CCObject* obj);
    
    void showBoomPar();
    void showSpePar();
    void showParByType();
    void removeParticle();
    void setCellState();
    void refreshSpr();
private:
    CCSafeObject<CCNode> m_parNode;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCSprite> m_bgSpr;
    int m_idx;
    bool m_bCanClick;
    bool m_isInBuf;
    int m_addedPos;//被连起来的次序 默认是0
};

class TrainingRwdView: public PopupBaseView
,public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    CREATE_FUNC(TrainingRwdView);
    bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName){return NULL;};
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    
    void onOkClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void addParticle();
    void playGiftAnim(float dt);
protected:
private:
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_congratureNode;
    CCSafeObject<CCNode> m_particleNode1;
    CCSafeObject<CCNode> m_particleNode2;
    CCSafeObject<CCLabelIF> m_titleTTF;
    CCSafeObject<CCLabelIF> m_congratuTTF;
    CCSafeObject<CCControlButton> m_okBtn;
    CCSafeObject<CCLabelIF> m_rwdInfoLabel;
    
};

class TrainingRwdCell: public CCNode
, public CCBMemberVariableAssigner
{
public:
    TrainingRwdCell(int type, int cnt): m_type(type), m_totalCnt(cnt){};
    static TrainingRwdCell* create(int type, int cnt);
    bool init();
    
    bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
private:
    CCSafeObject<CCLabelIF> m_infoLabel;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCNode> m_iconNode;
    
    int m_type;//1 2 3对应三种箱子
    int m_totalCnt;//箱子的总数
};

#endif /* defined(__IF__Test1__) */
