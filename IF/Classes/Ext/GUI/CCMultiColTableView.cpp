//
//  CCMultiColTableView.cpp
//  Sparta
//
//  Created by zhao miechal on 12-10-12.
//  Copyright (c) 2012年 Tech. All rights reserved.
//


#include "cocos2d.h"
#include "CCMultiColTableView.h"

NS_CC_EXT_BEGIN

CCMultiColTableView::CCMultiColTableView()
: m_pMultiTableViewDelegate(NULL)
, m_pMultiTableViewDataSource(NULL)
, m_horizontalOrder(kCCTableViewFillLeftRight)
{}

CCMultiColTableView::~CCMultiColTableView()
{}

CCMultiColTableView* CCMultiColTableView::create(CCMultiColTableViewDataSource* dataSource, CCSize size, CCNode *container)
{
    CCMultiColTableView *table = new CCMultiColTableView();
    table->initWithViewSize(size, container);
    table->autorelease();
    table->setMultiTableViewDataSource(dataSource);
    table->setDataSource(table);
    table->setDelegate(table); 
//    table->_updateContentSize();
    return table;
}

CCSize CCMultiColTableView::cellSizeForTable(CCTableView *table)
{
    CCSize size=m_pMultiTableViewDataSource->gridSizeForTable(this);
    if (_direction == kCCScrollViewDirectionHorizontal)
        size.height*=m_pMultiTableViewDataSource->numberOfGridsInCell(this);
    else
        size.width*=m_pMultiTableViewDataSource->numberOfGridsInCell(this);
    return size;
}

CCTableViewCell* CCMultiColTableView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    ssize_t  numberOfCells  = m_pMultiTableViewDataSource->numberOfCellsInTableView(this);
    ssize_t numberOfGridsInCell = m_pMultiTableViewDataSource->numberOfGridsInCell(this);
    
    
    CCTableViewCell *cell = table->dequeueCell();
    if (!cell){
        cell = new CCTableViewCell();
        cell->autorelease();
    }
    else {
        const auto& children = cell->getChildren();
        for (const auto& child : children)
        {
            CCTableViewCell *grid = dynamic_cast<CCTableViewCell*>(child);
            if (grid)
                m_pGridsFreed.push_back(grid);
        }
        
//        CCObject * pChild;
//        CCARRAY_FOREACH(cell->getChildren(), pChild) {
//            CCTableViewCell *grid = dynamic_cast<CCTableViewCell*>(pChild);
//            if (grid)
//                m_pGridsFreed.push_back(grid);
//        }
        cell->removeAllChildrenWithCleanup(true);
    }
    
    int startIndex = (m_horizontalOrder==kCCTableViewFillLeftRight?idx:(numberOfCells -  idx - 1))*numberOfGridsInCell;
    const CCSize& gridSize=m_pMultiTableViewDataSource->gridSizeForTable(this);
    const CCSize& cellSize=cellSizeForTable(this);
    cell->setContentSize(cellSize);
    for(unsigned int gridIdx=startIndex,colIdx=0;gridIdx < (numberOfGridsInCell+startIndex) ;gridIdx++,colIdx++)
    {
        CCTableViewCell * grid = m_pMultiTableViewDataSource->gridAtIndex(this,gridIdx);
        if(grid!=NULL)
        {
            grid->setIdx(gridIdx);
           // grid->setObjectID(gridIdx);
            grid->setAnchorPoint(CCPointZero);
            if (_direction == kCCScrollViewDirectionHorizontal) {
                if (_vordering == kCCTableViewFillTopDown)
                    grid->setPosition(ccp(0, gridSize.height*(numberOfGridsInCell-colIdx-1)));
                else
                    grid->setPosition(ccp(0, gridSize.height*colIdx));
            }
            else
                grid->setPosition(ccp(gridSize.width*colIdx, 0));
            cell->addChild(grid);
        }
    }
    return cell;
}

ssize_t CCMultiColTableView::numberOfCellsInTableView(CCTableView *table)
{
    return m_pMultiTableViewDataSource->numberOfCellsInTableView(this);
}

void CCMultiColTableView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    if (!m_pMultiTableViewDelegate)
        return;
    auto arr = cell->getChildren();
    for (auto child : arr)
    {
        CCTableViewCell *grid = dynamic_cast<CCTableViewCell*>(child);
        if(grid && isTouchInside(grid,m_pCurTouch))
            m_pMultiTableViewDelegate->gridTouched(this, grid);

    }
//    CCObject * pChild;
//    CCARRAY_FOREACH(cell->getChildren(), pChild) {
//        CCTableViewCell *grid = dynamic_cast<CCTableViewCell*>(pChild);
//        if(grid && cocos2d::extension::isTouchInside(grid,m_pCurTouch))
//            m_pMultiTableViewDelegate->gridTouched(this, grid);
//    }
}

void CCMultiColTableView::scrollViewDidScroll(CCScrollView* view)
{
    setDelegate(getMultiColTableViewDelegate());
    CCTableView::scrollViewDidScroll(view);
    setDelegate(this);
}

void CCMultiColTableView::scrollViewDidZoom(CCScrollView* view)
{
    setDelegate(getMultiColTableViewDelegate());
    CCTableView::scrollViewDidZoom(view);
    setDelegate(this);
}

void CCMultiColTableView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    m_pCurTouch = pTouch;
    CCTableView::onTouchEnded(pTouch, pEvent);
}

CCTableViewCell *CCMultiColTableView::dequeueGrid()
{
    CCTableViewCell *cell;
    
    if (m_pGridsFreed.size() == 0) {
        cell = NULL;
    } else {
        cell = m_pGridsFreed.front();
        cell->retain();
        m_pGridsFreed.pop_front();
        cell->autorelease();
    }
    return cell;
}

NS_CC_EXT_END