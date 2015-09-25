//
//  BuildingHospitalPopUpView.h
//  IF
//
//  Created by 李锐奇 on 14-3-6.
//
//

#ifndef __IF__BuildingHospitalPopUpView__
#define __IF__BuildingHospitalPopUpView__

#include "PopupBaseView.h"
#include "CommonInclude.h"
#include "CCMultiColTableView.h"
#include "TreatInfo.h"
#include "CCSliderBar.h"

#define TREAT_NUM_CHANGE "treatNumChange"
#define TREAT_ALL "treatAll"

enum CommandType{
    NORMAL_COMMAND,
    USEGOLD_COMMAND,
};

class BuildingHospitalPopUpView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate

{
public:
    static BuildingHospitalPopUpView* create(FunBuildInfo &info);
    
protected:    
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);

private:
    BuildingHospitalPopUpView(FunBuildInfo &info) : m_info(info){};
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);

    void onAllClick(CCObject *pSender, CCControlEvent event);
    void onInstantClick(CCObject *pSender, CCControlEvent event);
    void onHealthClick(CCObject *pSender, CCControlEvent event);
    void onSelectClick(CCObject *pSender, CCControlEvent event);
    void onTipBtnClick(CCObject *pSender, CCControlEvent event);

    
    void onClickWoodIcon(CCObject *pSender, CCControlEvent event);
    void onClickFoodIcon(CCObject *pSender, CCControlEvent event);
    void onClickIronIcon(CCObject *pSender, CCControlEvent event);
    void onClickStoneIcon(CCObject *pSender, CCControlEvent event);
    void confirmCallBack();
    
    void resetTreatNum(CCObject* p=NULL);
    std::string getPicPath();
    void refreshView(CCObject* p);
    void refreshTreatNum(CCObject* p);
    void generateArr();
    void setNumText();
    void showPorgress();
    void treat(int type);
    void sendCommand(int type);
    bool checkEmptyQuene();
    void instantCallBack();
    void healthCallBack(CCObject* p);
    
    CCSafeObject<CCControlButton> m_allBtn;
    CCSafeObject<CCLabelIF> m_goldText;
    CCSafeObject<CCLabelIF> m_healInstantBtnText;
    CCSafeObject<CCLabelIF> m_selectLabel;
    CCSafeObject<CCControlButton> m_instantBtn;
    CCSafeObject<CCControlButton> m_healthBtn;
    CCSafeObject<CCControlButton> m_selectBtn;
    
    CCSafeObject<CCLabelIF> m_troopText;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_bgNode;
    
//    CCSafeObject<CCNode> m_upNode;
    CCSafeObject<CCLabelIF> m_selText;
    CCSafeObject<CCLabelIF> m_totalText;
    CCSafeObject<CCLabelIF> m_healthLabel;
    CCSafeObject<CCLabelIF> m_timeText;
    
    CCSafeObject<CCControlButton> m_woodIcon;
    CCSafeObject<CCControlButton> m_foodIcon;
    CCSafeObject<CCControlButton> m_ironIcon;
    CCSafeObject<CCControlButton> m_stoneIcon;
    
    CCSafeObject<CCLabelIF> m_woodText;
    CCSafeObject<CCLabelIF> m_stoneText;
    CCSafeObject<CCLabelIF> m_ironText;
    CCSafeObject<CCLabelIF> m_foodText;
    
    CCSafeObject<CCLabelIF> m_woodLabel;
    CCSafeObject<CCLabelIF> m_stoneLabel;
    CCSafeObject<CCLabelIF> m_ironLabel;
    CCSafeObject<CCLabelIF> m_foodLabel;
    
    CCSafeObject<CCNode> m_woodNode;
    CCSafeObject<CCNode> m_stoneNode;
    CCSafeObject<CCNode> m_ironNode;
    CCSafeObject<CCNode> m_foodNode;
    CCSafeObject<CCLabelIF> m_noJnjureTxt;
    
    CCSafeObject<CCSprite> m_yesSprite;
    
//    CCSafeObject<CCNode> m_baseContainer;
//    CCSafeObject<CCNode> m_progressContainer;
    CCSafeObject<CCNode> m_downNode;

    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCScale9Sprite> m_renderBG;
//    CCSafeObject<CCScale9Sprite> m_progressBar;
//    CCSafeObject<CCScale9Sprite> m_progressBarBG;
    CCSafeObject<CCNode> m_needNode;
    CCSafeObject<CCLabelIF> m_tipTxt;
    
    CCSafeObject<CCMultiColTableView> m_tabView;

    FunBuildInfo m_info;
    CCSafeObject<CCArray> m_data;
    int m_orgTitleY;
    int m_orgListY;
    int m_orgHeight;
    int m_totalTimes;
};

//

class BuildingHospitalCell:
public CCBMemberVariableAssigner
, public CCBSelectorResolver
, public CCTableViewCell
,public CCEditBoxDelegate
{
public:
    static BuildingHospitalCell *create(TreatInfo *info, int buildId);
    void setData(TreatInfo *info);
    void treatAll(float dt);

private:
    BuildingHospitalCell(TreatInfo *info, int buildId) : m_info(info), m_buildId(buildId){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void refreshView();
    void sliderCallBack(CCObject*sender,CCControlEvent even);
    void onAddClick(CCObject *pSender, CCControlEvent event);
    void onSubClick(CCObject *pSender, CCControlEvent event);
    void onInfoClick(CCObject *pSender, CCControlEvent event);
    void editBoxReturn(CCEditBox *editBox);
    void onFireClick(CCObject *pSender, CCControlEvent event);

    void refreshNum();
    
    CCSafeObject<TreatInfo> m_info;
    CCSafeObject<CCNode> m_armyIconContainer;
    CCSafeObject<CCNode> m_levelNode;
    CCSafeObject<CCNode> m_sliderContainer;
//    CCSafeObject<CCControlButton> m_infoBtn;
    CCSafeObject<CCLabelIF> m_numText;
    CCSafeObject<CCLabelIF> m_numMaxText;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCSliderBar> m_trainSlider;
    CCSafeObject<CCNode> m_bg;
    CCSafeObject<CCNode> m_editBoxNode;
    CCEditBox* m_editBox;
    
    int m_buildId;
};

class HealthingView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static HealthingView* create(int buildId);
    HealthingView():m_buildId(0),m_curTime(0),m_sumTime(0),m_goldNum(0),m_qid(0){};
    virtual ~HealthingView(){};
    void updateInfo();
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(int buildId);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onInstantClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onEnterFrame(float dt);
    
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCLabelIF> m_msgLabel;
    
    CCSafeObject<CCControlButton> m_instantBtn;
    CCSafeObject<CCScale9Sprite> m_timeBar;
    
    CCSafeObject<CCLabelIF> m_btnMsgLabel;
    CCSafeObject<CCLabelIF> m_inBtnGoldNum;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCNode> m_mainNode;
    
    int m_buildId;
    int m_curTime;
    int m_sumTime;
    int m_goldNum;
    int m_qid;
};

#endif /* defined(__IF__BuildingHospitalPopUpView__) */
