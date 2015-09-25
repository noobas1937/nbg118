//
//  AchieveFinishView.h
//  IF
//
//  Created by ganxiaohua on 14-6-25.
//
//

#ifndef __IF__AchieveFinishView__
#define __IF__AchieveFinishView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "QuestController.h"
#include "CCMultiColTableView.h"
#include "QuestInfo.h"

class AchieveFinishView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    ~AchieveFinishView(){};
    AchieveFinishView(std::string id):m_id(id){};
    static AchieveFinishView *create(std::string id);
protected:
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void keypressedBtnCancel(CCObject * pSender, Control::EventType pCCControlEvent);
    void keypressedBtnOk(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void closehandle();
    void closehandle2();
    
    CCSafeObject<CCControlButton> m_btnOk;
    CCSafeObject<CCControlButton> m_btnCancel;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_descTxt;
    CCSafeObject<CCNode> m_ccbNode;
    std::string m_id;
};

#endif /* defined(__IF__AchieveFinishView__) */
