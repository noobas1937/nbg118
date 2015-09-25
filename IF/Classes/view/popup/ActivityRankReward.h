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

class ActivityRankReward : public CCTableViewCell
{
public:
    static ActivityRankReward *create(CCArray* datas,std::string title,bool flag=false,int type=-1);
    ActivityRankReward(CCArray* datas,std::string title,bool flag,int type):m_datas(datas),m_title(title),m_flag(flag),m_type(type){};
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
};

class ActivityRankRewardCell : public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static ActivityRankRewardCell *create(CCDictionary* info);
private:
    ActivityRankRewardCell(CCDictionary* info):m_info(info){};
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
};

#endif /* defined(__IF__ActivityRankReward__) */
