//
//  DefaultTableViewDelegate.h
//  IF
//
//  Created by chenliang on 14-3-26.
//
//

#ifndef __IF__DefaultTableViewDelegate__
#define __IF__DefaultTableViewDelegate__

#include "CommonInclude.h"

class DefaultTableViewDelegate : public CCTableViewDelegate
{
public:
    virtual ~DefaultTableViewDelegate() {}
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
};

#endif /* defined(__IF__DefaultTableViewDelegate__) */
