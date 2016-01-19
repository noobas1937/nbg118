

#ifndef __IF__MailGiftView__
#define __IF__MailGiftView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCTableViewTouchIFCell.h"
#include "HFHeadImgNode.h"
#define MAIL_GIFT_ITEM_COL 4
#define MSG_MAIL_GIFT_CHOOSE "msg.mail.gift.choose"
class MailGiftView{
public:
    static void addToolIcon(int toolID, CCNode* mParent, CCLabelIF* mName = NULL, CCLabelIF* mDes =NULL);
};

class MailGiftCell : public CCLayer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static MailGiftCell* create(CCSize size,int propID,int itemid = -1);
    MailGiftCell();
    virtual ~MailGiftCell();
    
    void refreshData(CCObject *ccObj = NULL);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
protected:
    virtual bool init(CCSize size,int itemid,int propID);
    void onChooseItem();
    void removeEffect();
    void addEffect();
//    void addPopParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
private:
    CCSafeObject<CCNode> m_headNode;
    CCSafeObject<CCLabelIF> m_lblTitle;
    CCSafeObject<CCLabelIF> m_lblDes;
    CCSafeObject<CCLabelIF> m_lblName;
    CCSafeObject<CCLabelIF> m_lblInfo;
    CCSafeObject<CCSprite> m_sprChoose;
    CCSafeObject<CCLayer> m_effLayer;
    CCSafeObject<CCNode> m_nodeCount;
    CCSafeObject<CCLabelIF> m_numLabel;
//    vector<CCParticleBatchNode*> m_parPopVec;
    CC_SYNTHESIZE(int, mItemID, ItemID);
    CC_PROPERTY(int, mItemCount, ItemCount);
    int mPropID;
    bool m_canChoose;
    std::string mToName;
    std::string mContentes;
    CCSize mContentSize;
};


class MailGiftItemView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
, public CCTableViewDataSource
, public DefaultTableViewDelegate{
public:
    static bool sortByUseOder(int itemId1, int itemId2);
    
    static MailGiftItemView* create(CCSize &viewSize,int propID);
    MailGiftItemView();
    virtual ~MailGiftItemView();
    
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void refreshData(CCObject *ccObj = NULL);
    void onChooseBack(CCObject *ccObj = NULL);
protected:
    virtual bool init(CCSize &viewSize,int propID);
    void generateData();
private:
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_nodeList;
    CCSafeObject<CCScale9Sprite> m_bgSpr;
//    CCSafeObject<CCLabelIF> m_lblTitle;
    CCSafeObject<CCArray> m_data;
    CCTableView* m_tabView;
//    CCPoint m_startPoint;
    int mPropID;
};

class MailGiftItemCell
:public CCBSelectorResolver
,public CCBMemberVariableAssigner
//,public CCTouchDelegate
,public CCTableViewTouchIFCell
{
public:
    static MailGiftItemCell* create(std::string itemids,CCNode* touchNode,int propID);
    MailGiftItemCell(){};
    virtual ~MailGiftItemCell(){};
    void setData(std::string itemids);
    void updateSelectState(CCObject *ccObj = NULL);
private:
    bool init(std::string itemids,CCNode* touchNode,int propID);
    
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
private:
    CCSafeObject<CCLabelIF> m_numLabel[MAIL_GIFT_ITEM_COL];
    CCSafeObject<CCSprite> m_picBG[MAIL_GIFT_ITEM_COL];
    CCSafeObject<CCNode> m_itemNode[MAIL_GIFT_ITEM_COL];
    CCSafeObject<CCNode> m_picNode[MAIL_GIFT_ITEM_COL];
    CCSafeObject<CCSprite>  m_selectSpr;
    int mItemID[MAIL_GIFT_ITEM_COL];
    CCSafeObject<CCNode> m_touchNode;
    CCPoint m_startPoint;
    int mPropID;
};

class MailGiftReadPopUpView : public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    MailGiftReadPopUpView(MailInfo& info):m_info(info),m_waitInterface(NULL){
        
    };
    static MailGiftReadPopUpView* create(MailInfo& info);
    void refreshContent(CCObject* p = NULL);
protected:
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onRewardClick(CCObject *pSender, CCControlEvent event);
    void onAddSaveClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onDeleteClick(CCObject *pSender, CCControlEvent event);
    void onReturnClick(CCObject *pSender, CCControlEvent event);
    void onWriteClick(CCObject *pSender, CCControlEvent event);
    void onOkDeleteMail();
    MailInfo& m_info;
    
    CCSafeObject<CCNode> m_totalNode;
//    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCScale9Sprite> m_buildBG;
//    CCSafeObject<CCScale9Sprite> m_line;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_mailTitle;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCLabelIF> m_contentText;
    CCSafeObject<CCLabelIF> m_rewardTitle;
    CCSafeObject<CCLabelIF> m_rewardDesc;
    CCSafeObject<CCScale9Sprite> m_sprContent;
    CCSafeObject<CCControlButton> m_rewardBtn;
    CCSafeObject<CCNode> m_giftNode;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCNode> m_userHeadContainer;
    
    CCSafeObject<CCControlButton> m_addSaveBtn;
//    CCSafeObject<CCControlButton> m_unSaveBtn;
    CCSafeObject<CCControlButton> m_deleteBtn;
    CCSafeObject<CCControlButton> m_returnBtn;
    CCSafeObject<CCSprite> m_returnSpr;
    CCSafeObject<CCControlButton> m_writeBtn;
    CCSafeObject<CCNode> m_rewardNode;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCRenderTexture> m_selfModelLayer;
    CCSafeObject<CCNode> m_nodeCount;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    
    WaitInterface* m_waitInterface;
};
#endif
