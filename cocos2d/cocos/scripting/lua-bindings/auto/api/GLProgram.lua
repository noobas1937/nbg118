
--------------------------------
-- @module GLProgram
-- @extend Ref
-- @parent_module cc

--------------------------------
--  returns the fragmentShader error log 
-- @function [parent=#GLProgram] getFragmentShaderLog 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- @overload self, char, char, string         
-- @overload self, char, char         
-- @function [parent=#GLProgram] initWithByteArrays
-- @param self
-- @param #char vShaderByteArray
-- @param #char fShaderByteArray
-- @param #string compileTimeDefines
-- @return bool#bool ret (return value: bool)

--------------------------------
-- 
-- @function [parent=#GLProgram] initWithVertexShaderFilename 
-- @param self
-- @param #string vertexFilename
-- @param #string fragFilename
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#GLProgram] addAttribute 
-- @param self
-- @param #string attributeName
-- @param #unsigned int index
-- @return GLProgram#GLProgram self (return value: cc.GLProgram)
        
--------------------------------
-- @overload self, string, string, string         
-- @overload self, string, string         
-- @function [parent=#GLProgram] initWithFilenames
-- @param self
-- @param #string vShaderFilename
-- @param #string fShaderFilename
-- @param #string compileTimeDefines
-- @return bool#bool ret (return value: bool)

--------------------------------
--  it will call glUseProgram() 
-- @function [parent=#GLProgram] use 
-- @param self
-- @return GLProgram#GLProgram self (return value: cc.GLProgram)
        
--------------------------------
--  returns the vertexShader error log 
-- @function [parent=#GLProgram] getVertexShaderLog 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- @overload self, mat4_table         
-- @overload self         
-- @function [parent=#GLProgram] setUniformsForBuiltins
-- @param self
-- @param #mat4_table modelView
-- @return GLProgram#GLProgram self (return value: cc.GLProgram)

--------------------------------
--  It will create 4 uniforms:<br>
-- - kUniformPMatrix<br>
-- - kUniformMVMatrix<br>
-- - kUniformMVPMatrix<br>
-- - GLProgram::UNIFORM_SAMPLER<br>
-- And it will bind "GLProgram::UNIFORM_SAMPLER" to 0
-- @function [parent=#GLProgram] updateUniforms 
-- @param self
-- @return GLProgram#GLProgram self (return value: cc.GLProgram)
        
--------------------------------
--  calls glUniform1i only if the values are different than the previous call for this same shader program.<br>
-- js setUniformLocationI32<br>
-- lua setUniformLocationI32
-- @function [parent=#GLProgram] setUniformLocationWith1i 
-- @param self
-- @param #int location
-- @param #int i1
-- @return GLProgram#GLProgram self (return value: cc.GLProgram)
        
--------------------------------
--  Reload all shaders, this function is designed for android<br>
-- when opengl context lost, so don't call it.
-- @function [parent=#GLProgram] reset 
-- @param self
-- @return GLProgram#GLProgram self (return value: cc.GLProgram)
        
--------------------------------
--   It will add a new attribute to the shader by calling glBindAttribLocation. 
-- @function [parent=#GLProgram] bindAttribLocation 
-- @param self
-- @param #string attributeName
-- @param #unsigned int index
-- @return GLProgram#GLProgram self (return value: cc.GLProgram)
        
--------------------------------
-- 
-- @function [parent=#GLProgram] initWithVertexShaderByteArray 
-- @param self
-- @param #char vertexByteArray
-- @param #char fragByteArray
-- @return bool#bool ret (return value: bool)
        
--------------------------------
--  Calls glGetAttribLocation. 
-- @function [parent=#GLProgram] getAttribLocation 
-- @param self
-- @param #string attributeName
-- @return int#int ret (return value: int)
        
--------------------------------
--  links the glProgram 
-- @function [parent=#GLProgram] link 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @overload self, char, char, string         
-- @overload self, char, char         
-- @function [parent=#GLProgram] createWithByteArrays
-- @param self
-- @param #char vShaderByteArray
-- @param #char fShaderByteArray
-- @param #string compileTimeDefines
-- @return GLProgram#GLProgram ret (return value: cc.GLProgram)

--------------------------------
-- @overload self, string, string, string         
-- @overload self, string, string         
-- @function [parent=#GLProgram] createWithFilenames
-- @param self
-- @param #string vShaderFilename
-- @param #string fShaderFilename
-- @param #string compileTimeDefines
-- @return GLProgram#GLProgram ret (return value: cc.GLProgram)

--------------------------------
-- Constructor.
-- @function [parent=#GLProgram] GLProgram 
-- @param self
-- @return GLProgram#GLProgram self (return value: cc.GLProgram)
        
return nil
