//
//  BuildQueueView.h
//  IF
//
//  Created by fubin on 14-2-18.
//
//

#ifndef IF_BuildQueueView_h
#define IF_BuildQueueView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "FunBuildInfo.h"
#include "CCTableViewTouchIFCell.h"
class BuildQueueView :
public CCTableViewTouchIFCell
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static BuildQueueView* create(string itemId, int qid, int type, bool allView = false,int cType = -1);

    ~BuildQueueView(){};
private:
    virtual void onExit();
    virtual void onEnter();
    
    bool init(int qid, int type,bool allView,int cType);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    void onEnterFrame(float dt);
    void setBtnStr();
    void onCDClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent);
    void confirmReturn();
    void onYes();
    void onUseTool();
    void refreshBtnData(CCObject* param);
    
    CCSafeObject<CCControlButton> m_cdBtn;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCScale9Sprite> m_bar;
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCNode> m_bg;
    CCSafeObject<CCNode> m_winPointNode;
    CCSafeObject<CCLabelIF> m_btnTxt;
    CCSafeObject<CCLabelIF> m_winPointTxt;
    CCSafeObject<CCSprite> m_wIcon;
    int m_qid;
    int m_cType;
};

#endif
