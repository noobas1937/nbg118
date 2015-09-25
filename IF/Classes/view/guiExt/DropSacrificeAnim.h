//
//  DropSacrificeAnim.h
//  IF
//
//  Created by lifangkai on 14-11-20.
//
//

#ifndef __IF__DropSacrificeAnim__
#define __IF__DropSacrificeAnim__

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "ParticleController.h"
#include "CCAniNode.h"

class DropSacrificeAnim: public CCAniNode
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    bool initDropRdCCB(int type,int hintType);
    static DropSacrificeAnim* create(int type,int hintType);
    DropSacrificeAnim(int type,int hintType):m_type(type),m_hintType(hintType){};
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    void onPlayEnd();
    void onPlayFlyEnd();
    virtual void onEnter();
    virtual void onExit();
    
protected:
     CCSafeObject<CCArray> m_dropSprs;
    CCSafeObject<CCArray> m_dropNodes;
    
    CCSafeObject<CCArray> m_show_1_Nodes;
    CCSafeObject<CCArray> m_show_2_Nodes;
    CCSafeObject<CCArray> m_show_5_Nodes;
    CCSafeObject<CCArray> m_show_10_Nodes;
    
    CCSafeObject<CCNode> m_show_1_Node;
    CCSafeObject<CCNode> m_show_2_Node;
    CCSafeObject<CCNode> m_show_5_Node;
    CCSafeObject<CCNode> m_show_10_Node;
    
    void onPlayMusic(float _time);
    void onDelete(float _time);
    string musicName;
    int m_type;
    int m_hintType;
};

#endif /* defined(__IF__DropSacrificeAnim__) */
