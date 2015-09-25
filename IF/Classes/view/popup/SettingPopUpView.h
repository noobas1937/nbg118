//
//  SettingPopUpView.h
//  IF
//
//  Created by 李锐奇 on 14-1-20.
//
//

#ifndef __IF__SettingPopUpView__
#define __IF__SettingPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCMultiColTableView.h"
#include "CCTableViewTouchIFCell.h"
enum SettingType
{
    //notGM
    Setting_BINDING,
    Setting_lang,
    Setting_Help,
    Setting_Push,
    Setting_innerSetting,//新添加
    Setting_bgMusic,
    Setting_EffectSound,
    Setting_INVITE,
    Setting_SEARCH,
    Setting_MOD,
    Setting_SOCIAL,
    Setting_Blocked,
    Setting_CDKEY,
    Setting_Forum,
    Setting_Clause,//新添加,服务条款
    Setting_Lobi,//日语 网页连接
    Setting_VK,//vk 网页连接
    Setting_WeiBo,//WeiBo 网页连接
    Setting_XiaoMi,
    Setting_KO,//韩语 网页
    //GM
    Setting_FeedBack,
    Setting_Account,

    Setting_Temp,
    Setting_HTP,
//    Setting_Update,
//    Setting_Set,
    Setting_GM,
    Setting_Elex,
    Setting_Max
};
class SettingPopUpView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCMultiColTableViewDataSource
, public CCMultiColTableViewDelegate
{
public:
    static SettingPopUpView* create();
    CCNode* getGuideNode(string _key);
private:
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    int m_touchCount;
    bool m_showElex;
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual ~SettingPopUpView();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    void onTimer(float dt);
    void generateShowSetting();
    CCSafeObject<CCLabelIF> m_tips;
    CCSafeObject<CCLabelIF> m_timeTitleText;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCScale9Sprite> m_popupBG;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCNode> m_listContainer;
    
    CCSafeObject<CCMultiColTableView> m_tabView;
    std::vector<int> m_showSettings;
    int time;
};

class SettingCell:
public CCTableViewTouchIFCell,
public CCBSelectorResolver,
public CCBMemberVariableAssigner
{
public:
    static SettingCell* create(unsigned int index);
    void setData(unsigned int index);
    void touchEvent();
    unsigned int m_index;
    CCSafeObject<CCSprite> m_iconBG;
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(unsigned int index);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void updateState();
    
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_itemName;
    CCSafeObject<CCNode> m_HintNode;
    CCSafeObject<CCLabelIF> m_HintNum;
    CCPoint m_touchPoint;
};

#endif /* defined(__IF__SettingPopUpView__) */
