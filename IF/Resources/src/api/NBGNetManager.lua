
--------------------------------
-- @module NBGNetManager
-- @parent_module nbg

--------------------------------
-- 
-- @function [parent=#NBGNetManager] setIsDebugMode 
-- @param self
-- @param #bool var
-- @return NBGNetManager#NBGNetManager self (return value: NBGNetManager)
        
--------------------------------
-- 
-- @function [parent=#NBGNetManager] getIsDebugMode 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#NBGNetManager] send 
-- @param self
-- @param #string cmd
-- @param #map_table requestData
-- @param #int luaHandlerOnResponse
-- @return NBGNetManager#NBGNetManager self (return value: NBGNetManager)
        
--------------------------------
-- 
-- @function [parent=#NBGNetManager] connect 
-- @param self
-- @param #string ip_addr
-- @param #int port
-- @param #int luaHandlerOnConnection
-- @return NBGNetManager#NBGNetManager self (return value: NBGNetManager)
        
--------------------------------
-- 
-- @function [parent=#NBGNetManager] login 
-- @param self
-- @param #map_table requestData
-- @param #int luaHandlerOnLogin
-- @return NBGNetManager#NBGNetManager self (return value: NBGNetManager)
        
--------------------------------
-- 
-- @function [parent=#NBGNetManager] getInstance 
-- @param self
-- @return NBGNetManager#NBGNetManager ret (return value: NBGNetManager)
        
return nil
