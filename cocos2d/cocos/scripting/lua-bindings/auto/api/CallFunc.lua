
--------------------------------
-- @module CallFunc
-- @extend ActionInstant
-- @parent_module cc

--------------------------------
--  Executes the callback.
-- @function [parent=#CallFunc] execute 
-- @param self
-- @return CallFunc#CallFunc self (return value: cc.CallFunc)
        
--------------------------------
--  initializes the action with the callback<br>
-- typedef void (Ref::*SEL_CallFunc)();<br>
-- deprecated Use the std::function API instead.
-- @function [parent=#CallFunc] initWithTarget 
-- @param self
-- @param #cc.Ref target
-- @return bool#bool ret (return value: bool)
        
--------------------------------
--  Set the selector target.<br>
-- param sel The selector target.
-- @function [parent=#CallFunc] setTargetCallback 
-- @param self
-- @param #cc.Ref sel
-- @return CallFunc#CallFunc self (return value: cc.CallFunc)
        
--------------------------------
-- 
-- @function [parent=#CallFunc] clone 
-- @param self
-- @return CallFunc#CallFunc ret (return value: cc.CallFunc)
        
--------------------------------
-- param time In seconds.
-- @function [parent=#CallFunc] update 
-- @param self
-- @param #float time
-- @return CallFunc#CallFunc self (return value: cc.CallFunc)
        
--------------------------------
-- 
-- @function [parent=#CallFunc] reverse 
-- @param self
-- @return CallFunc#CallFunc ret (return value: cc.CallFunc)
        
--------------------------------
-- 
-- @function [parent=#CallFunc] CallFunc 
-- @param self
-- @return CallFunc#CallFunc self (return value: cc.CallFunc)
        
return nil
