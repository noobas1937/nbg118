//
//  AllianceTitleRankCell.h
//  IF
//
//  Created by ganxiaohua on 14-7-25.
//
//

#ifndef __IF__AllianceTitleRankCell__
#define __IF__AllianceTitleRankCell__

#include "CommonInclude.h"

class AllianceTitleRankCell :public Layer
 
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    CREATE_FUNC(AllianceTitleRankCell);
    
    AllianceTitleRankCell(){};
    ~AllianceTitleRankCell(){}
    
    void setData(int rank,std::string title,CCArray* array);
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCNode> m_arrow;
    CCSafeObject<CCLabelIF> m_rankTxt;
    CCSafeObject<CCLabelIF> m_titleTxt;
    
    int m_rank;
    std::string m_title;
    CCSafeObject<CCArray> m_data;
};

#endif /* defined(__IF__AllianceTitleRankCell__) */
