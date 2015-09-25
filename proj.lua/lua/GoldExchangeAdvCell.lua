require "extern"
require "CCBReaderLoad"
require "common"

GoldExchangeAdvLuaCell  = GoldExchangeAdvLuaCell or {}
ccb["GoldExchangeAdvLuaCell"] = GoldExchangeAdvLuaCell

GoldExchangeAdvCell = class("GoldExchangeAdvCell",
	function()
        return CCLayer:create() 
	end
)
GoldExchangeAdvCell.__index = GoldExchangeAdvCell
function GoldExchangeAdvCell:create(parent,path,params)
	local node = GoldExchangeAdvCell.new()
	node:init(parent,path,params)
	return node
end
function GoldExchangeAdvCell:init(parent,path,params)
    local strPath = string.format(path)
	self.rootPath = strPath
    self.parentNode = parent
    
    self:initData(params)

    loadLuaResource(self.rootPath .. "/resources/".. self.data[19] .."adv.plist")

    local  proxy = CCBProxy:create()
    GoldExchangeAdvLuaCell.onClickCostBtn = function()
    	self:onClickCostBtn()
	end
    GoldExchangeAdvLuaCell.onPackageBtnClick = function()
        self:onPackageBtnClick()
    end
    local ccbiURL = strPath .. "/ccbi/GoldExchangeAdv"..self.data[19].."LuaCell.ccbi"
    --print ("adv ccb :" .. ccbiURL)

	local ccbnode = CCBReaderLoad(ccbiURL,proxy,true,"GoldExchangeAdvLuaCell")
    local  layer = tolua.cast(ccbnode,"CCLayer")
    if nil ~= GoldExchangeAdvLuaCell["m_timeLabel"] then
        self.m_timeLabel = tolua.cast(GoldExchangeAdvLuaCell["m_timeLabel"],"CCLabelTTF")
    end
    if nil ~= GoldExchangeAdvLuaCell["m_percentLabel"] then
        self.m_percentLabel = tolua.cast(GoldExchangeAdvLuaCell["m_percentLabel"],"CCLabelTTF")
    end
    if nil ~= GoldExchangeAdvLuaCell["m_moreLabel"] then
        self.m_moreLabel = tolua.cast(GoldExchangeAdvLuaCell["m_moreLabel"],"CCLabelTTF")
    end
    if nil ~= GoldExchangeAdvLuaCell["m_desLabel"] then
        self.m_desLabel = tolua.cast(GoldExchangeAdvLuaCell["m_desLabel"],"CCLabelTTF")
    end
    if nil ~= GoldExchangeAdvLuaCell["m_getLabel"] then
        self.m_getLabel = tolua.cast(GoldExchangeAdvLuaCell["m_getLabel"],"CCLabelTTF")
        if nil ~= self.m_getLabel then
    		--local getStr = string.format(LuaController:getLang1("115073",""))
    		--self.m_getLabel:setString(getStr)
    		self.m_getLabel:setVisible(false)
    	end
    end
    if nil ~= GoldExchangeAdvLuaCell["m_newPriceLabel"] then
        self.m_newPriceLabel = tolua.cast(GoldExchangeAdvLuaCell["m_newPriceLabel"],"CCLabelTTF")
    end
    if nil ~= GoldExchangeAdvLuaCell["m_getGoldNumText"] then
        self.m_getGoldNumText = tolua.cast(GoldExchangeAdvLuaCell["m_getGoldNumText"],"CCLabelBMFont")
    end
    if nil ~= GoldExchangeAdvLuaCell["m_costBtn"] then
        self.m_costBtn = tolua.cast(GoldExchangeAdvLuaCell["m_costBtn"],"CCControlButton")
        if nil ~= self.m_costBtn then
            --LuaController:addButtonLight(self.m_costBtn)
        end
    end
    if nil ~= GoldExchangeAdvLuaCell["m_showMoneyNode"] then
    	self.m_showMoneyNode = tolua.cast(GoldExchangeAdvLuaCell["m_showMoneyNode"],"CCLayerColor")
    end
    if nil ~= GoldExchangeAdvLuaCell["m_showMoreNode"] then
    	self.m_showMoreNode = tolua.cast(GoldExchangeAdvLuaCell["m_showMoreNode"],"CCLayerColor")
    	if nil ~= self.m_showMoreNode then
    		print ("show more node 111111")
            function onTouch(eventType, x, y)  
                if eventType == "began" then  
                    return self:onTouchBegan(x, y)  
                elseif eventType == "moved" then  
                    return self:onTouchMoved(x, y)  
                else  
                    return self:onTouchEnded(x, y)  
                end
            end
    		self.m_showMoreNode:registerScriptTouchHandler(onTouch)
    		self.m_showMoreNode:setTouchEnabled(true)
        end
    end
    if nil ~= GoldExchangeAdvLuaCell["m_packageBtn"] then
        self.m_packageBtn = tolua.cast(GoldExchangeAdvLuaCell["m_packageBtn"],"CCControlButton")
        if nil ~= self.m_packageBtn then
            local showPackBtn = false
            if self.data[21] ~= nil then
                if self.data[21] == "1" then
                    showPackBtn = true
                end
            end
            if showPackBtn == true then
                self.m_packageBtn:setVisible(true)
            else
                self.m_packageBtn:setVisible(false)
                if self.m_costBtn then
                    self.m_costBtn:setPositionX(10)
                    if self.m_newPriceLabel then
                        self.m_newPriceLabel:setPositionX(10)
                    end
                end
            end
            --LuaController:addButtonLight(self.m_costBtn)
        end
    end
    
    self:initView()
    self:scheduleBack()
    schedule(self.m_timeLabel,scheduleDealWithFunc({target = self}),1)
    self.parentNode:addChild(ccbnode)
    self:setContentSize(CCSizeMake(540, 220))
    ccbnode:setPositionY(-66)
    self.ccbNode = ccbnode

    local function onNodeEvent(event)
        if event == "enter" then
            --print "gold_exchange_adv_cell_enter"
        elseif event == "exit" then
            --print "gold_exchange_adv_cell_exit"
            self:removeAllEvent()
        end
    end
    self.ccbNode:registerScriptHandler(onNodeEvent)
end

function GoldExchangeAdvCell:initData(params)
    local paramsStr = string.format(params)
    --print("params:" .. paramsStr)
    self.data = {}
    local index = 1
    local startI = 1
    local fIndex = string.find(paramsStr,",",startI)
    local tmpValue = "" 
    while (true) do
        tmpValue = string.sub(paramsStr,startI,fIndex-1)
        --print(string.format(startI) .. "," .. string.format(fIndex-1) .. ", params" .. string.format(index) .. ":" .. tmpValue)
        self.data[index] = tmpValue
        index = index + 1
        startI = fIndex + 1
        fIndex = string.find(paramsStr,",",startI)
        if (fIndex == nil) then
            tmpValue = string.sub(paramsStr,startI,string.len(paramsStr))
            --print( string.format(startI) .. "," .. string.format(string.len(paramsStr)) .. ", params" .. string.format(index) .. ":" .. tmpValue)
            self.data[index] = tmpValue
            break
        end
    end
end
function GoldExchangeAdvCell:initView()
    if nil ~= self.m_getGoldNumText then
        local numKey = string.format(self.data[3]);
        local numStr = string.format(LuaController:getCMDLang(numKey))
        self.m_getGoldNumText:setString(numStr)
    end
    if nil ~= self.m_percentLabel then
        local percentStr = string.format(self.data[8])
        percentStr = percentStr .. "%"
        self.m_percentLabel:setString(percentStr)
        self.m_percentLabel:setScale(1.2)
    end
    local itemNull = false
    local itemStr = string.format(self.data[7])
    if string.len(itemStr) == 0 then
    	if nil ~= self.m_moreLabel then
    		self.m_moreLabel:setString("")
    	end
    	if nil ~= self.m_showMoreNode then
    		self.m_showMoreNode:setVisible(false)
    	end
    	if nil ~= self.m_desLabel then
    		self.m_desLabel:setString("")
    	end
    else
    	if nil ~= self.m_moreLabel then
    		local moreStr = string.format(LuaController:getLang("102271"))
    		self.m_moreLabel:setString(moreStr)
    	end
    	if nil ~= self.m_showMoreNode then
    		self.m_showMoreNode:setVisible(true)
    	end
    	if nil ~= self.m_desLabel then
        	local desStr = string.format(LuaController:getLang("101237"))
        	self.m_desLabel:setString(desStr)
        end
    end
    if nil ~= self.m_newPriceLabel then
		local dollar = string.format(self.data[4])
    	local pID = string.format(self.data[11])
    	local newPrice = string.format(LuaController:getDollarString(dollar,pID))
    	self.m_newPriceLabel:setString(newPrice)
	end
end
function GoldExchangeAdvCell:removeAllEvent()
    releaseLuaResource(self.rootPath .. "/resources/".. self.data[19] .."adv")
    GoldExchangeAdvLuaCell.onClickCostBtn = nil
    if nil ~= self.m_showMoreNode then
        self.m_showMoreNode:unregisterScriptTouchHandler()
    end
    if nil ~= self.m_costBtn then
        self.m_costBtn:setTouchEnabled(false)
    end
    if nil ~= self.m_timeLabel then
        self.m_timeLabel:stopAllActions()
    end
    if nil ~= self.ccbNode then
        self.ccbNode:unregisterScriptHandler()
    end
end
function GoldExchangeAdvCell:scheduleBack()
    --print "_____scheduleBack"
    if(self.m_timeLabel) then
        local curTime = LuaController:getWorldTime()
        local lastTime = 0
        local expTime = tonumber(self.data[14])
        local endTime = tonumber(self.data[13])
        if expTime>0 then
            local gapTime = endTime - curTime
            local count =  gapTime / (expTime * 3600)
            lastTime = endTime - (expTime*3600)*count-curTime;
        else
            lastTime = endTime - curTime
        end
        local timeStr = LuaController:getSECLang(lastTime)
        self.m_timeLabel:setString(timeStr)
        if (endTime - curTime <= 0 ) then
            self.m_costBtn:setEnabled(false)
            self:removeAllEvent()
            LuaController:removeAllPopup()
        end 
    end
end
function GoldExchangeAdvCell:onClickCostBtn()
    local itemid = string.format(self.data[1])
    LuaController:callPayment(itemid)
    self:removeAllEvent()
    LuaController:removeAllPopup()
end
function GoldExchangeAdvCell:onPackageBtnClick()
    if nil ~= self.data[21] then
        self:removeAllEvent()
        LuaController:removeAllPopup()
        local itemid = string.format(self.data[1])
        LuaController:toSelectUser(itemid,false,2)
    end
end
function GoldExchangeAdvCell:onTouchBegan(x, y)
	if(nil ~= self.m_showMoreNode) then
        if(nil ~= self.m_showMoreNode:getParent()) then
            local pos = self.m_showMoreNode:getParent():convertToNodeSpace(ccp(x,y))
            local rect = self.m_showMoreNode:boundingBox()
            if(rect:containsPoint(pos) == true) then
                return true
            end 
        end
    end
    return false
end
function GoldExchangeAdvCell:onTouchMoved(x, y)
	print "touch move"
end
function GoldExchangeAdvCell:onTouchEnded(x, y)
    if(nil == self.m_showMoreNode) then
    	print "touch node empty"
    	return
    end

    if self.m_showMoreNode:isVisible() == false then
    	print "touch node hide"
    	return
    end

   	if(nil == self.m_showMoreNode:getParent()) then
   		print "touch node parent empty"
   		return
   	end

    local pos = self.m_showMoreNode:getParent():convertToNodeSpace(ccp(x,y))
    local rect = self.m_showMoreNode:boundingBox()
    if(rect:containsPoint(pos) == true) then
    	local itemid = string.format(self.data[1])
    	LuaController:showDetailPopup(itemid)
    	print "touch inside"
    	return
   	end
   	print "touch outside"
end
function GoldExchangeAdvCell:getCostBtnRect()
    if nil ~= self.m_costBtn then
        local size = self.m_costBtn:getContentSize()
        local pos = ccp(self.m_costBtn:getPosition())
        --print ("pos1:[" .. string.format(pos.x) .. "," .. string.format(pos.y) .. "]")
        pos = self.m_costBtn:getParent():convertToWorldSpace(pos)
        --print ("pos2:[" .. string.format(pos.x) .. "," .. string.format(pos.y) .. "]")
        pos = self.ccbNode:convertToNodeSpace(pos)
        --print ("pos3:[" .. string.format(pos.x) .. "," .. string.format(pos.y) .. "]")
        --print ("size:[" .. string.format(size.width) .. "," .. string.format(size.height) .. "]")
        return pos.x, pos.y, size.width, size.height
    end
    return 0,0,0,0
end