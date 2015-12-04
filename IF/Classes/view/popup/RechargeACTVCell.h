//
//  RechargeACTVCell.h
//  IF
//
//  Created by fubin on 14-8-28.
//
//

#ifndef IF_RechargeACTVCell_h
#define IF_RechargeACTVCell_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "ArcPopupBaseView.h"
#include "CCLabelIF.h"

class RechargeACTVCell: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    RechargeACTVCell(int opentype):m_openType(opentype){};
    ~RechargeACTVCell();
    bool initRechargeACTVCell();
    static RechargeACTVCell* create(int opentype=12);
    static RechargeACTVCell* createMonthCard(int opentype=12);
    static RechargeACTVCell* createDailyRwd(int opentype=12);
    static RechargeACTVCell* createDailyGuideRwd(int opentype=12);
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
protected:
    bool initMonthCard();
    bool initDailyRwd();
    bool initDailyGuideRwd();
//    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCNode> m_mainNode;
    int m_openType;
private:
};

class ActivityBox: public CCAniNode
//, public cocos2d::CCTargetedTouchDelegate
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    ActivityBox(int type):m_type(type),m_dataItem(NULL),m_contentNode(NULL){};
    ~ActivityBox(){};
    bool initActivityBox();
    static ActivityBox* create(int type=0);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void setData();
    
    void loadSpine();
    void unLoadSpine();
    
protected:
    void onEnterFrame(float dt);
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCNode> m_contentNode;
    CCSafeObject<CCNode> m_ani;
private:
    GoldExchangeItem *m_dataItem;
    int m_type;
    void initLoverParticle();
    void initChunjieParticle();
};

#endif
