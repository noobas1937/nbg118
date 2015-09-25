//
//  AllianceCommentView.h
//  IF
//
//  Created by ganxiaohua on 15-1-14.
//
//

#ifndef __IF__AllianceCommentView__
#define __IF__AllianceCommentView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
//#include "CCScrollView.h"
#include "CCMultiColTableView.h"
#include "InputFieldMultiLine.h"
#include "DefaultTableViewDelegate.h"
#include "ChatServiceMessageListener.h"

///** 聊天颜色：普通玩家 */
//static const cocos2d::ccColor3B COLOR_TYPE_NORMAL = {132,69,80};//0xFF844550;
///** 聊天颜色：系统消息 */
//static const cocos2d::ccColor3B COLOR_TYPE_SYSTEM = {182,71,45};//0xFFB6472D;
///** 聊天颜色：GM */
//static const cocos2d::ccColor3B COLOR_TYPE_GM = {255,255,0};//0xFFFFFF00;
///** 聊天颜色：VIP玩家 */
//static const cocos2d::ccColor3B COLOR_TYPE_VIP = {163,61,178};//0xFFA33DB2;
///** 聊天颜色：私聊 */
//static const cocos2d::ccColor3B COLOR_TYPE_WHISPER = {140,109,29};//0xFF7E6D1D;
///** 聊天颜色：联盟 */
//static const cocos2d::ccColor3B COLOR_TYPE_ANLLIANCE = {0,85,33};//0xFF005521;
////钻石vip
//static const cocos2d::ccColor3B COLOR_TYPE_VIP_DIAMOND = {189,10,10};//0xFFbd0a0a;

class AllianceCommentView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
 
,public ChatServiceMessageListener
{
public:
    static AllianceCommentView* create(std::string allianceId);
    AllianceCommentView(std::string allianceId):type(0),delayTime(10),_lastPosY(0),_canRequest(true),m_cellNum(0),m_maxOffsetFlag(false),m_showCount(4),m_allianceId(allianceId){};
    virtual ~AllianceCommentView(){};
    int type;
    int delayTime;
    float _lastPosY;
    bool _canRequest;
	int m_cellNum;
	
	virtual void sendMessage(string msg);
	virtual void setHeightFromNative(int h, int usableHeightSansKeyboard);
	
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell){};
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
private:
    virtual void onEnter();
    virtual void onExit();
    CCSize getTextDimension(const char *text);
    void onAddlineEvent(CCObject * params);
    void onEditTextEvent(CCObject * params);
    void onInputFieldCloseEvent(CCObject* params);
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onPostBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void setChatName(CCObject* params);
    void updateChat(CCObject* obj);
    void onEnterFrame(float dt);
    void initDateInfo();
    void getServerData(CCObject* data);
    void callBackSuccess(CCObject* data);
    
    //    CCSafeObject<CCControlButton> m_closeBtn;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCControlButton> m_sendBtn;
    CCSafeObject<CCNode> m_editNode;
    CCSafeObject<CCNode> m_touchEditNode;
    CCSafeObject<CCScale9Sprite> m_editBg;
    CCSafeObject<CCNode> m_editBGNode;
    CCSafeObject<CCLabelIF> m_inputNum;
    
    CCSafeObject<InputFieldMultiLine> m_editBox;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCLabelIF> m_loadingMoreText;
    
    CCSafeObject<CCArray> m_data;
    float m_loadingMoreStartY;
    bool m_maxOffsetFlag;
    int m_showCount;
    bool m_isFirstVisitCountry;
    bool m_isFirstVisitAlliance;
    int m_oldEditPositionY;
    int m_oldTablePosY;
    CCSize m_oldTableViewSize;
    string m_sendMsg;
    string m_allianceId;
	
	void showNativeTextInput(float time);
	void popupCloseEvent(CCObject *params);
};

#endif /* defined(__IF__AllianceCommentView__) */
