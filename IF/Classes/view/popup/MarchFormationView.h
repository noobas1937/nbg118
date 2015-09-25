//
//  MarchFormationView.h
//  IF
//
//  Created by 〜TIAN~ on 15/7/21.
//
//

#ifndef __IF__MarchFormationView__
#define __IF__MarchFormationView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "CheckBox.h"
#include "CCSliderBar.h"

class MarchFormationView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public CCMultiColTableViewDataSource
, public CCMultiColTableViewDelegate
{
public:
    static MarchFormationView* create(int index);
    MarchFormationView(int index):m_index(index){};
    virtual ~MarchFormationView();
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
    virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(CCMultiColTableView *multiTable, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickQuickBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onAddClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickMarchBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onHelpClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void updateInfo();
    void generateData();
    void loadResource(float _time);
    void setAddBtnState();
    void selectAll();
    void unselectAll();
    void updateArmyNumber(CCObject* obj);
    void updateFormationChange(CCObject* obj);
    void updateLoadInfo(CCObject* obj);
    void onFormation1Click(CCObject *pSender, CCControlEvent event);
    void onFormation2Click(CCObject *pSender, CCControlEvent event);
    void getFormationDetail(int index);
    CCSafeObject<CCControlButton> m_marchBtn;
    CCSafeObject<CCControlButton> m_quickBtn;
    CCSafeObject<CCControlButton> m_helpBtn;
    CCSafeObject<CCLabelIF> m_hintText1;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCLabelIF> m_msg1Label;
    CCSafeObject<CCLabelIF> m_msg2Label;
    CCSafeObject<CCLabelIF> m_msg3Label;
    CCSafeObject<CCLabelIF> m_hintText;
    CCSafeObject<CCLabelIF> m_forTipsTxt;
    CCSafeObject<CCControlButton> m_addBtn;
    CCSafeObject<CCSprite> m_addIcon;
    CCSafeObject<CCSprite> m_addIcon1;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCScale9Sprite> m_renderBG;
    //    CCSafeObject<CheckBox> m_box;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCNode> m_alertNode;
    CCSafeObject<CCNode> m_forbidenNode;
    CCSafeObject<CCNode> m_iconContainer;
    CCSafeObject<CCMultiColTableView > m_tabView;
    CCSafeObject<CCArray> m_tmpArray;
    CCSafeObject<CCControlButton> m_formationBtn1;
    CCSafeObject<CCControlButton> m_formationBtn2;
    int m_index;
    map<std::string, int> m_selSoldierMap;
};

class MarchFormationCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewCell
,public CCEditBoxDelegate
{
public:
    static MarchFormationCell* create(CCNode* clickNode,string itemId, int num, int index);
    MarchFormationCell(){};
    void setData(string itemId, int num, int index);
    void refresh();
private:
    virtual bool init(CCNode* clickNode, string itemId, int num, int index);
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void valueChange(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onSubClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onAddClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void editBoxReturn(CCEditBox *editBox);
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCControlButton> m_subBtn;
    CCSafeObject<CCControlButton> m_addBtn;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_levelNode;
    CCSafeObject<CCNode> m_sliderNode;
    CCSafeObject<CCSliderBar> m_slider;
    CCSafeObject<CCNode> m_editNode;
    CCEditBox* m_editBox;
    int m_cntNum;
    int m_index;
    CCNode* m_clickNode;
public:
    string m_soldierId;
    int m_num;
};

enum{
    MARCH_TYPE,
    DEFENCE_TYPE,
};

#endif /* defined(__IF__MarchFormationView__) */
