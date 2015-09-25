require "extern"
require "CCBReaderLoad"
require "common"

GoldExchangeLuaIcon  = GoldExchangeLuaIcon or {}
ccb["GoldExchangeLuaIcon"] = GoldExchangeLuaIcon

GoldExchangeIcon = class("GoldExchangeIcon",
	function()
        return CCLayer:create() 
	end
)
GoldExchangeIcon.__index = GoldExchangeIcon
function GoldExchangeIcon:create(parent,path,params)
	local node = GoldExchangeIcon.new()
	node:init(parent,path,params)
	return node
end
function GoldExchangeIcon:init(parent,path,params)
	local strPath = string.format(path)
    self.rootPath = strPath
    self.parentNode = parent
    self:setData(params)

    loadLuaResource(self.rootPath .. "/resources/".. self.data[19] .."icon.plist")

    local ccbiUrl = strPath .. "/" .. self:getCCBI()
    --print(ccbiUrl)
    local proxy = CCBProxy:create()
   	local ccbnode = CCBReaderLoad(ccbiUrl,proxy,true,"GoldExchangeLuaIcon")
    if ccbnode ~= nil then
        local layer = tolua.cast(ccbnode,"CCLayer")
        if nil ~= GoldExchangeLuaIcon["m_timeLabel"] then
            self.m_timeLabel = tolua.cast(GoldExchangeLuaIcon["m_timeLabel"],"CCLabelTTF")
        end
        if nil ~= GoldExchangeLuaIcon["m_ani1"] then
            self.m_ani1Layer = tolua.cast(GoldExchangeLuaIcon["m_ani1"],"CCLayerColor")
        end
        if nil ~= GoldExchangeLuaIcon["m_ani2"] then
            self.m_ani2Layer = tolua.cast(GoldExchangeLuaIcon["m_ani2"],"CCLayerColor")
        end
        self:initParticale()
        self:initSkeleton()
        if nil ~= self.parentNode then
            self.parentNode:addChild(ccbnode)
        end
        if self.data[19] == "normal_gift" then
            if nil ~= self.data[10] then
                local keyStr = string.format(self.data[10])
                local titleStr = string.format(LuaController:getLang(keyStr))
                self.m_timeLabel:setString(titleStr)
            end
        else
            self:scheduleBack()
            schedule(self.m_timeLabel,scheduleDealWithFunc({target = self}),1)
        end
    end
end
function GoldExchangeIcon:setData(params)
	local paramsStr = string.format(params)
    --print("params:" .. paramsStr)
    self.data = {}
    local index = 1
    local startI = 1
    local fIndex = string.find(paramsStr,",",startI)
    local tmpValue = "" 
    while (true) do
        tmpValue = string.sub(paramsStr,startI,fIndex-1)
        --print("params" .. string.format(index) .. ":" .. tmpValue)
        self.data[index] = tmpValue
        index = index + 1
        startI = fIndex + 1
        fIndex = string.find(paramsStr,",",startI)
        if (fIndex == nil) then
            tmpValue = string.sub(paramsStr,startI,string.len(paramsStr))
            --print("params" .. string.format(index) .. ":" .. tmpValue)
            self.data[index] = tmpValue
            break
        end
    end
end
function GoldExchangeIcon:removeAllEvent()
    releaseLuaResource(self.rootPath .. "/resources/".. self.data[19] .."icon")
    if self.m_timeLabel ~= nil then
        self.m_timeLabel:stopAllActions()
    end
end
function GoldExchangeIcon:scheduleBack()
    --print "_____scheduleBack"
    if self.data ~= nil then
    	if nil ~= self.m_timeLabel then
            local curTime = LuaController:getWorldTime()
            local expTime = tonumber(self.data[14])
            local endTime = tonumber(self.data[13])
            local lastTime = endTime - curTime
            if expTime>0 then
                local count =(endTime- curTime)/(expTime*3600)
                lastTime=endTime-count*(expTime*3600)-curTime
            else
                lastTime = endTime - curTime
            end
            local timeStr = string.format(LuaController:getSECLang(lastTime))
            self.m_timeLabel:setString(timeStr)
            local isShow = 1
            if ((endTime - curTime) <= 0) then
                isShow = 0
            end
            if (string.format(self.data[15]) == "1") then
                isShow = 0
            end
            if isShow == 0 then
                self.m_timeLabel:setString("")
                self:removeAllEvent()
                if nil ~= self.parentNode then
                    self.parentNode:setVisible(false)
                end
            else
                if nil ~= self.parentNode then
                    self.parentNode:setVisible(true)
                end
            end
        end
    end
end

function GoldExchangeIcon:getCCBI()
    local actName = string.format(self.data[19])
    return "ccbi/GoldExchange"..actName.."LuaIcon.ccbi"
end
function GoldExchangeIcon:initSkeleton()
    if nil==self.m_ani1Layer then
        return
    end
    local skeletonPath = nil
    local targetPlatform = CCApplication:sharedApplication():getTargetPlatform()
    if (kTargetAndroid == targetPlatform) then
        targetPlatform = "android/"
    else
        targetPlatform = "ios/"
    end
    if self.data[19] == "resource_gift" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "normal_giftditu-zylb")
        if bFile == false then
            return
        end
        --print("build_alliance_initSkeleton")
        self.m_ani1Layer:removeAllChildrenWithCleanup(true)
        local altas = self.rootPath .. "/skeleton/"..targetPlatform .."normal_giftditu-zylb.atlas"
        local json = self.rootPath .. "/skeleton/"..targetPlatform .."normal_giftditu-zylb.json"
        local aniName = "animation"
        LuaController:addSkeletonAnimation(self.m_ani1Layer,json,altas,aniName,1)
        self.m_ani1Layer:setScale(0.9)
        self.m_ani1Layer:setPosition(ccp(0,-50))
    elseif self.data[19] == "oneyear" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "icon_oneyears")
        if bFile == false then
            return
        end
        self.m_ani1Layer:removeAllChildrenWithCleanup(true)
        local altas = self.rootPath .. "/skeleton/"..targetPlatform .."icon_oneyears.atlas"
        local json = self.rootPath .. "/skeleton/"..targetPlatform .."icon_oneyears.json"
        local aniName = "animation"
        LuaController:addSkeletonAnimation(self.m_ani1Layer,json,altas,aniName,1)
        self.m_ani1Layer:setScale(1)
        self.m_ani1Layer:setPosition(ccp(-10,-55))
    elseif self.data[19] == "cn_fight" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_cn_fight")
        if bFile == false then
            return
        end
        self.m_ani1Layer:removeAllChildrenWithCleanup(true)
        local altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_cn_fight.atlas"
        local json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_cn_fight.json"
        local aniName = "animation"
        LuaController:addSkeletonAnimation(self.m_ani1Layer,json,altas,aniName,1)
        self.m_ani1Layer:setScale(1)
        self.m_ani1Layer:setPosition(ccp(-10,-55))
    elseif self.data[19] == "jp_fight" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_jp_fight")
        if bFile == false then
            return
        end
        self.m_ani1Layer:removeAllChildrenWithCleanup(true)
        local altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_jp_fight.atlas"
        local json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_jp_fight.json"
        local aniName = "animation"
        LuaController:addSkeletonAnimation(self.m_ani1Layer,json,altas,aniName,1)
        self.m_ani1Layer:setScale(1)
        self.m_ani1Layer:setPosition(ccp(-10,-55))
    elseif self.data[19] == "qixi" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_qixi")
        if bFile == false then
            return
        end
        self.m_ani1Layer:removeAllChildrenWithCleanup(true)
        local altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_qixi.atlas"
        local json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_qixi.json"
        local aniName = "animation"
        LuaController:addSkeletonAnimation(self.m_ani1Layer,json,altas,aniName,1)
        self.m_ani1Layer:setScale(1)
        self.m_ani1Layer:setPosition(ccp(-10,-55))
    elseif self.data[19] == "peace_gift" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_peace_gift")
        if bFile == false then
            return
        end
        self.m_ani1Layer:removeAllChildrenWithCleanup(true)
        local altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_peace_gift.atlas"
        local json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_peace_gift.json"
        local aniName = "animation"
        LuaController:addSkeletonAnimation(self.m_ani1Layer,json,altas,aniName,1)
        self.m_ani1Layer:setScale(1)
        self.m_ani1Layer:setPosition(ccp(-10,-55))
    end
end
function GoldExchangeIcon:initParticale()
    if self.data[19] == "wargift" then
        if nil ~= self.m_ani1Layer then
            local size = self.m_ani1Layer:getContentSize()
            local particles1 = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesWar_1")
            if nil ~= particles1 then
                self.m_ani1Layer:addChild(particles1)
                particles1:setPosition(ccp(size.width*0.5,size.height*0.2))
            end
            local particles2 = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesWar_2")
            if nil ~= particles2 then
                self.m_ani1Layer:addChild(particles2)
                particles2:setPosition(ccp(size.width*0.5,size.height*0.2))
            end
            local particles3 = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesWar_3")
            if nil ~=particles3 then
                self.m_ani1Layer:addChild(particles3)
                particles3:setPosition(ccp(size.width*0.5,size.height*0.2))
            end
            if nil ~= self.m_ani2Layer then
                local particles4 = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesWar_4")
                if nil ~= particles4 then
                    self.m_ani2Layer:addChild(particles4)
                    particles4:setPosition(ccp(size.width*0.6,size.height*0.37))
                end
                local particles5 = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesWar_5")
                if nil ~= particles5 then
                    self.m_ani2Layer:addChild(particles5)
                    particles5:setPosition(ccp(size.width*0.4,size.height*0.37))
                end
                local particles6 = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesWar_6")
                if nil ~= particles6 then
                    self.m_ani2Layer:addChild(particles6)
                    particles6:setPosition(ccp(size.width*0.5,size.height*0.5))
                end
            end
        end
    elseif self.data[19] == "kingdom" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_1")
            if nil ~= particlesa then
                self.m_ani2Layer:addChild(particlesa)
                particlesa:setPosition(ccp(size.width*0.5,size.height*0.5))
            end
            local particlesb = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_2")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(ccp(size.width*0.5,size.height*0.5))
            end  
            local particlesc = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_3")
            if nil ~= particlesc then
                self.m_ani2Layer:addChild(particlesc)
                particlesc:setPosition(ccp(size.width*0.5,size.height*0.5))
            end
        end
    elseif self.data[19] == "build_kingdom" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_1")
            if nil ~= particlesa then
                self.m_ani2Layer:addChild(particlesa)
                particlesa:setPosition(ccp(size.width*0.5,size.height*0.5))
            end
            local particlesb = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_2")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(ccp(size.width*0.5,size.height*0.5))
            end  
            local particlesc = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_3")
            if nil ~= particlesc then
                self.m_ani2Layer:addChild(particlesc)
                particlesc:setPosition(ccp(size.width*0.5,size.height*0.5))
            end
        end
    elseif self.data[19] == "alliance" then
        if nil ~= self.m_ani1Layer then
            local size = self.m_ani1Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesAlliance_1")
            if nil ~= particlesa then
                self.m_ani1Layer:addChild(particlesa)
                particlesa:setPosition(ccp(size.width*0.5,size.height*0.5))
            end
            if nil ~= self.m_ani2Layer then
                local particlesb = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesAlliance_2")
                if nil ~= particlesb then
                    self.m_ani2Layer:addChild(particlesb)
                    particlesb:setPosition(ccp(size.width*0.38,size.height*0.65))
                end
                local particlesc = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesAlliance_3")
                if nil ~= particlesc then
                    self.m_ani2Layer:addChild(particlesc)
                    particlesc:setPosition(ccp(size.width*0.62,size.height*0.65))
                end
            end
        end
    elseif self.data[19] == "alliance1" then
        if nil ~= self.m_ani1Layer then
            local size = self.m_ani1Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesAlliance_1")
            if nil ~= particlesa then
                self.m_ani1Layer:addChild(particlesa)
                particlesa:setPosition(ccp(size.width*0.5,size.height*0.5))
            end
            if nil ~= self.m_ani2Layer then
                local particlesb = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesAlliance_2")
                if nil ~= particlesb then
                    self.m_ani2Layer:addChild(particlesb)
                    particlesb:setPosition(ccp(size.width*0.38,size.height*0.65))
                end
                local particlesc = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesAlliance_3")
                if nil ~= particlesc then
                    self.m_ani2Layer:addChild(particlesc)
                    particlesc:setPosition(ccp(size.width*0.62,size.height*0.65))
                end
            end
        end
    elseif self.data[19] == "king" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/Rose_3")
            if nil ~= particlesa then
                self.m_ani2Layer:addChild(particlesa)
                particlesa:setPosition(ccp(size.width*0.5,size.height*0.5))
            end
            local particlesb = LuaController:createParticleForLua(self.rootPath .. "/particles/VIPGlow_3")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(ccp(size.width*0.5,size.height*0.5))
            end
        end
    elseif self.data[19] == "war_resource" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/Rose_3")
            if nil ~= particlesa then
                self.m_ani2Layer:addChild(particlesa)
                particlesa:setPosition(ccp(size.width*0.5,size.height*0.5))
            end
            local particlesb = LuaController:createParticleForLua(self.rootPath .. "/particles/VIPGlow_3")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(ccp(size.width*0.5,size.height*0.5))
            end
        end
    elseif self.data[19] == "spring" then
        if nil ~= self.m_ani1Layer then
            local size = self.m_ani1Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/Rose_3")
            if nil ~= particlesa then
                self.m_ani1Layer:addChild(particlesa)
                particlesa:setPosition(ccp(size.width*0.5,size.height*0.5))
            end
        end
    elseif self.data[19] == "build_alliance" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesb = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_2")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(ccp(size.width*0.5,size.height*0.5))
            end  
            local particlesc = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_3")
            if nil ~= particlesc then
                self.m_ani2Layer:addChild(particlesc)
                particlesc:setPosition(ccp(size.width*0.5,size.height*0.5))
            end
        end
    elseif self.data[19] == "equip_build" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesEquipBuild_1")
            if nil ~= particlesa then
                self.m_ani2Layer:addChild(particlesa)
                particlesa:setPosition(ccp(size.width*0.5,0))
            end
            local particlesb= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesEquipBuild_2")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(ccp(size.width*0.5,0))
            end
            local particlesc= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesEquipBuild_3")
            if nil ~= particlesc then
                self.m_ani2Layer:addChild(particlesc)
                particlesc:setPosition(ccp(size.width*0.5,0))
            end
        end
    elseif self.data[19] == "trap_build" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesEquipBuild_1")
            if nil ~= particlesa then
                self.m_ani2Layer:addChild(particlesa)
                particlesa:setPosition(ccp(size.width*0.5,size.height*0.5))
            end
            local particlesb= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesEquipBuild_2")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(ccp(size.width*0.5,size.height*0.5))
            end
            local particlesc= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesEquipBuild_3")
            if nil ~= particlesc then
                self.m_ani2Layer:addChild(particlesc)
                particlesc:setPosition(ccp(size.width*0.5,size.height*0.5))
            end
        end
    elseif self.data[19] == "kingdom_war" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesEquipBuild_1")
            if nil ~= particlesa then
                self.m_ani2Layer:addChild(particlesa)
                particlesa:setPosition(ccp(size.width*0.5,0))
            end
            local particlesb= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesEquipBuild_2")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(ccp(size.width*0.5,0))
            end
            local particlesc= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesEquipBuild_3")
            if nil ~= particlesc then
                self.m_ani2Layer:addChild(particlesc)
                particlesc:setPosition(ccp(size.width*0.5,0))
            end
        end
    elseif self.data[19] == "ru_gift" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_1")
            if nil ~= particlesa then
                self.m_ani2Layer:addChild(particlesa)
                particlesa:setPosition(ccp(size.width*0.5,size.height*0.5))
            end
            local particlesb = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_2")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(ccp(size.width*0.5,size.height*0.5))
            end  
            local particlesc = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_3")
            if nil ~= particlesc then
                self.m_ani2Layer:addChild(particlesc)
                particlesc:setPosition(ccp(size.width*0.5,size.height*0.5))
            end
        end
    elseif self.data[19] == "first" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_1")
            if nil ~= particlesa then
                self.m_ani2Layer:addChild(particlesa)
                particlesa:setPosition(ccp(size.width*0.5,0))
            end
            local particlesb = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_2")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(ccp(size.width*0.5,size.height*0.5))
            end  
            local particlesc = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_3")
            if nil ~= particlesc then
                self.m_ani2Layer:addChild(particlesc)
                particlesc:setPosition(ccp(size.width*0.5,size.height*0.5))
            end
        end
    end
end