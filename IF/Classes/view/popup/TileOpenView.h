//
//  TileOpenView.h
//  IF
//
//  Created by fubin on 14-5-23.
//
//

#ifndef IF_TileOpenView_h
#define IF_TileOpenView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "ArcPopupBaseView.h"
#include "CCLabelIF.h"
#include "FunBuildInfo.h"
#include "BuildUpgradeView.h"

class TileOpenView: public ArcPopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static TileOpenView* create(int tileId);
    TileOpenView():m_tileId(0),_tmpGold(0),m_openNum(0){};
    virtual ~TileOpenView(){};
    CCNode *getGuideNode(int itemId);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(int tileId);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onCreateOrUpClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onOkUp();
    
    void updateInfo(CCObject* p = NULL);
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCControlButton> m_upBtn;
    CCSafeObject<CCNode> m_infoList;
    
//    CCSafeObject<CCNode> m_inBtnNode;
//    CCSafeObject<CCLabelIF> m_timeLabel;
//    CCSafeObject<CCLabelIF> m_btnMsgLabel;
//    CCSafeObject<CCLabelIF> m_inBtnGoldNum;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    
    CCSafeObject<CCLabelIF> m_nextLabel;
    CCSafeObject<CCLabelIF> m_nextMsgLabel;
    CCSafeObject<CCLabelIF> m_upBtnMsgLabel;
    
    int m_tileId;
    bool m_isReturn;
    int _tmpGold;
    CCSafeObject<CCScrollView> m_scrollView;
    int m_openNum;
};

#endif
