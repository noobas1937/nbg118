
--------------------------------
-- @module MenuItemFont
-- @extend MenuItemLabel
-- @parent_module cc

--------------------------------
--  Initializes a menu item from a string with a target/selector. 
-- @function [parent=#MenuItemFont] initWithString 
-- @param self
-- @param #string value
-- @param #function callback
-- @return bool#bool ret (return value: bool)
        
--------------------------------
--  Returns the name of the Font.<br>
-- js getFontNameObj<br>
-- js NA
-- @function [parent=#MenuItemFont] getFontNameObj 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
--  get font size .<br>
-- js getFontSize<br>
-- js NA
-- @function [parent=#MenuItemFont] getFontSizeObj 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
--  Deprecated Use getFontNameObj() instead.<br>
-- js NA
-- @function [parent=#MenuItemFont] fontNameObj 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- Set the font name .<br>
-- c++ can not overload static and non-static member functions with the same parameter types.<br>
-- so change the name to setFontNameObj.<br>
-- js setFontName<br>
-- js NA
-- @function [parent=#MenuItemFont] setFontNameObj 
-- @param self
-- @param #string name
-- @return MenuItemFont#MenuItemFont self (return value: cc.MenuItemFont)
        
--------------------------------
-- 
-- @function [parent=#MenuItemFont] fontSizeObj 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
--  Set font size.<br>
-- c++ can not overload static and non-static member functions with the same parameter types.<br>
-- so change the name to setFontSizeObj.<br>
-- js setFontSize<br>
-- js NA
-- @function [parent=#MenuItemFont] setFontSizeObj 
-- @param self
-- @param #int size
-- @return MenuItemFont#MenuItemFont self (return value: cc.MenuItemFont)
        
--------------------------------
--  Set the default font name. 
-- @function [parent=#MenuItemFont] setFontName 
-- @param self
-- @param #string name
-- @return MenuItemFont#MenuItemFont self (return value: cc.MenuItemFont)
        
--------------------------------
-- 
-- @function [parent=#MenuItemFont] fontName 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
--  Get default font size. 
-- @function [parent=#MenuItemFont] getFontSize 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#MenuItemFont] fontSize 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
--  Set default font size. 
-- @function [parent=#MenuItemFont] setFontSize 
-- @param self
-- @param #int size
-- @return MenuItemFont#MenuItemFont self (return value: cc.MenuItemFont)
        
--------------------------------
--  Get the default font name. 
-- @function [parent=#MenuItemFont] getFontName 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- js ctor
-- @function [parent=#MenuItemFont] MenuItemFont 
-- @param self
-- @return MenuItemFont#MenuItemFont self (return value: cc.MenuItemFont)
        
return nil
