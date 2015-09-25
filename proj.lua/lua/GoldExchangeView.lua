require "extern"
require "CCBReaderLoad"
require "common"
require "GoldExchangeCell"
require "GoldExchangeCell1"


GoldExchangeLuaView  = GoldExchangeLuaView or {}
ccb["GoldExchangeLuaView"] = GoldExchangeLuaView

GoldExchangeView = class("GoldExchangeView",
	function()
        return CCLayer:create() 
	end
)
GoldExchangeView.__index = GoldExchangeView
function GoldExchangeView:create(parent,path,params)
	local node = GoldExchangeView.new()
	node:init(parent,path,params)
	return node
end
function GoldExchangeView:init(parent,path,params)
    -- debug test 
    --local opStart = os.clock()
    local strPath = string.format(path)
    self.rootPath = strPath
	self.parentNode = parent
    self.isRemove =false
    self.effid1 = nil
    self.effid2 = nil
    self.effid3 = nil
    self.effid4 = nil
    self.effid5 = nil
    --opStart = os.clock()
    -- init exchangeitem 
    local paramsStr = string.format(params)
    --print("params:" .. paramsStr)
    self.data = {}
    local index = 1
    local startI = 1
    local fIndex = string.find(paramsStr,",",startI)
    local tmpValue = "" 
    while (true) do
        tmpValue = string.sub(paramsStr,startI,fIndex-1)
        --print(" params" .. string.format(index) .. ":" .. tmpValue)
        self.data[index] = tmpValue
        index = index + 1
        startI = fIndex + 1
        fIndex = string.find(paramsStr,",",startI)
        if (fIndex == nil) then
            tmpValue = string.sub(paramsStr,startI,string.len(paramsStr))
            --print(" params" .. string.format(index) .. ":" .. tmpValue)
            self.data[index] = tmpValue
            break
        end
    end
    self:initItems()
    self:initEquips()
    --print("init data ____" .. string.format(os.clock() - opStart))

    local popImg = string.format(self.data[19])
    --print("popImg___:" .. popImg)
    --init resource
    --opStart = os.clock()
    --loadCommonResource(7,true)
    --loadCommonResource(8,true)
    --if nil~=self.equips then
    --    loadCommonResource(308,true)
    --end
    --loadCommonResource(102,true)
    loadLuaResource(self.rootPath .. "/resources/"..popImg..".plist")
    --print("load resource ____" .. string.format(os.clock() - opStart))

    -- init ccbi
    --opStart = os.clock()
    local  proxy = CCBProxy:create()
    GoldExchangeLuaView.onClickCostBtn = function()
    	self:onClickCostBtn()
	end
   	GoldExchangeLuaView.onCloseBtnClick = function()
   		self:onCloseBtnClick()
    end
    GoldExchangeLuaView.onPackageBtnClick = function ()
        self:onPackageBtnClick()
    end
    local ccbiURL = strPath .. "/ccbi/GoldExchange"..popImg.."LuaView.ccbi"
    local  node  = CCBReaderLoad(ccbiURL,proxy,true,"GoldExchangeLuaView")
   	
    --print("load ccbi data ____" .. string.format(os.clock() - opStart))
    --opStart = os.clock()

    local  layer = tolua.cast(node,"CCLayer")
    if nil ~= GoldExchangeLuaView["m_timeLabel"] then
        self.m_timeLabel = tolua.cast(GoldExchangeLuaView["m_timeLabel"],"CCLabelTTF")
        if nil ~= self.m_timeLabel then
            self.m_timeLabel:setString("m_timeLabel")
        end
    end
    if nil ~= GoldExchangeLuaView["m_titleLabel"] then
        self.m_titleLabel = tolua.cast(GoldExchangeLuaView["m_titleLabel"],"CCLabelTTF")
        if nil ~= self.m_titleLabel then
            if nil ~= self.data[10] then
                local keyStr = string.format(self.data[10])
                --print(keyStr)
                local titleStr = string.format(LuaController:getLang(keyStr))
                --print(titleStr)
                self.m_titleLabel:setString(titleStr)
            end
        end
    end
    --print("load resource ____m_titleLabel")
    if nil ~= GoldExchangeLuaView["m_percentLabel"] then
        self.m_percentLabel = tolua.cast(GoldExchangeLuaView["m_percentLabel"],"CCLabelBMFont")
        if nil ~= self.m_percentLabel then
            if nil ~= self.data[8] then
                local percentStr = string.format(self.data[8])
                percentStr = percentStr .. "%"
                self.m_percentLabel:setString(percentStr)
                self.m_percentLabel:setScale(1.2)
            end
        end
    end
    --print("load resource ____m_percentLabel")
    if nil ~= GoldExchangeLuaView["m_getGoldNumText"] then
        self.m_getGoldNumText = tolua.cast(GoldExchangeLuaView["m_getGoldNumText"],"CCLabelBMFont")
        if nil ~= self.m_getGoldNumText then
            if nil ~= self.data[3] then
                local numKey = string.format(self.data[3]);
                --print(numKey)
                local numStr = string.format(LuaController:getCMDLang(numKey))
                --print(numStr)
                self.m_getGoldNumText:setString(numStr)
            end
        end
    end
    --print("load resource ____m_getGoldNumText")
    if nil ~= GoldExchangeLuaView["m_getLabel1"] then
        self.m_getLabel1 = tolua.cast(GoldExchangeLuaView["m_getLabel1"],"CCLabelTTF")
        if nil ~= self.m_getLabel1 then
            local lblGet1Str = string.format(LuaController:getLang1("101237",""))
            --print(lblGet1Str)
            self.m_getLabel1:setString(lblGet1Str)
        end
    end
    --print("load resource ____m_getLabel1")
    if nil ~= GoldExchangeLuaView["m_getLabel"] then
        self.m_getLabel = tolua.cast(GoldExchangeLuaView["m_getLabel"],"CCLabelTTF")
        if nil ~= self.m_getLabel then
            local lblGetStr = string.format(LuaController:getLang1("115073",""))
            --print(lblGetStr)
            self.m_getLabel:setString(lblGetStr)
        end
    end
    --print("load resource ____m_getLabel")
    if nil ~= GoldExchangeLuaView["m_newPriceLabel"] then
        self.m_newPriceLabel = tolua.cast(GoldExchangeLuaView["m_newPriceLabel"],"CCLabelTTF")
        if nil ~= self.m_newPriceLabel then
            if nil ~= self.data[4] then
                if nil ~=  self.data[11] then
                    local dollar = string.format(self.data[4])
                    --print(dollar)
                    local pID = string.format(self.data[11])
                    --print(pID)
                    local newPrice = string.format(LuaController:getDollarString(dollar,pID))
                    --print(newPrice)
                    self.m_newPriceLabel:setString(newPrice)
                end
            end
        end
    end
    --print("load resource ____m_newPriceLabel")
    if nil ~= GoldExchangeLuaView["m_oldPriceLabel"] then
        self.m_oldPriceLabel = tolua.cast(GoldExchangeLuaView["m_oldPriceLabel"],"CCLabelTTF")
        if nil ~= self.m_oldPriceLabel then
            if nil ~= self.data[9] then
                local dollar1 = string.format(self.data[9])
                --print(dollar1)
                local oldPrice = string.format(LuaController:getDollarString(dollar1,""))
                --print(oldPrice)
                self.m_oldPriceLabel:setString(oldPrice)
            end
        end
    end
    --print("load resource ____m_oldPriceLabel")
    if nil ~= GoldExchangeLuaView["m_lblDes"] then
        self.m_lblDes = tolua.cast(GoldExchangeLuaView["m_lblDes"],"CCLabelTTF")
        if nil ~= self.m_lblDes then
            local desString1 = nil
            if self.data[19] == "alliance" then
                desString1 = string.format(LuaController:getLang("101379"))
            end
            if self.data[19] == "alliance1" then
                desString1 = string.format(LuaController:getLang("101379"))
            end
            if self.data[19] == "build_alliance" then
                desString1 = string.format(LuaController:getLang("101379"))
            end
            if nil ~= desString1 then
                self.m_lblDes:setString(desString1)
            end

        end
    end
    --print("load resource ____m_lblDes")
    if nil ~= GoldExchangeLuaView["m_lblDes1"] then
        self.m_lblDes1 = tolua.cast(GoldExchangeLuaView["m_lblDes1"],"CCLabelTTF")
        if nil ~= self.m_lblDes1 then
            local desString2 = nil
            if self.data[19] == "alliance1" then
                desString2 = string.format(LuaController:getLang("101397"))
            end
            if self.data[19] == "build_alliance" then
                desString2 = string.format(LuaController:getLang("101397"))
            end
            if nil ~= desString2 then
                self.m_lblDes1:setString(desString2)
            end
        end
    end
    --print("load resource ____m_lblDes1")
    if nil ~= GoldExchangeLuaView["m_soleOutSpr"] then
        self.m_soleOutSpr = tolua.cast(GoldExchangeLuaView["m_soleOutSpr"],"CCSprite")
        if nil ~= self.m_soleOutSpr then
            -- sprite change image test code
            --local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("land.png");
            --if nil ~= frame then
            --    self.m_soleOutSpr:setDisplayFrame(frame)
            --end
            --print "m_soleOutSpr___init____"
        end
    end
    --print("load resource ____m_soleOutSpr")
    if nil ~= GoldExchangeLuaView["m_moreNode"] then
    	self.m_moreNode = tolua.cast(GoldExchangeLuaView["m_moreNode"],"CCLayerColor")
    	if nil ~= self.m_moreNode then
    		local w = self.m_moreNode:getContentSize().width
    		local h = self.m_moreNode:getContentSize().height
            self.m_moreNode:setVisible(false)
    		--print "m_moreNode___init____[" .. tostring(w) .. "," .. tostring(h) .. "]"
    		--print "m_moreNode___init____".. size.width .. "," .. size.height .."]"
    	end
    end
    --print("load resource ____m_moreNode")
    if nil ~= GoldExchangeLuaView["m_moreSpr"] then
        self.m_moreSpr = tolua.cast(GoldExchangeLuaView["m_moreSpr"],"CCSprite")
        if nil ~= self.m_moreSpr then
            --print "m_moreSpr___init____"
        end
    end
    --print("load resource ____m_moreSpr")
    if nil ~= GoldExchangeLuaView["m_sprCha"] then
        self.m_sprCha = tolua.cast(GoldExchangeLuaView["m_sprCha"],"CCSprite")
    end
    --print("load resource ____m_sprCha")
    if nil ~= GoldExchangeLuaView["m_moreLabel"] then
        self.m_moreLabel = tolua.cast(GoldExchangeLuaView["m_moreLabel"],"CCLabelTTF")
        if nil ~= self.m_moreLabel then
            local moreStr = string.format(LuaController:getLang("102162"))
            --print(moreStr)
            self.m_moreLabel:setString(moreStr)
        end
    end
    --print("load resource ____m_moreLabel")
    if nil ~= GoldExchangeLuaView["m_touchMoreNode"] then
    	self.m_touchMoreNode = tolua.cast(GoldExchangeLuaView["m_touchMoreNode"],"CCLayerColor")
    	if nil ~= self.m_touchMoreNode then
    		local size = self.m_touchMoreNode:getContentSize()
    		--print "m_touchMoreNode___init____[" .. size.width .. "," .. size.height .."]"
    	end
    end
    --print("load resource ____m_touchMoreNode")
    if nil ~= GoldExchangeLuaView["m_touchNode"] then
    	self.m_touchNode = tolua.cast(GoldExchangeLuaView["m_touchNode"],"CCLayerColor")
    	if nil ~= self.m_touchNode then
    		local size = self.m_touchNode:getContentSize()
    		function onTouch(eventType, x, y)  
				if eventType == "began" then  
					return self:onTouchBegan(x, y)  
				elseif eventType == "moved" then  
					return self:onTouchMoved(x, y)  
    			else  
        			return self:onTouchEnded(x, y)  
    			end
    		end
    		self.m_touchNode:registerScriptTouchHandler(onTouch)
    		self.m_touchNode:setTouchEnabled(true)
    		--print "m_touchNode___init____[" .. size.width .. "," .. size.height .."]"
    	end
    end
    --print("load resource ____m_touchNode")
    if nil ~= GoldExchangeLuaView["m_buyNode"] then
        self.m_buyNode = tolua.cast(GoldExchangeLuaView["m_buyNode"],"CCLayerColor")
    end
    --print("load resource ____m_buyNode")
    if nil ~= GoldExchangeLuaView["m_packageBtn"] then
        self.m_packageBtn = tolua.cast(GoldExchangeLuaView["m_packageBtn"],"CCControlButton")
    end
    --print("load resource ____m_packageBtn")
    if nil ~= GoldExchangeLuaView["m_costBtn"] then
        self.m_costBtn = tolua.cast(GoldExchangeLuaView["m_costBtn"],"CCControlButton")
        if nil ~= self.m_costBtn then
            local extWidth = 0
            local lableW = 0
            if nil ~= self.m_oldPriceLabel then
                lableW = self.m_oldPriceLabel:getContentSize().width
            end
            local maxWidth = (self.m_costBtn:getPreferredSize().width - 100) * 0.5
            --print ("lua____[" .. string.format(lableW) .. "," .. string.format(maxWidth) .. "]" )

            if lableW > maxWidth then
                --print ("lua____large1")
                extWidth = lableW - maxWidth
                if nil ~= self.m_sprCha then
                    local chaPosX = 0
                    if nil ~= self.m_oldPriceLabel then
                        if nil ~= self.m_sprCha then
                            self.m_oldPriceLabel:getPositionX()
                            self.m_sprCha:setPositionX(chaPosX - lableW * 0.5)
                        end
                    end
                end
            end
            if nil ~= self.m_newPriceLabel then
                lableW = self.m_newPriceLabel:getContentSize().width
                if lableW > maxWidth then
                    --print ("lua____large2")
                    extWidth = lableW - maxWidth + extWidth
                end
            end

            if extWidth > 0 then
                --print ("lua____large3")
                local costsize = self.m_costBtn:getContentSize()
                costsize.width = costsize.width + extWidth
                self.m_costBtn:setPreferredSize(costsize)
            end
            LuaController:addButtonLight(self.m_costBtn)
            local showPackBtn = false
            if self.data[21] ~= nil then
                if self.data[21] == "1" then
                    showPackBtn = true
                end
            end
            if showPackBtn == true then
                if self.m_packageBtn ~= nil then
                    self.m_packageBtn:setVisible(true)
                    if extWidth > 0 then
                        self.m_packageBtn:setPositionX(self.m_packageBtn:getPositionX() + extWidth*0.5)
                    end
                end
                if self.m_buyNode ~= nil then
                    self.m_buyNode:setPositionX(-35)
                end
            else
                if self.m_packageBtn ~= nil then
                    self.m_packageBtn:setVisible(false)
                end
                if self.m_buyNode ~= nil then
                    self.m_buyNode:setPositionX(0)
                end
            end
        end
    end
    --print("load resource ____m_costBtn")
    if nil ~= GoldExchangeLuaView["m_ani1"] then
        self.m_ani1Layer = tolua.cast(GoldExchangeLuaView["m_ani1"],"CCLayerColor")
    end
    if nil ~= GoldExchangeLuaView["m_ani2"] then
        self.m_ani2Layer = tolua.cast(GoldExchangeLuaView["m_ani2"],"CCLayerColor")
    end
    --print("load resource ____m_ani2")
    --print("init ccbi data ____" .. string.format(os.clock() - opStart))
    --opStart = os.clock()

    if nil ~= GoldExchangeLuaView["m_listNode"] then
    	self.m_listNode = tolua.cast(GoldExchangeLuaView["m_listNode"],"CCLayerColor")
    	if nil ~= self.m_listNode then
            self.m_listNode:setContentSize(CCSizeMake(self.m_listNode:getContentSize().width,self.m_listNode:getContentSize().height + 60));
    		self.m_listNode:setPositionY(self.m_listNode:getPositionY() - 60)
            local size = self.m_listNode:getContentSize()
            local scrollView1 = CCScrollView:create()
            if nil ~= scrollView1 then
                scrollView1:setViewSize(self.m_listNode:getContentSize())
                scrollView1:setPosition(CCPointMake(0,0))
                scrollView1:setScale(1.0)
                scrollView1:ignoreAnchorPointForPosition(true)
                scrollView1:setDirection(kCCScrollViewDirectionVertical)
                scrollView1:setClippingToBounds(true)
                scrollView1:setBounceable(true)

                local offsetY = 0
                if nil~= self.items then
                    local tCount = table.getn(self.items)
                    for i=1,tCount do
                        local sprite =  GoldExchangeCell:create(self.rootPath,self.items[i])
                        scrollView1:addChild(sprite)
                        sprite:setPositionX(0)
                        sprite:setPositionY(offsetY)
                        offsetY = offsetY + 65
                    end
                end
                if self.equips~=nil then
                    local eCount = table.getn(self.equips)
                    for j=1,eCount do
                        local eSprite = GoldExchangeCell1:create(self.rootPath,self.equips[j])
                        scrollView1:addChild(eSprite)
                        eSprite:setPositionX(0)
                        eSprite:setPositionY(offsetY)
                        offsetY = offsetY + 65
                    end
                end
                scrollView1:setContentSize(CCSizeMake(450,offsetY))
                local cH = self.m_listNode:getContentSize().height
                scrollView1:setContentOffset(CCPointMake(0, cH - offsetY))
                self.m_listNode:addChild(scrollView1);
            end
    		--print "m_listNode___init____[" .. size.width .. "," .. size.height .."]"
    	end
    end


    if nil ~= GoldExchangeLuaView["m_anlayer10"] then
        self.m_anlayer10 = tolua.cast(GoldExchangeLuaView["m_anlayer10"],"CCLayerColor")
    end
    if nil ~= GoldExchangeLuaView["m_anlayer11"] then
        self.m_anlayer11 = tolua.cast(GoldExchangeLuaView["m_anlayer11"],"CCLayerColor")
    end
    if nil ~= GoldExchangeLuaView["m_anlayer12"] then
        self.m_anlayer12 = tolua.cast(GoldExchangeLuaView["m_anlayer12"],"CCLayerColor")
    end
    if nil ~= GoldExchangeLuaView["m_anlayer13"] then
        self.m_anlayer13 = tolua.cast(GoldExchangeLuaView["m_anlayer13"],"CCLayerColor")
    end
    if nil ~= GoldExchangeLuaView["m_anlayer14"] then
        self.m_anlayer14 = tolua.cast(GoldExchangeLuaView["m_anlayer14"],"CCLayerColor")
    end
    
    --print("load resource ____m_listNode")
    --print("init scroll data ____" .. string.format(os.clock() - opStart))
    --opStart = os.clock()
    if nil ~= self.data[15] then
        if(self.data[15] == "1") then
            self.m_soleOutSpr:setVisible(true)
            self.m_costBtn:setEnabled(false)
        else
            self.m_soleOutSpr:setVisible(false)
            self.m_costBtn:setEnabled(true)
        end
    end
    

    self:scheduleBack()
    if nil ~= self.m_timeLabel then
        schedule(self.m_timeLabel,scheduleDealWithFunc({target = self}),1)  
    end
    --print("load resource ____m_soleOutSpr")

   	self.parentNode:addChild(node);
    local nodeSize = node:getContentSize()
    local winSize = CCDirector:sharedDirector():getWinSize()
    node:setPosition(CCPointMake((winSize.width-nodeSize.width)*0.5,(winSize.height-nodeSize.height)*0.5))

    self.ccbNode = node
    
    self:initParticale()
    self:initViewSkeleton()
    --print("load resource ____initParticale")
    local function onNodeEvent(event)
        if event == "enter" then
            --print "gold_exchange_adv_cell_enter"
        elseif event == "exit" then
            --print "gold_exchange_adv_cell_exit"
            if self.effid1 ~= nil then
                if self.m_anlayer10 ~= nil then
                    self.m_anlayer10:getScheduler():unscheduleScriptEntry(self.effid1)
                end
            end
            if self.effid2 ~= nil then
                if self.m_anlayer11 ~= nil then
                    self.m_anlayer11:getScheduler():unscheduleScriptEntry(self.effid2)
                end
            end
            if self.effid3 ~= nil then
                if self.m_anlayer12 ~= nil then
                    self.m_anlayer12:getScheduler():unscheduleScriptEntry(self.effid3)
                end
            end
            if self.effid4 ~= nil then
                if self.m_anlayer13 ~= nil then
                    self.m_anlayer13:getScheduler():unscheduleScriptEntry(self.effid4)
                end
            end
            if self.effid5 ~= nil then
                if self.m_anlayer14 ~= nil then
                    self.m_anlayer14:getScheduler():unscheduleScriptEntry(self.effid5)
                end
            end
            self:onExit()
        end
    end
    self.ccbNode:registerScriptHandler(onNodeEvent)
    --print("finish init view ____" .. string.format(os.clock() - opStart))
   	--PopupViewController:getInstance():addPopupView(self.parentNode,false,false)
    --print("load resource ____finish")
end
function GoldExchangeView:initViewSkeleton()
    --print("initSkeleton ____1" )
    if nil==self.m_ani1Layer then
        return
    end
    --print("initSkeleton ____2" )
    local skeletonPath = nil
    local targetPlatform = CCApplication:sharedApplication():getTargetPlatform()
    if (kTargetAndroid == targetPlatform) then
        targetPlatform = "android/"
    else
        targetPlatform = "ios/"
    end
    --print("initSkeleton ____3" )
    if self.data[19] == "peace_gift" then
        --print("initSkeleton ____4" )
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_peace_view")
        if bFile == false then
            return
        end
        --print("initSkeleton ____5" )
        self.m_ani1Layer:removeAllChildrenWithCleanup(true)
        local altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_peace_view.atlas"
        local json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_peace_view.json"
        local aniName = "jiemian"
        LuaController:addSkeletonAnimation(self.m_ani1Layer,json,altas,aniName,1)
        self.m_ani1Layer:setScale(1)
    end
    --print("initSkeleton ____6" )
end
function GoldExchangeView:initParticale()
    if self.data[19] == "jp_fight" then
        if nil~= self.m_ani1Layer then
            local winSize = CCDirector:sharedDirector():getWinSize()
            local particles1 = LuaController:createParticleForLua(self.rootPath .. "/particles/Japan_1")
            if nil ~= particles1 then
                self.m_ani1Layer:addChild(particles1,-2)
                particles1:setPosition(ccp(winSize.width*0.5,winSize.height));
            end

            local particles2 = LuaController:createParticleForLua(self.rootPath .. "/particles/Japan_1")
            if nil ~= particles2 then
            self.m_ani1Layer:addChild(particles2,-1)
                particles2:setPosition(particles1:getPosition());
            end
        end
    elseif self.data[19] == "war_resource" then
        if nil~= self.m_ani1Layer then
            if nil ~= self.m_ani2Layer then
                local size = self.m_ani1Layer:getContentSize()
                for i=1,4 do
                    local particle1= LuaController:createParticleForLua(self.rootPath .. "/particles/UiFire_"..string.format(i))
                    if nil ~= particle1 then
                        self.m_ani1Layer:addChild(particle1)
                        particle1:setPosition(ccp(size.width*0.5,size.height*0.5))
                    end
                    local particle2= LuaController:createParticleForLua(self.rootPath .. "/particles/UiFire_"..string.format(i))
                    if nil ~= particle2 then
                        self.m_ani2Layer:addChild(particle2)
                        particle2:setPosition(ccp(size.width*0.5,size.height*0.5))
                    end
                end
            end
        end
    elseif self.data[19] == "oneyear" then
        if nil~= self.m_anlayer10 then
            local function tick1()
                if nil ~= self.m_anlayer10 then
                    self.m_anlayer10:removeAllChildrenWithCleanup(true)
                    local size = self.m_anlayer10:getContentSize()
                    for i=1,5 do
                        local particle1= LuaController:createParticleForLua(self.rootPath .. "/particles/oyrFireworks_"..string.format(i))
                        if nil ~= particle1 then
                            self.m_anlayer10:addChild(particle1)
                            particle1:setPosition(ccp(size.width*0.5,size.height*0.5))
                            particle1:setScale(1.2)
                        end
                    end
                end
            end
            --tick1()
            self.effid1 = tonumber(self.m_anlayer10:getScheduler():scheduleScriptFunc(tick1, 2, false))
        end

        if nil~= self.m_anlayer11 then
            local function tick2()
                if nil ~= self.m_anlayer11 then
                    self.m_anlayer11:removeAllChildrenWithCleanup(true)
                    local size = self.m_anlayer11:getContentSize()
                    for i=1,5 do
                        local particle1= LuaController:createParticleForLua(self.rootPath .. "/particles/oyFireworks_"..string.format(i))
                        if nil ~= particle1 then
                            self.m_anlayer11:addChild(particle1)
                            particle1:setPosition(ccp(size.width*0.5,size.height*0.5))
                            particle1:setScale(0.5)
                        end
                    end
                end
            end
            --tick2()
            self.effid2 = tonumber(self.m_anlayer11:getScheduler():scheduleScriptFunc(tick2, 3.5, false))
        end

        if nil~= self.m_anlayer12 then
            local function tick3()
                if nil ~= self.m_anlayer12 then
                    self.m_anlayer12:removeAllChildrenWithCleanup(true)
                    local size = self.m_anlayer12:getContentSize()
                    for i=1,5 do
                        local particle1= LuaController:createParticleForLua(self.rootPath .. "/particles/oyrFireworks_"..string.format(i))
                        if nil ~= particle1 then
                            self.m_anlayer12:addChild(particle1)
                            particle1:setPosition(ccp(size.width*0.5,size.height*0.5))
                            particle1:setScale(1.5)
                        end
                    end
                end
            end
            --tick3()
            self.effid3 = tonumber(self.m_anlayer12:getScheduler():scheduleScriptFunc(tick3, 2.8, false))
        end

        if nil~= self.m_anlayer13 then
            local function tick4()
                if nil ~= self.m_anlayer13 then
                    self.m_anlayer13:removeAllChildrenWithCleanup(true)
                    local size = self.m_anlayer13:getContentSize()
                    for i=1,5 do
                        local particle1= LuaController:createParticleForLua(self.rootPath .. "/particles/oyFireworks_"..string.format(i))
                        if nil ~= particle1 then
                            self.m_anlayer13:addChild(particle1)
                            particle1:setPosition(ccp(size.width*0.5,size.height*0.5))
                            particle1:setScale(1.2)
                        end
                    end
                end
            end
            tick4()
            self.effid4 = tonumber(self.m_anlayer13:getScheduler():scheduleScriptFunc(tick4, 4.5, false))
        end

        if nil~= self.m_anlayer14 then
            local function tick5()
                if nil ~= self.m_anlayer14 then
                    self.m_anlayer14:removeAllChildrenWithCleanup(true)
                    local size = self.m_anlayer14:getContentSize()
                    for i=1,5 do
                        local particle1= LuaController:createParticleForLua(self.rootPath .. "/particles/oyFireworks_"..string.format(i))
                        if nil ~= particle1 then
                            self.m_anlayer14:addChild(particle1)
                            particle1:setPosition(ccp(size.width*0.5,size.height*0.5))
                            particle1:setScale(1.5)
                        end
                    end
                end
            end
            --tick5()
            self.effid5 = tonumber(self.m_anlayer14:getScheduler():scheduleScriptFunc(tick5, 5.5, false))
        end
    end
end
function GoldExchangeView:closeTween()
    if self.isRemove == true then
        return
    end
    self.isRemove = true
    --self:removeAllEvent()
    if nil ~= self.ccbNode then
        self.ccbNode:runAction(CCScaleTo:create(0.2, 0))
        self.parentNode:runAction(CCSequence:createWithTwoActions(CCDelayTime:create(0.2), CCHide:create()))
        local pPar = self.parentNode:getParent()
        if pPar ~= nil then
            for i=1,3 do
                --local particle1 = LuaController:createParticleForLua("particle/Collection_expF_" .. string.format(i))
                local particle2 = LuaController:createParticleForLua("particle/Collection_expF_" .. string.format(i))
                if nil ~=  particle2 then
                    if (i==1) then
                        --particle1:setStartColor(ccc4f(1.0, 0.77, 0, 1.0))
                        --particle1:setEndColor(ccc4f(0, 0, 0, 0))
                        --particle1:setPositionType(kCCPositionTypeRelative)

                        particle2:setStartColor(ccc4f(1.0, 0.77, 0, 1.0))
                        particle2:setEndColor(ccc4f(0, 0, 0, 0))
                        particle2:setPositionType(kCCPositionTypeRelative)
                    elseif (i==2) then
                        --particle1:setStartColor(ccc4f(1.0, 0.55, 0, 1.0))
                        --particle1:setEndColor(ccc4f(1, 0.96, 0.5, 0))
                        --particle1:setPositionType(kCCPositionTypeFree)

                        particle2:setStartColor(ccc4f(1.0, 0.55, 0, 1.0))
                        particle2:setEndColor(ccc4f(1, 0.96, 0.5, 0))
                        particle2:setPositionType(kCCPositionTypeFree)
                    else
                        --particle1:setStartColor(ccc4f(1.0, 0.9, 0, 1.0))
                        --particle1:setEndColor(ccc4f(0, 0, 0, 0))
                        --particle1:setPositionType(kCCPositionTypeRelative)

                        particle2:setStartColor(ccc4f(1.0, 0.9, 0, 1.0))
                        particle2:setEndColor(ccc4f(0, 0, 0, 0))
                        particle2:setPositionType(kCCPositionTypeRelative)
                    end

                    local winSize = CCDirector:sharedDirector():getWinSize()
                    --particle1:setPosition(ccp(winSize.width*0.5,winSize.height*0.5))
                    --particle1:setTag(1000+i)
                    --pPar:addChild(particle1)

                    particle2:setPosition(ccp(winSize.width*0.5,winSize.height*0.5))
                    particle2:setTag(2000+i)
                    pPar:addChild(particle2)
                    --local act1 = CCMoveTo:create(0.5, ccp(winSize.width-70,winSize.height-155))
                    --local ease1 = CCEaseExponentialOut:create(act1)
                    --particle1:runAction(ease1)

                    local act2 = CCMoveTo:create(0.5, ccp(winSize.width-40,winSize.height-35))
                    local ease2 = CCEaseExponentialOut:create(act2)
                    particle2:runAction(ease2)
                end
            end
        end
        performWithDelay(self.m_touchNode,delayDealWithFunc({target = self}),0.5)
    end
end
function GoldExchangeView:onClickCostBtn()
    local itemid = string.format(self.data[1])
    LuaController:callPayment(itemid)
    if(self.isRemove == false ) then
        self:destroySelf()
    end
end
function GoldExchangeView:onCloseBtnClick()
   self:closeTween()
end
function GoldExchangeView:destroySelf()
    --self:removeAllEvent()
    LuaController:removeAllPopup()
    --loadCommonResource(7,false)
    --loadCommonResource(8,false)
    --if nil~= self.equips then
    --    loadCommonResource(308,false)
    --end
    --releaseLuaResource(self.rootPath .. "/resources/"..self.data[19])
end
function GoldExchangeView:onExit()
    --print ("gold_exchange_view_exit")
    self:removeAllEvent()
    --if nil~= self.equips then
    --    loadCommonResource(308,false)
    --end
    releaseLuaResource(self.rootPath .. "/resources/"..self.data[19])
end
function GoldExchangeView:onPackageBtnClick()
    if nil ~= self.data[21] then
        if(self.isRemove == false ) then
            self:destroySelf()
        end
        local itemid = string.format(self.data[1])
        LuaController:toSelectUser(itemid,false,1)
    end
end
function GoldExchangeView:removeAllEvent()
    if self.parentNode ~= nil then
        local pPar = self.parentNode:getParent()
        --print ("gold_exchange_view_exit_1")
        if nil ~= pPar then
            for i=1,3 do
                --if pPar:getChildByTag(1000+i) ~= nil then
                --    pPar:removeChildByTag(1000+i,true)
                --end
                --print ("gold_exchange_view_exit_2")
                if pPar:getChildByTag(2000+i) ~= nil then
                    --print ("gold_exchange_view_exit_3_" .. i)
                    pPar:removeChildByTag(2000+i,true)
                end
            end
        end
    end

    GoldExchangeLuaView.onClickCostBtn = nil
    GoldExchangeLuaView.onCloseBtnClick = nil
    GoldExchangeLuaView.onPackageBtnClick = nil
    if nil~= self.m_touchNode then
        self.m_touchNode:unregisterScriptTouchHandler()
    end
    if nil ~= self.m_costBtn then
        self.m_costBtn:setTouchEnabled(false)
    end
    if nil ~=self.m_timeLabel then
        self.m_timeLabel:stopAllActions()
    end
    if nil ~= self.ccbNode then
        self.ccbNode:unregisterScriptHandler()
    end
end
function GoldExchangeView:callbackFunc()
	if(self.isRemove == true) then
        self:destroySelf()
    end
end
function GoldExchangeView:scheduleBack()
    --print "_____scheduleBack"
    if nil == self.data[14] then
        return
    end
    --print("load resource scheduleBack1")
    if nil ==  self.data[13] then
        return
    end
    if nil == self.m_soleOutSpr then
        return
    end
    if nil == self.m_costBtn then
        return
    end

    --print("load resource scheduleBack2")
    if self.m_timeLabel ~= nil then
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
            self.m_soleOutSpr:setVisible(false)
            self.m_costBtn:setEnabled(true)
            if(self.isRemove == false ) then
                self:destroySelf()
            end
        end 
    end
    --print("load resource scheduleBack3")
end
function GoldExchangeView:onTouchBegan(x, y)
	--print "touch began"
    if(self.isRemove==false) then
        if(nil ~= self.m_touchNode) then
            if(nil ~= self.m_touchNode:getParent()) then
                local pos = self.m_touchNode:getParent():convertToNodeSpace(ccp(x,y))
                local rect = self.m_touchNode:boundingBox()
                if(rect:containsPoint(pos) == true) then
                    return false
                end 
            end
        end
        if nil ~= self.m_costBtn then
            if(nil ~= self.m_costBtn:getParent()) then
                local pos = self.m_costBtn:getParent():convertToNodeSpace(ccp(x,y))
                local rect = self.m_costBtn:boundingBox()
                if(rect:containsPoint(pos) == true) then
                    return false
                end 
            end
        end
        if nil ~= self.m_packageBtn then
            if(nil ~= self.m_packageBtn:getParent()) then
                local pos = self.m_packageBtn:getParent():convertToNodeSpace(ccp(x,y))
                local rect = self.m_packageBtn:boundingBox()
                if(rect:containsPoint(pos) == true) then
                    return false
                end 
            end
        end
    end
    return true
end
function GoldExchangeView:onTouchMoved(x, y)
	print "touch move"
end
function GoldExchangeView:onTouchEnded(x, y)
    self:onCloseBtnClick()
end
function GoldExchangeView:initItems()
    self.items = nil
    if nil == self.data[7] then
        return
    end
    local itemsStr = string.format(self.data[7])
    if(itemsStr == "") then
        return
    end
    
    --print(itemsStr)
    self.items = {}
    local itemIndex = 1
    local itemSIndex = 1
    local itemFIndex = string.find(itemsStr,"|",itemSIndex)
    while(true) do
        local itemValueTmp = string.sub(itemsStr,itemSIndex,itemFIndex-1)
        local itemFindIndex =  string.find(itemValueTmp,";",1);
        local tabelValue1 = string.sub(itemValueTmp,1,itemFindIndex-1)
        local tabelVaule2 = string.sub(itemValueTmp,itemFindIndex+1,string.len(itemValueTmp))
        local iValue = {tabelValue1,tabelVaule2}
        self.items[itemIndex] = iValue;
        --print("items" .. string.format(itemIndex) .. ":" .. tabelValue1 .."," .. tabelVaule2)
        itemIndex = itemIndex + 1
        itemSIndex =  itemFIndex + 1
        itemFIndex = string.find(itemsStr,"|",itemSIndex)
        if itemFIndex == nil then
            itemValueTmp = string.sub(itemsStr,itemSIndex,string.len(itemsStr))
            itemFindIndex =  string.find(itemValueTmp,";",1)
            tabelValue1 = string.sub(itemValueTmp,1,itemFindIndex-1)
            tabelVaule2 = string.sub(itemValueTmp,itemFindIndex+1,string.len(itemValueTmp))
            iValue = {tabelValue1,tabelVaule2}
            self.items[itemIndex] = iValue;
            --print("items" .. string.format(itemIndex) .. ":" .. tabelValue1 .."," .. tabelVaule2)
            return
        end
    end
end
function GoldExchangeView:initEquips()
    self.equips = nil
    if nil == self.data[20] then
        return
    end
    local itemsStr = string.format(self.data[20])
    if(itemsStr == "") then
        return
    end
    
    self.equips = {}
    local itemIndex = 1
    local itemSIndex = 1
    local itemFIndex = string.find(itemsStr,"|",itemSIndex)
    if itemFIndex == nil then
        local itemValueTmp = string.sub(itemsStr,itemSIndex,string.len(itemsStr))
        local itemFindIndex =  string.find(itemValueTmp,";",1)
        local tabelValue1 = string.sub(itemValueTmp,1,itemFindIndex-1)
        local tabelVaule2 = string.sub(itemValueTmp,itemFindIndex+1,string.len(itemValueTmp))
        local iValue = {tabelValue1,tabelVaule2}
        self.equips[itemIndex] = iValue;
        print("equips" .. string.format(itemIndex) .. ":" .. tabelValue1 .."," .. tabelVaule2)
        return
    end

    while(true) do
        local itemValueTmp = string.sub(itemsStr,itemSIndex,itemFIndex-1)
        local itemFindIndex =  string.find(itemValueTmp,";",1);
        local tabelValue1 = string.sub(itemValueTmp,1,itemFindIndex-1)
        local tabelVaule2 = string.sub(itemValueTmp,itemFindIndex+1,string.len(itemValueTmp))
        local iValue = {tabelValue1,tabelVaule2}
        self.equips[itemIndex] = iValue;
        --print("equips" .. string.format(itemIndex) .. ":" .. tabelValue1 .."," .. tabelVaule2)
        itemIndex = itemIndex + 1
        itemSIndex =  itemFIndex + 1
        itemFIndex = string.find(itemsStr,"|",itemSIndex)
        if itemFIndex == nil then
            itemValueTmp = string.sub(itemsStr,itemSIndex,string.len(itemsStr))
            itemFindIndex =  string.find(itemValueTmp,";",1)
            tabelValue1 = string.sub(itemValueTmp,1,itemFindIndex-1)
            tabelVaule2 = string.sub(itemValueTmp,itemFindIndex+1,string.len(itemValueTmp))
            iValue = {tabelValue1,tabelVaule2}
            self.equips[itemIndex] = iValue;
            --print("equips" .. string.format(itemIndex) .. ":" .. tabelValue1 .."," .. tabelVaule2)
            return
        end
    end
end