
#ifndef __IF__TreasureMapView__
#define __IF__TreasureMapView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
//#include "CCClipNode.h"
#include "WorldController.h"

#define MSG_TREASURE_MAP_INFO_CHANGE "msg.treasure.map.info.change"

class TreasureMapView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCEditBoxDelegate
{
public:
    static TreasureMapView* create(int color,int itemid);
    TreasureMapView();
    virtual ~TreasureMapView();
    
    void refreshData(CCObject* ccObj);
protected:

    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int color,int itemid);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    void editBoxReturn(CCEditBox *editBox);
    void editBoxEditingDidBegin(CCEditBox* editBox) ;
    
    
    void onNextBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onPreBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onDestroyBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onPointBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onCloseBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
private:
    void addTreasureNode(int idx,float posX);
    void onClickTip();
    void onDestroyMap();
    bool isMarch(unsigned int targetidx);
    
    CCSafeObject<CCControlButton> m_preBtn;
    CCSafeObject<CCControlButton> m_nextBtn;
    CCSafeObject<CCNode> m_nodeTouch;
    CCSafeObject<CCControlButton> m_btnDestroy;
    CCSafeObject<CCControlButton> m_btnPoint;
    CCSafeObject<CCSprite> m_marchIcon;
    CCSafeObject<CCLabelIF> m_lblTitle;
    CCSafeObject<CCNode> m_editBoxNode;
    CCSafeObject<CCNode> m_nodePos[5];
    CCSafeObject<CCSprite> m_starSpr[5];
    CCSafeObject<CCSprite>  m_mineSpr[3];
    CCSafeObject<CCSprite>  m_boxSpr[3];
    CCSafeObject<CCSprite> m_sprPos;
    CCSafeObject<CCNode> m_nodeItem;
    CCSafeObject<CCNode> m_nodeTip;
    CCSafeObject<CCEditBox> m_editBox;
    int mColor;
    int mItemid;
    vector<TreasureInfo> mData;
    WaitInterface *mWaitInterface;
//    CCClipNode *mClipNode;
//    CCNode *mMapNode;
    int mTotalPage;
    int mPage;
    bool m_editOpen;
    bool m_inited;
};


//class TreasureMapNode: public CCNode
//, public CCBMemberVariableAssigner
//, public CCBSelectorResolver
//, public CCTargetedTouchDelegate{
//public:
//    static TreasureMapNode* create(const TreasureInfo &info);
//    TreasureMapNode();
//    virtual ~TreasureMapNode();
//protected:
//    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
//    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
//    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
//    bool init(const TreasureInfo &info);
//    
//    virtual void onEnter();
//    virtual void onExit();
//    
//    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//private:
//    void onDestroyBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
//    
//    CCSafeObject<CCLabelIF> m_lblPoint;
//    CCSafeObject<CCNode> m_touchNode;
//    CCSafeObject<CCControlButton> m_btnDestroy;
//    TreasureInfo mInfo;
//    CCPoint m_touchPoint;
//};

#endif /* defined(__IF__TreasureMapView__) */
