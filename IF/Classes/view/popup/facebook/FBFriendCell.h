//
//  FBFriendCell.h
//  IF
//
//  Created by ganxiaohua on 14-11-19.
//
//

#ifndef __IF__FBFriendCell__
#define __IF__FBFriendCell__
#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "FBFriendInfo.h"
#include "CCTableViewTouchIFCell.h"
class FBFriendCell :
public CCTableViewTouchIFCell
 
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    CREATE_FUNC(FBFriendCell);
    
    FBFriendCell(){};
    ~FBFriendCell(){}
    void setData(FBFriendInfo* info,CCNode* clickArea);
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    
    
    void onGoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void refreshHelpData(CCObject* param);
    void helpSuccess(CCObject* param);
    
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCNode> m_eventNode;
    CCSafeObject<CCNode> m_defIcon;
    CCSafeObject<CCScale9Sprite> m_clickNode;
    CCSafeObject<CCLabelIFTTF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_eventTxt;
    CCSafeObject<CCLabelIF> m_txt1;
    CCSafeObject<CCLabelIF> m_txt2;
    CCSafeObject<CCNode> m_helpNode;
    CCSafeObject<CCLabelIF> m_helpTxt;
    CCSafeObject<CCNode> m_ccbNode;
    
    CCSafeObject<FBFriendInfo> m_info;
    CCNode* m_clickArea;
    CCPoint m_touchPos;
    int m_index;
};

#endif /* defined(__IF__FBFriendCell__) */
