//
//  WorldBannerRankView.h
//  IF
//
//  Created by wangchaoyi on 15/3/24.
//
//

#ifndef __IF__WorldBannerRankView__
#define __IF__WorldBannerRankView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"
#include "BackpackView.h"
#include "ToolInfo.h"
#include "CCTableViewTouchIFCell.h"
class WorldBannerRankCell;

class WorldBannerRankView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static WorldBannerRankView* create(int type=0);
    WorldBannerRankView(int type):m_type(type){};
    virtual ~WorldBannerRankView();
    void updateInfo();
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    //void refreashData(CCObject* obj);
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    
    CCSafeObject<CCLabelIF> m_textLabel1;
    CCSafeObject<CCLabelIF> m_textLabel2;
    CCSafeObject<CCLabelIF> m_textLabel3;
    CCSafeObject<CCNode> m_totalNode;
    
    CCSafeObject<CCTableView> m_tabView;
    
    CCSafeObject<CCArray> m_data;
    int m_type;
    
    std::map<string, int> m_bannerNumMap;
    std::vector<pair<string,int>> m_bannerNumVec;
    void sortBanner();
    static bool cmp(const std::pair<string, int>& item1, const std::pair<string, int>& item2);
};

class WorldBannerRankCell :
public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static WorldBannerRankCell* create(std::string banner, int count, int index);
    WorldBannerRankCell(std::string banner, int count, int index):m_bannerStr(banner),m_bannerCount(count),m_index(index){};
    void setData(std::string banner, int count, int index);
private:
    bool init(std::string banner, int count, int index);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    //void refreashData(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    
    CCSafeObject<CCLabelIF> m_numText;
    CCSafeObject<CCLabelIFTTF> m_text1;
    CCSafeObject<CCLabelIF> m_text2;
    
    CCSafeObject<CCScale9Sprite> m_sprBG1;
    CCSafeObject<CCScale9Sprite> m_sprBG2;
    CCSafeObject<CCScale9Sprite> m_sprBG3;
    CCSafeObject<CCScale9Sprite> m_sprBG4;
    CCSafeObject<CCSprite> m_numspr1;
    CCSafeObject<CCSprite> m_numspr2;
    CCSafeObject<CCSprite> m_numspr3;
    CCSafeObject<CCSprite> m_bannerSpr;
    
    CCSafeObject<CCNode> m_bannerNode;
    CCSafeObject<CCNode> m_hintBGNode;
    std::string m_bannerStr;
    int m_bannerCount;
    CCPoint m_startPoint;
    int m_index;
    
};

#endif /* defined(__IF__WorldBannerRankView__) */
