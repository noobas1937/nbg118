//
//  ScrollTableView.h
//  IF
//
//  Created by ganxiaohua on 14-5-14.
//
//

#ifndef __IF__ScrollTableView__
#define __IF__ScrollTableView__

#include "CommonInclude.h"

class  ScrollTableView: public CCScrollView
{
public:
    ~ScrollTableView();
    ScrollTableView();
    static ScrollTableView* create(CCSize size, int cellW, int num);
    virtual void setContentOffset(CCPoint offset, bool animated = false);
    virtual CCPoint minContainerOffset();
    virtual CCPoint maxContainerOffset();
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual bool onTouchBegan(CCTouch* touch, CCEvent* event);
    void scrollFinish(CCNode* p = NULL);
    void scrollAniEnd(CCNode* param);
    void scrollingPos(float dt);
    long getNowTime();
    void setScrollPostion();
    void moveOneCell(bool up=false);
    CCPoint initPos;
    int minX;
    CCNode* touchArea;
    CCNode* clickArea;
    int gapX;
    CCPoint m_endPoint;
    CCPoint worldPoint;
    CCPoint m_startPoint;
    bool m_moveUp;
    int m_index;
    int m_arrIndex;
    bool m_aniStatus;//没有动，true, 动，或点击了都是false
private:
    int m_cellW;
    int m_num;
    unsigned long  m_clickTime;
    unsigned long  m_tTouchTime;
    bool m_canClickMove;
    CCPoint m_touchBeganPoint;
    
};

#endif /* defined(__IF__ScrollTableView__) */
