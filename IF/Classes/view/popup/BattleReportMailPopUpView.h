//
//  BattleReportMailPopUpView.h
//  IF
//
//  Created by 李锐奇 on 14-3-26.
//
//

#ifndef __IF__BattleReportMailPopUpView__
#define __IF__BattleReportMailPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "HFHeadImgNode.h"

enum BattleResult{
    WIN,
    DRAW,
    LOOSE
};
struct BattleReportWarEffect{
//    int effectid;
    std::string effectName;
    float effectValue;
    int effectType;
    int effectOrder;
    int effectMark;
};

class BattleReportMailPopUpView : public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
//, public CCTouchDelegate

{
public:
    static BattleReportMailPopUpView *create(MailInfo *info);
    
private:
    BattleReportMailPopUpView(MailInfo *info) : m_info(info){};
    ~BattleReportMailPopUpView();
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    void onReportClick(CCObject *pSender, CCControlEvent event);
    void onDeleteClick(CCObject *pSender, CCControlEvent event);
    void onBattleDetailClick(CCObject *pSender, CCControlEvent event);
    void onAddSaveClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onReturnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onWriteClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onShareClick(CCObject *pSender, CCControlEvent event);
    void onOkDeleteMail();
    void getContent();
    int getTotalDead(CCArray *arr);
    void shareSuccess(CCObject* param);
    CCArray* sortResource();
    CCSafeObject<MailInfo> m_info;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCControlButton> m_reportBtn;
    CCSafeObject<CCControlButton> m_deleteBtn;
    CCSafeObject<CCLabelIF> m_mailTitle;
    CCSafeObject<CCLabelIF> m_attackText;
    CCSafeObject<CCScale9Sprite> m_hintBG;
//    CCSafeObject<CCSprite> m_hintLine;
    CCSafeObject<CCLabelIF> m_hintText;
//    CCSafeObject<CCLabelIF> m_upTitle;
//    CCSafeObject<CCNode> m_winNode;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_upNode;
    CCSafeObject<CCNode> m_firstNode;
    CCSafeObject<CCNode> m_failNode;
    
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCScale9Sprite> m_buildBG;
//    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCScale9Sprite> m_defBg;
    CCSafeObject<CCScale9Sprite> m_attBg;
    CCSafeObject<CCControlButton> m_detailBtn;
    CCSafeObject<CCControlButton> m_battleBtn;
    //CCSafeObject<CCLabelIF> m_deleteBtnTitle;
//    CCSafeObject<CCSprite> m_savespr;
//    CCSafeObject<CCSprite> m_unsavespr;
    CCSafeObject<CCControlButton> m_addSaveBtn;
    CCSafeObject<CCControlButton> m_writeBtn;
    CCSafeObject<CCControlButton> m_returnBtn;
    CCSafeObject<CCSprite> m_returnSpr;
//    CCSafeObject<CCControlButton> m_unSaveBtn;
    CCSafeObject<CCControlButton> m_shareBtn;
    CCSafeObject<CCNode> m_ListNode;
    CCSafeObject<CCNode> m_battlePicNode;
    void refreshView(CCObject* p = NULL);
    void getData();
    int getReportResult();
    int addRewards(int offY=0);
    int addUserInfo(int startX, int startY, CCArray *arr, CCArray *arr1,bool boss=false);
    int handleMonsterBattleInfo(int offY=0);
    int addMonsterAndUserInfo(int startX, int startY, CCArray *arr, CCArray *arr1,bool boss=false);
    int handleLianShengInfo(int offY=0);
    
    CCSafeObject<CCScrollView> m_scrollView;
    int m_totalH;
    int m_Listheight;
    bool isaddHeight;
    std::string attUid;
    std::string defUid;
    bool isatkHelp;
    bool isdefHelp;
    std::string ownerUid;//为了区分是查邮件的人，还是自己就是邮件的拥有者
    
    void autoResizeHintNode();
    
    static bool sortByUseOder(BattleReportWarEffect eff1, BattleReportWarEffect eff2);
private:
    void generateWarEff();
    void initBattleReportWarEffect(std::map<int,int> &effMap,bool attack);
    int addUserEffect(int startX, int startY,std::vector<BattleReportWarEffect> &effArr);
    std::string getEffectDescByID(int effectID);
    std::vector<BattleReportWarEffect> mAtkWarEff;
    std::vector<BattleReportWarEffect> mDefWarEff;
    
};

//-----

class GeneralInfoCell : public CCNode
, public CCBMemberVariableAssigner

{
public:
    static GeneralInfoCell *create(GeneralInfo *info, CCDictionary *dict,bool boss=false);
    void showGeneralInfo();
private:
    GeneralInfoCell(GeneralInfo *info, CCDictionary *dict,bool boss) : m_info(info), m_dict(dict),m_isBoss(boss){};
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_lvText;
    CCSafeObject<CCLabelIF> m_expText;
    CCSafeObject<CCNode> m_headContainer;
    GeneralInfo*         m_info;
    CCSafeObject<CCDictionary> m_dict;
    CCSafeObject<CCNode> m_forceNode;
    
    CCSafeObject<CCLabelIF> m_killText;
    CCSafeObject<CCLabelIF> m_killNumText;
    CCSafeObject<CCLabelIF> m_loseText;
    CCSafeObject<CCLabelIF> m_loseNumText;
    CCSafeObject<CCLabelIF> m_surplusText;
    CCSafeObject<CCLabelIF> m_surplusNumText;
    bool m_isBoss;
};

//-----

class PlayerInfoCell : public CCNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner

{
public:
    static PlayerInfoCell *create(CCDictionary *obj,CCArray* generals,MailInfo *mail);
    void cellTouch(CCTouch *pTouch);
private:
    PlayerInfoCell(CCDictionary *info,CCArray* generals,MailInfo *mail) : m_info(info),m_generals(generals),m_mail(mail){};
    virtual bool init();
    CCSafeObject<CCSprite> m_pic;
    CCSafeObject<CCLabelIF> m_pos;
    CCSafeObject<CCNode> m_posBG;
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void onBossPosClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onPosBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void playHeadClick(CCObject *pSender, CCControlEvent event);
    void addShowTeamInfo();
    
    CCSafeObject<Label> m_nameText;
    CCSafeObject<CCLabelIF> m_posText;
    CCSafeObject<CCLabelIF> m_powerText;
//    CCSafeObject<CCLabelIF> m_powerText1;
    CCSafeObject<CCLabelIF> m_totalText;
    CCSafeObject<CCLabelIF> m_totalNumText;
    CCSafeObject<CCLabelIF> m_killText;
    CCSafeObject<CCLabelIF> m_killNumText;
    CCSafeObject<CCLabelIF> m_loseText;
    CCSafeObject<CCLabelIF> m_loseNumText;
    CCSafeObject<CCLabelIF> m_woundedText;
    CCSafeObject<CCLabelIF> m_woundedNumText;
    CCSafeObject<CCLabelIF> m_surplusText;
    CCSafeObject<CCLabelIF> m_surplusNumText;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCDictionary> m_info;
    CCSafeObject<CCArray> m_generals;
    CCSafeObject<CCScale9Sprite> m_underlineSpr;
    CCSafeObject<CCNode> m_bossNode;
    CCSafeObject<CCNode> m_playerNode;
    CCSafeObject<CCLabelIF> m_bossPos;
    CCSafeObject<CCLabelIF> m_bossDesc;
    CCSafeObject<CCNode> m_bossPic;
    CCSafeObject<CCScale9Sprite> m_expBar;
    CCSafeObject<CCLabelIF> m_bossNumTxt;
    
    CCSafeObject<CCLabelIF> m_trapLossTxt;
    CCSafeObject<CCLabelIF> m_trapLossNum;
    CCSafeObject<CCScale9Sprite> m_boxBg;
    CCSafeObject<CCSprite> m_iconBtn;
    
    CCSafeObject<MailInfo> m_mail;
    
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    CCSafeObject<CCRenderTexture> m_selfModelLayer;
    int m_pointID;
    
};
class PlayerEffectCell : public CCNode
, public CCBMemberVariableAssigner
{
public:
    static PlayerEffectCell *create(std::vector<BattleReportWarEffect> &effArr);
private:
    PlayerEffectCell(){};
    virtual bool init(std::vector<BattleReportWarEffect> &effArr);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    float createContent(std::string desc,std::string percent,float nowH);
    CCSafeObject<CCScale9Sprite> m_sprBG;
//    CCSafeObject<CCScale9Sprite> m_sprTitleBG;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCNode> m_nodeList;
    
};

//---
class ReportNormalRewardCell : public CCNode{
public:
    static ReportNormalRewardCell *create(int type, int num,int itemId,int flag=1);
private:
    virtual bool init(int type, int num,int itemId,int flag);
};

//monster
class MonsterInfoCell : public CCNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner

{
public:
    static MonsterInfoCell *create(CCDictionary *obj,CCArray* generals,MailInfo *mail);
    void cellTouch(CCTouch *pTouch);
private:
    MonsterInfoCell(CCDictionary *info,CCArray* generals,MailInfo *mail) : m_info(info),m_generals(generals),m_mail(mail){};
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void onPosBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCLabelIFTTF> m_title;
    CCSafeObject<CCLabelIF> m_txt1;
    CCSafeObject<CCLabelIF> m_txt2;
    CCSafeObject<CCLabelIF> m_txt3;
    CCSafeObject<CCLabelIF> m_txtv1;
    CCSafeObject<CCLabelIF> m_txtv2;
    CCSafeObject<CCLabelIF> m_txtv3;
    CCSafeObject<CCLabelIF> m_txtTip;
    CCSafeObject<CCLabelIF> m_posText;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_posBG;
    
    CCSafeObject<MailInfo> m_mail;
    CCSafeObject<CCDictionary> m_info;
    CCSafeObject<CCArray> m_generals;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    CCSafeObject<CCRenderTexture> m_selfModelLayer;
    int m_pointID;
    
};
class BattleReportRewardCell : public CCNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner

{
public:
    static BattleReportRewardCell *create(int type, int value, int num, int total);
    void cellTouch(CCTouch *pTouch);
private:
    BattleReportRewardCell(int type, int value, int num, int total):m_type(type),m_value(value),m_num(num),m_total(total){};
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void updateTime(float  _time);
    CCSafeObject<CCNode> m_rewardNode1;
    CCSafeObject<CCLabelIF> m_numText1;
    CCSafeObject<CCLabelIF> m_nameText1;
    CCSafeObject<CCNode> m_picNode1;
    CCSafeObject<CCNode> m_proNode1;
    CCSafeObject<CCProgressTimer> m_proTimer;
    int m_type;
    int m_value;
    int m_num;
    int m_total;
    int fromNum;
    int toNum;
    int dt;
    double m_percentage;
};
#endif /* defined(__IF__BattleReportMailPopUpView__) */
