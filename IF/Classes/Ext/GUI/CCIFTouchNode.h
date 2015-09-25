//
//  CCIFTouchNode.h
//  IF
//
//  Created by 童悦 on 15/8/4.
//
//

#ifndef __IF__CCIFTouchNode__
#define __IF__CCIFTouchNode__

#include "cocosextV3.h"
NS_CC_EXT_BEGIN

class CCIFTouchNode :public Node
{
public:
    CCIFTouchNode();
    
    void setTouchEnabled(bool enabled);
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onRetainTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
    void setSwallowsTouches(bool swallowsTouches);
    void setTouchMode(Touch::DispatchMode mode)
    {
        
    }
private:
    bool _touchEnabled;
    EventListener* _touchListener;
    bool    _swallowsTouches;
    
};
NS_CC_EXT_END
#endif  /* defined(__IF__CCIFTouchNode__) */
