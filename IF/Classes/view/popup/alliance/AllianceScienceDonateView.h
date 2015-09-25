//
//  AllianceScienceDonateView.h
//  IF
//
//  Created by fubin on 14-4-9.
//
//

#ifndef IF_AllianceScienceDonateView_h
#define IF_AllianceScienceDonateView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "ScienceInfo.h"
#include "CCClipNode.h"
#include "CCGraySprite.h"
#include "BuildUpgradeView.h"
#include "CCAniNode.h"
#include "CCStarBar.h"

class AllianceScienceDonateView :
public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
//, public CCKeypadDelegate
{
public:
    static AllianceScienceDonateView* create(int scienceId);
    AllianceScienceDonateView():m_scienceId(0),m_isUping(false),_tmpGold(0),_lastTime(-1),_sumTime(0),_isOpen(true),m_canDonate(true){};
    virtual ~AllianceScienceDonateView(){};
    void updateInfo(CCObject* obj);
private:
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
    {
        if(keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
        {
            this->closeSelf();
        }
    }
//    virtual void keyBackClicked(){this->closeSelf();}
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool init(int scienceId);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickBtn1(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickBtn2(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickBtn3(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onRefreshData(CCObject* obj);
    void onSendDonate(int type);
    void onPlayDonate();
    void onPlayText(float _time);
    void onHideDonate(float _time);
    void onEnterFrame(float dt);
    void onCostCDDonate();
    
    CCSafeObject<CCScale9Sprite> m_bar;
    CCSafeObject<CCLabelIF> m_resLabel;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_parNode;
    
    CCSafeObject<CCNode> m_touch1Node;
    CCSafeObject<CCNode> m_resIcon1;
    CCSafeObject<CCNode> m_touch2Node;
    CCSafeObject<CCNode> m_resIcon2;
    CCSafeObject<CCNode> m_touch3Node;
    CCSafeObject<CCNode> m_resIcon3;
    
    CCSafeObject<CCLabelIF> m_donate1Label;
    CCSafeObject<CCLabelIF> m_con1Label;
    CCSafeObject<CCLabelIF> m_pro1Label;
    CCSafeObject<CCLabelIF> m_get1Label;
    
    CCSafeObject<CCLabelIF> m_donate2Label;
    CCSafeObject<CCLabelIF> m_con2Label;
    CCSafeObject<CCLabelIF> m_pro2Label;
    CCSafeObject<CCLabelIF> m_get2Label;
    
    CCSafeObject<CCLabelIF> m_donate3Label;
    CCSafeObject<CCLabelIF> m_con3Label;
    CCSafeObject<CCLabelIF> m_pro3Label;
    CCSafeObject<CCLabelIF> m_get3Label;
    CCSafeObject<CCSprite> m_resIcon21;
    CCSafeObject<CCSprite> m_resIcon22;
    CCSafeObject<CCSprite> m_resIcon23;
    
    CCSafeObject<CCNode> m_barNode;
    CCSafeObject<CCLabelIF> m_barLabel;
    
    CCSafeObject<CCNode> m_starNode;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_curLabel;
    CCSafeObject<CCLabelIF> m_curNumLabel;
    CCSafeObject<CCLabelIF> m_nextLabel;
    CCSafeObject<CCLabelIF> m_nextNumLabel;
    
    CCSafeObject<CCSprite> m_jianTouSpr;
    CCSafeObject<CCNode> m_clipBarNode;
    
    CCSafeObject<CCNode> m_selNode1;
    CCSafeObject<CCNode> m_selNode2;
    CCSafeObject<CCNode> m_selNode3;
    
    CCSafeObject<CCControlButton> m_btn1;
    CCSafeObject<CCControlButton> m_btn2;
    CCSafeObject<CCControlButton> m_btn3;
    
    CCSafeObject<CCNode> m_coolNode;
    CCSafeObject<CCLabelIF> m_coolTimeLabel;
    CCSafeObject<CCLabelIF> m_noticeLabel;
    
    CCSafeObject<CCNode> m_shortNode;
    CCSafeObject<CCNode> m_longNode;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_shortTouchNode;
    CCSafeObject<CCNode> m_longTouchNode;
    
    CCSafeObject<CCStarBar> m_starBar;
    CCSafeObject<CCClipNode> m_clipNode;
    
    int m_scienceId;
    int m_isUping;
    int _tmpGold;
    int _lastTime;
    int _sumTime;
    bool _isOpen;
    bool m_canDonate;
    
    int m_curDonate;
    int m_endDonate;
    int m_sumDonate;
    int m_preDonate;
};

#endif
