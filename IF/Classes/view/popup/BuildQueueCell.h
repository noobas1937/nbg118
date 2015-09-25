//
//  BuildQueueCell.h
//  IF
//
//  Created by fubin on 14-7-31.
//
//

#ifndef IF_BuildQueueCell_h
#define IF_BuildQueueCell_h

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "ParticleController.h"
#include "CCAniNode.h"
#include "AllianceIntroTip.h"

#define Queue_ST_INIT -1
#define Queue_ST_IDLE 0
#define Queue_ST_WORK 1
#define Queue_ST_LOCK 2

class BuildQueueCell: public CCAniNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    BuildQueueCell():m_qid(0),m_isFree(false),m_curTime(0),m_curState(Queue_ST_LOCK),m_introTip(NULL){};
    ~BuildQueueCell(){};
    bool initBuildQueueCell(int qid);
    static BuildQueueCell* create(int qid);
    
    void setQid(int qid);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    int m_qid;
protected:
    void onShowInfo();
    void onShow2Info();
    void onEnterFrame(float _tiem);
    void onAddParticle();
    void addIntroTip();
    void removeIntroTip();
    void onYesOpen();
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_proNode;
    CCSafeObject<CCNode> m_infoNode;
    CCSafeObject<CCNode> m_hammerNode;
    CCSafeObject<CCNode> m_touchNode;
    
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCLabelIF> m_freeLabel;
    CCSafeObject<CCLabelIF> m_handLabel;
    CCSafeObject<CCScale9Sprite> m_handBG1;
    CCSafeObject<CCSprite> m_handBG2;
    CCSafeObject<CCSprite> m_handBG3;
    
    CCSafeObject<CCNode> m_info2Node;
    CCSafeObject<CCLabelIF> m_hand2Label;
    CCSafeObject<CCLabelIF> m_hand3Label;
    CCSafeObject<CCScale9Sprite> m_hand2BG1;
    CCSafeObject<CCSprite> m_hand2BG3;
    
    CCSafeObject<CCProgressTimer> m_proTimer;
    
    CCSafeObject<CCNode> m_particleNode;
    
    AllianceIntroTip *m_introTip;
private:
    bool m_isFree;
    int m_curTime;
    int m_curState;
    bool m_flyHintFlag;
    void flyHint();
    void resetHintFlag(float _time);
};

#endif
