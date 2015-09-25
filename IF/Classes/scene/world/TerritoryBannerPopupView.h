//
//  TerritoryBannerPopupView.h
//  IF
//
//  Created by 〜TIAN~ on 15/8/19.
//
//

#ifndef __IF__TerritoryBannerPopupView__
#define __IF__TerritoryBannerPopupView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
class TerritoryBannerPopupView : public PopupBaseView{
public:
    //    CREATE_FUNC(IFAddTowerPopup);
    static TerritoryBannerPopupView* create(int tileIndex, std::string banner);
    TerritoryBannerPopupView(int tileIndex, std::string banner):m_tileIndex(tileIndex),m_banner(banner){};
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
    CCSafeObject<CCNode> m_showNode;
    CCSafeObject<CCNode> m_batchNode;
    CCSafeObject<CCControlButton> m_cancelBtn;
    CCSafeObject<CCControlButton> m_confirmBtn;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_numText;
    CCSafeObject<CCLabelIF> m_btnLabel;
    CCSafeObject<CCLabelIF> m_btnLabel1;
    CCSafeObject<CCSprite> m_numBG;
    bool isMoving;
    string m_uid;
    int m_tileIndex;
    string m_banner;
    CCSafeObject<CCSpriteBatchNode> m_parentBatchNode;
};

#endif /* defined(__IF__TerritoryBannerPopupView__) */
