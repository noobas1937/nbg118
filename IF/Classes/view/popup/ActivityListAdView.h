//
//  ActivityListAdView.h
//  IF
//
//  Created by chenfubi on 15/1/9.
//
//

#ifndef __IF__ActivityListAdView__
#define __IF__ActivityListAdView__

#include "CommonInclude.h"
#include "DefaultTableViewDelegate.h"
#include "ActivityEventObj.h"
#include "CCClipNode.h"
#include "CCTableViewTouchIFCell.h"
class ActivityListAdView : public Layer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
//,public CCTouchDelegate
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
	static ActivityListAdView* create();
    ActivityListAdView(){};
    virtual ~ActivityListAdView(){};
    
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view);
    
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onGameTick(float time);
    void initPageNode(int offSetX);
    
private:
    CCSafeObject<CCNode> m_pageNode;
    CCSafeObject<CCNode> m_listNode;
    CCTableView* m_tabView;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCLabelIF> m_actTxt;
    
    CCSafeObject<CCNode> m_layerNode;
    
    int m_x;

};

class ActivityListAdNewCell:
public CCTableViewTouchIFCell
{
public:
    static ActivityListAdNewCell* create(ActivityEventObj* obj);
    ActivityListAdNewCell(ActivityEventObj* obj): m_obj(obj) {};
    void setData(ActivityEventObj*);
    virtual bool init();
protected:
private:
    ActivityEventObj* m_obj;
};

class ActivityAdLuaCell:
public CCLayer
  
{
public:
    static ActivityAdLuaCell* create(ActivityEventObj* obj);
    ActivityAdLuaCell(ActivityEventObj* obj): m_obj(obj){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
    ActivityEventObj* m_obj;
};

//class cell
class ActivityListAdCell :
public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static ActivityListAdCell* create(ActivityEventObj* obj);
    void setData(ActivityEventObj* obj);
    ActivityListAdCell(){};
    virtual ~ActivityListAdCell(){};
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(ActivityEventObj* obj);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onGameTick(float time);
    
private:
    CCSafeObject<CCSprite> m_activityAdImg;
    CCSafeObject<CCLabelIF> m_timeLabel1;
    CCSafeObject<CCLabelIF> m_nameTTF;
    CCSafeObject<CCLabelIF> m_infoTTF;
    CCSafeObject<CCNode> m_bgNode;
    
    CCSafeObject<CCClipNode> m_clipNode;
    
    CCSafeObject<ActivityEventObj> m_actObj;
    
};


#endif /* defined(__IF__ActivityListAdView__) */
