//
//  AllianceRankAttrView.h
//  IF
//
//  Created by ganxiaohua on 14-8-6.
//
//

#ifndef __IF__AllianceRankAttrView__
#define __IF__AllianceRankAttrView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "QuestController.h"
#include "CCMultiColTableView.h"

class AllianceRankAttrView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
{
public:
    static AllianceRankAttrView *create();
    
protected:
    // scrollview delegate

private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void onChangeClick(CCObject *pSender, CCControlEvent event);
    
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_rankNode;
    CCSafeObject<CCNode> m_scrollNode;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCNode> m_scrollBg;
    CCSafeObject<CCScrollView> m_scrollView;
    
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIF> m_rankTxt5;
    CCSafeObject<CCLabelIF> m_rankTxt4;
    CCSafeObject<CCLabelIF> m_rankTxt3;
    CCSafeObject<CCLabelIF> m_rankTxt2;
    CCSafeObject<CCLabelIF> m_rankTxt1;
    CCSafeObject<CCLabelIF> m_tip;
    
    //
    CCSafeObject<CCLabelIF> m_txt0;
    CCSafeObject<CCLabelIF> m_txt1;
    CCSafeObject<CCLabelIF> m_txt2;
    CCSafeObject<CCLabelIF> m_txt3;
    CCSafeObject<CCLabelIF> m_txt4;
    CCSafeObject<CCLabelIF> m_txt5;
    CCSafeObject<CCLabelIF> m_txt6;
    CCSafeObject<CCLabelIF> m_txt7;
    CCSafeObject<CCLabelIF> m_txt8;
    CCSafeObject<CCLabelIF> m_txt9;
    CCSafeObject<CCLabelIF> m_txt10;
    CCSafeObject<CCLabelIF> m_txt11;
    CCSafeObject<CCLabelIF> m_txt12;
    CCSafeObject<CCLabelIF> m_txt13;
    CCSafeObject<CCLabelIF> m_txt14;
    CCSafeObject<CCLabelIF> m_txt15;
    CCSafeObject<CCLabelIF> m_txt16;
    CCSafeObject<CCLabelIF> m_txt17;
    CCSafeObject<CCLabelIF> m_txt18;
    CCSafeObject<CCLabelIF> m_txt19;
    CCSafeObject<CCLabelIF> m_txt20;
    CCSafeObject<CCLabelIF> m_txt21;
    CCSafeObject<CCLabelIF> m_txt22;
    CCSafeObject<CCLabelIF> m_txt23;
};

class AllRankAttrCellHd: public CCNode
{
public:
    static AllRankAttrCellHd* create(int idx);
    AllRankAttrCellHd(int idx): m_idx(idx){};
    bool init();
protected:
private:
    CCSafeObject<CCLabelIF> m_txt;
    int m_idx;
};
#endif /* defined(__IF__AllianceRankAttrView__) */
