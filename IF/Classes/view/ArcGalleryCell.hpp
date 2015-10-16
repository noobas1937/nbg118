//
//  ArcGalleryCell.hpp
//  IF
//
//  Created by tao.yu on 15/10/16.
//
//

#ifndef ArcGalleryCell_hpp
#define ArcGalleryCell_hpp

#include <stdio.h>
#include "CommonInclude.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "ScrollTableView.h"

class ArcGalleryCell :
public Layer
,public CCBMemberVariableAssigner
{
public:
    static ArcGalleryCell* create();
    ArcGalleryCell(){};
    CCSafeObject<CCSprite> m_button;
    CCSafeObject<CCLabelIF> m_buttonTxt;
    CCSafeObject<CCNode> m_buttonNode;
    CCSafeObject<CCNode> m_txtNode;
    CCSafeObject<CCSprite> m_icon;
    CCSafeObject<CCSprite> m_lockIcon;
    CCSafeObject<CCSprite> m_LvSpr;
    CCSafeObject<CCNode> m_LvNode;
    
    CCSafeObject<CCNode> m_lockNode;
    CCSafeObject<CCNode> m_lockLvNode;
    CCSafeObject<CCLabelIF> m_buttonLockTxt;
private:
    bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    

};
#endif /* ArcGalleryCell_hpp */
