
#ifndef __IF__StoreBuyConfirmDialog__
#define __IF__StoreBuyConfirmDialog__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCBExtension.h"
#include "CCTypesExt.h"
#include "PopupBaseView.h"
#include "CCSliderBar.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define MSG_BUY_CONFIRM_OK "buy.confirm.ok"
#define MSG_BUY_CONFIRM_OK_WITHOUT_TWEEN "buy.confirm.ok.without.tween"
class StoreBuyConfirmDialog :
public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public cocos2d::CCTargetedTouchDelegate
, public CCEditBoxDelegate
{
public:
    StoreBuyConfirmDialog(void);
    virtual ~StoreBuyConfirmDialog(void);
    static StoreBuyConfirmDialog* show(const char *url,const char* title,const char* desc,int price,int color,CCCallFuncO * func,CCPoint startPos,int priceType = Gold,long cCount = 0);
    void setEffNodeRect(CCRect rect);
    void playBuyEffect(CCObject *ccObj = NULL);
    void playBuyWithoutEffect(CCObject *ccObj = NULL);
    void showSliderBar(int max);
    
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
protected:
    void onEnter();
    void onExit();
    bool init(const char* url, const char* title,const char* desc,int price,int color,int priceType = Gold,long cCount = 0);
    
    // ccb routines
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char* pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void keypressedBtnOk(CCObject * pSender, Control::EventType pCCControlEvent);
    void onAddClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onSubClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onOKBuy();
    
    void closeDialog();
    void showDialog();
    void showBG();
    void showEffect();
    void flyIcon();
    void flyIcon1();
    void hideEffect();
    void onOkBuyResource();
    
    void moveSlider(CCObject * pSender, Control::EventType pCCControlEvent);
    void setCostString();
    void removeWaitInter(CCObject*);
private:
    CC_SYNTHESIZE_SAFEOBJECT(CCCallFuncO, m_onYes, YesCallback);
    CC_SYNTHESIZE_SAFEOBJECT(CCCallFunc, m_onClose, CloseCallback);
    CC_SYNTHESIZE(CCPoint, m_startPos, StartPos);
    CCPoint m_effNodePos;
    CCSize m_effNodeSize;
    CCSafeObject<CCControlButton> m_btnOk;
    CCSafeObject<CCLabelIF> m_costNum;
    CCSafeObject<CCNode> m_costNode;
    CCSafeObject<CCNode> m_nodeIcon;
    CCSafeObject<CCSprite> m_sprIconBG;
    CCSafeObject<CCLabelIF> m_lblDesc;
    CCSafeObject<CCNode> m_subNode;
    CCSafeObject<CCLabelIF> m_lblTitle;
    CCSafeObject<CCLabelIF> m_lblEffect;
    CCSafeObject<CCNode> m_sprBG;
    CCSafeObject<CCSprite> m_sprEffect;
    CCSafeObject<CCSprite> m_sprEffect1;
    CCSafeObject<CCNode> m_editNode;
    CCSafeObject<CCNode> m_numNode;
    CCSafeObject<CCNode> m_barNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCSliderBar> m_slider;
    CCSafeObject<CCEditBox> m_editBox;
    int m_price;
    int m_priceType;
    int m_numAll;
    int m_numCurrent;
    WaitInterface* m_waitInterface;
};
#endif
