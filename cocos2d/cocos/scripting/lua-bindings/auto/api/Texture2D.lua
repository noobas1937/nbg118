
--------------------------------
-- @module Texture2D
-- @extend Ref
-- @parent_module cc

--------------------------------
--  Gets max T. 
-- @function [parent=#Texture2D] getMaxT 
-- @param self
-- @return float#float ret (return value: float)
        
--------------------------------
-- 
-- @function [parent=#Texture2D] stringForFormat 
-- @param self
-- @return char#char ret (return value: char)
        
--------------------------------
--  Returns the pixel format.<br>
-- since v2.0
-- @function [parent=#Texture2D] getStringForFormat 
-- @param self
-- @return char#char ret (return value: char)
        
--------------------------------
-- @overload self, cc.Image, int, string         
-- @overload self, cc.Image, string         
-- @function [parent=#Texture2D] initWithImage
-- @param self
-- @param #cc.Image image
-- @param #int format
-- @param #string path
-- @return bool#bool ret (return value: bool)

--------------------------------
--  Gets max S 
-- @function [parent=#Texture2D] getMaxS 
-- @param self
-- @return float#float ret (return value: float)
        
--------------------------------
--  Release only the gl texture.<br>
-- js NA<br>
-- lua NA
-- @function [parent=#Texture2D] releaseGLTexture 
-- @param self
-- @return Texture2D#Texture2D self (return value: cc.Texture2D)
        
--------------------------------
--  Whether or not the texture has their Alpha premultiplied. 
-- @function [parent=#Texture2D] hasPremultipliedAlpha 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
--  Gets the height of the texture in pixels. 
-- @function [parent=#Texture2D] getPixelsHigh 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @overload self, int         
-- @overload self         
-- @function [parent=#Texture2D] bitsPerPixelForFormat
-- @param self
-- @param #int format
-- @return unsigned int#unsigned int ret (return value: unsigned int)

--------------------------------
-- @overload self, int         
-- @overload self         
-- @function [parent=#Texture2D] getBitsPerPixelForFormat
-- @param self
-- @param #int format
-- @return unsigned int#unsigned int ret (return value: unsigned int)

--------------------------------
--  Gets the texture name. 
-- @function [parent=#Texture2D] getName 
-- @param self
-- @return unsigned int#unsigned int ret (return value: unsigned int)
        
--------------------------------
-- @overload self, char, cc.FontDefinition         
-- @overload self, char, string, float, size_table, int, int         
-- @function [parent=#Texture2D] initWithString
-- @param self
-- @param #char text
-- @param #string fontName
-- @param #float fontSize
-- @param #size_table dimensions
-- @param #int hAlignment
-- @param #int vAlignment
-- @return bool#bool ret (return value: bool)

--------------------------------
--  Sets max T. 
-- @function [parent=#Texture2D] setMaxT 
-- @param self
-- @param #float maxT
-- @return Texture2D#Texture2D self (return value: cc.Texture2D)
        
--------------------------------
--  Draws a texture inside a rect.
-- @function [parent=#Texture2D] drawInRect 
-- @param self
-- @param #rect_table rect
-- @return Texture2D#Texture2D self (return value: cc.Texture2D)
        
--------------------------------
--  Get the texture content size.
-- @function [parent=#Texture2D] getContentSize 
-- @param self
-- @return size_table#size_table ret (return value: size_table)
        
--------------------------------
-- 
-- @function [parent=#Texture2D] getAlphaName 
-- @param self
-- @return unsigned int#unsigned int ret (return value: unsigned int)
        
--------------------------------
--  Sets alias texture parameters:<br>
-- - GL_TEXTURE_MIN_FILTER = GL_NEAREST<br>
-- - GL_TEXTURE_MAG_FILTER = GL_NEAREST<br>
-- warning Calling this method could allocate additional texture memory.<br>
-- since v0.8
-- @function [parent=#Texture2D] setAliasTexParameters 
-- @param self
-- @return Texture2D#Texture2D self (return value: cc.Texture2D)
        
--------------------------------
--  Sets antialias texture parameters:<br>
-- - GL_TEXTURE_MIN_FILTER = GL_LINEAR<br>
-- - GL_TEXTURE_MAG_FILTER = GL_LINEAR<br>
-- warning Calling this method could allocate additional texture memory.<br>
-- since v0.8
-- @function [parent=#Texture2D] setAntiAliasTexParameters 
-- @param self
-- @return Texture2D#Texture2D self (return value: cc.Texture2D)
        
--------------------------------
--  Generates mipmap images for the texture.<br>
-- It only works if the texture size is POT (power of 2).<br>
-- since v0.99.0
-- @function [parent=#Texture2D] generateMipmap 
-- @param self
-- @return Texture2D#Texture2D self (return value: cc.Texture2D)
        
--------------------------------
-- Get texutre name, dimensions and coordinates message by a string.<br>
-- js NA<br>
-- lua NA
-- @function [parent=#Texture2D] getDescription 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
--  Gets the pixel format of the texture. 
-- @function [parent=#Texture2D] getPixelFormat 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
--  Set a shader program to the texture.<br>
-- It's used by drawAtPoint and drawInRect
-- @function [parent=#Texture2D] setGLProgram 
-- @param self
-- @param #cc.GLProgram program
-- @return Texture2D#Texture2D self (return value: cc.Texture2D)
        
--------------------------------
-- 
-- @function [parent=#Texture2D] setTextureName 
-- @param self
-- @param #char file
-- @return Texture2D#Texture2D self (return value: cc.Texture2D)
        
--------------------------------
--  Get content size. 
-- @function [parent=#Texture2D] getContentSizeInPixels 
-- @param self
-- @return size_table#size_table ret (return value: size_table)
        
--------------------------------
--  Gets the width of the texture in pixels. 
-- @function [parent=#Texture2D] getPixelsWide 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- Drawing extensions to make it easy to draw basic quads using a Texture2D object.<br>
-- These functions require GL_TEXTURE_2D and both GL_VERTEX_ARRAY and GL_TEXTURE_COORD_ARRAY client states to be enabled.<br>
-- Draws a texture at a given point. 
-- @function [parent=#Texture2D] drawAtPoint 
-- @param self
-- @param #vec2_table point
-- @return Texture2D#Texture2D self (return value: cc.Texture2D)
        
--------------------------------
--  Get a shader program from the texture.
-- @function [parent=#Texture2D] getGLProgram 
-- @param self
-- @return GLProgram#GLProgram ret (return value: cc.GLProgram)
        
--------------------------------
-- 
-- @function [parent=#Texture2D] getFileName 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
--  Whether or not the texture has mip maps.
-- @function [parent=#Texture2D] hasMipmaps 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
--  Sets max S. 
-- @function [parent=#Texture2D] setMaxS 
-- @param self
-- @param #float maxS
-- @return Texture2D#Texture2D self (return value: cc.Texture2D)
        
--------------------------------
-- 
-- @function [parent=#Texture2D] useDownloadResource 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#Texture2D] isSmallTexture 
-- @param self
-- @param #string textureFile
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#Texture2D] setDownloadFlag 
-- @param self
-- @param #bool down
-- @return Texture2D#Texture2D self (return value: cc.Texture2D)
        
--------------------------------
-- 
-- @function [parent=#Texture2D] setDefaultAlphaPixelFormat 
-- @param self
-- @param #int format
-- @return Texture2D#Texture2D self (return value: cc.Texture2D)
        
--------------------------------
--  Returns the alpha pixel format.<br>
-- since v0.8
-- @function [parent=#Texture2D] getDefaultAlphaPixelFormat 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
--  Treats (or not) PVR files as if they have alpha premultiplied.<br>
-- param haveAlphaPremultiplied <br>
-- Since it is impossible to know at runtime if the PVR images have the alpha channel premultiplied, it is<br>
-- possible load them as if they have (or not) the alpha channel premultiplied.<br>
-- By default it is disabled.<br>
-- deprecated, please use Image::setPVRImagesHavePremultipliedAlpha() instead.<br>
-- since v0.99.5
-- @function [parent=#Texture2D] PVRImagesHavePremultipliedAlpha 
-- @param self
-- @param #bool haveAlphaPremultiplied
-- @return Texture2D#Texture2D self (return value: cc.Texture2D)
        
--------------------------------
-- 
-- @function [parent=#Texture2D] defaultAlphaPixelFormat 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
--  sets the default pixel format for UIImagescontains alpha channel.<br>
-- param format<br>
-- If the UIImage contains alpha channel, then the options are:<br>
-- - generate 32-bit textures: Texture2D::PixelFormat::RGBA8888 (default one)<br>
-- - generate 24-bit textures: Texture2D::PixelFormat::RGB888<br>
-- - generate 16-bit textures: Texture2D::PixelFormat::RGBA4444<br>
-- - generate 16-bit textures: Texture2D::PixelFormat::RGB5A1<br>
-- - generate 16-bit textures: Texture2D::PixelFormat::RGB565<br>
-- - generate 8-bit textures: Texture2D::PixelFormat::A8 (only use it if you use just 1 color)<br>
-- How does it work ?<br>
-- - If the image is an RGBA (with Alpha) then the default pixel format will be used (it can be a 8-bit, 16-bit or 32-bit texture)<br>
-- - If the image is an RGB (without Alpha) then: If the default pixel format is RGBA8888 then a RGBA8888 (32-bit) will be used. Otherwise a RGB565 (16-bit texture) will be used.<br>
-- This parameter is not valid for PVR / PVR.CCZ images.<br>
-- since v0.8
-- @function [parent=#Texture2D] setSmallFlag 
-- @param self
-- @param #bool small
-- @return Texture2D#Texture2D self (return value: cc.Texture2D)
        
--------------------------------
-- js ctor
-- @function [parent=#Texture2D] Texture2D 
-- @param self
-- @return Texture2D#Texture2D self (return value: cc.Texture2D)
        
return nil
