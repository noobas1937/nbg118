#ifndef __IF__PropSpeedupView__
#define __IF__PropSpeedupView__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCBExtension.h"
#include "CCTypesExt.h"
#include "PopupBaseView.h"
#include "WaitInterface.h"
#include "CCSliderBar.h"
#include "NBSlider.h"

USING_NS_CC;
USING_NS_CC_EXT;

class PropSpeedupView :
public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public cocos2d::CCTargetedTouchDelegate
, public CCEditBoxDelegate{
public:
    PropSpeedupView(void);
    virtual ~PropSpeedupView(void);
    static PropSpeedupView* show(int type, int bid , int qid);
    //virtual void scrollViewDidScroll(CCScrollView* view);
    //virtual void scrollViewDidZoom(CCScrollView* view);
    void refreshView(CCObject* ccObj = NULL);
    void checkClose(CCObject* ccObj = NULL);
protected:
    void onEnter();
    void onExit();
    bool init(int type,int bid, int qid);
    
    // ccb routines
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char* pMemberVariableName, cocos2d::CCNode * pNode);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void keypressedBtnLast(CCObject * pSender, Control::EventType pCCControlEvent);
    void keypressedBtnNext(CCObject * pSender, Control::EventType pCCControlEvent);
    void closeDialog();
    void onUseClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onAddClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onSubClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void sliderCallBack(Ref *pSender, NBSlider::EventType type);
    void editBoxReturn(CCEditBox *editBox);
    void editBoxEditingDidBegin(CCEditBox* editBox) {m_editOpen = true;}
    
    void initDataList();
    void updateProgressBar(float pUpdate = 0);
    void autoBoundsScroll();
    static bool sortByOrder(int itm1, int itm2);
    void initSliderBar();
    void refreshEditBox();
    void playAnimation();
    void animationCallback();
    void sureToUseTool();
    void useTool();
    void setSelectSpritePosition();
private:
    CCSafeObject<CCNode> m_sprBG;
    CCSafeObject<CCNode> m_nodeTime;
    CCSafeObject<CCScale9Sprite> m_progressTime;
    CCSafeObject<CCLabelIF> m_progressTxt;
    CCSafeObject<CCNode> m_nodeProp;
    CCSafeObject<CCNode> m_nodeProp1;
    CCSafeObject<CCLabelIF> m_lblInfo;
    CCSafeObject<CCLabelIF> m_lblTitle;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCNode> m_editNode;
//    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCSprite> m_sprBtn;
//    CCSafeObject<CCLabelIF> m_lblBtn;
    CCSafeObject<CCControlButton> m_useBtn;
    CCSafeObject<NBSlider> m_trainSlider;
    CCSafeObject<CCEditBox> m_editBox;
    CCSafeObject<CCNode> m_editBoxNode;
    CCSafeObject<CCNode> m_sliderContainer;
    CCSafeObject<CCControlButton> m_addBtn;
    CCSafeObject<CCControlButton> m_subBtn;
    CCSafeObject<CCScale9Sprite> m_sprSelect;
    CCSafeObject<CCLabelIF> m_lblTimeTip;
    WaitInterface* m_waitInterface;
    
    CC_SYNTHESIZE_READONLY(int, m_proptype, PropType);
    CC_SYNTHESIZE_READONLY(int, m_qID, QID);
    CC_SYNTHESIZE_READONLY(int, m_buildingID, BuildingID);
    std::vector<int> m_Tools;
    int m_clickIndex;
    int m_lastClickIndex;
    CCPoint m_dragPos;
    CCPoint m_scrollOffset;
    bool m_isDrag;
    int m_totalTime;
    bool m_editOpen;
    
    int m_chooseCount;
    int m_maxCount;
    int m_addTime;
    float m_buildTime;
    bool m_isSendingCMD;
};

class PropSpeedupCell :
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static PropSpeedupCell* create(int itemId);
    PropSpeedupCell();
private:
    bool init(int itemId);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_lblName;
    CCSafeObject<CCLabelIF> m_lblCount;
    CCSafeObject<CCNode> m_nodeIcon;
    CC_SYNTHESIZE_READONLY(int, mitemID, ItemID);
};
#endif
