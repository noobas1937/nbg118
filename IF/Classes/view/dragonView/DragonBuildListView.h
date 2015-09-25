//
//  DragonBuildListView.h
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

#ifndef __IF__DragonBuildListView__
#define __IF__DragonBuildListView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "ArcPopupBaseView.h"
#include "CCLabelIF.h"
#include "FunBuildInfo.h"
#include "ArcScrollView.h"
#include "BuildUpgradeView.h"

class DragonBuildListView : public ArcPopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
{
public:
    static DragonBuildListView* create(int pos);
    DragonBuildListView():m_pos(0),m_openNum(0),m_itemId(0),m_initEnd(false),gBuildId(0){};
    ~DragonBuildListView(){};
    void updateInfo(int pos);
    CCNode *getGuideNode(string _key);
    
private:
    virtual void onExit();
    virtual void onEnter();
    
    bool init(int pos);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onCloseView();
    void arcButtonClick();
    void refeash(int idx);
    void onInitEnd(float _time);
    
    void AnimationCallback();
    void AnimationCallbackEnd();
    
    void onCreateOrUpClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCNode> m_btnNode1;
    CCSafeObject<CCNode> m_arcNode;
    CCSafeObject<CCNode> m_upNode;
    CCSafeObject<CCLabelIF> m_upBtnMsgLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_lockLabel;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCControlButton> m_upBtn;
    
    CCSafeObject<CCNode> m_titleLvNode;
    CCSafeObject<CCScale9Sprite> m_titleLvBG;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCNode> m_moveNode;
    CCSafeObject<CCNode> m_guidNode;
    
    CCSafeObject<ArcScrollView> m_arcScroll;
    
    vector<int> m_buildIds;
    map<int,string> m_buildLockInfos;
    
    int m_pos;
    int m_openNum;
    int m_itemId;
    bool m_initEnd;
    int gBuildId;
};

#endif /* defined(__IF__DragonBuildListView__) */
