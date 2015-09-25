//
//  YesNoDialog.h
//  IF
//
//  Created by zhaohf on 13-9-04.
//
//

#ifndef __YesNoDialog__
#define __YesNoDialog__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCBExtension.h"
#include "CCTypesExt.h"
#include "CCModelLayer.h"
#include "LocalController.h"
#include "PopupBaseView.h"
USING_NS_CC;
USING_NS_CC_EXT;

class YesNoDialog :
public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner

{
public:

	YesNoDialog(void);
    virtual ~YesNoDialog(void);
    static YesNoDialog* show(const char * content,std::function<void()> function,int gold = 0);
    //YesNoDialog
    static YesNoDialog* show(const char * content,CCCallFunc* yesfunc,int gold=0,bool canShowMulti=false, CCCallFunc* nofunc=NULL, CCCallFunc* selfunc=NULL, const char * buttonStr = "", const char * iconName = "");
    static YesNoDialog* marchAlertShow(const char *content,CCCallFunc*yesfunc,CCCallFunc*nofunc);
    static YesNoDialog* lotteryShow(const char * content,CCCallFunc* yesfunc,int gold=0,bool alignCenter=false, CCCallFunc* nofunc=NULL, CCCallFunc* selfunc=NULL, const char * buttonStr = "", const char * iconName = "");
    //YesNoDialog
    static YesNoDialog* showTime(const char * content,CCCallFunc* yesfunc,int time, const char * btnMsg="", bool isBuild=true, int sumTime=0, int sumGold=0);
    static YesNoDialog* showResCost(const char * content,CCCallFunc* yesfunc, const char * btnMsg, bool isBuild, int sumCost,int resType);
    //YesDialog
    static YesNoDialog* showYesDialog(const char * content,bool canShowMulti=false,CCCallFunc * Yesfunc = NULL,bool originConfirm = false);
    //YesNoDialog Variable Button Title
    static YesNoDialog* showVariableTitle(const char * content,CCCallFunc * func,const char * buttonName,bool isCloseCallback = false, int jsonTime = 0);
    //quit dialog
    static YesNoDialog* showQuitDialog(CCCallFunc * func,bool forLoading=false);
    //YesNoDialog Variable Button Title gold
    static YesNoDialog* showButtonAndGold(const char * content,CCCallFunc * func,const char * buttonName,int gold, const char* desDialog = "");
    //YesNoDialog Variable Button Title priceType value
    static YesNoDialog* showButtonAndPriceType(const char * content,CCCallFunc * func,const char * buttonName,int type,int value);
    static YesNoDialog* showAllianceConfirm(const char * content,const char * buttonName);
    static YesNoDialog* showConfirmCloseAllView(const char * content,const char * buttonName);
    //YesNoDialog
    static YesNoDialog* showQueueConfirm(const char * content,CCCallFunc* yesfunc,const char* t1,const char* t2,int time1,int time2, const char * buttonName,int gold);
    static YesNoDialog* showTimeWithDes(const char * content,const char* timeDes,int time);
    static void gotoPayTips();
    void gotoPayCallback();
    
 	bool init(const char *content,const char * buttonName = _lang("confirm").c_str(), int timeJson = 0);
    void setUpAutoClose();
    void doAutoClose(float t);
    void hideNoButton();
    void showCancelButton();
    void setYesButtonTitle(const char *content);
    void setYesText(const char *content);
    void setNoButtonTitle(const char *content);
    void showMuteTimeNode();
    void keypressedBtnCancel(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickNoticeBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void keyPressedBtnClose(CCObject * pSender, Control::EventType pCCControlEvent);
    
    // for position modify
    CCSafeObject<CCControlButton> m_btnOk;
    CCSafeObject<CCControlButton> m_btnCancel;
    CCSafeObject<CCLabelIF> m_goldNum;
    CCSafeObject<CCLabelIF> m_btnText;
    CCSafeObject<CCSprite> m_goldIcon;
    CCSafeObject<CCNode> m_resNode;
    CCSafeObject<CCLabelIF> m_titleText;
    
    CCSafeObject<CCNode> m_noticeNode;
    CCSafeObject<CCControlButton> m_noticeBtn;
    CCSafeObject<CCSprite> m_noticeSpr;
    CCSafeObject<CCLabelIF> m_noticeLabel;
    CCSafeObject<CCNode> m_subNode;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCNode> m_sprBG;
    CCSafeObject<CCLabelTTF> m_contentLabel;
    
    CCSafeObject<CCNode> m_muteTimeNode;
    CCSafeObject<CCLabelIF> m_txt1;
    CCSafeObject<CCLabelIF> m_txt2;
    CCSafeObject<CCLabelIF> m_txt3;
    CCSafeObject<CCLabelIF> m_txt4;
    CCSafeObject<CCNode> m_box1;
    CCSafeObject<CCNode> m_box2;
    CCSafeObject<CCNode> m_box3;
    CCSafeObject<CCNode> m_box4;
    CCSafeObject<CCNode> m_selectSpr;
    bool m_isAlliance;
    bool m_closeAll;
protected:
    void onEnter();
    void doEnter();
    void onExit();
    
    void onEnterFrame(float dt);
    void onUpdateLabel(float dt);
    void onEnterFrame1(float dt);
	// ccb routines
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char* pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
	void keypressedBtnOk(CCObject * pSender, Control::EventType pCCControlEvent);
    void keyPressedBtnMoreGame(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void addToSubLayer(CCNode* node);
    void clearSubLayer();
    
//    CCSafeObject<CCModelLayerColor> m_layerColorBG;
    CCSafeObject<CCLayer> m_subLayer;
    CCSafeObject<CCRenderTexture> m_renderTexture;
    int m_opacity;//蒙板透明度
    int m_time;
    bool m_isBuild;
    std::vector<std::string> mContents;
    std::string mTimeDes;
    
    int m_sumTime;
    int m_sumGold;
    
private:

    
    static YesNoDialog* getInstance();
    static YesNoDialog* _yesNoDialogInstance;
    static string m_dialogStr;
    static string m_buttonName;
    int m_index;
    static CCCallFunc* m_callBackFunc;
    static bool m_isCloseCallback;
    void showYesDialogResume(float time);
    void showVariableTitleResume(float time);
    void showConfirmCloseAllViewResume(float time);
    int getBannedTime(int index);
    
    
    void serverUpdateInfo(const char * content, int timeJson);
    
    //回调
    CC_SYNTHESIZE_SAFEOBJECT(CCCallFunc, m_onYes, YesCallback);
    CC_SYNTHESIZE_SAFEOBJECT(CCCallFunc, m_onNo, NoCallback);
    CC_SYNTHESIZE_SAFEOBJECT(CCCallFunc, m_onClose, CloseCallback);
    CC_SYNTHESIZE_SAFEOBJECT(CCCallFunc, m_onSel, SelCallback);
    
    // function callback
    std::function<void()> m_function;
    
    CC_SYNTHESIZE(bool, isCloseOnNoButton, CloseOnNoButton);
    CC_SYNTHESIZE(bool, disableWhenTouched, DisableWhenTouched);

    int m_Endtime;

};


///////////////////////////////////////////////////////
//
// ConfirmCoinView
//
///////////////////////////////////////////////////////

class ConfirmCoinView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static ConfirmCoinView* create();
    ConfirmCoinView();
    ~ConfirmCoinView();
protected:
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void keyPressedBtnOk(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    CCSafeObject<CCLabelIF> m_descriptionText;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCNode> m_nodeAdv;
    CCSafeObject<CCControlButton> m_confirmBtn;
    CCSafeObject<CCNode> m_touchNode;
};

#endif