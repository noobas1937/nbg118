//
//  UseCKFSkillView.h
//  IF
//
//  Created by ganxiaohua on 15/8/10.
//
//

#ifndef __IF__UseCKFSkillView__
#define __IF__UseCKFSkillView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCMultiColTableView.h"
#include "CCTableViewTouchIFCell.h"
class UseCKFSkillView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static UseCKFSkillView *create();
    
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
    virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onCloseBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void cellClick(CCObject* param);
    void initQueueData();
    void refreshQueue(CCObject* param);
    void refreshData(CCObject* param);
    
    std::map<int, std::vector<int> > m_statusGroup;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCLabelIF> m_skillTitle;
    CCSafeObject<CCLabelIF> m_numTxt;
    
    CCSafeObject<CCSprite> m_sprR;
    CCSafeObject<CCSprite> m_sprL;
    
    CCSafeObject<CCNode> m_speedNode;
    CCSafeObject<CCNode> m_speedArea;
    CCSafeObject<CCNode> m_list;
    CCSafeObject<CCLabelIF> m_speedTxt;
    
    CCSafeObject<CCMultiColTableView> m_tabView;
    CCSafeObject<CCScrollView> m_scrollView;
    std::string m_skillId;
};

class UseCKFSkillCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static UseCKFSkillCell *create(std::string pic);
    void setData(std::string pic);
    CCSafeObject<CCControlButton> m_btnReward;
private:
    UseCKFSkillCell(std::string skillId):m_skillId(skillId){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void sureToUsePoint();
    void successCallBack(CCObject* param);
    
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCNode> m_clickNode;
    bool m_isDraging;
    int m_type;
    std::string m_skillId;
    bool m_touchMove;
    float touchX;
};
#endif /* defined(__IF__UseCKFSkillView__) */
