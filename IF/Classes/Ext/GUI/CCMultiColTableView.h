//
//  CCMultiColTableView.h
//  Sparta
//
//  Created by zhao miechal on 12-10-12.
//  Copyright (c) 2012年 Tech. All rights reserved.
//

#ifndef __CCMULTICOLTABLEVIEW_H__
#define __CCMULTICOLTABLEVIEW_H__

#include "CCBExtension.h"
#include <deque>
//#include "CCTableView.h"

NS_CC_EXT_BEGIN

typedef enum {
    kCCTableViewFillLeftRight,
    kCCTableViewFillRightLeft
} CCTableViewHorizontalFillOrder;

class CCMultiColTableView;

class CCMultiColTableViewDataSource
{
public:
    /**
     * a Grid instance at a given index
     *
     * @param idx index to search for a grid
     * @return grid found at idx
     */
    virtual CCTableViewCell* gridAtIndex(CCMultiColTableView *multiTable, unsigned int idx) = 0;
    /**
     * Returns number of cells in a given table view.
     *
     * @return number of cells
     */
    virtual ssize_t numberOfCellsInTableView(CCMultiColTableView *multiTable) = 0;
    /**
     * Returns number of grids in a given cell
     *
     * @return number of grids
     */
    virtual unsigned int numberOfGridsInCell(CCMultiColTableView *multiTable)=0;
    /**
     * rows height for a given table.
     *
     * @param table table to hold the instances of Class
     * @return row size
     */
    virtual CCSize gridSizeForTable(CCMultiColTableView *table) = 0;
};

class CCMultiColTableViewDelegate :public CCTableViewDelegate
{
public:
    /**
     * Delegate to respond touch event
     *
     * @param mutilTable  contains the given grid
     * @param grid that is touched
     */
    virtual void gridTouched(CCMultiColTableView* table, CCTableViewCell* grid) = 0;
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell){}
};

class CCMultiColTableView : public CCTableView, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    ~CCMultiColTableView();
    CCMultiColTableView();
    static CCMultiColTableView* create(CCMultiColTableViewDataSource* dataSource, CCSize size, CCNode *container=NULL);
    
    CCTableViewCell *dequeueGrid();
    
    CCTouch *m_pCurTouch; // make it visible for hack use
protected:
    CCTableViewCell *dequeueCell() {return CCTableView::dequeueCell();}
    // tableview datasource
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    // tableview delegate
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell) ;
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view);
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view);
    
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
//    CCTouch *m_pCurTouch;
    std::deque<CCSafeObject<CCTableViewCell> > m_pGridsFreed;
    
    CC_SYNTHESIZE(CCMultiColTableViewDataSource*, m_pMultiTableViewDataSource, MultiTableViewDataSource);
    CC_SYNTHESIZE(CCMultiColTableViewDelegate*, m_pMultiTableViewDelegate, MultiColTableViewDelegate);
    CC_SYNTHESIZE(CCTableViewHorizontalFillOrder, m_horizontalOrder, HorizontalFillOrder);
    
private:
    
};

NS_CC_EXT_END
#endif