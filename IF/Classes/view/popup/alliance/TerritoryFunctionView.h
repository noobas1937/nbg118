//
//  TerritoryFunctionView.h
//  IF
//
//  Created by 〜TIAN~ on 15/5/19.
//
//

#ifndef __IF__TerritoryFunctionView__
#define __IF__TerritoryFunctionView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"

class TerritoryFunctionView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static TerritoryFunctionView* create(int stat);
    TerritoryFunctionView(int stat):m_stat(stat){};
    ~TerritoryFunctionView(){};
    void updateInfo();
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(int stat);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLayerColor> m_viewBg;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCLabelIF> m_descriptionText;
    CCSafeObject<CCScrollView> m_scrollView;

    int m_scrollH;
    int m_listH;
    int m_stat;
    CCSafeObject<CCNode> m_viewBg2;
    float m_viewBg2_py;
};

class TerritoryFunctionCell : public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewCell
{
public:
    static TerritoryFunctionCell* create(std::string buff, std::string description, std::string title, std::string index, int stat);
    TerritoryFunctionCell(std::string buff, std::string description, std::string title, std::string index, int stat):m_buff(buff),m_description(description),m_title(title),m_index(index),m_stat(stat){};
    void setData(std::string buff, std::string description, std::string title, std::string index, int stat);
private:
    bool init(std::string buff, std::string description, std::string title, std::string index,int stat);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCControlButton> m_useBtn;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCNode> m_picNode;
    std::string m_buff;
    std::string m_description;
    std::string m_title;
    std::string m_index;
    int m_stat;
};
#endif /* defined(__IF__TerritoryFunctionView__) */
