//
//  SceneContainer.h
//  IF
//
//  Created by 邹 程 on 13-8-29.
//
//

#ifndef __IF__SceneContainer__
#define __IF__SceneContainer__

#include "SceneController.h"

class SceneContainer : public CCScene //,public CCKeypadDelegate
{
public:
    CREATE_FUNC(SceneContainer)
    friend class SceneController;
    virtual void keyBackClicked();
    void onExit();
    void onEnter();
    void setKeyboardEnabled(bool enabled);
private:
    bool init();
    bool _keyboardEnabled;
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* unused_event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* unused_event);
    EventListenerKeyboard* _keyboardListener;
};

#endif /* defined(__IF__SceneContainer__) */
