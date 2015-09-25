//
//  EvaluationView.h
//  IF
//
//  Created by chenfubi on 15/3/12.
//
//

#ifndef __IF__EvaluationView__
#define __IF__EvaluationView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

class EvaluationView : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
//,public CCTouchDelegate
{
public:
	static EvaluationView* create();
	EvaluationView(){};
	virtual ~EvaluationView(){};
private:
	bool init();
	virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onBtnClick(int type);
    void sendCmd();
    
private:
    CCSafeObject<CCLabelIF> m_titileTTF;
    CCSafeObject<CCLabelIF> m_infoTTF;
    CCSafeObject<CCLabelIF> m_btnTTF0;
    CCSafeObject<CCLabelIF> m_btnTTF1;
    CCSafeObject<CCLabelIF> m_btnTTF2;
    CCSafeObject<CCNode> m_touchNode0;
    CCSafeObject<CCNode> m_touchNode1;
    CCSafeObject<CCNode> m_touchNode2;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_infoNode;

};

#include "CommandBase.h"
#define EVALUATEIONCMD "grade.never.show"

class EvaluationCmd : public CommandBase
{
public:
    EvaluationCmd():CommandBase(EVALUATEIONCMD)
    {
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__EvaluationView__) */
