function __G__TRACKBACK__(msg)
    print("----------------------------------------")
    print("LUA ERROR: " .. tostring(msg) .. "\n")
    print(debug.traceback())
    print("----------------------------------------")
end

collectgarbage("setpause", 100) 
collectgarbage("setstepmul", 5000)

function addPackagePath(path)
	local strPath = string.format(path)
	local packagePath = string.format(package.path)
	packagePath = strPath .. "?.lua;" .. packagePath
	package.path = packagePath
end
function showGoldExchangeView(path,params)
	--local strPath = string.format(path)
	--local packagePath = string.format(package.path)
	--packagePath = strPath .. "?.lua;" .. packagePath
	--package.path = packagePath
	require("GoldExchangeView")
	local view = LuaController:getInstance():getSaleViewContainer()
	if nil ~= view then
		GoldExchangeView:create(view,path,params)
	end
end
function createGoldExchangeAdvCell(path,params)
	local view = LuaController:getInstance():getAdvCellContainer()
	if nil ~= view then
		require("GoldExchangeAdvCell")
		GoldExchangeAdvCell:create(view,path,params)
	end
end
function createGoldExchangeAdvCell1(path,params)
	local view = LuaController:getInstance():getAdvCellContainer()
	if nil ~= view then
		require("GoldExchangeAdvCell")
		local node = GoldExchangeAdvCell:create(view,path,params)
		return node:getCostBtnRect()
	end
	return 0,0,0,0
end
function updateGoldExchangeAdvCell(params)
	local cell = LuaController:getInstance():getAdvCell()
	if nil ~= cell then
		cell:setData(params)
	end
end
function createGoldExchangeIcon(path,params)
	local view = LuaController:getInstance():getIconContainer()
	if nil ~= view then
		require("GoldExchangeIcon")
		GoldExchangeIcon:create(view,path,params)
	end
end

function addChatNotice(params,path,time)
	print ("step _____ 0")
	local view = LuaController:getInstance():getChatNoticeContainer()
	if nil ~= view then
		print ("step _____ 1")
		require("ChatNoticeView")
		local node = ChatNoticeView:create(view,path,params,time,false)
		ChatNoticeView:create(view,path,params,time,true)
		return node:getTime()
	end
	return 0
end

function createActivityAdCell( path, params )
	require("ActivityAdCell")
	return ActivityAdCell:create(path, params)
end

function createActivityListCellSprite( path, params )
	require("ActivityListCellSprite")
	return ActivityListCellSprite:create(path, params)
end

function getActTips( actId )
	if actId == "57006" then
        return string.format(LuaController:getLang("137493"));
    else 
    	return string.format(LuaController:getLang("137493"));
    end
end

function getCollectLabel( actId )
	if actId == "57006" then
        return string.format(LuaController:getLang("137497"));
    else 
    	return string.format(LuaController:getLang("137497"));
    end
end

function getBuyBtnLabel( actId)
	if actId == "57006" then
        return string.format(LuaController:getLang("137491"));
    else 
    	return string.format(LuaController:getLang("137491"));
    end
end

function getActRefTip( actId )
	if actId == "57006" then
        return string.format(LuaController:getLang("137496"));
    else 
    	return string.format(LuaController:getLang("137496"));
    end
end

--google translate
function getTranslateLink(path,orgContent,targetLan,linkidx)
	local agent = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/534.24 (KHTML, like Gecko) Chrome/11.0.696.71 Safari/534.24"
	local url = ""
	local paramsurl = ""
	local httpType = ""
	if linkidx == 0 then
		url = "http://translate.google.com/translate_a/t"
		paramsurl = "client=te&text=" .. orgContent .. "&sl=auto&tl=" .. targetLan .. "&ie=UTF-8&oe=UTF-8"
		httpType = "get"
	elseif linkidx == 1 then
		url = "http://fanyi.baidu.com/v2transapi"
		if targetLan == "zh_TW" then
			targetLan = "cht"
		elseif targetLan == "zh_CN" then
			targetLan = "zh"
		elseif targetLan == "zh-TW" then
			targetLan = "cht"
		elseif targetLan == "zh-CN" then
			targetLan = "zh"
		end

		paramsurl = "from=auto&to=" .. targetLan .. "&query="..orgContent.."&transtype=realtime&simple_means_flag=3"
		httpType = "post"
	end
	local num = math.random(5)
	if num == 1 then
		agent="Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/534.24 (KHTML, like Gecko) Chrome/11.0.696.71 Safari/534.24" 
	elseif num == 2 then
		agent="Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2) Gecko/20100115 Firefox/3.6"		
	elseif num == 3 then
		agent="Mozilla/5.0 (Linux; U; Android 4.1.1; fr-fr; MB525 Build/JRO03H; CyanogenMod-10) AppleWebKit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30"	
	elseif num == 4 then
		agent="Mozilla/5.0 (Macintosh; Intel Mac OS X 10_6_4) AppleWebKit/534.30 (KHTML, like Gecko) Chrome/12.0.742.100 Safari/534.30"	
	else
		agent="Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2) Gecko/20100115 Firefox/3.6"	
	end

	return agent,url,paramsurl,httpType
end
function praseTranslateMsg(path,params)
	local strLen = string.len(params)
	for i=strLen,1,-1 do
		local laststr = string.sub(params,-1)
		--print (laststr)
		if laststr == "}" then
			break
		else
			local len1 = string.len(params) - 1
			params = string.sub(params,1,len1)
		end
	end

	print ("praseMSG" .. params)

	local json = require "dkjson"
	local data = json.decode(params)
	--print ("test 3")

	local targetContent = ""
	local srcLang = ""
	local srcContent = ""

	if data ~=nil then
		if data.sentences ~= nil then
			local tCount = table.getn(data.sentences)
            for i=1,tCount do
				if data.sentences[i] ~= nil then
					if data.sentences[i].trans ~= nil then
						targetContent = targetContent .. data.sentences[i].trans
					end
					if data.sentences[i].orig ~= nil then
						srcContent = srcContent .. data.sentences[i].orig
					end
				end
			end
			if data.src ~= nil then
				srcLang = data.src
			end
		elseif data.trans_result ~= nil then  --baidu
			if data.trans_result.from ~= nil then
				srcLang = data.trans_result.from
				if srcLang == "zh" then
					srcLang = "zh-CN"
				elseif srcLang == "cht" then
					srcLang = "zh-TW"
				end
			end
			if data.trans_result.data ~= nil then
				if data.trans_result.data[1] ~= nil then
					if data.trans_result.data[1].dst ~= nil then
						targetContent = data.trans_result.data[1].dst
					end
					if data.trans_result.data[1].src ~= nil then
						srcContent = data.trans_result.data[1].src
					end
				end
			end
		end
	end
	return targetContent,srcLang,srcContent
end
