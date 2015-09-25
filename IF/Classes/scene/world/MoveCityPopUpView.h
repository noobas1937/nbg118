//
//  MoveCityPopUpView.h
//  IF
//
//  Created by 李锐奇 on 14-8-20.
//
//

#ifndef __IF__MoveCityPopUpView__
#define __IF__MoveCityPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

enum MoveCityType{
    MoveCity_Castle,
    MoveCity_Wood,
    MoveCity_Stone,
    MoveCity_Iron,
    MoveCity_Food,
    MoveCity_Silver,
    MoveCity_Gold,
    MoveCity_Monster,
};


class MoveCityPopUpView : public PopupBaseView
////, public CCTouchDelegate
{
public:
    static MoveCityPopUpView* create(MoveCityType type,int itemid = 0);
    void addMailUid(string mailid);
    void setAlMoveType(int type);
protected:
    virtual int getFreeMoveItemId();
    virtual int getMoveItemId();
    virtual void doWhenHasItem(CCDictionary *dict);
    virtual bool init(MoveCityType type,int itemid = 0);
    virtual void onEnter();
    virtual void onExit();
    virtual void onMoveCityClick(CCObject *pSender, CCControlEvent event);
    void doClick(CCObject* obj);
    void confirmMove();
    virtual void doWhenUseMoney(CCDictionary *dict);
    void onUseTool();
private:
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void createButton();
    void createCitySprite();
    int getTileCount();
    void onCancelClick(CCObject *pSender, CCControlEvent event);
    int getItemNum();
    void refreshShowNode(bool isDrag = true);
    void reDraw(CCNode* obj);
    CCPoint getMovePoint();
    void confirmBuy();
    void enableBtn(bool b);
    bool isCanRelocationTile(CCPoint &pt);
    bool isInRelicAndNormalBlock(std::vector<CCPoint> &vector);
    void useTool();
    bool checkCanUseWinPoint();
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_cityNode;
    CCSafeObject<CCNode> m_showNode;
    CCPoint movingStartPt;
    CCPoint currentCenterPt;
    bool isSendingRequest;
    
    CCSafeObject<CCControlButton> m_cancelBtn;
    CCSafeObject<CCControlButton> m_confirmBtn;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_numText;
    CCSafeObject<CCLabelIF> m_btnLabel;
    CCSafeObject<CCLabelIF> m_btnLabel1;
    bool isMoving;
    std::string m_mailUid;
    
    int m_alMoveType;//0 默认；2 迁往盟主； 3 迁往领地；
    int m_tmpCnt;
    int m_moveCnt;
    MoveCityType mCityType;
    int m_toolID;
};

#endif /* defined(__IF__MoveCityPopUpView__) */
