//
//  FunBuildView.h
//  IF
//
//  Created by fubin on 14-5-26.
//
//

#ifndef IF_FunBuildView_h
#define IF_FunBuildView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "ArcPopupBaseView.h"
#include "CCLabelIF.h"
#include "FunBuildInfo.h"

class BuildResCell;
class ResCell;

class FunBuildView: public ArcPopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static FunBuildView* create(int buildId);
    FunBuildView():m_buildId(0),m_resType(-1),m_openNum(0){};
    virtual ~FunBuildView(){};
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
    
    CCSafeObject<CCScale9Sprite> m_info_BG;
    CCSafeObject<CCScale9Sprite> m_buildBG2;
    CCSafeObject<CCNode> BGNode2;
    CCSafeObject<CCNode> m_nbNameNode;
    CCSafeObject<CCSprite> m_msg_BG;
    
    int m_buildId;
    int m_resType;
    int m_openNum;
    int m_qid;
    CCSafeObject<CCScrollView> m_scrollView;
};

class BuildResCell :
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static BuildResCell* create(int type, string msg, int value, string pic="", string tmpStr="", int infoType=0);
    BuildResCell():m_infoType(0){};
    void setOtherInfo(string str);
    void offSetMsgX(int offx);
    CCSafeObject<CCScale9Sprite> m_bg;
private:
    bool init(int type, string msg, int value, string picStr, string tmpStr, int infoType);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void onHelpBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_valueLabel;
    CCSafeObject<CCLabelIF> m_addLabel;
    CCSafeObject<CCControlButton> m_helpBtn;
    int m_infoType;
};
#endif
