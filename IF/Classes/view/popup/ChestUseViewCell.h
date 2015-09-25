//
//  ChestUseViewCell.h
//  LastEmpire
//
//  Created by zhangjun on 2015-03-24 18:37:40.
//
//
		
#ifndef __LastEmpire__ChestUseViewCell__
#define __LastEmpire__ChestUseViewCell__
		
#include "CommonInclude.h"
#include "CCBExtension.h"
//#include "CCLabelIF.h"

class ChestUseViewCell :
public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
,public CCTableViewCell
{
public:
    ChestUseViewCell();
    virtual ~ChestUseViewCell();
    virtual bool init();
    virtual void onExit();
    virtual void onEnter();
    static ChestUseViewCell* create();
    
    void setData(int itemId,string nameStr,int countStr,int itemType);
		
    bool onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);
protected:

    CCSafeObject<CCScale9Sprite> m_spr9Bg;
    CCSafeObject<CCSprite> m_sprChestBg;
    CCSafeObject<CCNode> m_nodeChest;
    CCSafeObject<Label> m_labCount;
    CCSafeObject<Label> m_labDes;
};

//end 3
		
#endif /* defined(__LastEmpire__ChestUseViewCell__) */
		
