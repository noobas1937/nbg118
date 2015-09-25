//
//  ChatView.h
//  IF
//
//  Created by fubin on 13-9-25.
//
//

#ifndef IF_ChatView_h
#define IF_ChatView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
//#include "CCScrollView.h"
#include "CCMultiColTableView.h"
#include "InputFieldMultiLine.h"
#include "DefaultTableViewDelegate.h"
#include "ChatServiceMessageListener.h"
#include "FlyRollingText.h"

/** 聊天颜色：普通玩家 */
static const cocos2d::ccColor3B COLOR_TYPE_NORMAL = {132,69,80};//0xFF844550;
/** 聊天颜色：系统消息 */
static const cocos2d::ccColor3B COLOR_TYPE_SYSTEM = {182,71,45};//0xFFB6472D;
/** 聊天颜色：GM */
static const cocos2d::ccColor3B COLOR_TYPE_GM = {255,255,0};//0xFFFFFF00;
/** 聊天颜色：VIP玩家 */
static const cocos2d::ccColor3B COLOR_TYPE_VIP = {163,61,178};//0xFFA33DB2;
/** 聊天颜色：私聊 */
static const cocos2d::ccColor3B COLOR_TYPE_WHISPER = {140,109,29};//0xFF7E6D1D;
/** 聊天颜色：联盟 */
static const cocos2d::ccColor3B COLOR_TYPE_ANLLIANCE = {0,85,33};//0xFF005521;
//钻石vip
static const cocos2d::ccColor3B COLOR_TYPE_VIP_DIAMOND = {189,10,10};//0xFFbd0a0a;

enum ChatBtnType{
    C_INVITE = 0
    ,C_BLOCK_PLAYER
    ,C_BLOCK_ALLIANCE
    ,C_MSG_COPY
    ,C_SEND_MAIL
    ,C_VIEW_PLAYER
    ,C_JOIN
    ,C_BAN
};
enum CountryChatType{
    C_COUNTRY_MSG,
    C_COUNTRY_NOTICE
};
class ChatView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
 
,public ChatServiceMessageListener
{
public:
    static ChatView* create(int type = 0,string allianceId="",int countryType = 0);
    ChatView():type(0),delayTime(10),_lastPosY(0),_canRequest(true),m_cellNum(0),m_maxOffsetFlag(false),m_showCount(4),m_allianceId(string()),m_countryChatType(C_COUNTRY_MSG){};
    virtual ~ChatView(){};
    int type;
    int delayTime;
    float _lastPosY;
    bool _canRequest;
	int m_cellNum;
	
	virtual void onTextChanged(string msg);
	void onTextChangedHandler(float time);
	virtual void sendMessage(string msg);
	void onSendMessageHandler(float time);
	virtual void onBackPressed();
	void onBackPressedHandler(float time);
	virtual void setHeightFromNative(int h, int usableHeightSansKeyboard);
    void onClose();
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    //	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    //    virtual CCTableViewCell gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    //    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    //    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
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
    virtual bool init(int _type,string allianceId,int countryType);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onPostBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    //    void onCloseView(CCObject * pSender, Control::EventType pCCControlEvent);
    void onChangeState(CCObject * pSender, Control::EventType pCCControlEvent);
    void onUnionClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onCountryClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onJoinClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void setChatName(CCObject* params);
    void updateChat(CCObject* obj);
    void onEnterFrame(float dt);
    void initDateInfo();
    void reNameFunc();
    void reSetRequest(float _time);
    void showJointxt(bool show);
    void getServerData(CCObject* data);
    void callBackSuccess(CCObject* data);
    
    //    CCSafeObject<CCControlButton> m_closeBtn;
    CCSafeObject<CCControlButton> m_countryBtn;
    CCSafeObject<CCControlButton> m_unionBtn;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCControlButton> m_sendBtn;
    CCSafeObject<CCControlButton> m_btnState;
    CCSafeObject<CCNode> m_editNode;
    CCSafeObject<CCNode> m_touchEditNode;
    CCSafeObject<CCScale9Sprite> m_editBg;
    CCSafeObject<CCNode> m_editBGNode;
    CCSafeObject<CCLabelIF> m_inputNum;
    CCSafeObject<CCNode> m_tabNode;
    CCSafeObject<CCScale9Sprite> m_noticeBG1;
    CCSafeObject<CCSprite> m_noticeBG2;
    CCSafeObject<CCNode> m_noticeBGNode;
    CCSafeObject<CCNode> m_noticeTxtNode;
    
    CCSafeObject<FlyRollingText> m_rollingText;
    CCSafeObject<InputFieldMultiLine> m_editBox;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCLabelIF> m_loadingMoreText;
    float m_loadingMoreStartY;
    bool m_maxOffsetFlag;
    int m_showCount;
    bool m_isFirstVisitCountry;
    bool m_isFirstVisitAlliance;
    int m_oldEditPositionY;
    int m_oldTablePosY;
    CCSize m_oldTableViewSize;
	string m_allianceId;
    string m_sendMsg;
    
	void showNativeTextInput(float time);
	void hideNativeTextInput();
	void set2dxTextInputVisible(bool visible);
	void popupCloseEvent(CCObject *params);
    
    CountryChatType m_countryChatType;
    void switchState(bool bInit);
    void setButtonSprite(CCControlButton* btn,std::string imgName);
    void sendNotice();
    void sureSendNoticeByTool();
    void sureSendNoticeByPoint();
    void cancelSendNotice();
    
};

class ChatFunView :
public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
{
public:
    static ChatFunView* create(int idx, int type);
    ChatFunView():m_idx(0),m_type(0),m_uid(""),m_name(""),m_msg(""){};
    virtual ~ChatFunView();
    void updateInfo();
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(int idx, int type);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onCopyBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onMailBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onViewBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBlockBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
//    void onCancelBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onInviteClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onJoinClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBanClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void callFunc(CCObject* param);
    void joinFunc(CCObject* param);
    void bandFunc(CCObject* param);
    void inviteFunc(CCObject* obj);
    void onYesBlock();
    void onYesBlockAlliance();
    void onYesBan();
    void setBanBtnStatus(CCObject* obj);
    
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCNode> m_titleBg;
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCControlButton> m_banBtn;
    CCSafeObject<CCNode> m_titleNode;
    CCSafeObject<CCNode> m_btnNode;
    
    CCSafeObject<PlayerInfo> m_chaterInfo;
    vector<string> m_buttonNames;
    vector<ChatBtnType> m_buttonFuns;
    std::vector<CCControlButton*> m_buttonNodes;
    int m_idx;
    int m_type;
    string m_uid;
    string m_name;
    string m_msg;
    string m_allianceId;
};

#endif /* defined(__IF__ChatView__) */

