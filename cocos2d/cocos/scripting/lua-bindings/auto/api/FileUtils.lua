
--------------------------------
-- @module FileUtils
-- @parent_module cc

--------------------------------
--  Returns the fullpath for a given filename.<br>
-- First it will try to get a new filename from the "filenameLookup" dictionary.<br>
-- If a new filename can't be found on the dictionary, it will use the original filename.<br>
-- Then it will try to obtain the full path of the filename using the FileUtils search rules: resolutions, and search paths.<br>
-- The file search is based on the array element order of search paths and resolution directories.<br>
-- For instance:<br>
-- We set two elements("/mnt/sdcard/", "internal_dir/") to search paths vector by setSearchPaths,<br>
-- and set three elements("resources-ipadhd/", "resources-ipad/", "resources-iphonehd")<br>
-- to resolutions vector by setSearchResolutionsOrder. The "internal_dir" is relative to "Resources/".<br>
-- If we have a file named 'sprite.png', the mapping in fileLookup dictionary contains `key: sprite.png -> value: sprite.pvr.gz`.<br>
-- Firstly, it will replace 'sprite.png' with 'sprite.pvr.gz', then searching the file sprite.pvr.gz as follows:<br>
-- /mnt/sdcard/resources-ipadhd/sprite.pvr.gz      (if not found, search next)<br>
-- /mnt/sdcard/resources-ipad/sprite.pvr.gz        (if not found, search next)<br>
-- /mnt/sdcard/resources-iphonehd/sprite.pvr.gz    (if not found, search next)<br>
-- /mnt/sdcard/sprite.pvr.gz                       (if not found, search next)<br>
-- internal_dir/resources-ipadhd/sprite.pvr.gz     (if not found, search next)<br>
-- internal_dir/resources-ipad/sprite.pvr.gz       (if not found, search next)<br>
-- internal_dir/resources-iphonehd/sprite.pvr.gz   (if not found, search next)<br>
-- internal_dir/sprite.pvr.gz                      (if not found, return "sprite.png")<br>
-- If the filename contains relative path like "gamescene/uilayer/sprite.png",<br>
-- and the mapping in fileLookup dictionary contains `key: gamescene/uilayer/sprite.png -> value: gamescene/uilayer/sprite.pvr.gz`.<br>
-- The file search order will be:<br>
-- /mnt/sdcard/gamescene/uilayer/resources-ipadhd/sprite.pvr.gz      (if not found, search next)<br>
-- /mnt/sdcard/gamescene/uilayer/resources-ipad/sprite.pvr.gz        (if not found, search next)<br>
-- /mnt/sdcard/gamescene/uilayer/resources-iphonehd/sprite.pvr.gz    (if not found, search next)<br>
-- /mnt/sdcard/gamescene/uilayer/sprite.pvr.gz                       (if not found, search next)<br>
-- internal_dir/gamescene/uilayer/resources-ipadhd/sprite.pvr.gz     (if not found, search next)<br>
-- internal_dir/gamescene/uilayer/resources-ipad/sprite.pvr.gz       (if not found, search next)<br>
-- internal_dir/gamescene/uilayer/resources-iphonehd/sprite.pvr.gz   (if not found, search next)<br>
-- internal_dir/gamescene/uilayer/sprite.pvr.gz                      (if not found, return "gamescene/uilayer/sprite.png")<br>
-- If the new file can't be found on the file system, it will return the parameter filename directly.<br>
-- This method was added to simplify multiplatform support. Whether you are using cocos2d-js or any cross-compilation toolchain like StellaSDK or Apportable,<br>
-- you might need to load different resources for a given file in the different platforms.<br>
-- since v2.1
-- @function [parent=#FileUtils] fullPathForFilename 
-- @param self
-- @param #string filename
-- @return string#string ret (return value: string)
        
--------------------------------
-- Gets string from a file.
-- @function [parent=#FileUtils] getStringFromFile 
-- @param self
-- @param #string filename
-- @return string#string ret (return value: string)
        
--------------------------------
-- Sets the filenameLookup dictionary.<br>
-- param pFilenameLookupDict The dictionary for replacing filename.<br>
-- since v2.1
-- @function [parent=#FileUtils] setFilenameLookupDictionary 
-- @param self
-- @param #map_table filenameLookupDict
-- @return FileUtils#FileUtils self (return value: cc.FileUtils)
        
--------------------------------
-- Removes a file.<br>
-- param filepath The full path of the file, it must be an absolute path.<br>
-- return True if the file have been removed successfully, false if not.
-- @function [parent=#FileUtils] removeFile 
-- @param self
-- @param #string filepath
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- Checks whether the path is an absolute path.<br>
-- note On Android, if the parameter passed in is relative to "assets/", this method will treat it as an absolute path.<br>
-- Also on Blackberry, path starts with "app/native/Resources/" is treated as an absolute path.<br>
-- param path The path that needs to be checked.<br>
-- return True if it's an absolute path, false if not.
-- @function [parent=#FileUtils] isAbsolutePath 
-- @param self
-- @param #string path
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- Renames a file under the given directory.<br>
-- param path     The parent directory path of the file, it must be an absolute path.<br>
-- param oldname  The current name of the file.<br>
-- param name     The new name of the file.<br>
-- return True if the file have been renamed successfully, false if not.
-- @function [parent=#FileUtils] renameFile 
-- @param self
-- @param #string path
-- @param #string oldname
-- @param #string name
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- Loads the filenameLookup dictionary from the contents of a filename.<br>
-- note The plist file name should follow the format below:<br>
-- code<br>
-- <?xml version="1.0" encoding="UTF-8"?><br>
-- <!DOCTYPE plist PUBLIC "-AppleDTD PLIST 1.0EN" "http:www.apple.com/DTDs/PropertyList-1.0.dtd"><br>
-- <plist version="1.0"><br>
-- <dict><br>
-- <key>filenames</key><br>
-- <dict><br>
-- <key>sounds/click.wav</key><br>
-- <string>sounds/click.caf</string><br>
-- <key>sounds/endgame.wav</key><br>
-- <string>sounds/endgame.caf</string><br>
-- <key>sounds/gem-0.wav</key><br>
-- <string>sounds/gem-0.caf</string><br>
-- </dict><br>
-- <key>metadata</key><br>
-- <dict><br>
-- <key>version</key><br>
-- <integer>1</integer><br>
-- </dict><br>
-- </dict><br>
-- </plist><br>
-- endcode<br>
-- param filename The plist file name.<br>
-- since v2.1<br>
-- js loadFilenameLookup<br>
-- lua loadFilenameLookup
-- @function [parent=#FileUtils] loadFilenameLookupDictionaryFromFile 
-- @param self
-- @param #string filename
-- @return FileUtils#FileUtils self (return value: cc.FileUtils)
        
--------------------------------
--  Checks whether to pop up a message box when failed to load an image. <br>
-- return True if pop up a message box when failed to load an image, false if not.
-- @function [parent=#FileUtils] isPopupNotify 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#FileUtils] getValueVectorFromFile 
-- @param self
-- @param #string filename
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- Removes a directory.<br>
-- param dirPath  The full path of the directory, it must be an absolute path.<br>
-- return True if the directory have been removed successfully, false if not.
-- @function [parent=#FileUtils] removeDirectory 
-- @param self
-- @param #string dirPath
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- Gets the array of search paths.<br>
-- return The array of search paths.<br>
-- see fullPathForFilename(const char*).<br>
-- lua NA
-- @function [parent=#FileUtils] getSearchPaths 
-- @param self
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- 
-- @function [parent=#FileUtils] writeToFile 
-- @param self
-- @param #map_table dict
-- @param #string fullPath
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- Converts the contents of a file to a ValueMap.<br>
-- param filename The filename of the file to gets content.<br>
-- return ValueMap of the file contents.<br>
-- note This method is used internally.
-- @function [parent=#FileUtils] getValueMapFromFile 
-- @param self
-- @param #string filename
-- @return map_table#map_table ret (return value: map_table)
        
--------------------------------
-- 
-- @function [parent=#FileUtils] getValueMapFromData 
-- @param self
-- @param #char filedata
-- @param #int filesize
-- @return map_table#map_table ret (return value: map_table)
        
--------------------------------
-- Sets the array of search paths.<br>
-- You can use this array to modify the search path of the resources.<br>
-- If you want to use "themes" or search resources in the "cache", you can do it easily by adding new entries in this array.<br>
-- note This method could access relative path and absolute path.<br>
-- If the relative path was passed to the vector, FileUtils will add the default resource directory before the relative path.<br>
-- For instance:<br>
-- On Android, the default resource root path is "assets/".<br>
-- If "/mnt/sdcard/" and "resources-large" were set to the search paths vector,<br>
-- "resources-large" will be converted to "assets/resources-large" since it was a relative path.<br>
-- param searchPaths The array contains search paths.<br>
-- see fullPathForFilename(const char*)<br>
-- since v2.1<br>
-- In js:var setSearchPaths(var jsval);<br>
-- lua NA
-- @function [parent=#FileUtils] setSearchPaths 
-- @param self
-- @param #array_table searchPaths
-- @return FileUtils#FileUtils self (return value: cc.FileUtils)
        
--------------------------------
-- Retrieve the file size.<br>
-- note If a relative path was passed in, it will be inserted a default root path at the beginning.<br>
-- param filepath The path of the file, it could be a relative or absolute path.<br>
-- return The file size.
-- @function [parent=#FileUtils] getFileSize 
-- @param self
-- @param #string filepath
-- @return long#long ret (return value: long)
        
--------------------------------
-- Sets the array that contains the search order of the resources.<br>
-- param searchResolutionsOrder The source array that contains the search order of the resources.<br>
-- see getSearchResolutionsOrder(), fullPathForFilename(const char*).<br>
-- since v2.1<br>
-- In js:var setSearchResolutionsOrder(var jsval)<br>
-- lua NA
-- @function [parent=#FileUtils] setSearchResolutionsOrder 
-- @param self
-- @param #array_table searchResolutionsOrder
-- @return FileUtils#FileUtils self (return value: cc.FileUtils)
        
--------------------------------
-- Append search order of the resources.<br>
-- see setSearchResolutionsOrder(), fullPathForFilename().<br>
-- since v2.1
-- @function [parent=#FileUtils] addSearchResolutionsOrder 
-- @param self
-- @param #string order
-- @param #bool front
-- @return FileUtils#FileUtils self (return value: cc.FileUtils)
        
--------------------------------
-- Add search path.<br>
-- since v2.1
-- @function [parent=#FileUtils] addSearchPath 
-- @param self
-- @param #string path
-- @param #bool front
-- @return FileUtils#FileUtils self (return value: cc.FileUtils)
        
--------------------------------
-- Checks whether a file exists.<br>
-- note If a relative path was passed in, it will be inserted a default root path at the beginning.<br>
-- param filename The path of the file, it could be a relative or absolute path.<br>
-- return True if the file exists, false if not.
-- @function [parent=#FileUtils] isFileExist 
-- @param self
-- @param #string filename
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- Purges full path caches.
-- @function [parent=#FileUtils] purgeCachedEntries 
-- @param self
-- @return FileUtils#FileUtils self (return value: cc.FileUtils)
        
--------------------------------
-- Gets full path from a file name and the path of the relative file.<br>
-- param filename The file name.<br>
-- param pszRelativeFile The path of the relative file.<br>
-- return The full path.<br>
-- e.g. filename: hello.png, pszRelativeFile: /User/path1/path2/hello.plist<br>
-- Return: /User/path1/path2/hello.pvr (If there a a key(hello.png)-value(hello.pvr) in FilenameLookup dictionary. )
-- @function [parent=#FileUtils] fullPathFromRelativeFile 
-- @param self
-- @param #string filename
-- @param #string relativeFile
-- @return string#string ret (return value: string)
        
--------------------------------
-- Windows fopen can't support UTF-8 filename<br>
-- Need convert all parameters fopen and other 3rd-party libs<br>
-- param filename std::string name file for convertation from utf-8<br>
-- return std::string ansi filename in current locale
-- @function [parent=#FileUtils] getSuitableFOpen 
-- @param self
-- @param #string filenameUtf8
-- @return string#string ret (return value: string)
        
--------------------------------
-- Sets writable path.
-- @function [parent=#FileUtils] setWritablePath 
-- @param self
-- @param #string writablePath
-- @return FileUtils#FileUtils self (return value: cc.FileUtils)
        
--------------------------------
-- Sets whether to pop-up a message box when failed to load an image.
-- @function [parent=#FileUtils] setPopupNotify 
-- @param self
-- @param #bool notify
-- @return FileUtils#FileUtils self (return value: cc.FileUtils)
        
--------------------------------
-- Checks whether the path is a directory.<br>
-- param dirPath The path of the directory, it could be a relative or an absolute path.<br>
-- return True if the directory exists, false if not.
-- @function [parent=#FileUtils] isDirectoryExist 
-- @param self
-- @param #string dirPath
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- Set default resource root path.
-- @function [parent=#FileUtils] setDefaultResourceRootPath 
-- @param self
-- @param #string path
-- @return FileUtils#FileUtils self (return value: cc.FileUtils)
        
--------------------------------
-- Gets the array that contains the search order of the resources.<br>
-- see setSearchResolutionsOrder(const std::vector<std::string>&), fullPathForFilename(const char*).<br>
-- since v2.1<br>
-- lua NA
-- @function [parent=#FileUtils] getSearchResolutionsOrder 
-- @param self
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- Creates a directory.<br>
-- param dirPath The path of the directory, it must be an absolute path.<br>
-- return True if the directory have been created successfully, false if not.
-- @function [parent=#FileUtils] createDirectory 
-- @param self
-- @param #string dirPath
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- Gets the writable path.<br>
-- return  The path that can be write/read a file in
-- @function [parent=#FileUtils] getWritablePath 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- Destroys the instance of FileUtils.
-- @function [parent=#FileUtils] destroyInstance 
-- @param self
-- @return FileUtils#FileUtils self (return value: cc.FileUtils)
        
--------------------------------
--  @deprecated Use destroyInstance() instead 
-- @function [parent=#FileUtils] purgeFileUtils 
-- @param self
-- @return FileUtils#FileUtils self (return value: cc.FileUtils)
        
--------------------------------
--  @deprecated Use getInstance() instead 
-- @function [parent=#FileUtils] sharedFileUtils 
-- @param self
-- @return FileUtils#FileUtils ret (return value: cc.FileUtils)
        
--------------------------------
-- Gets the instance of FileUtils.
-- @function [parent=#FileUtils] getInstance 
-- @param self
-- @return FileUtils#FileUtils ret (return value: cc.FileUtils)
        
return nil
