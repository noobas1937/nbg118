//
//  FavoriteList.h
//  IF
//
//  Created by 邹 程 on 14-2-26.
//
//

#ifndef __IF__FavoriteList__
#define __IF__FavoriteList__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "WorldController.h"
#include "PopupBaseView.h"
#include "CCTableViewTouchIFCell.h"
class FavoriteList;

enum GotoButtonMoveDir
{
    GotoButtonMoveNone,
    GotoButtonMoveLeft,
    GotoButtonMoveRight
};

class FavoriteListCell : public CCTableViewTouchIFCell
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
 
{
public:
    
    FavoriteListCell(const FavoriteTile& tile,FavoriteList* list):
    m_tile(tile)
    ,m_index(WorldController::getIndexByPoint(tile.point))
    ,container(list)
    ,m_gotoButtonClicked(true)
    ,m_moveDir(GotoButtonMoveNone)
    ,m_movedToLeftFlag(false)
    {}
    
    static FavoriteListCell* create(const FavoriteTile& tile,FavoriteList* list) {
        auto ret = new FavoriteListCell(tile,list);
        if (ret->init()) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    
    bool init();
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onEnter();
    virtual void onExit();
    
    
private:
    const FavoriteTile& m_tile;
    unsigned int m_index;
    
    FavoriteList * container;
        
    CCSafeObject<CCSprite> m_icon;
    CCSafeObject<CCLabelIF> m_position;
    CCSafeObject<CCLabelIF> m_deleteLabel;
    CCSafeObject<CCLabelIF> m_editLabel;
    CCSafeObject<Label> m_name;
    CCSafeObject<CCScale9Sprite> m_editBG;
    CCSafeObject<CCScale9Sprite> m_gotoTileBG1;
//    CCSafeObject<CCScale9Sprite> m_gotoTileBG2;
    CCSafeObject<CCScale9Sprite> m_deleteBG;
    CCSafeObject<CCNode> m_gotoButtonNode;
    
    bool m_gotoButtonClicked;
    
    void onDeleteFavorite();
    void onEditFavorite();
    void onGotoFavoriteTile();
    CCPoint m_touchPoint;
    GotoButtonMoveDir m_moveDir;
    bool m_movedToLeftFlag;
    
    void updateListCellPosition(CCObject* p);
    
};



class FavoriteList : public PopupBaseView
,public CCTableViewDataSource
,public CCBMemberVariableAssigner
,public CCBSelectorResolver

{
    
public:
    
    CREATE_FUNC(FavoriteList)
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    
    
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
    void onClickButtonTab(CCObject * pSender, Control::EventType pCCControlEvent);

    void sortFavoriteList();
    static bool cmp(const std::pair<double,FavoriteTile>& item1, const std::pair<double,FavoriteTile>& item2);
    
    bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    void updateList(CCObject* obj);
    
    void onDeleteCallback(CCObject* obj);
    
    void closeThis();
    
    CCSize m_nodeSize;
    CCTableView* m_table;
    
private:
    CCSafeObject<CCNode> m_subNode;
    CCSafeObject<CCScale9Sprite> m_subNodeBg;
    CCSafeObject<CCControlButton> m_button[4];
    CCSafeObject<CCLabelIF> m_noFavoTxt;
    void generateData();
    CCSafeObject<CCArray> m_data;
    FavoriteType m_currentTab;
    int m_FavoCount;
    std::vector<pair<double,FavoriteTile>> m_FavoVec;
};

#endif /* defined(__IF__FavoriteList__) */
