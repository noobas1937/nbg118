//
//  NBGNode.h
//  nbg
//
//  Created by guojiang on 4/29/15.
//
//

#ifndef __nbg__NBGNode__
#define __nbg__NBGNode__

/**
 * NBGNode 的 contentSize 大小就是 ccbi 的大小
 */

#include "NBGCCBReader.h"

#define CREATE_CCB_FUNC_WITH_DEFAULT_VARNAMES(__BA_CLASS__, __BA_VAR_CLASS__) \
static __BA_CLASS__* createWithCCB(const char* resFilePath) \
{ \
    __BA_CLASS__ *pRet = new __BA_CLASS__(); \
    if (pRet && pRet->init()) \
    { \
        pRet->autorelease(); \
        __BA_VAR_CLASS__ rimvn; \
        pRet->loadNodeResource(resFilePath, &rimvn); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = nullptr; \
        return nullptr; \
    } \
}

enum BANodeResourceType
{
    BANodeResourceType_CCBI,
    BANodeResourceType_JSON, // TODO
    BANodeResourceType_MANUAL, // 使用代码创建节点 重载 NBGNode::createChildrenManual
};

class BANodeResourceInfo
{
public:
    BANodeResourceInfo()
    : jsonData(nullptr)
    {}
    virtual ~BANodeResourceInfo() {}
    
    const char* jsonData; // TODO 用于从 JSON 配置里创建节点
};

class NBGNode
: public Node
{
public:
    template<class TNode, class TInfo>
    static TNode* createWithCCB(const char* ccbi)
    {
        TNode *pRet = new TNode();
        if (pRet && pRet->init())
        {
            pRet->autorelease();
            TInfo rimvn;
            pRet->loadNodeResource(ccbi, &rimvn);
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = nullptr;
            return nullptr;
        }
    }
    static NBGNode* createWithoutMemVars(const char* ccbi) { return createWithCCB<NBGNode, BANodeResourceInfo>(ccbi); }
    
    // -----------------------------
    
    NBGNode();
    virtual ~NBGNode();
    
    // 主要用于定义 assignMemVarCallback
    virtual void beforeLoadNodeResource(const BANodeResourceInfo* nri) {}
    // 添加一个资源Node为本Node的子Node
    void loadNodeResource(const char* resFilePath, const BANodeResourceInfo* nri = nullptr, BANodeResourceType nrt = BANodeResourceType_CCBI);
    // 主要用于添加按钮或者其他节点到监听队列 addTouchEventListener 需要注意添加先后次序会导致点击先后次序
    virtual void afterLoadNodeResource() {}
    
    // 从 ccbi 文件或者其他编辑器文件读取出来的 Node
    // 暂时不需要
    // Node* getNode() { return m_pNode; }
    
    void setAnimationDelegate(IBA2CCBAnimationDelegate *pDelegate);
    IBA2CCBAnimationDelegate* getAnimationDelegate() { return m_delegate; }
    void playAnimation(const char* animationName);
    void playAnimation(const char* animationName, float fTweenDuration);
    
    // ccb 中包含 export 的 Label 节点的内容将会存储在以下 map 中
    std::map<Label*, const char*>& getLabelTxtMap() { return m_labelTxtMap; }
    
    int luaHandler_onTouchClick;
    
protected:
    // -----------------------------
    
    virtual void loadCCB(const char* resFilePath, const BANodeResourceInfo* nri);
    virtual void loadJSON(const BANodeResourceInfo* nri) {} // TODO
    virtual void createChildrenManual() {}
    
    // -----------------------------
    
    IBA2CCBAnimationDelegate* m_delegate;
    NBGCCBReader* m_pNBGCCBReader;
    // 从 ccbi 文件或者其他编辑器文件读取出来的 Node
    Node* m_pNode;
    
    // ccb 中包含 export 的 Label 节点的内容将会存储在以下 map 中
    std::map<Label*, const char*> m_labelTxtMap;
};

#endif /* defined(__nbg__NBGNode__) */
