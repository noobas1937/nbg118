//
//  NewPlotView.h
//  IF
//
//  Created by 李锐奇 on 14-3-24.
//
//

#ifndef __IF__NewPlotView__
#define __IF__NewPlotView__

#include "CommonInclude.h"
#include "CCAniNode.h"

class NewPlotView :
public CCAniNode
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
//, public CCKeypadDelegate
, public CCBSelectorResolver

{
public:
    static NewPlotView* create(std::string dialogs);
    CCNode *getGuideNode();
private:
    NewPlotView(std::string dialogs) : m_dialogs(dialogs),m_moveType(0),m_canClick(false),m_curGuideId(""){};
    
    virtual bool isClickSwallowed(){return true;}
    virtual int getPriority(){return 0;}
    
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onCloseClick(CCObject *pSender, CCControlEvent event);
    void onAtkClick(CCObject *pSender, CCControlEvent event);

    void onEnter();
    void onExit();
    
    bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void refreshWord();
    void playMove();
    
    void canClick(float t);

    CCSafeObject<CCLabelIF> m_contentText;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCControlButton> m_closeBtn;
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_txtNode;
    CCSafeObject<CCNode> m_nextNode;
    
    CCSafeObject<CCLabelIF> m_content2Text;
    CCSafeObject<CCLabelIF> m_name2Text;
    CCSafeObject<CCNode> m_main2Node;
    CCSafeObject<CCNode> m_icon2Node;
    CCSafeObject<CCNode> m_txt2Node;
    CCSafeObject<CCNode> m_next2Node;
    
    CCSafeObject<CCSprite> m_leftArr;
    CCSafeObject<CCSprite> m_rightArr;
    
    CCSafeObject<CCControlButton> m_atkBtn;

    std::string m_dialogs;
    int m_moveType;
    bool m_canClick;
    string m_curGuideId;
};

#endif /* defined(__IF__NewPlotView__) */
