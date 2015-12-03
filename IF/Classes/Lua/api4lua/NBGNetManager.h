//
//  NBGNetManager.h
//  nbg
//
//  Created by gj on 9/1/15.
//
//

#ifndef __nbg__NBGNetManager__
#define __nbg__NBGNetManager__

#include <string>
#include "cocos2d.h"
#include "api4lua_support.h"

// 头文件 include LuaValueDict 相关文件会导致 lua binding 编译出错

using namespace cocos2d;

class NBGNetManager
{
public:
    static NBGNetManager* getInstance();
    void connect(const std::string& ip_addr, int port, CXLUAFUNC luaHandlerOnConnection);
    
    void login(const ValueMap& requestData, CXLUAFUNC luaHandlerOnLogin);
    void send(const std::string& cmd, const ValueMap& requestData, CXLUAFUNC luaHandlerOnResponse);
    
    void handle_lua_onConnection(bool success);
    
    void onResponse(/* LuaValueDict* */ void* pLuaValueDict);
    
    // ---
    CC_SYNTHESIZE(bool, m_isDebugMode, IsDebugMode);
//    CC_SYNTHESIZE(CXLUAFUNC, luaHandler_onConnection, _luaHandler_onConnection);
//    CC_SYNTHESIZE(CXLUAFUNC, luaHandler_onResponse, _luaHandler_onResponse);
    
protected:
    static NBGNetManager* m_pInstance;
    NBGNetManager();
    
    CXLUAFUNC luaHandler_onConnection;
    CXLUAFUNC luaHandler_onResponse;
};

#endif /* defined(__nbg__NBGNetManager__) */
