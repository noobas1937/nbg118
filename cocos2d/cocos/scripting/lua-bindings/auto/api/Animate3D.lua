
--------------------------------
-- @module Animate3D
-- @extend ActionInterval
-- @parent_module cc

--------------------------------
-- 
-- @function [parent=#Animate3D] setPlayBack 
-- @param self
-- @param #bool reverse
-- @return Animate3D#Animate3D self (return value: cc.Animate3D)
        
--------------------------------
-- 
-- @function [parent=#Animate3D] setKeyFrameUserInfo 
-- @param self
-- @param #int keyFrame
-- @param #map_table userInfo
-- @return Animate3D#Animate3D self (return value: cc.Animate3D)
        
--------------------------------
-- get & set speed, negative speed means playing reverse 
-- @function [parent=#Animate3D] getSpeed 
-- @param self
-- @return float#float ret (return value: float)
        
--------------------------------
-- set animate quality
-- @function [parent=#Animate3D] setQuality 
-- @param self
-- @param #int quality
-- @return Animate3D#Animate3D self (return value: cc.Animate3D)
        
--------------------------------
-- 
-- @function [parent=#Animate3D] setWeight 
-- @param self
-- @param #float weight
-- @return Animate3D#Animate3D self (return value: cc.Animate3D)
        
--------------------------------
-- 
-- @function [parent=#Animate3D] removeFromMap 
-- @param self
-- @return Animate3D#Animate3D self (return value: cc.Animate3D)
        
--------------------------------
-- get & set play reverse, these are deprecated, use set negative speed instead
-- @function [parent=#Animate3D] getPlayBack 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#Animate3D] initWithFrames 
-- @param self
-- @param #cc.Animation3D animation
-- @param #int startFrame
-- @param #int endFrame
-- @param #float frameRate
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#Animate3D] getOriginInterval 
-- @param self
-- @return float#float ret (return value: float)
        
--------------------------------
-- 
-- @function [parent=#Animate3D] setSpeed 
-- @param self
-- @param #float speed
-- @return Animate3D#Animate3D self (return value: cc.Animate3D)
        
--------------------------------
-- @overload self, cc.Animation3D, float, float         
-- @overload self, cc.Animation3D         
-- @function [parent=#Animate3D] init
-- @param self
-- @param #cc.Animation3D animation
-- @param #float fromTime
-- @param #float duration
-- @return bool#bool ret (return value: bool)

--------------------------------
-- get & set origin interval
-- @function [parent=#Animate3D] setOriginInterval 
-- @param self
-- @param #float interval
-- @return Animate3D#Animate3D self (return value: cc.Animate3D)
        
--------------------------------
-- get & set blend weight, weight must positive
-- @function [parent=#Animate3D] getWeight 
-- @param self
-- @return float#float ret (return value: float)
        
--------------------------------
-- get animate quality
-- @function [parent=#Animate3D] getQuality 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @overload self, cc.Animation3D, float, float         
-- @overload self, cc.Animation3D         
-- @function [parent=#Animate3D] create
-- @param self
-- @param #cc.Animation3D animation
-- @param #float fromTime
-- @param #float duration
-- @return Animate3D#Animate3D ret (return value: cc.Animate3D)

--------------------------------
--  get animate transition time between 3d animations 
-- @function [parent=#Animate3D] getTransitionTime 
-- @param self
-- @return float#float ret (return value: float)
        
--------------------------------
-- create Animate3D by frame section, [startFrame, endFrame)<br>
-- param animation used to generate animate3D<br>
-- param startFrame<br>
-- param endFrame<br>
-- param frameRate default is 30 per second<br>
-- return Animate3D created using animate
-- @function [parent=#Animate3D] createWithFrames 
-- @param self
-- @param #cc.Animation3D animation
-- @param #int startFrame
-- @param #int endFrame
-- @param #float frameRate
-- @return Animate3D#Animate3D ret (return value: cc.Animate3D)
        
--------------------------------
--  set animate transition time between 3d animations 
-- @function [parent=#Animate3D] setTransitionTime 
-- @param self
-- @param #float transTime
-- @return Animate3D#Animate3D self (return value: cc.Animate3D)
        
--------------------------------
-- 
-- @function [parent=#Animate3D] startWithTarget 
-- @param self
-- @param #cc.Node target
-- @return Animate3D#Animate3D self (return value: cc.Animate3D)
        
--------------------------------
-- 
-- @function [parent=#Animate3D] reverse 
-- @param self
-- @return Animate3D#Animate3D ret (return value: cc.Animate3D)
        
--------------------------------
-- 
-- @function [parent=#Animate3D] clone 
-- @param self
-- @return Animate3D#Animate3D ret (return value: cc.Animate3D)
        
--------------------------------
-- 
-- @function [parent=#Animate3D] stop 
-- @param self
-- @return Animate3D#Animate3D self (return value: cc.Animate3D)
        
--------------------------------
-- 
-- @function [parent=#Animate3D] update 
-- @param self
-- @param #float t
-- @return Animate3D#Animate3D self (return value: cc.Animate3D)
        
--------------------------------
-- 
-- @function [parent=#Animate3D] step 
-- @param self
-- @param #float dt
-- @return Animate3D#Animate3D self (return value: cc.Animate3D)
        
--------------------------------
-- 
-- @function [parent=#Animate3D] Animate3D 
-- @param self
-- @return Animate3D#Animate3D self (return value: cc.Animate3D)
        
return nil
