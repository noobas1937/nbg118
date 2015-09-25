//
//  BuildMoreInfoView.h
//  IF
//
//  Created by fubin on 14-2-21.
//
//

#ifndef IF_BuildMoreInfoView_h
#define IF_BuildMoreInfoView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"

class BuildMoreInfoView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static BuildMoreInfoView* create(int buildId, bool isUp=false, int pos=-1);
    BuildMoreInfoView():m_buildId(0),m_pos(-1),m_isUp(false),m_lv(-1){};
    virtual ~BuildMoreInfoView(){};
    void updateInfo();
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual void doWhenClose();
    virtual bool init(int buildId, bool isUp, int pos);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onCloseClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    int analyzeInfo(string moreInfomation);
    
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCControlButton> m_closeBtn;
    
    CCSafeObject<CCNode> m_itemBgNode;
    CCSafeObject<CCNode> m_itemNode;
    CCSafeObject<CCScrollView> m_scrollView;
    
    int m_buildId;
    int m_pos;
    bool m_isUp;
    int m_lv;
};

#endif
