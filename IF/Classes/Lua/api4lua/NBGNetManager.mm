//
//  NBGNetManager.mm
//  nbg
//
//  Created by gj on 9/1/15.
//
//

#include "NBGNetManager.h"
#include "CCLuaEngine.h"

// -------------------------------------------------------------------
#pragma mark - iOS
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#pragma mark * NBGNetManager_iOS interface

#import <SFS2XAPIIOS/SmartFox2XClient.h>

@interface NBGNetManager_iOS : NSObject <ISFSEvents> {
    SmartFox2XClient* _sfsClient;
}
@property (nonatomic, retain) SmartFox2XClient* sfsClient;
+(NBGNetManager_iOS*)shared;
-(void)connect:(NSString *)ip_addr port:(int)port;
-(void)loginWithData:(const ValueMap&)data;
-(void)sendWithCmd:(NSString*)cmd data:(const ValueMap&)requestData;
@end

#pragma mark * NBGNetManager_iOS implementation

@implementation NBGNetManager_iOS

@synthesize sfsClient = _sfsClient;

static NBGNetManager_iOS* _shared_NBGNetManager_iOS = nil;
+(NBGNetManager_iOS*)shared {
    if (!_shared_NBGNetManager_iOS) {
        _shared_NBGNetManager_iOS = [[NBGNetManager_iOS alloc] init];
    }
    return _shared_NBGNetManager_iOS;
}
- (id)init {
    if (self = [super init]) {
        BOOL isDebugMode = NBGNetManager::getInstance()->getIsDebugMode();
        _sfsClient = [[SmartFox2XClient alloc] initSmartFoxWithDebugMode:isDebugMode delegate:self];
        [_sfsClient setUseBlueBox:true];
    }
    return self;
}
-(void)dealloc {
    [_sfsClient release];
    [super dealloc];
}

-(void)connect:(NSString *)ip_addr port:(int)port {
    if (_sfsClient != nil && ![_sfsClient isConnected]) {
        [_sfsClient connect:ip_addr port:port];
    }
}

-(void)loginWithData:(const ValueMap&)requestData {
    SFSObject* obj = [[[SFSObject alloc] init] autorelease];
    [NBGNetManager_iOS ValueMap2SFSObject:requestData.at("data").asValueMap() obj:obj];
    
    NSString * zone = [NSString stringWithUTF8String:requestData.at("zone").asString().c_str()];
    NSString * username = [NSString stringWithUTF8String:requestData.at("username").asString().c_str()];
    NSString * password = [NSString stringWithUTF8String:requestData.at("password").asString().c_str()];
    [_sfsClient send:[LoginRequest requestWithUserName:username password:password zoneName:zone params:obj]];
}

-(void)sendWithCmd:(NSString*)cmd data:(const ValueMap&)requestData {
    SFSObject* obj=[[[SFSObject alloc] init] autorelease];
    [NBGNetManager_iOS ValueMap2SFSObject:requestData obj:obj];
    dispatch_async(dispatch_get_main_queue(), ^{
        [_sfsClient send:[ExtensionRequest requestWithExtCmd:cmd params:obj]];
    });
}

#pragma mark * cocos2d value to sfs2x

+(void)ValueMap2SFSObject:(const ValueMap&)valueMap obj:(SFSObject*)obj {
    if (!obj) return;
    for (auto it = valueMap.begin(); it != valueMap.end(); ++it) {
        NSString* key = [NSString stringWithUTF8String:it->first.c_str()];
        if (it->second.getType() == Value::Type::BYTE) {
            [obj putByte:key value:it->second.asByte()];
        } else if (it->second.getType() == Value::Type::INTEGER) {
            [obj putInt:key value:it->second.asInt()];
        } else if (it->second.getType() == Value::Type::FLOAT) {
            [obj putFloat:key value:@(it->second.asFloat())];
        } else if (it->second.getType() == Value::Type::DOUBLE) {
            [obj putDouble:key value:@(it->second.asDouble())];
        } else if (it->second.getType() == Value::Type::BOOLEAN) {
            [obj putBool:key value:it->second.asBool()];
        } else if (it->second.getType() == Value::Type::STRING) {
            [obj putUtfString:key value:[NSString stringWithUTF8String:it->second.asString().c_str()]];
        } else if (it->second.getType() == Value::Type::VECTOR) {
            SFSArray* arr = [[[SFSArray alloc] init] autorelease];
            [NBGNetManager_iOS ValueVector2SFSArray:it->second.asValueVector() sfsArray:arr];
            [obj putSFSArray:key value:arr];
        } else if (it->second.getType() == Value::Type::MAP) {
            SFSObject* sub_obj = [[[SFSObject alloc] init] autorelease];
            [NBGNetManager_iOS ValueMap2SFSObject:it->second.asValueMap() obj:sub_obj];
            [obj putSFSObject:key value:sub_obj];
        } else if (it->second.getType() == Value::Type::INT_KEY_MAP) {
#warning TODO: valueMap2SFSObject INT_KEY_MAP
        }
    }
}

+(void)ValueVector2SFSArray:(const ValueVector&)valueVector sfsArray:(SFSArray*)arr {
    if (!arr) return;
    for (auto it = valueVector.begin(); it != valueVector.end(); ++it) {
        if (it->getType() == Value::Type::BYTE) {
            [arr addByte:it->asByte()];
        } else if (it->getType() == Value::Type::INTEGER) {
            [arr addInt:it->asInt()];
        } else if (it->getType() == Value::Type::FLOAT) {
            [arr addFloat:@(it->asFloat())];
        } else if (it->getType() == Value::Type::DOUBLE) {
            [arr addDouble:@(it->asDouble())];
        } else if (it->getType() == Value::Type::BOOLEAN) {
            [arr addBool:it->asBool()];
        } else if (it->getType() == Value::Type::STRING) {
            [arr addUtfString:[NSString stringWithUTF8String:it->asString().c_str()]];
        } else if (it->getType() == Value::Type::VECTOR) {
            SFSArray* sub_arr = [[[SFSArray alloc] init] autorelease];
            [NBGNetManager_iOS ValueVector2SFSArray:it->asValueVector() sfsArray:sub_arr];
            [arr addSFSArray:sub_arr];
        } else if (it->getType() == Value::Type::MAP) {
            SFSObject* sub_obj = [[[SFSObject alloc] init] autorelease];
            [NBGNetManager_iOS ValueMap2SFSObject:it->asValueMap() obj:sub_obj];
            [arr addSFSObject:sub_obj];
        } else if (it->getType() == Value::Type::INT_KEY_MAP) {
#warning TODO: ValueVector2SFSObject INT_KEY_MAP
        }
    }
}

#pragma mark * sfs2x to lua value

+(void)addNSObject2LuaValueDict:(id)key obj:(id)obj map:(LuaValueDict&)map {
    if ([obj isKindOfClass:[NSString class]]) {
        map[[key UTF8String]] = LuaValue::stringValue([obj UTF8String]);
    } else if ([obj isKindOfClass:[NSNumber class]]) {
        map[[key UTF8String]] = LuaValue::stringValue([[obj stringValue] UTF8String]);
    } else if ([obj isKindOfClass:[NSDictionary class]]) {
        LuaValueDict sub_map;
        for (id subKey in [obj allKeys]) {
            id subValue = [obj objectForKey:subKey];
            [NBGNetManager_iOS addNSObject2LuaValueDict:subKey obj:subValue map:sub_map];
        }
        map[[key UTF8String]] = LuaValue::dictValue(sub_map);
    } else if ([obj isKindOfClass:[NSArray class]]) {
        LuaValueArray sub_vec;
        for (id sub_value in obj) {
            [NBGNetManager_iOS addNSObject2LuaValueArray:sub_value vec:sub_vec];
        }
        map[[key UTF8String]] = LuaValue::arrayValue(sub_vec);
    }
}

+(void)addNSObject2LuaValueArray:(id)obj vec:(LuaValueArray&)vec {
    if ([obj isKindOfClass:[NSString class]]) {
        vec.push_back(LuaValue::stringValue([obj UTF8String]));
    } else if ([obj isKindOfClass:[NSNumber class]]) {
        vec.push_back(LuaValue::stringValue([[obj stringValue] UTF8String]));
    } else if ([obj isKindOfClass:[NSDictionary class]]) {
        LuaValueDict sub_map;
        for (id subKey in [obj allKeys]) {
            id subValue = [obj objectForKey:subKey];
            [NBGNetManager_iOS addNSObject2LuaValueDict:subKey obj:subValue map:sub_map];
        }
        vec.push_back(LuaValue::dictValue(sub_map));
    } else if ([obj isKindOfClass:[NSArray class]]) {
        LuaValueArray sub_vec;
        for (id sub_value in obj) {
            [NBGNetManager_iOS addNSObject2LuaValueArray:sub_value vec:sub_vec];
        }
        vec.push_back(LuaValue::arrayValue(sub_vec));
    }
}

+(void)SFSObject2LuaValueDict:(SFSObject*)obj map:(LuaValueDict&)map {
    for (id key in [obj getKeys]) {
        id value = [obj getClass:key];
        if ([value isKindOfClass:[SFSObject class]]) {
            LuaValueDict sub_map;
            [NBGNetManager_iOS SFSObject2LuaValueDict:value map:sub_map];
            map[[key UTF8String]] = LuaValue::dictValue(sub_map);
        } else if ([value isKindOfClass:[SFSArray class]]) {
            LuaValueArray sub_vec;
            [NBGNetManager_iOS SFSArray2LuaValueArray:value vec:sub_vec];
            map[[key UTF8String]] = LuaValue::arrayValue(sub_vec);
        } else {
            [NBGNetManager_iOS addNSObject2LuaValueDict:key obj:obj map:map];
        }
    }
}

+(void)SFSArray2LuaValueArray:(SFSArray*)obj vec:(LuaValueArray&)vec {
    for (int i = 0; i < [obj size]; ++i) {
        id value = [obj getClass:i];
        if ([value isKindOfClass:[SFSObject class]]) {
            LuaValueDict sub_map;
            [NBGNetManager_iOS SFSObject2LuaValueDict:value map:sub_map];
            vec.push_back(LuaValue::dictValue(sub_map));
        } else if ([value isKindOfClass:[SFSArray class]]) {
            LuaValueArray sub_vec;
            [NBGNetManager_iOS SFSArray2LuaValueArray:value vec:sub_vec];
            vec.push_back(LuaValue::arrayValue(sub_vec));
        } else {
            [NBGNetManager_iOS addNSObject2LuaValueArray:value vec:vec];
        }
    }
}

#pragma mark * NBGNetManager_iOS ISFSEvents

-(void)onConnection:(SFSEvent *)evt {
    NBGNetManager::getInstance()->handle_lua_onConnection([[evt.params objectForKey:@"success"] boolValue]);
}

-(void)onConnectionLost:(SFSEvent *)evt {
    CCLOG("[NBG-print] Connection was lost, Reason: %s", [[evt.params objectForKey:@"reason"] UTF8String]);
}

-(void)onConnectionRetry:(SFSEvent *)evt {
// Freeze your GUI and provide some feedback to the Player
}

-(void)onConnectionResume:(SFSEvent *)evt {
// Unfreeze the GUI and let the player continue with the game...
}

-(void)onConectionAttempHTTP:(SFSEvent *)evt {
// Provide feedback to client on BlueBox(HTTP) connection attempt.
}

-(void)onUDPSocketConnect:(SFSEvent *)evt {
    if ([[evt.params objectForKey:@"success"] boolValue]) {
        CCLOG("[NBG-print] UDP connected");
    } else {
        CCLOG("[NBG-print] UDP not connected");
    }
}

-(void)onConfigLoadSuccess:(SFSEvent *)evt {
    CCLOG("[NBG-print] Config file loaded, now connecting...");
    // [smartFox connect:smartFox.currentIp port:smartFox.currentPort];
}

-(void)onConfigLoadFailure:(SFSEvent *)evt {
    CCLOG("[NBG-print] Failed loading config file: %s", [[evt.params objectForKey:@"message"] UTF8String]);
}

-(void)onLogin:(SFSEvent *)evt {
    CCLOG("[NBG-print] Hi, I have just logged in as: %s", [[[evt.params objectForKey:@"user"] name] UTF8String]);
    LuaValueDict dict;
    dict["cmd"] = LuaValue::stringValue("login");
    LuaValueDict params;
    [NBGNetManager_iOS SFSObject2LuaValueDict:[[evt params] objectForKey:@"data"] map:params];
    dict["params"] = LuaValue::dictValue(params);
    NBGNetManager::getInstance()->onResponse(&dict);
}

-(void)onLoginError:(SFSEvent *)evt {
    CCLOG("[NBG-print] Login Failed. Reason: %s" ,[[evt.params objectForKey:@"errorMessage"] UTF8String]);
    LuaValueDict dict;
    dict["cmd"] = LuaValue::stringValue("login");
    NSString * errorMessage = [[evt params] objectForKey:@"errorMessage"];
    dict["errorMessage"] = LuaValue::stringValue([errorMessage UTF8String]);
    NSNumber * errorCode = [[evt params] objectForKey:@"errorCode"];
    dict["errorCode"] = LuaValue::intValue([errorCode intValue]);
    NBGNetManager::getInstance()->onResponse(&dict);
}

-(void)onLogout:(SFSEvent *)evt {
    CCLOG("[NBG-print] Logout was successful");
    // Proceed with more logic here...
}

-(void)onRoomAdd:(SFSEvent *)evt {
    CCLOG("[NBG-print] A new Room was added: %s", [[evt.params objectForKey:@"room"] UTF8String]);
}

-(void)onRoomRemove:(SFSEvent *)evt {
    CCLOG("[NBG-print] A new Room was removed: %s", [[evt.params objectForKey:@"room"] UTF8String]);
}

-(void)onRoomJoin:(SFSEvent *)evt {
    SFSRoom *room = [evt.params objectForKey:@"room"];
    CCLOG("[NBG-print] The Room %s was successfully joined!", [room.name UTF8String]);
}

-(void)onRoomJoinError:(SFSEvent *)evt {
    CCLOG("[NBG-print] Join Room Failure:  %s", [[evt.params objectForKey:@"errorMessage"] UTF8String]);
}

-(void)onUserEnterRoom:(SFSEvent *)evt {
    SFSRoom *room = [evt.params objectForKey:@"room"];
    SFSUser *user = [evt.params objectForKey:@"user"];
    CCLOG("[NBG-print] User: %s has just joined Room: %s", [user.name UTF8String], [room.name UTF8String]);
}

-(void)onUserExitRoom:(SFSEvent *)evt {
    SFSRoom *room = [evt.params objectForKey:@"room"];
    SFSUser *user = [evt.params objectForKey:@"user"];
    CCLOG("[NBG-print] User: %s has just left Room: %s", [user.name UTF8String], [room.name UTF8String]);
}

-(void)onUserCountChange:(SFSEvent *)evt {
    SFSRoom *room = [evt.params objectForKey:@"room"];
    int uCount = [[evt.params objectForKey:@"uCount"] intValue];
    int sCount = [[evt.params objectForKey:@"sCount"] intValue];

    CCLOG("[NBG-print] Room: %s contains %d users and %d spectators", [room.name UTF8String], uCount, sCount);
}

-(void)onPublicMessage:(SFSEvent *)evt {
    SFSUser *sender = [evt.params objectForKey:@"sender"];
    CCLOG("[NBG-print] %s: %s", [sender.name UTF8String], [[evt.params objectForKey:@"message"] UTF8String]);
}

-(void)onPrivateMessage:(SFSEvent *)evt {
    SFSUser *sender = [evt.params objectForKey:@"sender"];
    CCLOG("[NBG-print] PM: %s, from: %s", [[evt.params objectForKey:@"message"] UTF8String], [sender.name UTF8String]);
}

-(void)onModeratorMessage:(SFSEvent *)evt {
    SFSUser *sender = [evt.params objectForKey:@"sender"];
    CCLOG("[NBG-print] Moderator says: %s, from: %s", [[evt.params objectForKey:@"message"] UTF8String], [sender.name UTF8String]);
}

-(void)onAdminMessage:(SFSEvent *)evt {
    CCLOG("[NBG-print] Admin says: %s", [[evt.params objectForKey:@"message"] UTF8String]);
}

-(void)onObjectMessage:(SFSEvent *)evt {
    SFSObject *obj = [evt.params objectForKey:@"message"];
    CCLOG("[NBG-print] Received data: %s", [[obj getDump:YES] UTF8String]);
}

-(void)onExtensionResponse:(SFSEvent *)evt {
    LuaValueDict dict;
    dict["cmd"] = LuaValue::stringValue([[[evt params] objectForKey:@"cmd"] UTF8String]);
    LuaValueDict params;
    [NBGNetManager_iOS SFSObject2LuaValueDict:[[evt params] objectForKey:@"params"] map:params];
    dict["params"] = LuaValue::dictValue(params);
    NBGNetManager::getInstance()->onResponse(&dict);
}

@end

#define NET_CLIENT [NBGNetManager_iOS shared]
#endif

// -------------------------------------------------------------------
#pragma mark - Mac
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#endif

// -------------------------------------------------------------------
#pragma mark - NBGNetManager
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
void NBGNetManager::connect(const std::string& ip_addr, int port, CXLUAFUNC luaHandlerOnConnection)
{
    luaHandler_onConnection = luaHandlerOnConnection;
    [NET_CLIENT connect:[NSString stringWithUTF8String:ip_addr.c_str()] port:port];
}

void NBGNetManager::login(const ValueMap& requestData, CXLUAFUNC luaHandlerOnLogin)
{
    luaHandler_onResponse = luaHandlerOnLogin;
    [NET_CLIENT loginWithData:requestData];
}

void NBGNetManager::send(const std::string& cmd, const ValueMap& requestData, CXLUAFUNC luaHandlerOnResponse)
{
    luaHandler_onResponse = luaHandlerOnResponse;
    [NET_CLIENT sendWithCmd:[NSString stringWithUTF8String:cmd.c_str()] data:requestData];
}

#endif
