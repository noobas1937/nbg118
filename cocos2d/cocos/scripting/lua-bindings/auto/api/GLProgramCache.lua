
--------------------------------
-- @module GLProgramCache
-- @extend Ref
-- @parent_module cc

--------------------------------
-- 
-- @function [parent=#GLProgramCache] loadDefaultShaders 
-- @param self
-- @return GLProgramCache#GLProgramCache self (return value: cc.GLProgramCache)
        
--------------------------------
--  adds a GLProgram to the cache for a given name 
-- @function [parent=#GLProgramCache] addGLProgram 
-- @param self
-- @param #cc.GLProgram program
-- @param #string key
-- @return GLProgramCache#GLProgramCache self (return value: cc.GLProgramCache)
        
--------------------------------
-- 
-- @function [parent=#GLProgramCache] reloadCustomGLProgram 
-- @param self
-- @param #string key
-- @param #char vShaderByteArray
-- @param #char fShaderByteArray
-- @param #bool isETC
-- @return GLProgram#GLProgram ret (return value: cc.GLProgram)
        
--------------------------------
-- 
-- @function [parent=#GLProgramCache] addProgram 
-- @param self
-- @param #cc.GLProgram program
-- @param #string key
-- @return GLProgramCache#GLProgramCache self (return value: cc.GLProgramCache)
        
--------------------------------
--  reload the default shaders 
-- @function [parent=#GLProgramCache] reloadDefaultGLPrograms 
-- @param self
-- @return GLProgramCache#GLProgramCache self (return value: cc.GLProgramCache)
        
--------------------------------
-- 
-- @function [parent=#GLProgramCache] reloadDefaultShaders 
-- @param self
-- @return GLProgramCache#GLProgramCache self (return value: cc.GLProgramCache)
        
--------------------------------
-- 
-- @function [parent=#GLProgramCache] getProgram 
-- @param self
-- @param #string key
-- @return GLProgram#GLProgram ret (return value: cc.GLProgram)
        
--------------------------------
-- 
-- @function [parent=#GLProgramCache] programForKey 
-- @param self
-- @param #string key
-- @return GLProgram#GLProgram ret (return value: cc.GLProgram)
        
--------------------------------
--  elex 增加自定义shader，如果已有同名，返回复用同名 
-- @function [parent=#GLProgramCache] addCustomGLProgram 
-- @param self
-- @param #string key
-- @param #char vShaderByteArray
-- @param #char fShaderByteArray
-- @param #bool isETC
-- @return GLProgram#GLProgram ret (return value: cc.GLProgram)
        
--------------------------------
--  loads the default shaders 
-- @function [parent=#GLProgramCache] loadDefaultGLPrograms 
-- @param self
-- @return GLProgramCache#GLProgramCache self (return value: cc.GLProgramCache)
        
--------------------------------
--  returns a GL program for a given key 
-- @function [parent=#GLProgramCache] getGLProgram 
-- @param self
-- @param #string key
-- @return GLProgram#GLProgram ret (return value: cc.GLProgram)
        
--------------------------------
--  purges the cache. It releases the retained instance. 
-- @function [parent=#GLProgramCache] destroyInstance 
-- @param self
-- @return GLProgramCache#GLProgramCache self (return value: cc.GLProgramCache)
        
--------------------------------
--  @deprecated Use destroyInstance() instead 
-- @function [parent=#GLProgramCache] purgeSharedShaderCache 
-- @param self
-- @return GLProgramCache#GLProgramCache self (return value: cc.GLProgramCache)
        
--------------------------------
--  returns the shared instance 
-- @function [parent=#GLProgramCache] getInstance 
-- @param self
-- @return GLProgramCache#GLProgramCache ret (return value: cc.GLProgramCache)
        
--------------------------------
--  @deprecated Use getInstance() instead 
-- @function [parent=#GLProgramCache] sharedShaderCache 
-- @param self
-- @return GLProgramCache#GLProgramCache ret (return value: cc.GLProgramCache)
        
--------------------------------
-- Constructor.<br>
-- js ctor
-- @function [parent=#GLProgramCache] GLProgramCache 
-- @param self
-- @return GLProgramCache#GLProgramCache self (return value: cc.GLProgramCache)
        
return nil
