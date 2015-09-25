//
//  CCTableViewTouchIFCell.h
//  IF
//
//  Created by 童悦 on 15/7/2.
//
//

#ifndef __IF__CCTableViewTouchIFCell__
#define __IF__CCTableViewTouchIFCell__
#include "cocosextV3.h"
NS_CC_EXT_BEGIN

class CCTableViewTouchIFCell :public TableViewCell
{
public:
    CCTableViewTouchIFCell();
    
    void setTouchEnabled(bool enabled);
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    void setSwallowsTouches(bool swallowsTouches);
private:
    bool _touchEnabled;
    EventListener* _touchListener;
    bool    _swallowsTouches;

};
NS_CC_EXT_END
#endif /* defined(__IF__CCTableViewTouchIFCell__) */
