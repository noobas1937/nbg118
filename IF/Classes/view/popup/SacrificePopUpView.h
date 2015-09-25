//
//  SacrificePopUpView.h
//  IF
//
//  Created by lifangkai on 14-11-19.
//
//

#ifndef __IF__SacrificePopUpView__
#define __IF__SacrificePopUpView__
#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"
#include "BackpackView.h"
#include "ToolInfo.h"
#include "StoreBagView.h"
#include "StoreMallView.h"
#include "CCLabelIFBMFont.h"
#include "CCTableViewTouchIFCell.h"
class SacrificeCell;
class SacrificeOneCell;

class SacrificePopUpView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
{
public:
    static SacrificePopUpView* create();
    SacrificePopUpView(){m_hint=0;m_num=0;m_resource=-1;};
    virtual ~SacrificePopUpView(){};
    void refresh(CCObject* p);
    CCNode* getGuideNode();
    void showNextTip();
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
    void getData();
    void refreashData(CCObject* obj);
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onTipBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void addCommonAnim();
    void addRewardPoolAnim();
    void addDropAnim();
    void addShakeAnim();
    void addGetNumAnim();
    void GetNumAnimEnd();
    void EndHintAnim();
    void EndHintAnim1();
    void addScienceOutAnim();
    void addLvUptAnim();
    CCSafeObject<CCNode>  m_totalNode;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_battlePicNode;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCLabelIF> m_desText;
    CCSafeObject<CCControlButton> m_tipBtn;
    CCSafeObject<CCNode> m_leftNode1;
    CCSafeObject<CCNode> m_leftNode2;
    CCSafeObject<CCNode> m_rightNode1;
    CCSafeObject<CCNode> m_rightNode2;
    CCSafeObject<CCNode> m_middleNode;
    CCSafeObject<CCNode> m_middleRewardNode;
    CCSafeObject<CCNode> m_hintNode;
    CCSafeObject<CCNode> m_dropNode;
    CCSafeObject<CCNode> m_getNumNode;
    CCSafeObject<CCLabelBMFont> m_getNumText;
    CCSafeObject<CCLabelIFBMFont> m_HintSpr;
    CCSafeObject<CCArray>m_data;
    
    CCSafeObject<CCTableView> m_tabView;
    int m_hint;
    int m_num;
    int m_resource;
    bool m_isInit;
};
//cell Class
class SacrificeCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewCell
{
public:
    static SacrificeCell* create(int type);
    SacrificeCell(int type):m_type(type){};
    void setData(int type);
    bool init();
    CCSafeObject<CCNode> m_cellNode;
private:

    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    

    int m_type;
};
//.one cell
class SacrificeOneCell :
public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
, public CCTableViewTouchIFCell
{
public:
    static SacrificeOneCell* create(int type);
    SacrificeOneCell(int type):m_type(type),m_waitInterface(NULL){};
    ~SacrificeOneCell(){};
    void setData(int type);
private:
    virtual void onExit();
    virtual void onEnter();
    
    bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void buyPray();
    void playScaleAction();
    
    CCSafeObject<CCScale9Sprite> m_graySpr;
    CCSafeObject<CCScale9Sprite> m_hintBG;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_openNode;
    CCSafeObject<CCNode> m_totalNode;
    
    CCSafeObject<CCLabelIF> m_closeDesText;
    CCSafeObject<CCLabelIF> m_clickText;
    CCSafeObject<CCLabelIF> m_costNumText;
    CCSafeObject<CCLabelIF> m_getNumText;
    CCSafeObject<CCLayerColor> m_layerColor;

    CCSafeObject<CCSprite> m_goldSpr;

    
    CCPoint m_touchPoint;
    int m_type;
    WaitInterface* m_waitInterface;

};
#endif /* defined(__IF__SacrificePopUpView__) */
