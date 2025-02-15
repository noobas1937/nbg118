
--------------------------------
-- @module AnimationCache
-- @extend Ref
-- @parent_module cc

--------------------------------
--  Returns a Animation that was previously added.<br>
-- If the name is not found it will return nil.<br>
-- You should retain the returned copy if you are going to use it.<br>
-- return A Animation that was previously added. If the name is not found it will return nil.
-- @function [parent=#AnimationCache] getAnimation 
-- @param self
-- @param #string name
-- @return Animation#Animation ret (return value: cc.Animation)
        
--------------------------------
--  Adds a Animation with a name.<br>
-- param animation An animation.<br>
-- param name The name of animation.
-- @function [parent=#AnimationCache] addAnimation 
-- @param self
-- @param #cc.Animation animation
-- @param #string name
-- @return AnimationCache#AnimationCache self (return value: cc.AnimationCache)
        
--------------------------------
--  Adds an animation from an NSDictionary.<br>
-- Make sure that the frames were previously loaded in the SpriteFrameCache.<br>
-- param dictionary An NSDictionary.<br>
-- param plist The path of the relative file,it use to find the plist path for load SpriteFrames.<br>
-- since v1.1<br>
-- js NA
-- @function [parent=#AnimationCache] addAnimationsWithDictionary 
-- @param self
-- @param #map_table dictionary
-- @param #string plist
-- @return AnimationCache#AnimationCache self (return value: cc.AnimationCache)
        
--------------------------------
--  @deprecated. Use removeAnimation() instead<br>
-- js NA<br>
-- lua NA
-- @function [parent=#AnimationCache] removeAnimationByName 
-- @param self
-- @param #string name
-- @return AnimationCache#AnimationCache self (return value: cc.AnimationCache)
        
--------------------------------
-- 
-- @function [parent=#AnimationCache] init 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- deprecated. Use getAnimation() instead<br>
-- js NA<br>
-- lua NA
-- @function [parent=#AnimationCache] animationByName 
-- @param self
-- @param #string name
-- @return Animation#Animation ret (return value: cc.Animation)
        
--------------------------------
--  Deletes a Animation from the cache.<br>
-- param name The name of animation.
-- @function [parent=#AnimationCache] removeAnimation 
-- @param self
-- @param #string name
-- @return AnimationCache#AnimationCache self (return value: cc.AnimationCache)
        
--------------------------------
--  Adds an animation from a plist file.<br>
-- Make sure that the frames were previously loaded in the SpriteFrameCache.<br>
-- since v1.1<br>
-- js addAnimations<br>
-- lua addAnimations<br>
-- param plist An animation from a plist file.
-- @function [parent=#AnimationCache] addAnimationsWithFile 
-- @param self
-- @param #string plist
-- @return AnimationCache#AnimationCache self (return value: cc.AnimationCache)
        
--------------------------------
--  Purges the cache. It releases all the Animation objects and the shared instance.<br>
-- js NA
-- @function [parent=#AnimationCache] destroyInstance 
-- @param self
-- @return AnimationCache#AnimationCache self (return value: cc.AnimationCache)
        
--------------------------------
--  @deprecated Use getInstance() instead. 
-- @function [parent=#AnimationCache] sharedAnimationCache 
-- @param self
-- @return AnimationCache#AnimationCache ret (return value: cc.AnimationCache)
        
--------------------------------
--  @deprecated Use destroyInstance() instead. 
-- @function [parent=#AnimationCache] purgeSharedAnimationCache 
-- @param self
-- @return AnimationCache#AnimationCache self (return value: cc.AnimationCache)
        
--------------------------------
--  Returns the shared instance of the Animation cache <br>
-- js NA
-- @function [parent=#AnimationCache] getInstance 
-- @param self
-- @return AnimationCache#AnimationCache ret (return value: cc.AnimationCache)
        
--------------------------------
-- js ctor
-- @function [parent=#AnimationCache] AnimationCache 
-- @param self
-- @return AnimationCache#AnimationCache self (return value: cc.AnimationCache)
        
return nil
