//
//  AllianceCheckView.h
//  IF
//
//  Created by liweiyu on 14-8-26.
//
//

#ifndef __IF__AllianceCheckView__
#define __IF__AllianceCheckView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
//#include "CCTableView.h"

enum ButtonState {
    ButtonSlogan = 0
    ,ButtonOpen
    ,ButtonName
    ,ButtonBriName
    ,ButtonLang
    ,ButtonJJ
    ,ButtonCheck
    ,ButtonSearch
    ,ButtonPaihang
};

class AllianceCheckView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public CCTableViewDelegate
{
public:
    static AllianceCheckView *create();
    AllianceCheckView():m_cellCnt(15),currCelect(-1){};
    ~AllianceCheckView(){};
protected:
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    //tableView
    void flagView();
    
    virtual void tableCellTouched(  cocos2d::extension::TableView* table, CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(  cocos2d::extension::TableView *table, ssize_t idx);
	virtual cocos2d::CCSize cellSizeForTable(  cocos2d::extension::TableView *table);
    virtual CCTableViewCell* tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(  cocos2d::extension::TableView *table);
    
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};

    void setButtonState(CCNode* node,ButtonState state);
    void refresh();
    void onChangeClick(CCObject *pSender, CCControlEvent event);
    void onCancleClick(CCObject *pSender, CCControlEvent event);
    void saveSuccess(CCObject* p);
      cocos2d::extension::TableView* m_tabView;
    unsigned int m_cellCnt;
    std::string m_currentPic;
    int currCelect;
    bool touchAllow;
    
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCLabelIF> m_nameTxt;
    
    CCSafeObject<CCNode> m_node1;
    CCSafeObject<CCNode> m_btnNode1;
    CCSafeObject<CCLabelIF> m_nameText1;
    
    CCSafeObject<CCNode> m_node2;
    CCSafeObject<CCNode> m_btnNode2;
    CCSafeObject<CCLabelIF> m_nameText2;
    
    CCSafeObject<CCNode> m_node3;
    CCSafeObject<CCNode> m_btnNode3;
    CCSafeObject<CCLabelIF> m_nameText3;
    
    CCSafeObject<CCNode> m_node4;
    CCSafeObject<CCNode> m_btnNode4;
    CCSafeObject<CCLabelIF> m_nameText4;
    
    CCSafeObject<CCNode> m_node5;
    CCSafeObject<CCNode> m_btnNode5;
    CCSafeObject<CCLabelIF> m_nameText5;
    
    CCSafeObject<CCNode> m_node6;
    CCSafeObject<CCNode> m_btnNode6;
    CCSafeObject<CCLabelIF> m_nameText6;
    
    CCSafeObject<CCNode> m_node7;
    CCSafeObject<CCNode> m_btnNode7;
    CCSafeObject<CCLabelIF> m_nameText7;
    
    CCSafeObject<CCNode> m_node8;
    CCSafeObject<CCNode> m_btnNode8;
    CCSafeObject<CCLabelIF> m_nameText8;
    
    CCSafeObject<CCNode> m_node9;
    CCSafeObject<CCNode> m_btnNode9;
    CCSafeObject<CCLabelIF> m_nameText9;
    
    
    CCSafeObject<CCNode> m_nodeClick1;
    CCSafeObject<CCNode> m_nodeClick2;
    CCSafeObject<CCNode> m_nodeClick3;
    CCSafeObject<CCNode> m_nodeClick4;
    CCSafeObject<CCNode> m_nodeClick5;
    CCSafeObject<CCNode> m_nodeClick6;
    CCSafeObject<CCNode> m_nodeClick7;
    CCSafeObject<CCNode> m_nodeClick8;
    CCSafeObject<CCNode> m_nodeClick9;
    
    CCSafeObject<CCNode> m_parNode;
    CCSafeObject<CCSprite> m_rankUp;
    CCSafeObject<CCSprite> m_rankDown;
    
    CCSafeObject<CCNode> m_flagList;
    CCSafeObject<CCControlButton> m_btnChange;
    CCSafeObject<CCControlButton> m_btnCancle;
    CCSafeObject<CCLabelIF> m_goldTxt;
    CCSafeObject<CCLabelIF> m_btnTxt;
    CCSafeObject<CCSprite> m_goldSpr;
    
    std::vector<CCNode*> m_buttonNodes;
    std::vector<ButtonState> m_buttonState;
    std::vector<CCParticleBatchNode*>m_parVec;
};

class FlagCell:public CCTableViewCell
{
public:
    FlagCell(){};
    ~FlagCell(){};
    
    static FlagCell* create(int idx, int celect = -1);
    void setData(int idx, int celect = -1);
private:
    bool init(int idx, int celect = -1);
};
 
#endif /* defined(__IF__AllianceCheckView__) */
