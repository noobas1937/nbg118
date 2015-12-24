//
//  UpdateAllianceInfoView.h
//  IF
//
//  Created by ganxiaohua on 14-7-9.
//
//
/*
 "zh-Hant",中文繁体
 en,英语
 de,德国
 ar,阿根廷
 "zh-Hans",中文简体
 fr,法国
 ja,日本
 nl,荷兰
 it,意大利
 es,西班牙
 ko,韩国
 pt,葡萄牙
 "pt-PT",葡萄牙
 da,丹麦
 fi,芬兰
 nb,挪威
 sv,瑞典
 ru,俄罗斯
 pl,波兰
 tr,土耳其
 uk,乌克兰
 hr,克罗地亚
 cs,捷克
 el,希腊
 he,希伯来
 ro,罗马尼亚
 sk,斯洛伐克
 th,泰国
 id,印度尼西亚
 ms,马来西亚
 "en-GB",英文（英国）
 ca,加泰罗尼亚
 hu,匈牙利
 vi,越南
 */
#ifndef __IF__UpdateAllianceInfoView__
#define __IF__UpdateAllianceInfoView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "QuestController.h"
#include "CCMultiColTableView.h"
#include "QuestInfo.h"
#include "CCTableViewTouchIFCell.h"
class LangListCell : public Layer
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static LangListCell *create(std::string lang, bool celected);
    void setData(std::string lang, bool celected);
    void setCelect(bool b);
private:
    LangListCell(std::string lang, bool celected) : m_lang(lang), m_celected(celected) { _ignoreAnchorPointForPosition = false;};
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    std::string m_lang;
    bool m_celected;
    
    CCSafeObject<CCNode> m_celectNode;
    CCSafeObject<CCNode> m_clicktNode;
    CCSafeObject<CCLabelIF> m_langText;
};
class LangList : public Layer
,public CCMultiColTableViewDelegate
,public CCMultiColTableViewDataSource
 
{
public:
    static LangList* create(CCNode* parent);
private:
    LangList(CCNode* parent): m_parent(parent) { _ignoreAnchorPointForPosition = false;};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
    void changeLangSuccess(CCObject* p);
    
    CCSafeObject<CCMultiColTableView> m_tabView;
    CCSafeObject<CCNode> m_parent;
    int m_langCnt;
    vector<string> m_langVec;
    vector<LangListCell*> m_cellVec;
    CCSize m_size;
    int m_curCeleted;
};
//=====================
class UpdateAllianceInfoView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    ~UpdateAllianceInfoView(){};
    UpdateAllianceInfoView(){};
    static UpdateAllianceInfoView *create(int open = -1);
protected:
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int open = -1);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onClickClose(CCObject * pSender, CCControlEvent pCCControlEvent);
    void updatePosition(CCObject* data);
    
    CCSafeObject<CCNode> m_funNode;
    CCSafeObject<CCScale9Sprite> m_background;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCArray> m_cell;
    
    CCSafeObject<CCNode> m_nbTouchNodeForChangeFlag;
    
    CCSafeObject<CCNode> m_allianceIcon;
    CCSafeObject<CCLabelIF> m_lvTxt;
    
    CCSafeObject<CCLayerColor> m_nb_bg1;
    CCSafeObject<CCLayerColor> m_nb_bg2;
    
};

class AllianceChangeFunCell : public Layer
 
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCEditBoxDelegate
{
public:
    static AllianceChangeFunCell *create(std::string title,AllianceInfo* info,int index=0);
    void clickHandle(CCObject *pSender, CCControlEvent event);
    CCSafeObject<CCScale9Sprite> m_bg;
    void reset();
    int m_index;
    void open();
protected:
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
private:
    AllianceChangeFunCell(std::string title,AllianceInfo* info,int index) : m_titleStr(title),m_info(info),m_index(index){ _ignoreAnchorPointForPosition = false;};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void editBoxReturn(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    
    void onSaveClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onEditClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onNameModifyClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onModifyAbbrClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onStrictBtnclick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void modifyRankClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void modifyTNameClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void saveSuccess(CCObject* p);
    void attrSuccess(CCObject* p);
    void saveNameSuccess(CCObject* p);
    void modifyAbbrSuccess(CCObject* p);
    void checkAllianceNameAndAbbr(float t);
    void updateModifyBtnStatus(CCObject* data);
    void onRestrictSuccess(CCObject* data);
    void onCheckAllianceTag(CCObject* data);
    void modifyRankTitleCallBack(CCObject* data);
    void modifyRankTitleSuccess();
    void checkRankTitle(float t);
    void checkAllianceRestrictChange(float t);
    void modifyTerritoryTitleCallBack(CCObject* data);
    void modifyTerritoryTitleSuccess();
    void checkTerritoryTitle(float t);
    void showTabView(float t);
    void CommandAllianceRestrict(float t);
    void strictSuccess(CCObject* p);
    bool isNumStr(std::string);
    void cellEnlarge();
    std::string settingcastle;
    std::string settingFight;
    
    std::string m_titleStr;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCNode> m_funNode1;
    CCSafeObject<CCControlButton> m_btnSave;
    CCSafeObject<CCControlButton> m_btnEdit;
    CCSafeObject<CCLabelIFTTF> m_announceTxt;
    CCSafeObject<CCNode> m_announceNode;
    CCSafeObject<CCEditBox> m_announceEditBox;
    
    CCSafeObject<CCNode> m_funNode2;
    CCSafeObject<CCLabelIF> m_touchTxt1;
    CCSafeObject<CCLabelIF> m_touchTxt2;
    CCSafeObject<CCSprite> m_touchBtn1;
    CCSafeObject<CCScale9Sprite> m_touchBtnBg;
    
    CCSafeObject<CCNode> m_itemcon;
    CCSafeObject<CCNode> m_funNode;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCScale9Sprite> m_bg2;
    CCSafeObject<CCSprite> m_arrow;
    CCSafeObject<CCLabelIF> m_tipResuitTxt;
    
    //name
    CCSafeObject<CCLabelIF> m_nameTip1;
    CCSafeObject<CCLabelIF> m_nameTip2;
    CCSafeObject<CCNode> m_inputNameNode;
    CCSafeObject<CCControlButton> m_modifyBtn;
    CCSafeObject<CCNode> m_funNameNode;
    CCSafeObject<CCLabelIF> m_modifyNameTxt;
    CCSafeObject<CCLabelIF> m_nameGoldTxt;
    CCSafeObject<CCNode> m_nameGold;
    CCSafeObject<CCNode> m_nameOKNode;
    //Abbr
    CCSafeObject<CCLabelIF> m_abbrTip1;
    CCSafeObject<CCLabelIF> m_abbrTip2;
    CCSafeObject<CCNode> m_inputAbbrNode;
    CCSafeObject<CCControlButton> m_modifyAbbrBtn;
    CCSafeObject<CCNode> m_funAbbrNode;
    CCSafeObject<CCNode> m_strictNode;
    CCSafeObject<CCLabelIF> m_modifyAbbrTxt;
    CCSafeObject<CCLabelIF> m_abbrGoldTxt;
    CCSafeObject<CCSprite> m_brifGold;
    CCSafeObject<CCEditBox> m_nameAndAbbrEditBox;
    CCSafeObject<CCNode> m_abbrOKNode;
    
    //flag
    CCSafeObject<CCLabelIF> m_flagTitle1;
    CCSafeObject<CCLabelIF> m_flagTitle2;
    CCSafeObject<CCNode> m_oldFlagNode;
    CCSafeObject<CCControlButton> m_modifyFlagBtn;
    CCSafeObject<CCNode> m_newFlagNode;
    CCSafeObject<CCLabelIF> m_modifyFlagTxt;
    CCSafeObject<CCLabelIF> m_flagGoldTxt;
    CCSafeObject<CCNode> m_flagList;
    CCSafeObject<CCNode> m_funFlagNode;
    CCSafeObject<CCMultiColTableView> m_tabView;
    CCSafeObject<CCArray> m_data;
    
    //lang
    CCSafeObject<CCNode> m_funLangNode;
    CCSafeObject<CCNode> m_langList;
    CCSafeObject<CCLabelIF> m_allianceLangIntr;
    
    //rank title
    CCSafeObject<CCLabelIF> m_oldTitle;
    CCSafeObject<CCLabelIF> m_newTitle;
    
    CCSafeObject<CCLabelIFTTF> m_rank5Txt;
    CCSafeObject<CCNode> m_inputRank5;
    CCSafeObject<CCLabelIFTTF> m_rank4Txt;
    CCSafeObject<CCNode> m_inputRank4;
    CCSafeObject<CCLabelIFTTF> m_rank3Txt;
    CCSafeObject<CCNode> m_inputRank3;
    CCSafeObject<CCLabelIFTTF> m_rank2Txt;
    CCSafeObject<CCNode> m_inputRank2;
    CCSafeObject<CCLabelIFTTF> m_rank1Txt;
    CCSafeObject<CCNode> m_inputRank1;
    CCSafeObject<CCLabelIF> m_rankTip;
    CCSafeObject<CCControlButton> m_rankBtn;
    CCSafeObject<CCNode> m_funRankNode;
    CCSafeObject<CCEditBox> m_rank5EditBox;
    CCSafeObject<CCEditBox> m_rank4EditBox;
    CCSafeObject<CCEditBox> m_rank3EditBox;
    CCSafeObject<CCEditBox> m_rank2EditBox;
    CCSafeObject<CCEditBox> m_rank1EditBox;
    CCSafeObject<CCSprite> m_check1;
    CCSafeObject<CCSprite> m_check2;
    CCSafeObject<CCSprite> m_check3;
    CCSafeObject<CCSprite> m_check4;
    CCSafeObject<CCSprite> m_check5;
    
    //tname title
    CCSafeObject<CCLabelIF> m_oldTNameTitle;
    CCSafeObject<CCLabelIF> m_newTNameTitle;
    
    CCSafeObject<CCLabelIFTTF> m_name1Txt;
    CCSafeObject<CCNode> m_inputName1;
    CCSafeObject<CCLabelIFTTF> m_name2Txt;
    CCSafeObject<CCNode> m_inputName2;
    CCSafeObject<CCLabelIFTTF> m_name3Txt;
    CCSafeObject<CCNode> m_inputName3;
    CCSafeObject<CCLabelIFTTF> m_name4Txt;
    CCSafeObject<CCNode> m_inputName4;
    CCSafeObject<CCLabelIFTTF> m_name5Txt;
    CCSafeObject<CCNode> m_inputName5;
    CCSafeObject<CCLabelIF> m_nameTip;
    CCSafeObject<CCControlButton> m_nameBtn;
    CCSafeObject<CCNode> m_funTNameNode;
    CCSafeObject<CCEditBox> m_name1EditBox;
    CCSafeObject<CCEditBox> m_name2EditBox;
    CCSafeObject<CCEditBox> m_name3EditBox;
    CCSafeObject<CCEditBox> m_name4EditBox;
    CCSafeObject<CCEditBox> m_name5EditBox;
    CCSafeObject<CCSprite> m_tcheck1;
    CCSafeObject<CCSprite> m_tcheck2;
    CCSafeObject<CCSprite> m_tcheck3;
    CCSafeObject<CCSprite> m_tcheck4;
    CCSafeObject<CCSprite> m_tcheck5;
    CCSafeObject<CCSprite> m_lock1;
    CCSafeObject<CCSprite> m_lock2;
    CCSafeObject<CCSprite> m_lock3;
    CCSafeObject<CCSprite> m_lock4;
    CCSafeObject<CCSprite> m_lock5;
    
    //strict
    CCSafeObject<CCEditBox> m_castlelevel;
    CCSafeObject<CCEditBox> m_fightability;
    CCSafeObject<CCNode> m_FightLevelNode;
    CCSafeObject<CCNode> m_castleLevelNode;
    CCSafeObject<CCLabelIFTTF> m_castleText;
    CCSafeObject<CCLabelIFTTF> m_FightText;
    CCSafeObject<CCLabelIFTTF> m_ResTitle;
    CCSafeObject<CCScale9Sprite> m_leftBar;
    CCSafeObject<CCScale9Sprite> m_rightBar;
    CCSafeObject<CCLabelIF> m_DesText;
    CCSafeObject<CCControlButton> m_strictBtn;
    
    AllianceInfo* m_info;
    unsigned long m_clickTime;
    bool m_open;
    bool m_moveFlag;
    int m_currAttr;
    std::string m_lastname;
    CCPoint m_touchPoint;
};


class AllianceFlagCell : public CCTableViewTouchIFCell
 
{
public:
    static AllianceFlagCell *create(std::string icon);
    void setData(std::string icon);
private:
    AllianceFlagCell(std::string icon) : m_icon(icon){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    CCSafeObject<CCNode> m_flagNode;
    std::string m_icon;
};

#endif /* defined(__IF__UpdateAllianceInfoView__) */
