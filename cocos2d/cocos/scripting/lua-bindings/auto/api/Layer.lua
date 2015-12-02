
--------------------------------
-- @module Layer
-- @extend Node
-- @parent_module cc

--------------------------------
-- deprecated Please override onKeyReleased and check the keycode of KeyboardEvent::KeyCode::Menu(KEY_BACKSPACE) instead. <br>
-- js NA
-- @function [parent=#Layer] keyBackClicked 
-- @param self
-- @return Layer#Layer self (return value: cc.Layer)
        
--------------------------------
-- 
-- @function [parent=#Layer] ccTouchBegan 
-- @param self
-- @param #cc.Touch pTouch
-- @param #cc.Event pEvent
-- @return bool#bool ret (return value: bool)
        
--------------------------------
--  whether or not it will receive keyboard or keypad events<br>
-- You can enable / disable accelerometer events with this property.<br>
-- it's new in cocos2d-x<br>
-- js NA
-- @function [parent=#Layer] isKeyboardEnabled 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#Layer] setAccelerometerInterval 
-- @param self
-- @param #double interval
-- @return Layer#Layer self (return value: cc.Layer)
        
--------------------------------
-- 
-- @function [parent=#Layer] ccTouchesCancelled 
-- @param self
-- @param #cc.__Set pTouches
-- @param #cc.Event pEvent
-- @return Layer#Layer self (return value: cc.Layer)
        
--------------------------------
-- 
-- @function [parent=#Layer] ccTouchesMoved 
-- @param self
-- @param #cc.__Set pTouches
-- @param #cc.Event pEvent
-- @return Layer#Layer self (return value: cc.Layer)
        
--------------------------------
-- 
-- @function [parent=#Layer] isSwallowsTouches 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#Layer] getTouchMode 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
--  whether or not it will receive Touch events.<br>
-- You can enable / disable touch events with this property.<br>
-- Only the touches of this node will be affected. This "method" is not propagated to it's children.<br>
-- since v0.8.1<br>
-- js NA
-- @function [parent=#Layer] isTouchEnabled 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#Layer] ccTouchMoved 
-- @param self
-- @param #cc.Touch pTouch
-- @param #cc.Event pEvent
-- @return Layer#Layer self (return value: cc.Layer)
        
--------------------------------
-- 
-- @function [parent=#Layer] setTouchEnabled 
-- @param self
-- @param #bool value
-- @return Layer#Layer self (return value: cc.Layer)
        
--------------------------------
-- 
-- @function [parent=#Layer] setKeyboardEnabled 
-- @param self
-- @param #bool value
-- @return Layer#Layer self (return value: cc.Layer)
        
--------------------------------
-- 
-- @function [parent=#Layer] isKeypadEnabled 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#Layer] ccTouchesEnded 
-- @param self
-- @param #cc.__Set pTouches
-- @param #cc.Event pEvent
-- @return Layer#Layer self (return value: cc.Layer)
        
--------------------------------
-- 
-- @function [parent=#Layer] setTouchMode 
-- @param self
-- @param #int mode
-- @return Layer#Layer self (return value: cc.Layer)
        
--------------------------------
--  whether or not it will receive Accelerometer events<br>
-- You can enable / disable accelerometer events with this property.<br>
-- since v0.8.1<br>
-- js NA
-- @function [parent=#Layer] isAccelerometerEnabled 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#Layer] setKeypadEnabled 
-- @param self
-- @param #bool value
-- @return Layer#Layer self (return value: cc.Layer)
        
--------------------------------
-- 
-- @function [parent=#Layer] ccTouchEnded 
-- @param self
-- @param #cc.Touch pTouch
-- @param #cc.Event pEvent
-- @return Layer#Layer self (return value: cc.Layer)
        
--------------------------------
-- 
-- @function [parent=#Layer] ccTouchCancelled 
-- @param self
-- @param #cc.Touch pTouch
-- @param #cc.Event pEvent
-- @return Layer#Layer self (return value: cc.Layer)
        
--------------------------------
-- swallowsTouches of the touch events. Default is true <br>
-- js NA
-- @function [parent=#Layer] setSwallowsTouches 
-- @param self
-- @param #bool swallowsTouches
-- @return Layer#Layer self (return value: cc.Layer)
        
--------------------------------
-- 
-- @function [parent=#Layer] ccTouchesBegan 
-- @param self
-- @param #cc.__Set pTouches
-- @param #cc.Event pEvent
-- @return Layer#Layer self (return value: cc.Layer)
        
--------------------------------
-- 
-- @function [parent=#Layer] setTouchPriority 
-- @param self
-- @param #int touch
-- @return Layer#Layer self (return value: cc.Layer)
        
--------------------------------
-- 
-- @function [parent=#Layer] setAccelerometerEnabled 
-- @param self
-- @param #bool value
-- @return Layer#Layer self (return value: cc.Layer)
        
--------------------------------
--  If isTouchEnabled, this method is called onEnter. Override it to change the<br>
-- way Layer receives touch events.<br>
-- ( Default: TouchDispatcher::sharedDispatcher()->addStandardDelegate(this,0); )<br>
-- Example:<br>
-- void Layer::registerWithTouchDispatcher()<br>
-- {<br>
-- TouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,INT_MIN+1,true);<br>
-- }<br>
-- since v0.8.0<br>
-- js NA
-- @function [parent=#Layer] registerWithTouchDispatcher 
-- @param self
-- @return Layer#Layer self (return value: cc.Layer)
        
--------------------------------
-- 
-- @function [parent=#Layer] keyMenuClicked 
-- @param self
-- @return Layer#Layer self (return value: cc.Layer)
        
--------------------------------
--  Creates a fullscreen black layer.<br>
-- return An autoreleased Layer object.
-- @function [parent=#Layer] create 
-- @param self
-- @return Layer#Layer ret (return value: cc.Layer)
        
--------------------------------
-- 
-- @function [parent=#Layer] init 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#Layer] getDescription 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- 
-- @function [parent=#Layer] Layer 
-- @param self
-- @return Layer#Layer self (return value: cc.Layer)
        
return nil
