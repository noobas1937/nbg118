//
//  ResourceTileInfoPopUpView.h
//  IF
//
//  Created by 李锐奇 on 14-8-21.
//
//

#ifndef __IF__ResourceTileInfoPopUpView__
#define __IF__ResourceTileInfoPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "WorldController.h"
#include "DefaultTableViewDelegate.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"

class ResourceTileInfoPopUpView : public PopupBaseView
//, public CCTouchDelegate
, public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static ResourceTileInfoPopUpView *create(WorldCityInfo &info);
    virtual void update(float time);
protected:
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
private:
    ResourceTileInfoPopUpView(WorldCityInfo &info) : m_info(info){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    void onToolUseBtnClick(cocos2d::CCObject * pSender, CCControlEvent pCCControlEvent);
    void generateData();
    void refreshView();
    void troopDataBack(CCObject *obj);
    void getTroopData();
    
    WorldCityInfo &m_info;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCLabelIF> m_nameAndLevelText;
//    CCSafeObject<CCLabelIF> m_coordText;
    CCSafeObject<CCLabelIF> m_capText;
    CCSafeObject<CCLabelIF> m_speedText;
    CCSafeObject<CCLabelIF> m_speedAddText;
    CCSafeObject<CCLabelIF> m_speedTimeText;
    CCSafeObject<CCLabelIF> m_alreadyGetText;
    CCSafeObject<CCLabelIF> m_introText;
    CCSafeObject<CCLabelIF> m_troopTitleText;
    CCSafeObject<CCLabelIF> m_capLabel;

    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCSprite> m_line;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCMultiColTableView> m_tabView;
    CCSafeObject<CCControlButton> m_toolUseBtn;
    CCSafeObject<CCLabelIF> m_smalIntoTxt;

    std::string marchId;
};

class ResourceTroopInfo : public CCTableViewCell
, public CCBMemberVariableAssigner

{
public:
    static ResourceTroopInfo *create(CCDictionary *dict);
    void setData(CCDictionary *dict);
private:
    virtual bool init(CCDictionary *dict);
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);

    void refreshView();
    CCSafeObject<CCDictionary> m_dict;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_levelNode;

};

#endif /* defined(__IF__ResourceTileInfoPopUpView__) */
