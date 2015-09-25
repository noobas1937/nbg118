//
//  AllianceResourceTradeView.h
//  IF
//
//  Created by ganxiaohua on 14-4-4.
//
//

#ifndef __IF__AllianceResourceTradeView__
#define __IF__AllianceResourceTradeView__

#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "PlayerInfo.h"
#include "CCSliderBar.h"

class AllianceResourceTradeView: public PopupBaseView
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
{
public:
    static AllianceResourceTradeView* create(PlayerInfo* info);
    static void createResurceTradeByUid(const string& uid,int pointId,int mainLv);
    
    AllianceResourceTradeView(const string& uid,int pointId,int mainLv):m_uid(uid),m_pointId(pointId),m_mainLv(mainLv){};
    
    AllianceResourceTradeView(PlayerInfo* info):m_info(info){};
    virtual ~AllianceResourceTradeView(){};
    void updateInfo();
    void tradeSuccess(CCObject* p);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    
    void updateTradeValue(CCObject* obj);
    void updateTradeData(bool isEnd=false);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onCloseClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtnTradeClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void updateChangeStatus(float t);
    void playResourceAni();
    void getPlayerData(CCObject* obj);
    
    CCSafeObject<CCLabelIF> m_wood;
    CCSafeObject<CCLabelIF> m_stone;
    CCSafeObject<CCLabelIF> m_iron;
    CCSafeObject<CCLabelIF> m_food;
    CCSafeObject<CCLabelIF> m_rateTxt;
    CCSafeObject<CCLabelIF> m_loadTxt;
    CCSafeObject<CCLabelIF> m_resTxt;
    CCSafeObject<CCLabelIF> m_infoTxt;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCControlButton> m_btnTrade;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_viewBg;
    
    CCSafeObject<CCLabelIF> m_cityNameTxt;
    CCSafeObject<CCLabelIF> m_tradeWood;
    CCSafeObject<CCLabelIF> m_tradeFood;
    CCSafeObject<CCLabelIF> m_tradeIron;
    CCSafeObject<CCLabelIF> m_tradeStone;
    CCSafeObject<CCLabelIF> m_tradePlayerTxt;
    CCSafeObject<CCNode> m_ironNode;
    CCSafeObject<CCNode> m_stoneNode;
    CCSafeObject<CCNode> m_coverNode1;
    CCSafeObject<CCNode> m_coverNode2;
    
    CCSafeObject<CCNode> m_funNode1;
    CCSafeObject<CCNode> m_funNode2;
    CCSafeObject<CCNode> m_buttomNode;
    CCSafeObject<CCNode> m_scrollNode;
    CCSafeObject<CCNode> m_ironTradeNode;
    CCSafeObject<CCNode> m_stoneTradeNode;
    
    CCSafeObject<CCNode> m_cityIcon;
    CCSafeObject<CCNode> m_woodIcon;
    CCSafeObject<CCNode> m_foodIcon;
    CCSafeObject<CCNode> m_ironIcon;
    CCSafeObject<CCNode> m_stoneIcon;
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCArray> m_resList;
    CCSafeObject<PlayerInfo> m_info;
    int m_page;
    int m_rate = 0;
    unsigned long m_lastTotalFood;
    unsigned long m_tradeToServerFood;
    unsigned long m_lastTrade;
    int m_pointId;
    std::string m_uid;
    int m_mainLv;
};

class AllianceResourceCell :
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static AllianceResourceCell* create(PlayerInfo* player,int index);
    AllianceResourceCell(PlayerInfo* player,int index):m_info(player),m_index(index){};
    unsigned long m_currentValue;
    unsigned long m_tradeValue;
    int m_index;
    void resetData();
    CCSafeObject<CCSliderBar> m_slider;
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void sliderCallBack(CCObject*sender,CCControlEvent even);
        
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_valueTxt;
    CCSafeObject<CCNode> m_resNode;
    CCSafeObject<CCNode> m_sliderNode;
    CCSafeObject<CCNode> m_coverNode;
    CCSafeObject<CCLabelIF> m_coverTxt;
    
    AllianceResourceTradeView* m_tradeView;
    PlayerInfo* m_info;
    bool m_invalidSliderMove;
};
#endif /* defined(__IF__AllianceResourceTradeView__) */
