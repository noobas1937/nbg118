//
//  LotteryAct2ShowView.h
//  IF
//
//  Created by chenfubi on 15/2/6.
//
//

#ifndef __IF__LotteryAct2ShowView__
#define __IF__LotteryAct2ShowView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

class LotteryAct2ShowView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
{
public:
    static LotteryAct2ShowView* create();
	virtual bool init();
    LotteryAct2ShowView():m_aniIndex(-1)/*, m_itemIndex(0)*/{};
    virtual ~LotteryAct2ShowView(){};
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void initAniNode();
    int hasTouchNode(cocos2d::CCTouch *pTouch);
    void setDesNode(int index);
    string getIcon(string itemId);
    string getColor(string itemId);
    
private:
    CCSafeObject<CCNode> m_aniListNode;
    CCSafeObject<CCNode> m_midNode;
    CCSafeObject<CCNode> m_desNode;
    CCSafeObject<CCLabelIF> m_desName;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_titleTTF;
    CCSafeObject<CCScale9Sprite> m_desBg;
    CCSafeObject<CCScale9Sprite> m_bg;
    
    int m_aniIndex;
//    int m_itemIndex;
};

#endif /* defined(__IF__LotteryAct2ShowView__) */
