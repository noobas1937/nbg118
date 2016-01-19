//
//  DetectMailPopUpView.h
//  IF
//
//  Created by 李锐奇 on 14-4-15.
//
//

#ifndef __IF__DetectMailPopUpView__
#define __IF__DetectMailPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "HFHeadImgNode.h"
class DetectBgCell;

class DetectMailPopUpView : public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
//, public CCTouchDelegate

{
public:
    static DetectMailPopUpView *create(MailInfo *info);
    
private:
    DetectMailPopUpView(MailInfo *info) : m_info(info){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onDeleteClick(CCObject *pSender, CCControlEvent event);
    void onAddSaveClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onShareClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onReturnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onWriteClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void refreshView(CCObject* p = NULL);
    void getData();
    
    void addPlayerInfo();
    void addCityDefence();
    void addResourceInfo();
    void addSelfHeroInfo();
    void addFortInfo();
    void addTowerInfo();
    void addScienceInfo();
    void addTalentInfo();
    void addAbilityInfo();
    void addAllianceHelpInfo();
    void onOkDeleteMail();
    void resetCellBG(DetectBgCell *cell);
    
    CCLabelIF *createLabel(int x, int y, std::string str, int fontSize = 20);
    
    CCSafeObject<MailInfo> m_info;
    int m_totalH;
    std::string m_abilityId;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCControlButton> m_deleteBtn;
    CCSafeObject<CCLabelIF> m_mailTitle;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCLabelIF> m_titleText;
   // CCSafeObject<CCLabelIF> m_attackText;
   // CCSafeObject<CCLabelIF> m_titleText;
    //CCSafeObject<CCLabelIF> m_vsText;
  //  CCSafeObject<CCLabelIF> m_deleteBtnTitle;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCScale9Sprite> m_buildBG;    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_listNode;

    CCSafeObject<CCControlButton> m_addSaveBtn;
//    CCSafeObject<CCControlButton> m_unSaveBtn;
    CCSafeObject<CCControlButton> m_shareBtn;
    CCSafeObject<CCControlButton> m_returnBtn;
    CCSafeObject<CCSprite> m_returnSpr;
    CCSafeObject<CCControlButton> m_writeBtn;

    
};

class DetectPlayerInfoCell : public CCNode
, public CCBMemberVariableAssigner

{
public:
    static DetectPlayerInfoCell *create(CCDictionary *dict,int pointType,MailInfo* info);
    void cellTouch(CCTouch *pTouch);
private:
    DetectPlayerInfoCell(CCDictionary *dict,int pointType,MailInfo* info) : m_dict(dict),m_pointType(pointType),m_info(info){};
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    CCDictionary *m_dict;
    int m_pointType;
    CCSafeObject<CCLabelIF> m_playLv;
    CCSafeObject<Label> m_playName;
    CCSafeObject<CCLabelIF> m_playPoint;
    CCSafeObject<CCNode> m_playHeadNode;
    CCSafeObject<CCNode> m_palyInfoNode;
    CCSafeObject<CCNode> m_posBG;
    CCSafeObject<CCNode> m_failNode;
    CCSafeObject<CCLabelIF> m_failText;
    CCSafeObject<CCLabelIF> m_playerTitle;
    CCSafeObject<CCLabelIF> m_towerTipTitle;
    CCSafeObject<CCNode> m_battlePicNode;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    CCSafeObject<MailInfo> m_info;
    CCSafeObject<CCRenderTexture> m_selfModelLayer;
};

class DetectedByPlayerCell : public Layer
, public CCBMemberVariableAssigner
 

{
public:
    static DetectedByPlayerCell *create(MailInfo* dict);
    void cellTouch(CCTouch *pTouch);
private:
    DetectedByPlayerCell(MailInfo* dict) : m_info(dict){_ignoreAnchorPointForPosition = false;
    setAnchorPoint(Vec2(0,0));};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    CCSafeObject<MailInfo> m_info;
    CCSafeObject<CCNode> m_playHeadNode;
    CCSafeObject<CCLabelIF> m_tipText;
    CCSafeObject<CCLabelIF> m_nameText;
//    CCSafeObject<CCLabelIF> m_playerTitle;
    CCSafeObject<CCNode> m_battlePicNode;
    CCSafeObject<CCSprite> m_headBG;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    CCSafeObject<CCNode> m_PicNode;
    CCSafeObject<CCRenderTexture> m_selfModelLayer;
    CCPoint m_startPoint;
};

//---

class DetectGeneralInfoCell : public CCNode
, public CCBMemberVariableAssigner

{
public:
    static DetectGeneralInfoCell *create(CCDictionary *dict);
private:
    DetectGeneralInfoCell(CCDictionary *dict) : m_dict(dict){};
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    CCDictionary *m_dict;
    CCSafeObject<CCLabelIF> m_levelText;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_numText;
    CCSafeObject<CCNode> m_headContainer;
    CCSafeObject<CCNode> m_armyIconContainer;
};

//-----

class DetectSiceneInfoCell : public CCNode
, public CCBMemberVariableAssigner

{
public:
    static DetectSiceneInfoCell *create(CCDictionary *dict);
private:
    DetectSiceneInfoCell(CCDictionary *dict) : m_dict(dict){};
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    CCDictionary *m_dict;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCNode> m_iconContainer;
};

class DetectBgCell : public CCNode
, public CCBMemberVariableAssigner

{
public:
    static DetectBgCell *create(std::string title);
    CCSafeObject<CCNode> m_bg;
//    CCSafeObject<CCNode> m_kuangbg;
    CCSafeObject<CCNode> m_titleBG;

    int getBgCellHeight();
    void setBgHeight(float bgH);
    void setNumTitle(std::string str1, std::string str2);
    void setTitle(std::string str1, std::string str2, std::string str3);
private:
    DetectBgCell(std::string title) : m_titleStr(title){};
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    std::string m_titleStr;
    CCSafeObject<CCLabelIF> m_title;
    CCSafeObject<CCLabelIF> m_title1;
    CCSafeObject<CCLabelIF> m_title2;
    CCSafeObject<CCNode> m_head;


};

class MailItemCell : public CCNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner

{
public:
    static MailItemCell *create(std::string title,std::string icon,std::string id="");
    void clickAbilityHandle(CCObject *pSender, CCControlEvent event);
    CCDictionary* m_detectReport;
    GeneralInfo* m_generalInfo;
private:
    MailItemCell(std::string title,std::string icon,std::string id) : m_titleStr(title),m_iconPath(icon),m_id(id){};
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    std::string m_titleStr;
    std::string m_iconPath;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCControlButton> m_btn;
    CCSafeObject<CCNode> m_icon;
    std::string m_id;
    
};
class DetectRewardPreviewCell : public CCTableViewCell
{
public:
    static DetectRewardPreviewCell *create(int type, int value, int num = 0);
    void setData(int type, int value, int num = 0);
    
private:
    DetectRewardPreviewCell(int type, int value, int num) : m_type(type), m_value(value), m_num(num){};
    virtual bool init();
    void refreshView();
    int m_type;
    int m_value;
    int m_num;
};

class AllianceHelpCell : public CCNode
, public CCBMemberVariableAssigner

{
public:
    static AllianceHelpCell *create(CCDictionary *dict, CCArray *arr);
    int getH();
private:
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool init(CCDictionary *dict, CCArray *arr);
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_lvText;
    CCSafeObject<CCNode> m_icon;

    CCSafeObject<CCDictionary> m_dict;
    CCSafeObject<RenderTexture> m_selfModelLayer;
    int m_totalH;
};
#endif /* defined(__IF__DetectMailPopUpView__) */
