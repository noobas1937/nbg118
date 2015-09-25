//
//  AllianceWarHeadCell.h
//  IF
//
//  Created by ganxiaohua on 14-9-22.
//
//

#ifndef __IF__AllianceWarHeadCell__
#define __IF__AllianceWarHeadCell__

#include "CommonInclude.h"
#include "HFHeadImgNode.h"

class AllianceWarHeadCell : public CCNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static AllianceWarHeadCell *create(std::string icon,int index,string uid,int picVer,int mType=-1);
    
private:
    AllianceWarHeadCell(std::string icon,int index,string uid,int picVer,int mType) : m_icon(icon),m_index(index),m_mType(mType),m_uid(uid),m_picVer(picVer){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    std::string m_icon;
    std::string m_uid;
    int m_picVer;
    int m_index;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_bg;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    int m_mType;
};
#endif /* defined(__IF__AllianceWarHeadCell__) */
