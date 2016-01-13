//
//  CCBExtension.h
//  Sparta
//
//  Created by 谢文杰 on 12-10-9.
//  Copyright (c) 2012年 Elex-Tech. All rights reserved.
//

#ifndef Sparta_CCBExtension_h
#define Sparta_CCBExtension_h


//#include "cocos-ext.h"
#include "CCTypesExt.h"
#include "cocosextV3.h"

NS_CC_EXT_BEGIN

void setButtonTitle(CCControlButton * btn, const char* title, float widthPercent=0.9, float heightPercent=0.75,bool isOrigin=false);
/**
 * CCB member node wrapper.
 * @note Member node in CCB is not retained on assignment as retain has been
 * done in onAssignCCBMemberVariable, but release on destruct. So operator=
 * should only be used for CCBMemberAssigner, use setNode instead.
 */

//template <class CCNodeTypePointer>
//class CCBSafeNode {
//public:
//    CCBSafeNode()
//    : m_node(NULL) {}
//    ~CCBSafeNode()
//    {CC_SAFE_RELEASE(m_node);}
//    
//    CCNodeTypePointer& operator=(const CCNodeTypePointer& node){
//        CCAssert(m_node==NULL, "reassigning CCBSafeNode, caution of memory leak.");
//        m_node=node;
//        return m_node;
//    }
//    operator CCNodeTypePointer&()
//    {return m_node;}
//    CCNodeTypePointer operator->()
//    {return m_node;}
//    bool operator==(const CCNodeTypePointer& node)
//    {return m_node==node;}
//    bool operator!=(const CCNodeTypePointer& node)
//    {return m_node!=node;}
//    
//    CC_SYNTHESIZE_RETAIN(CCNodeTypePointer, m_node, Node);
//};

/**
 * A general ccb file loader.
 * This will load @p pCCBFileName in to @p pParent, using @p pOwner as
 * document owner.
 * @return Root node of CCB loaded.
 * @remarks CCB loaded will not be added to any node if @p pParent is NULL.
 * @pAniman variable used for controling ccb animation , do retain and release by you self
 */
CCNode* CCBLoadFile(const char * pCCBFileName, CCNode* pParent=NULL, CCObject* pOwner=NULL, bool batchFlag=false);

/** Generic custome class loader based @a CCBBaseLoader */
template <class ClassType, class CCBBaseLoader>
class CCBCustomClassLoader: public CCBBaseLoader {
public:
    typedef CCBCustomClassLoader<ClassType, CCBBaseLoader> tCCBCustomClassLoader;
    
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(tCCBCustomClassLoader, loader);

    /** a general CCBBaseLoader based custom class ccb file loader */
    static ClassType* loadFromFile(const char * pCCBFileName, const char * pClassName, CCNode* pParent = NULL, CCObject* pOwner = NULL) {
        /* get shared CCNodeLoaderLibrary */
        CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
        if (!ccNodeLoaderLibrary)
            return NULL;
        ccNodeLoaderLibrary->registerCCNodeLoader(pClassName, tCCBCustomClassLoader::loader());
        
        /* Create an CCBReader. */
        cocosbuilder::CCBReader * ccbReader = new cocosbuilder::CCBReader(ccNodeLoaderLibrary);
        if (!ccbReader)
            return NULL;
        
        ClassType * node = dynamic_cast<ClassType*>(ccbReader->readNodeGraphFromFile(pCCBFileName, pOwner));
        if(node != nullptr) {
            if (pParent)
                pParent->addChild(node);
        } else {
            CCLOG("Error in Load cocosBuilder data :%s", pCCBFileName);
        }
        ccbReader->release();
        return node;

    }
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ClassType);
};

/** Generic custom layer loader for layer rooted ccb file */
template <class NodeType>
class CCBCustomLayerLoader: public CCLayerLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCBCustomLayerLoader<NodeType>, loader);
    
    /** a general layer based custom class ccb file loader */
    static NodeType* loadFromFile(const char * pCCBFileName, const char * pClassName, CCNode* pParent = NULL, CCObject* pOwner = NULL) {
        return CCBCustomClassLoader<NodeType, CCLayerLoader>::loadFromFile(pCCBFileName, pClassName, pParent, pOwner);
    }
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(NodeType);
};

/** Generic custom node loader for node rooted ccb file */
template <class NodeType>
class CCBCustomNodeLoader: public CCNodeLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCBCustomNodeLoader<NodeType>, loader);
    
    /** a general node based custom class ccb file loader */
    static NodeType* loadFromFile(const char * pCCBFileName, const char * pClassName, CCNode* pParent = NULL, CCObject* pOwner = NULL) {
        return CCBCustomClassLoader<NodeType, CCNodeLoader>::loadFromFile(pCCBFileName, pClassName, pParent, pOwner);
    }
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(NodeType);
};

/**
 * General owner class for cocosbuilder.
 * If you want SelectorResolver, simply inherite from CCBGeneralOwner.
 *
 * usage:
 * CCBGeneralOwner* owner=CCBGeneralOwner::createFromCCB("test.ccbi",
 *                                                       parentNode);
 * if (owner) {
 *     owner->getNode<CCLabelTTF*>("title")->setText("test");
 *     owner->getNode<CCSprite*>("icon")->setVisible(false);
 * }
 */
template <class OwnerType=cocos2d::CCObject>
class CCBGeneralOwner
: public OwnerType
, public CCBMemberVariableAssigner
{
public:
    typedef std::map<std::string, CCSafeObject<cocos2d::CCNode> > NodeMap;
    
    CCBGeneralOwner(){};
    virtual ~CCBGeneralOwner(){};
    
    static CCBGeneralOwner* createFromCCB(const std::string& ccbiFileName, cocos2d::CCNode* pParent = NULL) {
        CCBGeneralOwner* owner=new CCBGeneralOwner();
        if (owner && owner->initWithFile(ccbiFileName, pParent)) {
            owner->autorelease();
            return owner;
        }
        CC_SAFE_DELETE(owner);
        return NULL;
    }
    
    bool initWithFile(const std::string& ccbiFileName, cocos2d::CCNode* pParent = NULL) {
        m_pCCBNode=CCBLoadFile(ccbiFileName.c_str(), pParent, this);
        return m_pCCBNode!=nullptr;
    }
    
    bool hasNode(const std::string& name)
        { return m_nodes.count(name)>0; }
    
    /** 如果node存在并且类型一致就返回，否则返回一个临时创建的对象 */
    template<class NodeType>
    NodeType* getNode(const std::string& name) {
        NodeMap::iterator iter=m_nodes.find(name);
        if (iter!=m_nodes.end()) {
            NodeType* node=dynamic_cast<NodeType*>(iter->second.getObject());
            if (node)
                return node;
        }
        return NodeType::create();
    }
    
    CCNode* getNode(const std::string& name) {return getNode<CCNode>(name);}
    CCLayer* getLayer(const std::string& name) {return getNode<CCLayer>(name);}
    CCSprite* getSprite(const std::string& name) {return getNode<CCSprite>(name);}
    CCControlButton* getButton(const std::string& name) {return getNode<CCControlButton>(name);}
    CCLabelTTF* getLabel(const std::string& name) {return getNode<CCLabelTTF>(name);}
    
protected:
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, cocos2d::CCString * pMemberVariableName, cocos2d::CCNode * pNode) {
        if (pTarget==this) {
            m_nodes[pMemberVariableName->getCString()]=pNode;
            return true;
        }
        return false;
    }
    
private:
    NodeMap m_nodes;
    CCSafeObject<cocos2d::CCNode> m_pCCBNode;
};

typedef CCBGeneralOwner<CCNode> CCBGeneralNodeOwner;
typedef CCBGeneralOwner<CCLayer> CCBGeneralLayerOwner;

NS_CC_EXT_END

/**
 * define a show function for a specific type
 * @__TYPE__ class type to add show()
 */
#define SHOW_FUNC(__TYPE__) \
static __TYPE__* show() \
{ \
__TYPE__ *pRet = new __TYPE__(); \
if (pRet && pRet->init()) \
{ \
pRet->autorelease(); \
cocos2d::CCDirector::sharedDirector()->getRunningScene()->addChild(pRet); \
return pRet; \
} \
else \
{ \
delete pRet; \
pRet = NULL; \
return NULL; \
} \
}

#endif
