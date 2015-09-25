//
//  InnerSettingView.h
//  IF
//
//  Created by zhangjun on 15-4-13.
//
//

#ifndef __IF__InnerSettingView__
#define __IF__InnerSettingView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCTableViewTouchIFCell.h"
#define IS_AUTO_DOWNLOAD_ON "isAutoDownloadOn"
#define DALTONSTYLE "Daltonstyle"

enum InnerSettingType
{
    ISetting_Sound,
    ISetting_TaskTips,
    ISetting_Equip,
    ISetting_Download,
    ISetting_DaltonStyle,
    ISetting_Max
};

class InnerSettingInfo : public CCObject{
public:
    CREATE_FUNC(InnerSettingInfo);
    void parse(CCDictionary *dict);
    InnerSettingInfo():status1(0),status2(0),type(0),sound(0){};
    ~InnerSettingInfo(){};
    
    int status1;
    int status2;
    int type;
    int sound;
    
private:
    virtual bool init(){return true;};
};

class InnerSettingView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static InnerSettingView* create();
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual ~InnerSettingView();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);

    void setDataList();

    CCSafeObject<CCScale9Sprite> m_popupBG;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCTableView> m_tabView;
    
    CCSafeObject<CCArray>m_data;
};

class InnerCell :
public CCTableViewTouchIFCell,
public CCBSelectorResolver,
public CCBMemberVariableAssigner
//public CCTargetedTouchDelegate
{
public:
    static InnerCell* create(int index);
    void setData(int index);
    void showActivityDetail();
private:
    InnerCell(int index){};
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int index);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void refreshView();
    int m_index;
    CCPoint m_touchPoint;
    CCSafeObject<CCLabelIF> m_TitleText;
    CCSafeObject<InnerSettingInfo>m_info;
    
    CCSafeObject<CCNode> m_nodeType1;
    CCSafeObject<CCSprite> m_soundOnSpr;
    CCSafeObject<CCSprite> m_soundOffSpr;
    CCSafeObject<CCSprite> m_11touchBtnBg;
    CCSafeObject<CCSprite> m_11touchBtn;
    CCSafeObject<CCSprite> m_11touchBtn1;
    CCSafeObject<CCSprite> m_musicOnSpr;
    CCSafeObject<CCSprite> m_musicOffSpr;
    CCSafeObject<CCSprite> m_12touchBtnBg;
    CCSafeObject<CCSprite> m_12touchBtn;
    CCSafeObject<CCSprite> m_12touchBtn1;
    
    CCSafeObject<CCNode> m_nodeType2;
    CCSafeObject<CCLabelIF> m_2desText;
    CCSafeObject<CCSprite> m_2touchBtnBg;
    CCSafeObject<CCSprite> m_2touchBtn;
    CCSafeObject<CCSprite> m_2touchBtn1;
    
    std::string touchIndex;
};


#endif /* defined(__IF__PushSettingView__) */
