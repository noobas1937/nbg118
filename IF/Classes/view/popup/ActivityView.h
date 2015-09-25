//
//  ActivityView.h
//  IF
//
//  Created by xxrdsg on 15-6-3.
//
//

#ifndef __IF__ActivityView__
#define __IF__ActivityView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "ActivityEventObj.h"

class ActivityView:public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static ActivityView* create(ActivityEventObj*);
    ActivityView(ActivityEventObj* obj): m_obj(obj){};
    void onEnter();
    void onExit();
    bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void onClickRwdBtn(CCObject *pSender, CCControlEvent event);
    
protected:
private:
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCNode> m_labelNode;
    CCSafeObject<CCLabelIF> m_infoLabel;
    CCSafeObject<CCLayerColor> m_listNode;
    CCSafeObject<CCLayerColor> m_bottomNode;
    CCSafeObject<CCControlButton> m_rwdBtn;
    ActivityEventObj* m_obj;
};

#endif /* defined(__IF__ActivityView__) */
