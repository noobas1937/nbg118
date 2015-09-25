//
//  AllianceAreaPopupView.h
//  IF
//
//  Created by wangdianzhen on 15/4/10.
//
//

#ifndef __IF__AllianceAreaPopupView__
#define __IF__AllianceAreaPopupView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
class AllianceAreaPopupView : public PopupBaseView
//, public CCTouchDelegate
{
public:
//    CREATE_FUNC(AllianceAreaPopupView);
    static AllianceAreaPopupView* create(string aareaId,int tileIndex);
    AllianceAreaPopupView(string aareaId,int tileIndex):m_aareaId(aareaId),m_tileIndex(tileIndex){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
private:
    virtual bool onTouchBegan(CCTouch* pTouch,CCEvent* pEvent);
    virtual void onTouchMoved(CCTouch* pTouch,CCEvent* pEvent);
    virtual void onTouchEnded(CCTouch* pTouch,CCEvent* pEvent);
    
    void onCancelBtnClick(CCObject* pSender, CCControlEvent event);
    void onConfirmBtnClick(CCObject* pSender,CCControlEvent event);
    
    void refreshShowNode(bool isDrag = true);
    void reDraw(CCNode* obj);
    CCPoint getMovePoint();
    void enableBtn(bool b);
    bool isCanRelocationTile(CCPoint &pt);
    void doBuy();
    float getMapScale();
    CCPoint getInLayerPoint(CCPoint & tilePoint);
    CCNode * getMapNode();
    bool isInRelicAndNormalBlock(std::vector<CCPoint> &vector);
private:
    CCPoint movingStartPt;
    CCPoint currentCenterPt;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_cityNode;
    CCSafeObject<CCNode> m_showNode;
    CCSafeObject<CCControlButton> m_cancelBtn;
    CCSafeObject<CCControlButton> m_confirmBtn;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_numText;
    CCSafeObject<CCLabelIF> m_btnLabel;
    CCSafeObject<CCLabelIF> m_btnLabel1;
    CCSafeObject<CCSprite> m_numBG;
    bool isMoving;
    string m_aareaId;
    int m_tileIndex;
};

#endif /* defined(__IF__AllianceAreaPopupView__) */
