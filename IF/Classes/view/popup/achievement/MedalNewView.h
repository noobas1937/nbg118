//
//  MedalNewView.h
//  IF
//
//  Created by chenfubi on 15/4/3.
//
//

#ifndef __IF__MedalNewView__
#define __IF__MedalNewView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "MedalInfo.h"

enum medalType
{
    medalType_My,
    medalType_Other,
};

class MedalNewView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
	static MedalNewView* create();
	MedalNewView(){};
	virtual ~MedalNewView(){};
private:
	virtual bool init();
	virtual void onEnter();
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void getData(CCObject* pObj);
    
private:
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_viewNode;

};


//CLASS MedalNewNode
class MedalNewNode : public CCNode
, public CCTableViewDataSource
, public DefaultTableViewDelegate
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
	static MedalNewNode* create(int type = medalType_My);
	MedalNewNode(){};
	virtual ~MedalNewNode(){};
    CCSafeObject<CCNode> m_listNode;

protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void reloadData(CCObject* pObj);
    
private:
	virtual bool init(int type);
	virtual void onEnter();
    virtual void onExit();
    
private:
    CCSafeObject<CCLabelIF> m_rdMsgLabel;
    
    CCSafeObject<CCArray> m_data;
    int m_type;
};

//CLASS MedalNewNodeCell
class MedalNewNodeCell : public CCTableViewCell
{
public:
	static MedalNewNodeCell* create(CCArray* a, int type);
	MedalNewNodeCell(){};
	virtual ~MedalNewNodeCell(){};
    void setData(CCArray* a, int type);

private:
	virtual bool init(CCArray* a, int type);
	virtual void onEnter();
    virtual void onExit();
    
};

//CLASS MedalCell
class MedalCell : public Layer
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
{
public:
    static MedalCell* create(string medalId, int type);
    MedalCell():m_medalInfo(NULL){ _ignoreAnchorPointForPosition = false; setAnchorPoint(Vec2(0,0));};
	virtual ~MedalCell(){};
    void setData(string medalId, int type);
    
private:
	virtual bool init(string medalId, int type);
	virtual void onEnter();
    virtual void onExit();
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void touchCallBack(CCObject* pObj);
    void addParticle();
    CCNode *getParentTouchNode();

private:
    CCSafeObject<CCSprite> m_medalIcon;
    CCSafeObject<CCLabelIF> m_nameTTF;
    CCSafeObject<CCLabelIF> m_speStarTTF;
    CCSafeObject<CCNode> m_speStarNode;
    CCSafeObject<CCNode> m_starNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCSprite> m_rectSp;
    
    MedalInfo* m_medalInfo;
    int m_type;
    
};

#endif /* defined(__IF__MedalNewView__) */
