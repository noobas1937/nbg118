//
//  ActivityRankReward.h
//  IF
//
//  Created by ganxiaohua on 14-9-3.
//
//

#ifndef __IF__ActivityRankReward__
#define __IF__ActivityRankReward__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "PopupViewController.h"

#define ARR_CELL_0 "ActivityRewardCell"
#define ARR_CELL_1 "ActivityRewardCell_long"

//#define ARR_W_0

class ActivityRankReward : public CCTableViewCell
{
public:
    static ActivityRankReward *create(CCArray* datas,std::string title,bool flag=false,int type=-1, string cell_ccb = ARR_CELL_0);
    ActivityRankReward(CCArray* datas,std::string title,bool flag,int type, string cell_ccb):m_datas(datas),m_title(title),m_flag(flag),m_type(type),m_cell_ccb(cell_ccb){};
    ~ActivityRankReward(){};
    void setData(CCArray* datas,std::string title,bool flag,int type=-1);
    int getTotalH();
private:
    virtual bool init();
    
    CCSafeObject<CCLabelIF> m_lable;
    CCSafeObject<CCNode> m_container;
    CCSafeObject<CCArray> m_datas;
    std::string m_title;
    bool m_flag;
    int m_type;
    string m_cell_ccb;
};

class ActivityRankRewardCell : public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static ActivityRankRewardCell *create(CCDictionary* info, string cell_ccb);
private:
    ActivityRankRewardCell(CCDictionary* info, string cell_ccb)
    :m_info(info)
    ,m_cell_ccb(cell_ccb){};
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCNode> m_iconNode;
    CCDictionary* m_info;
    string m_cell_ccb;
};

#endif /* defined(__IF__ActivityRankReward__) */
