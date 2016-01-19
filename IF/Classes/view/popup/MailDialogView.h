//
//  MailDialogView.h
//  IF
//
//  Created by lifangkai on 14-9-18.
//
//

#ifndef __IF__MailDialogView__
#define __IF__MailDialogView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
////#include "CCScrollView.h"
#include "CCMultiColTableView.h"
#include "InputFieldMultiLine.h"
#include "DefaultTableViewDelegate.h"
#include "MailDialogInfo.h"
#include "ChatServiceMessageListener.h"
#include "CCTableViewTouchIFCell.h"
class MailDialogView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
 
,public ChatServiceMessageListener
{
public:
    static MailDialogView* create(MailInfo* info);
    MailDialogView(MailInfo* info):m_mailInfo(info){};
	virtual ~MailDialogView();
	
	virtual void onTextChanged(string msg);
	void onTextChangedHandler(float time);
	virtual void sendMessage(string msg);
//	void onSendMessageHandler(float time);
	virtual void onBackPressed();
	void onBackPressedHandler(float time);
	virtual void setHeightFromNative(int h, int usableHeightSansKeyboard);
	
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    //	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    //    virtual CCTableViewCell gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx);
    //    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    //    virtual ssize_t numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell){};
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
private:
    virtual void onEnter();
    virtual void onExit();
    void refresh(CCObject* obj);
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void onPostBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onAddSaveClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onReturnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void readDialogContent();
    void addLoadingAni();
    void removeLoadingAni();
    void sendReloadMoreMail(float _time);
    void sendReloadMoreMailEvent(CCObject* obj);
    void refreshAddList(cocos2d::CCObject *obj);
    void setData();
    void closeInputRefeshList();
    string getModLangeage(string modLangAbbreviation);

    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
//    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCControlButton> m_sendBtn;
    CCSafeObject<CCNode> m_editNode;
    CCSafeObject<CCNode> m_bgNode;
//    CCSafeObject<CCScale9Sprite> m_editBg;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCControlButton> m_addSaveBtn;
    CCSafeObject<CCControlButton> m_unSaveBtn;
    CCSafeObject<CCControlButton> m_returnBtn;
    CCSafeObject<CCSprite> m_returnSpr;
    CCSafeObject<InputFieldMultiLine> m_editBox;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCArray> m_data;
    
    CCSafeObject<CCLabelIFTTF> m_contentText;
    
    CCSafeObject<MailInfo> m_mailInfo;
    bool m_isLoadMore;
    float m_currMinOffsetY;
    float m_currOffsetY;
	float m_newMinOffsetY;
	
	void showNativeTextInput(float time);
	void popupCloseEvent(CCObject *params);
};

#include "HFHeadImgNode.h"

class MailDialogCell :
public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
, public CCTableViewTouchIFCell
{
public:
    static MailDialogCell* create(MailDialogInfo* dialogInfo,MailInfo* mailInfo, int idx);
    MailDialogCell(MailDialogInfo* dialogInfo,MailInfo* mailInfo,int index):m_dialogInfo(dialogInfo),m_mailInfo(mailInfo),m_idx(index){};
    ~MailDialogCell(){};
    void setData(MailDialogInfo* dialogInfo,MailInfo* mailInfo,int index);
private:
    virtual void onExit();
    virtual void onEnter();
    
    bool init();

    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void initContents(string info, int lastWidth, int maxNum, int maxWidth);
    void onShieldClick(cocos2d::CCObject *pTarget, CCControlEvent touchEvent);
    void onYesClick();
    void setTranslation(CCObject* msg);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTranslateClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void openChatFun();
    void setCellStatus(bool isHint);
    bool checkSameLangBySystem(string oLang);
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCNode> m_timeNode;
    
    CCSafeObject<CCNode> m_cellnode1;
    CCSafeObject<CCNode> m_headNode1;
    CCSafeObject<CCLabelIF> m_timeText1;
    CCSafeObject<CCLabelIFTTF> m_contentText1;
    

    CCSafeObject<CCScale9Sprite> m_cellBGSpr1;
    CCSafeObject<CCScale9Sprite> m_cellBGSpr11;
    CCSafeObject<CCScale9Sprite> m_headBGSpr1;
    CCSafeObject<CCSprite> m_cellBGSprArrow11;
    CCSafeObject<CCSprite> m_cellBGSprArrow1;
    
    CCSafeObject<CCNode> m_cellnode2;
    CCSafeObject<CCNode> m_headNode2;
    CCSafeObject<CCLabelIFTTF> m_contentText2;
    CCSafeObject<CCScale9Sprite> m_cellBGSpr2;
    CCSafeObject<CCScale9Sprite> m_cellBGSpr21;
    CCSafeObject<CCLabelIF> m_tipLoadingText;
    CCSafeObject<CCScale9Sprite> m_headBGSpr2;
    CCSafeObject<CCSprite> m_cellBGSprArrow2;
    CCSafeObject<CCSprite> m_cellBGSprArrow21;
    CCSafeObject<CCNode> m_translateNode2;
    CCSafeObject<CCLabelIF> m_translateLabel2;
    CCSafeObject<CCNode> m_langNode2;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    
    int m_type;

    string m_uid;
    CCSize m_bgSize;
    int m_timeNodeY;
    CCPoint m_nameLabelPt;
    CCPoint m_vipLabelPt;
    CCPoint m_translatePt;
    int m_touchNode3Y;
    int m_msgLabelY;
    CCSize m_msgDimensions;
    CCPoint m_touchPoint;
    
    int m_idx;
    MailDialogInfo* m_dialogInfo;
    MailInfo* m_mailInfo;
    
    bool m_isTranslate;
};

#endif /* defined(__IF__MailDialogView__) */
