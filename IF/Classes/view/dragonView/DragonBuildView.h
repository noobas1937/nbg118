//
//  DragonBuildView.h
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

#ifndef __IF__DragonBuildView__
#define __IF__DragonBuildView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "ArcPopupBaseView.h"
#include "CCLabelIF.h"
#include "FunBuildInfo.h"

#include "FunBuildView.h"

class DragonBuildView: public ArcPopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static DragonBuildView* create(int buildId);
    DragonBuildView():m_buildId(0),m_resType(-1),m_openNum(0){};
    virtual ~DragonBuildView(){};
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(int buildId);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void updateInfo();
    void onYesDestroy();
    
    void onFirstCancelQueue();
    void onLastCancelQueue();
    void onResetDesBtn();
    
    void onMoreClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onDestroyClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCControlButton> m_instantBtn;
    CCSafeObject<CCControlButton> m_upBtn;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCLabelIF> m_btnMsgLabel;
    CCSafeObject<CCLabelIF> m_upBtnMsgLabel;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    
    CCSafeObject<CCNode> m_moreNode;
    CCSafeObject<CCNode> m_destroyNode;
    
    CCSafeObject<CCLabelIF> m_lvLabel;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_nextLabel;
    CCSafeObject<CCLabelIF> m_nextLvLabel;
    CCSafeObject<CCLabelIF> m_nextMsgLabel;
    
    int m_buildId;
    int m_resType;
    int m_openNum;
    int m_qid;
    CCSafeObject<CCScrollView> m_scrollView;
};


#endif /* defined(__IF__DragonBuildView__) */
