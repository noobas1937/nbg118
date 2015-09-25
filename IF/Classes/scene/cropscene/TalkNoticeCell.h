//
//  TalkNoticeCell.h
//  IF
//
//  Created by fubin on 14-9-4.
//
//

#ifndef IF_TalkNoticeCell_h
#define IF_TalkNoticeCell_h

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "FunBuildInfo.h"
#include "ParticleController.h"
#include "Person.h"
#include "CCLabelBatch.h"
#include "CCAniNode.h"

#define MSG_TALK_ACT "msg_talk_act"
#define MSG_TALK_REMOVE "msg_talk_remove"
#define MSG_TALK_CROSS_SERVER_ACTIVITY "msg_talk_cross_server_activity"

class TalkNoticeCell: public CCAniNode
 
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    TalkNoticeCell():m_type(0),m_dIdx(0),m_target(NULL){};
    ~TalkNoticeCell(){};
    bool initTalkNoticeCell(int type);
    static TalkNoticeCell* create(int type);
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onClickThis();
    
    virtual void onEnter();
    virtual void onExit();
    
protected:
    
private:
    CCSafeObject<CCScale9Sprite> m_bg9Spr;
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCSafeObject<CCNode> m_touchNode;
    vector<string> m_dialogs;
    
    void onShow(CCObject* obj);
    void onRemove(CCObject* obj);
    void onHide(float _time);
    void onEnterFrame(float dt);
    int m_type;
    int m_dIdx;
    int m_count;
    CCSafeObject<CCNode> m_target;
};

#endif
