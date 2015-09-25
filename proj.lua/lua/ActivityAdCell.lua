require "extern"
require "CCBReaderLoad"
require "common"

ActivityAdLuaCell = ActivityAdLuaCell or {}
ccb["ActivityAdLuaCell"] = ActivityAdLuaCell

ActivityAdCell = class("ActivityAdCell", function() return CCLayer:create() end)
ActivityAdCell.__index = ActivityAdCell

function ActivityAdCell:create( path, params)
    local node = ActivityAdCell.new()
    node:init(path, params)
    return node
end

function ActivityAdCell:init( path, params )
    self.rootPath = string.format(path)
    self.params = params
    loadLuaResource(self.rootPath .. "/resources/activity_" .. self.params.id .. ".plist")

    local proxy = CCBProxy:create()
    local ccbiURL = self.rootPath .. "/ccbi/Activity" .. self.params.id .. "AdLuaCell.ccbi"
    local node = CCBReaderLoad(ccbiURL, proxy, true, "ActivityAdLuaCell")
    local layer = tolua.cast(node, "CCLayer")

    if nil ~= ActivityAdLuaCell["m_nameLabel"] then
        self.m_nameLabel = tolua.cast(ActivityAdLuaCell["m_nameLabel"], "CCLabelTTF")
        if nil ~= self.m_nameLabel then
            self.m_nameLabel:setString(self.params.name)
            if self.params.id ~= "57006" then
                local tc = self.m_nameLabel:getColor()
                tc.r = 255
                tc.g = 219
                tc.b = 117
                if self.params.id == "57014" then
                    tc.r = 144
                    tc.g = 234
                    tc.b = 255
                end
                if self.params.id == "57015" then
                    tc.r = 0
                    tc.g = 228
                    tc.b = 255
                end
                self.m_nameLabel:setColor(tc)
            end
        end
    end

    if nil ~= ActivityAdLuaCell["m_desLabel"] then
        self.m_desLabel = tolua.cast(ActivityAdLuaCell["m_desLabel"], "CCLabelTTF")
        if nil ~= self.m_desLabel then
            self.m_desLabel:setString(self.params.desc)
            if self.params.id ~= "57006" then
                local tc = self.m_desLabel:getColor()
                tc.r = 0
                tc.g = 249
                tc.b = 0
                if self.params.id == "57014" then
                    tc.r = 209
                    tc.g = 132
                    tc.b = 189
                end
                if self.params.id == "57015" then
                    tc.r = 246
                    tc.g = 255
                    tc.b = 255
                end
                self.m_desLabel:setColor(tc)
            end
        end
    end

    if nil ~= ActivityAdLuaCell["m_timeLabel"] then
        self.m_timeLabel = tolua.cast(ActivityAdLuaCell["m_timeLabel"], "CCLabelTTF")
    end

    if nil ~= node then
        local function tick(  )
            if nil == self.m_timeLabel then return end
            if nil ~= self.m_timeLabel then
                self.m_timeLabel:setString("")
                return
            end
            --[[
            local nowTime = LuaController:getWorldTime()
            local startTime = tonumber(self.params.startTime)
            local endTime = tonumber(self.params.endTime)
            if nowTime < startTime then
                self.m_timeLabel:setString(string.format(LuaController:getLang1("105804", string.format(LuaController:getSECLang(startTime - nowTime)))))
            elseif nowTime< endTime then
                self.m_timeLabel:setString(string.format(LuaController:getLang1("105805", string.format(LuaController:getSECLang(endTime - nowTime)))))
            else
                self.m_timeLabel:setString(string.format(LuaController:getLang("105800")))
            end]]
        end
        local function eventHandler( eventType )
            if eventType == "enter" then
                tick()
                self.m_entryId = tonumber(node:getScheduler():scheduleScriptFunc(tick, 1, false))
            elseif eventType == "exit" then
                if nil ~= self.m_entryId then
                    node:getScheduler():unscheduleScriptEntry(self.m_entryId)
                end
            elseif eventType == "cleanup" then
                releaseLuaResource(self.rootPath .. "/resources/activity_" .. self.params.id)
                node:unregisterScriptHandler()
            end
        end
        node:registerScriptHandler(eventHandler)
        self:addChild(node)
    end
end