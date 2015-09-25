//
//  MakingSoldierView.h
//  IF
//
//  Created by ganxiaohua on 14-5-22.
//
//

#ifndef __IF__MakingSoldierView__
#define __IF__MakingSoldierView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "ArcPopupBaseView.h"
#include "ArcScrollView.h"

class MakingSoldierView: public ArcPopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static MakingSoldierView* create(std::string armyId,QueueInfo* info,int buildingId);
    MakingSoldierView(std::string armyId,QueueInfo* info,int buildingId):m_armyId(armyId),m_info(info),m_buildingId(buildingId){};
    ~MakingSoldierView(){};
protected:
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void killCDClick(CCObject * pSender, Control::EventType pCCControlEvent);
    int getGoldByTime(int time);
    void updateTime(float t = 0.0);
    void closeHandle();
    CCSafeObject<CCControlButton> m_killCDBtn;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCLabelIF> m_btnLabel;
    CCSafeObject<CCLabelIF> m_inBtnGoldNum;
    CCSafeObject<CCNode> m_soldierNode;
    CCSafeObject<CCScale9Sprite> m_timeProgress;
    CCSafeObject<CCScale9Sprite> m_soldierBg;
    QueueInfo* m_info;
    std::string m_armyId;
    int m_haveTime;
    int m_resIndex;
    int m_buildingId;
};
#endif /* defined(__IF__MakingSoldierView__) */
