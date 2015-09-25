//
//  StoreMallCellTip.h
//  IF
//
//  Created by lifangkai on 14-12-1.
//
//

#ifndef __IF__StoreMallCellTip__
#define __IF__StoreMallCellTip__
#include "CCBExtension.h"
#include "PopupBaseView.h"

class StoreMallCellTip : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static StoreMallCellTip* create(string msg,string title,float dy=0,CCTextAlignment align=kCCTextAlignmentCenter);
    StoreMallCellTip(){};
    virtual ~StoreMallCellTip(){};
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(string msg,string title,float dy,CCTextAlignment align);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickCloseBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCNode> m_rewardList;
    CCSafeObject<CCScale9Sprite> m_rewardBG;
    CCSafeObject<CCLabelIF> m_titleName;
    CCTextAlignment m_align;
};


class StoreMallCellTipReward : public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner

{
public:
    static StoreMallCellTipReward *create(int type,int value,int num,int color=0,bool gray = false);
    void setData(int type,int value,int num,int color,bool gray);
    CCNode* getPicNode();
private:
    StoreMallCellTipReward(int type,int value,int num,int color=0,bool gray = false):m_type(type),m_value(value),m_num(num),m_color(color),m_gay(gray){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_numText;
    CCSafeObject<CCControlButton> m_rewardBtn;
    int m_type;
    int m_value;
    int m_num;
    int m_color;
    bool m_gay;
};

#endif /* defined(__IF__StoreMallCellTip__) */
