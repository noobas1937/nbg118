//
//  UseItemStatusView.h
//  IF
//
//  Created by ganxiaohua on 14-4-18.
//
//

#ifndef __IF__UseItemStatusView__
#define __IF__UseItemStatusView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"

class UseItemStatusView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static UseItemStatusView* create(int type, std::string title, std::string description = "");
    UseItemStatusView(int type, std::string title, std::string description):m_type(type),m_title(title),m_description(description),m_endTime(0){};
    ~UseItemStatusView(){};
    void updateInfo();
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
//    void onClickBuyBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void updateTime(float t = 0.0);
    void resetTime(CCObject* param);
    
    void refeashData();
    
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCScale9Sprite> m_bar;
    CCSafeObject<CCNode> m_timeNode;
    CCSafeObject<CCLabelIF> m_descriptionText;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSize m_barSize;
    CCSafeObject<CCSprite> m_barRight;
    int m_type;
    std::string m_title;
    std::string m_description;
    int m_endTime;
    int m_time;
    int m_scrollH;
    int m_listH;
    
    CCNode *m_headParticleNode;//进度条光效
    CCSafeObject<CCNode> m_viewBg2;
    float m_viewBg2_py;
};

class UseItemStatusCell : public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewCell
{
public:
    static UseItemStatusCell* create(int itemId, string objId, int qid);
    UseItemStatusCell():m_itemId(0),m_objId(""),m_price(0){};
    void setData(int itemId, string objId, int qid);
private:
    bool init(int itemId, string objId, int qid);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickUseBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickBuyBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onUseTool();
    void onYes();
    void sureToCancel();
    
    void onEnterFrame(float dt);
    
    CCSafeObject<CCControlButton> m_useBtn;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_des2Label;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_lockNode;
    
    CCSafeObject<CCNode> m_buyNode;
    CCSafeObject<CCControlButton> m_buyBtn;
    CCSafeObject<CCLabelIF> m_btnMsgLabel;
    CCSafeObject<CCLabelIF> m_inBtnGoldNum;
    
    int m_itemId;
    string m_objId;
    int m_qid;
    float m_price;
    bool m_isByBuy;
};
#endif /* defined(__IF__UseItemStatusView__) */
