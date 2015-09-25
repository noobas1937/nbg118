//
//  LotteryRotateView.h
//  IF
//
//  Created by chenfubi on 15-03-02.
//
//

#ifndef __IF__LotteryRotateView__
#define __IF__LotteryRotateView__

#include "CommonInclude.h"

class LotteryActCell;
class LotteryRotateView : public cocos2d::Layer
//, public CCTouchDelegate
{
public:
    LotteryRotateView();
    virtual ~LotteryRotateView();
    
    bool init();
    
    static LotteryRotateView* create();
    
    bool isDragging() {return m_bDragging;}
    bool isTouchMoved() { return m_bTouchMoved; }
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    const char* getRotateData() const;
    void setRotateData(map<int, vector<string>> rewardMap, bool anim);
    CC_SYNTHESIZE(bool, m_enabled, Enabled);
    
private:
    virtual void onEnter();
    virtual void onExit();
    void setContentOffsetForMove();
    CCRect getViewRect();
    //    virtual void draw();
    CCNode* getPanelChild();
    
    float getAngleByTouchPoint(CCPoint touchPoint);
    
    void rotateFinished();
    
    void _addRotateCell(int idx, const char*  itemStr);
    void addAnimationRotateCells();
    void _addAnimationRotateCell(float dt);
    
    void _clearRotateCells();
    
    void setRotationAngle(float angle);
    
    void rotatingHandler(float dt);
    
    void doCellSort();
    
    void place(LotteryActCell* obj, map<LotteryActCell*, char> & visited,map<LotteryActCell*, LotteryActCell*> & objsMap,map<LotteryActCell*, vector<LotteryActCell*> > & deps);
    
    bool isChipEnough();
    
    void yesDialogBack(CCObject* pObj);
    
    void playAnimation(int useMoney);
    
    void getCmdData(CCObject* pObj);
    
protected:
    bool m_bDragging;
    
    /**
     * Container holds scroll view contents, Sets the scrollable container object of the scroll view
     */
    CCNode* m_pContainer;
    /**
     * Determiens whether user touch is moved after begin phase.
     */
    bool m_bTouchMoved;
    
    /**
     * Touch point
     */
    CCPoint m_tTouchPoint;
    /**
     * UITouch objects to detect multitouch
     */
    CCArray* m_pTouches;
    
private:
    float m_fEndTime3;
    float m_fTimeGap3;
    int m_fTimeCnt3;
    int m_fTimeIdx3;
    
    float currRotationAngle;
    float startRotationAngle;
    CCPoint m_startTouchPoint;
    long m_startTouchTime;
    long m_lastTouchMoveTime;
    bool m_rotating;
    bool m_AnimCellAdding;
    int m_AnimCellIndex;
    float m_fEndAngle;
    
    float m_tScrollAngel;
    map<int, vector<string>> m_rotatedata;
    float m_rotateAngle;
    int m_depth;
    float m_speed3;
    bool m_hasGetReward;
    float m_ftimeGap4;
    bool m_isSendNotify0;
    bool m_isSendNotify1;
    LotteryActCell* m_targetCell;
    
};

#include "CCAniNode.h"

class LotteryActCell
:public CCBSelectorResolver
,public CCAniNode
,public CCBMemberVariableAssigner
{
public:
    static LotteryActCell* create(string itemId);
    LotteryActCell():m_radians(0.0f),m_itemId(""){};
    virtual ~LotteryActCell(){};
    void setAngle(float DegreeAngle);
    float getAngle(){ return m_radians; }
    CC_SYNTHESIZE_READONLY(string, m_itemId, ItemId);
    
private:
    bool init(string itemId);
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);

    string getIcon();
    void setIconDisplayFrame();
    void setBottomBgFrame();
private:
    CCSafeObject<CCSprite> m_bottomBg;
    CCSafeObject<CCSprite> m_icon;
    
    float m_radians;
};

#endif

