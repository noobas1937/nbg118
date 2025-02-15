
--------------------------------
-- @module CCBAnimationManager
-- @extend Ref
-- @parent_module cc

--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] moveAnimationsFromNode 
-- @param self
-- @param #cc.Node fromNode
-- @param #cc.Node toNode
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] setAutoPlaySequenceId 
-- @param self
-- @param #int autoPlaySequenceId
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] getDocumentCallbackNames 
-- @param self
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] actionForSoundChannel 
-- @param self
-- @param #cc.CCBSequenceProperty channel
-- @return Sequence#Sequence ret (return value: cc.Sequence)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] setBaseValue 
-- @param self
-- @param #cc.Value value
-- @param #cc.Node pNode
-- @param #string propName
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] getDocumentOutletNodes 
-- @param self
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] getLastCompletedSequenceName 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] setRootNode 
-- @param self
-- @param #cc.Node pRootNode
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] runAnimationsForSequenceNamedTweenDuration 
-- @param self
-- @param #char pName
-- @param #float fTweenDuration
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] addDocumentOutletName 
-- @param self
-- @param #string name
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] getSequences 
-- @param self
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] getRootContainerSize 
-- @param self
-- @return size_table#size_table ret (return value: size_table)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] setDocumentControllerName 
-- @param self
-- @param #string name
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] setObject 
-- @param self
-- @param #cc.Ref obj
-- @param #cc.Node pNode
-- @param #string propName
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] getContainerSize 
-- @param self
-- @param #cc.Node pNode
-- @return size_table#size_table ret (return value: size_table)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] actionForCallbackChannel 
-- @param self
-- @param #cc.CCBSequenceProperty channel
-- @return Sequence#Sequence ret (return value: cc.Sequence)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] getDocumentOutletNames 
-- @param self
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] addDocumentCallbackControlEvents 
-- @param self
-- @param #int eventType
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] init 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] getKeyframeCallbacks 
-- @param self
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] getDocumentCallbackControlEvents 
-- @param self
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] setRootContainerSize 
-- @param self
-- @param #size_table rootContainerSize
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] runAnimationsForSequenceIdTweenDuration 
-- @param self
-- @param #int nSeqId
-- @param #float fTweenDuraiton
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] getRunningSequenceName 
-- @param self
-- @return char#char ret (return value: char)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] getAutoPlaySequenceId 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] addDocumentCallbackName 
-- @param self
-- @param #string name
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] getRootNode 
-- @param self
-- @return Node#Node ret (return value: cc.Node)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] addDocumentOutletNode 
-- @param self
-- @param #cc.Node node
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] getSequenceDuration 
-- @param self
-- @param #char pSequenceName
-- @return float#float ret (return value: float)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] addDocumentCallbackNode 
-- @param self
-- @param #cc.Node node
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)
        
--------------------------------
-- @overload self, char         
-- @overload self, char, float         
-- @overload self, int, float         
-- @function [parent=#CCBAnimationManager] runAnimations
-- @param self
-- @param #int nSeqId
-- @param #float fTweenDuraiton
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)

--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] runAnimationsForSequenceNamed 
-- @param self
-- @param #char pName
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] getSequenceId 
-- @param self
-- @param #char pSequenceName
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] getDocumentCallbackNodes 
-- @param self
-- @return array_table#array_table ret (return value: array_table)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] setSequences 
-- @param self
-- @param #array_table seq
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] debug 
-- @param self
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)
        
--------------------------------
-- 
-- @function [parent=#CCBAnimationManager] getDocumentControllerName 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- js ctor
-- @function [parent=#CCBAnimationManager] CCBAnimationManager 
-- @param self
-- @return CCBAnimationManager#CCBAnimationManager self (return value: cc.CCBAnimationManager)
        
return nil
