
--------------------------------
-- @module MenuItemToggle
-- @extend MenuItem
-- @parent_module cc

--------------------------------
--  Sets the array that contains the subitems. 
-- @function [parent=#MenuItemToggle] setSubItems 
-- @param self
-- @param #array_table items
-- @return MenuItemToggle#MenuItemToggle self (return value: cc.MenuItemToggle)
        
--------------------------------
--  Initializes a menu item with a item. 
-- @function [parent=#MenuItemToggle] initWithItem 
-- @param self
-- @param #cc.MenuItem item
-- @return bool#bool ret (return value: bool)
        
--------------------------------
--  Sets the index of the selected item. 
-- @function [parent=#MenuItemToggle] setSelectedIndex 
-- @param self
-- @param #unsigned int index
-- @return MenuItemToggle#MenuItemToggle self (return value: cc.MenuItemToggle)
        
--------------------------------
--  Gets the index of the selected item. 
-- @function [parent=#MenuItemToggle] getSelectedIndex 
-- @param self
-- @return unsigned int#unsigned int ret (return value: unsigned int)
        
--------------------------------
-- deprecated Use `getSelectedItem` instead.
-- @function [parent=#MenuItemToggle] selectedItem 
-- @param self
-- @return MenuItem#MenuItem ret (return value: cc.MenuItem)
        
--------------------------------
--  Return the selected item. 
-- @function [parent=#MenuItemToggle] getSelectedItem 
-- @param self
-- @return MenuItem#MenuItem ret (return value: cc.MenuItem)
        
--------------------------------
--  Add more menu item. 
-- @function [parent=#MenuItemToggle] addSubItem 
-- @param self
-- @param #cc.MenuItem item
-- @return MenuItemToggle#MenuItemToggle self (return value: cc.MenuItemToggle)
        
--------------------------------
-- 
-- @function [parent=#MenuItemToggle] setEnabled 
-- @param self
-- @param #bool var
-- @return MenuItemToggle#MenuItemToggle self (return value: cc.MenuItemToggle)
        
--------------------------------
-- 
-- @function [parent=#MenuItemToggle] activate 
-- @param self
-- @return MenuItemToggle#MenuItemToggle self (return value: cc.MenuItemToggle)
        
--------------------------------
-- 
-- @function [parent=#MenuItemToggle] unselected 
-- @param self
-- @return MenuItemToggle#MenuItemToggle self (return value: cc.MenuItemToggle)
        
--------------------------------
-- 
-- @function [parent=#MenuItemToggle] selected 
-- @param self
-- @return MenuItemToggle#MenuItemToggle self (return value: cc.MenuItemToggle)
        
--------------------------------
-- js ctor
-- @function [parent=#MenuItemToggle] MenuItemToggle 
-- @param self
-- @return MenuItemToggle#MenuItemToggle self (return value: cc.MenuItemToggle)
        
return nil
