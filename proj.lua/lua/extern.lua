
--Create an class.
function class(classname, super)
    local superType = type(super)
    local cls

    if superType ~= "function" and superType ~= "table" then
        superType = nil
        super = nil
    end

    if superType == "function" or (super and super.__ctype == 1) then
        -- inherited from native C++ Object
        cls = {}

        if superType == "table" then
            -- copy fields from super
            for k,v in pairs(super) do cls[k] = v end
            cls.__create = super.__create
            cls.super    = super
        else
            cls.__create = super
        end

        cls.ctor    = function() end
        cls.__cname = classname
        cls.__ctype = 1

        function cls.new(...)
            local instance = cls.__create(...)
            -- copy fields from class to native object
            for k,v in pairs(cls) do instance[k] = v end
            instance.class = cls
            instance:ctor(...)
            return instance
        end

    else
        -- inherited from Lua Object
        if super then
            cls = clone(super)
            cls.super = super
        else
            cls = {ctor = function() end}
        end

        cls.__cname = classname
        cls.__ctype = 2 -- lua
        cls.__index = cls

        function cls.new(...)
            local instance = setmetatable({}, cls)
            instance.class = cls
            instance:ctor(...)
            return instance
        end
    end

    return cls
end

function schedule(node, callback, delay)
    local delay = CCDelayTime:create(delay)
    local callfunc = CCCallFunc:create(callback)
    local sequence = CCSequence:createWithTwoActions(delay, callfunc)
    local action = CCRepeatForever:create(sequence)
    node:runAction(action)
    return action
end

function performWithDelay(node, callback, delay)
    local delay = CCDelayTime:create(delay)
    local callfunc = CCCallFunc:create(callback)
    local sequence = CCSequence:createWithTwoActions(delay, callfunc)
    node:runAction(sequence)
    return sequence
end

function delayDealWithFunc(...)               
    local args = ...
    local ret = function ()
        args.target:callbackFunc()
    end
    return ret        
end
function scheduleDealWithFunc( ... )
    local args = ...
    local ret = function ()
        args.target:scheduleBack()
    end
    return ret
end
function isTouchInside( node, x, y )
    if nil == node then return false end
    if nil == node:getParent() then return false end
    local pos = node:getParent():convertToNodeSpace(ccp(x, y))
    local rect = node:boundingBox()
    if rect:containsPoint(pos) == true then 
        return true
    else
        return false
    end
end
function parseObject(obj)
    if nil == obj then return nil end
    local tempObj = tolua.cast(obj, "CCObject")
    local objType = LuaController:getObjectType(tempObj)
    print(objType)
    if "NULL" == objType then return nil end
    if "CCString" == objType then
        local ret = nil
        local stringObj = tolua.cast(obj, "CCString")
        ret = stringObj:getCString()
        return ret
    end
    if "CCArray" == objType then
        local ret = {}
        local arr = tolua.cast(obj, "CCArray")
        local arrCnt = arr:count()
        for i = 1, arrCnt do
            local arrObj = arr:objectAtIndex(i - 1)
            arrObj = tolua.cast(arrObj, "CCObject")
            ret[i] = parseObject(arrObj)
        end
        return ret
    end
    if "CCDictionary" == objType then
        local ret = {}
        local dic = tolua.cast(obj, "CCDictionary")
        local keyArr = dic:allKeys()
        local keyArrCnt = keyArr:count()
        for i = 1, keyArrCnt do
            local key = keyArr:objectAtIndex(i - 1)
            key = tolua.cast(key, "CCString")
            key = key:getCString()
            local dicObj = tolua.cast(dic:objectForKey(key), "CCObject")
            ret[key] = parseObject(dicObj)
        end
        return ret
    end
    if "CCInteger" == objType then
        local ret = nil
        local intObj = tolua.cast(obj, "CCInteger")
        ret = intObj:getValue()
        return ret
    end
end




