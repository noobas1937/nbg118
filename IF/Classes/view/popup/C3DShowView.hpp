//
//  C3DShowView.hpp
//  IF
//
//  Created by tao.yu on 15/9/29.
//
//

#ifndef C3DShowView_hpp
#define C3DShowView_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "Titan.h"
#include "CCTypesExt.h"
#include "CCIFTouchNode.h"
#include "NBSprite3D.h"
using namespace cocos2d;
class C3DShowView: public CCIFTouchNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
    
    
public:
    C3DShowView():
    m_Model(nullptr)
    {};
    virtual ~C3DShowView(){};
    static C3DShowView* create(const char* modelPath, const char* texPath);
    
    CCSafeObject<NBSprite3D>& getModel() { return m_Model; }
    void setAnimationNames(std::vector<std::string> name) { m_names = name; }
//    void 
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    CCSafeObject<NBSprite3D> m_Model;
    bool init(const char* modelPath, const char* texPath);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCNode> m_modelPos;
    CCSafeObject<CCLayerColor> m_touchLayer;
    Vec2 m_TouchPoint;
    bool m_TouchMoved;
    std::vector<std::string> m_names;
};
#endif /* C3DShowView_hpp */
