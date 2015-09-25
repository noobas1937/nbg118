//
//  AllianceUnlockView.h
//  IF
//
//  Created by ganxiaohua on 15-1-15.
//
//

#ifndef __IF__AllianceUnlockView__
#define __IF__AllianceUnlockView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCMultiColTableView.h"
#include "HFHeadImgNode.h"

#define REFRESH_DATA_LIST "refreshDataList"

class AllianceShieldInfo : public cocos2d::CCObject{
public:
    AllianceShieldInfo():
    name(""),
    uuid(""),
    abbr(""),
    uid(""),
    picVer(0)
    {};
    std::string name;
    std::string uuid;
    std::string allianceId;
    int type;
    std::string icon;
    std::string abbr;
    int picVer;
    string uid;
};

class AllianceUnlockView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
,public cocos2d::extension::CCMultiColTableViewDataSource
,public cocos2d::extension::CCMultiColTableViewDelegate

{
public:
    static AllianceUnlockView* create();
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){};
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
    void onCloseBtnClick(CCObject *pSender, CCControlEvent event);
    void onTab1Click(CCObject *pSender, CCControlEvent event);
    void onTab2Click(CCObject *pSender, CCControlEvent event);
    void refreshView(CCObject* p);
    void loadDataBack();
    void getData();
    void getServerData(CCObject* param);
    void getShowData();
    
    CCSafeObject<CCControlButton> m_closeBtn;
    CCSafeObject<CCControlButton> m_tab1;
    CCSafeObject<CCControlButton> m_tab2;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_tip;
    CCSafeObject<CCNode> m_container;
    CCSafeObject<CCScale9Sprite> m_BG;
    CCSafeObject<CCMultiColTableView> m_tabView;
    
    CCSafeObject<CCArray> m_srcData;
    CCSafeObject<CCArray> m_data;
};

class AllianceUnlockCell :
public CCTableViewCell,
public CCBSelectorResolver,
public CCBMemberVariableAssigner
{
public:
    static AllianceUnlockCell* create(AllianceShieldInfo *info);
    void setCellData(AllianceShieldInfo *info);
    
private:
    AllianceUnlockCell(AllianceShieldInfo *info) : m_info(info){};
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onUnBlockClick(CCObject *pSender, CCControlEvent event);
    AllianceShieldInfo *m_info;

    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCNode> m_picContainer;
    CCSafeObject<CCControlButton> m_unBlockBtn;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
};

#endif /* defined(__IF__AllianceUnlockView__) */
