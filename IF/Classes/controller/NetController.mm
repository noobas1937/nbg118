//
//  NCController.m
//  Age of Empire
//
//  Created by Limin on 11-11-4.
//  Copyright 2011年 . All rights reserved.
//

#include "NetController.h"

#include "CommonInclude.h"
#include "../Ext/CCDevice.h"

#import "NSString+MD5Addition.h"
#import <netinet/in.h>
#import <SystemConfiguration/SCNetworkReachability.h>
#import <SFS2XAPIIOS/ConfigData.h>
#import "SFHFKeychainUtils.h"


NetController::~NetController()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, NET_CONNECTED);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, NET_CONNECTION_LOST);
    [[NetControllerIOS shared] release];
}

bool NetController::isProcessing()
{
    return m_isSending;
}

bool NetController::isConnected()
{
    return [NetControllerIOS shared].sfsClient!=nil && [[NetControllerIOS shared].sfsClient isConnected] && [[NetControllerIOS shared] connectedToNetwork];
}

void NetController::disconnect()
{
    CCLOGFUNC();
    [[NetControllerIOS shared] disconnect];
}

void NetController::doConnect(CCObject* p)
{
    m_isRetry = false;
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(NetController::onConnection), NET_CONNECTED, NULL);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NetController::checkSFSConnectTimeout), this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(NetController::checkSFSConnectTimeout), this, SFS_CONNECT_TIMEOUT, false, 0, 0.0f);
    
	[[NetControllerIOS shared] connect];
}

bool NetController::isNetWorkOK(){
    return [[NetControllerIOS shared] connectedToNetwork];
}

void NetController::doLogin(LoginCommand *request)
{
    [[NetControllerIOS shared] doLogin:request];
}

void NetController::doLogout(LogoutCommand *request)
{
    [[NetControllerIOS shared] logoutRequest];
}

void NetController::doSend(CommandBase *request)
{
    [[NetControllerIOS shared] doSend:request];
}

void NetController::doJoinRoom()
{
    [[NetControllerIOS shared] doJoinRoom];
}

void addValueToCCDict(id key, id value, CCDictionary* pDict);
void addItemToCCArray(id item, CCArray* pArray);

void addItemToCCArray(id item, CCArray *pArray)
{
    // add string value into array
    if ([item isKindOfClass:[NSString class]]) {
        CCString* pValue = new CCString([item UTF8String]);
        
        pArray->addObject(pValue);
        pValue->release();
        return;
    }
    
    // add number value into array(such as int, float, bool and so on)
    if ([item isKindOfClass:[NSNumber class]]) {
        NSString* pStr = [item stringValue];
        CCString* pValue = new CCString([pStr UTF8String]);
        
        pArray->addObject(pValue);
        pValue->release();
        return;
    }
    
    // add dictionary value into array
    if ([item isKindOfClass:[NSDictionary class]]) {
        CCDictionary* pDictItem = new CCDictionary();
        for (id subKey in [item allKeys]) {
            id subValue = [item objectForKey:subKey];
            addValueToCCDict(subKey, subValue, pDictItem);
        }
        pArray->addObject(pDictItem);
        pDictItem->release();
        return;
    }
    
    // add array value into array
    if ([item isKindOfClass:[NSArray class]]) {
        CCArray *pArrayItem = new CCArray();
        pArrayItem->init();
        //        pArrayItem->init();
        for (id subItem in item) {
            addItemToCCArray(subItem, pArrayItem);
        }
        pArray->addObject(pArrayItem);
        pArrayItem->release();
        return;
    }
}

void addValueToCCDict(id key, id value, CCDictionary* pDict)
{
    // the key must be a string
    CCAssert([key isKindOfClass:[NSString class]], "The key should be a string!");
    std::string pKey = [key UTF8String];
    
    // the value is a new dictionary
    if ([value isKindOfClass:[NSDictionary class]]) {
        CCDictionary* pSubDict = new CCDictionary();
        for (id subKey in [value allKeys]) {
            id subValue = [value objectForKey:subKey];
            addValueToCCDict(subKey, subValue, pSubDict);
        }
        pDict->setObject(pSubDict, pKey.c_str());
        pSubDict->release();
        return;
    }
    
    // the value is a string
    if ([value isKindOfClass:[NSString class]]) {
        CCString* pValue = new CCString([value UTF8String]);
        
        pDict->setObject(pValue, pKey.c_str());
        pValue->release();
        return;
    }
    
    // the value is a number
    if ([value isKindOfClass:[NSNumber class]]) {
        NSString* pStr = [value stringValue];
        CCString* pValue = new CCString([pStr UTF8String]);
        
        pDict->setObject(pValue, pKey.c_str());
        pValue->release();
        return;
    }
    
    // the value is a array
    if ([value isKindOfClass:[NSArray class]]) {
        CCArray *pArray = new CCArray();
                pArray->init();
        for (id item in value) {
            addItemToCCArray(item, pArray);
        }
        pDict->setObject(pArray, pKey.c_str());
        pArray->release();
        return;
    }
}

@implementation NetControllerIOS

@synthesize sfsClient = _sfsClient;

void sfsArrayToDictionary(SFSArray * obj, CCArray * array);
void sfsObjectToDictionary(SFSObject * obj, CCDictionary * dict) {
    for (id subKey in [obj getKeys]) {
        id subValue = [obj getClass:subKey];
        if ([subValue isKindOfClass:[SFSObject class]]) {
            CCDictionary *dict2=CCDictionary::create();
            sfsObjectToDictionary(subValue, dict2);
            dict->setObject(dict2, [subKey UTF8String]);
        }
        else if ([subValue isKindOfClass:[SFSArray class]]) {
            CCArray *arr=CCArray::create();
            sfsArrayToDictionary(subValue, arr);
            dict->setObject(arr, [subKey UTF8String]);
        }
        else
            addValueToCCDict(subKey, subValue, dict);
    }
}

void sfsArrayToDictionary(SFSArray * obj, CCArray * array) {
    for (int i=0; i < [obj size]; ++i) {
        id subValue = [obj getClass:i];
        if ([subValue isKindOfClass:[SFSObject class]]) {
            CCDictionary *dict2=CCDictionary::create();
            sfsObjectToDictionary(subValue, dict2);
            array->addObject(dict2);
        }
        else if ([subValue isKindOfClass:[SFSArray class]]) {
            CCArray *arr=CCArray::create();
            sfsArrayToDictionary(subValue, arr);
            array->addObject(arr);
        }
        else
            addItemToCCArray(subValue, array);
    }
}

#pragma mark -
#pragma mark Common Method

-(bool)connectedToNetwork{
    // Create zero addy
    struct sockaddr_in zeroAddress;
    bzero(&zeroAddress, sizeof(zeroAddress));
    zeroAddress.sin_len = sizeof(zeroAddress);
    zeroAddress.sin_family = AF_INET;
    
    // Recover reachability flags
    SCNetworkReachabilityRef defaultRouteReachability = SCNetworkReachabilityCreateWithAddress(NULL, (struct sockaddr *)&zeroAddress);
    SCNetworkReachabilityFlags flags;
    
    BOOL didRetrieveFlags = SCNetworkReachabilityGetFlags(defaultRouteReachability, &flags);
    CFRelease(defaultRouteReachability);
    
    if (!didRetrieveFlags)
    {
        NSLog(@"Error. Could not recover network reachability flags");
        return NO;
    }
    
    BOOL isReachable = flags & kSCNetworkFlagsReachable;
    BOOL needsConnection = flags & kSCNetworkFlagsConnectionRequired;
    return (isReachable && !needsConnection) ? YES : NO;
}

-(void)send:(NSString*)cmd param:(SFSObject*)param {
     m_isSending = true;
    //判断是否存在网络
    if (![self connectedToNetwork]) {
        [self disconnect];
        return;
    }
    
    CCLOG("NCController::send: %s", [cmd UTF8String]);
    
    NSDate* dat = [NSDate dateWithTimeIntervalSinceNow:0];
    NSTimeInterval a=[dat timeIntervalSince1970]*1000;
    NSString *stt = [NSString stringWithFormat:@"%.0f",a];
    [param putUtfString:@"sendtime" value:stt];
    [param putUtfString:@"ccmmdd" value:cmd];
	[_sfsClient send:[ExtensionRequest requestWithExtCmd:cmd params:param]];
    m_isSending = false;
}

//50.97.129.47  SjoCbfxVpZwQcJ0  Qo6sDewiJr7W6DG
-(void)onSendTimeout {
    NSLog(@"NC::sendTimeout Connect Status:%d user: %@",_sfsClient.isConnected,_sfsClient.mySelf);
    
    if(!_sfsClient.isConnected){//断开连接后,重连
        [self performSelector:@selector(connect)];
    }
}

-(void)connect {
    std::string ip = NetController::shared()->getRealIp();
    int port = NetController::shared()->getRealPort();

    //判断是否存在网络
    if (![self connectedToNetwork]) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(NET_CONNECTION_LOST);
        return;
    }

    if (_sfsClient != nil && ![_sfsClient isConnected]) {
        [_sfsClient connect:[NSString stringWithUTF8String:ip.c_str()] port:port];
        NetController::shared()->resetConnectionUse(false);
        return;
    }
    
#if COCOS2D_DEBUG > 0
    SmartFox2XClient* sfs = [[SmartFox2XClient alloc] initSmartFoxWithDebugMode:YES delegate:self];
#else
    SmartFox2XClient* sfs = [[SmartFox2XClient alloc] initSmartFoxWithDebugMode:NO delegate:self];
#endif
    [self setSfsClient: sfs];
    [sfs release];
    [_sfsClient setUseBlueBox:true];
//    [_sfsClient setReconnectionSeconds:5];
    [_sfsClient connect:[NSString stringWithUTF8String:ip.c_str()] port:port];
    NetController::shared()->resetConnectionUse(false);
}

-(void)onConnectTimeout {
	[self setSfsClient:nil];
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(NET_CONNECTION_LOST);
}

-(void)onConnection:(SFSEvent *)evt {
	bool isSuccess = [[evt.params objectForKey:@"success"] boolValue];
	CCLOGFUNCF("isSuccess:%d", isSuccess);
	if (isSuccess) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(NET_CONNECTED);
	} else {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(NET_CONNECTION_LOST);
	}
}

-(void)onConnectionLost:(SFSEvent *)evt {
	NSString* reason = [evt.params objectForKey:@"reason"];
	CCLOG("connection lost reason $$$ %s", [reason UTF8String]);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(NET_CONNECTION_LOST);
    
    if (_sfsClient!=nil && [_sfsClient isConnected] && ![reason isEqualToString:@"manual"]) {
        [self disconnect];
        if([reason isEqualToString:@"kick"]){
           CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(SFS_KICK);
        }
    }
}

-(void)onPublicMessage:(SFSEvent *)evt {
    NSString* msg = [evt.params objectForKey:@"message"];
    auto obj = CCString::create([msg UTF8String]);
    CCDictionary * result=CCDictionary::create();
    addValueToCCDict(@"cmd", @"push.chat", result);
    CCDictionary * params=CCDictionary::create();
    sfsObjectToDictionary([evt.params objectForKey:@"data"], params);
    params->setObject(obj, "msg");
    result->setObject(params, "params");
    
    NetController::shared()->handleRecieve(result);
}

-(void)onConnectionResume:(SFSEvent *)evt {
	CCLOGTIME();
}

-(void)onConnectionRetry:(SFSEvent *)evt {
	CCLOGTIME();
}

-(void)disconnect {
    [_sfsClient disconnect];
	[self setSfsClient:nil];
}

-(void)doLogin:(const LoginCommand*)request {
    SFSObject* obj=[[[SFSObject alloc] init] autorelease];
    [NetControllerIOS dictionaryToSFSObject:request->getData() obj:obj];
    NSString * zone=[NSString stringWithUTF8String:NetController::shared()->getZone().c_str()];
    NSString * username=[NSString stringWithUTF8String:request->getUsername().c_str()];
    NSString * password=[NSString stringWithUTF8String:request->getPassword().c_str()];
    [_sfsClient send:[LoginRequest requestWithUserName:username password:password zoneName:zone params:obj]];
}

-(void)onLogin:(SFSEvent *)evt {
    CCDictionary * result=CCDictionary::create();
    addValueToCCDict(@"cmd", @"login", result);
    CCDictionary * params=CCDictionary::create();
    sfsObjectToDictionary([[evt params] objectForKey:@"data"], params);
    result->setObject(params, "params");
    NetController::shared()->handleRecieve(result);
}

- (void)onLoginError:(SFSEvent *)evt {
    NSString *errorCode = [[evt params] objectForKey:@"errorMessage"];
    CCLOG("login error code from ios: %s $$$",[errorCode UTF8String]);
    // when error username or password , don't disconnect 
    if (![errorCode isEqual: @"5"] && ![errorCode isEqual: @"6"] && ![errorCode isEqual: @"4"]) {
        [self disconnect];
    }
    // when error is ban user, add new params
    if ([errorCode isEqual: @"4"] || [errorCode isEqual: @"E001"] || [errorCode isEqual:@"E002"]) {
        CCDictionary * result=CCDictionary::create();
        addValueToCCDict(@"cmd", @"login", result);
        CCDictionary * params=CCDictionary::create();
        params->setObject(CCString::create([errorCode UTF8String]), "errorMessage");
        result->setObject(params, "params");
        NetController::shared()->handleRecieve(result);
        return;
    }
    [self onLogin:evt];
}

-(void)doSend:(const CommandBase*)request {
    SFSObject* obj=[[[SFSObject alloc] init] autorelease];
    [NetControllerIOS dictionaryToSFSObject:request->getData() obj:obj];
    dispatch_async(dispatch_get_main_queue(), ^{
        [_sfsClient send:[ExtensionRequest requestWithExtCmd:[NSString stringWithUTF8String:request->getCommand().c_str()] params:obj]];
    });

}

-(void)onExtensionResponse:(SFSEvent*)evt {
    CCDictionary * result=CCDictionary::create();
    addValueToCCDict(@"cmd", [[evt params] objectForKey:@"cmd"], result);
    CCDictionary * params=CCDictionary::create();
    sfsObjectToDictionary([[evt params] objectForKey:@"params"], params);
    result->setObject(params, "params");
    NetController::shared()->handleRecieve(result);
}

-(void)logoutRequest {
    [_sfsClient send:[LogoutRequest request]];
}

-(void)onLogout:(SFSEvent *)evt {
    CCDictionary * result=CCDictionary::create();
    addValueToCCDict(@"cmd", @"logout", result);
    addValueToCCDict(@"params", [evt params], result);
    NetController::shared()->handleRecieve(result);
}

-(void)doJoinRoom {
    string roomName = GlobalData::shared()->playerInfo.roomName;
    if (roomName=="") {
        roomName = GlobalData::shared()->playerInfo.country;
    }
    NSString * name=[NSString stringWithUTF8String:roomName.c_str()];
    [_sfsClient send:[JoinRoomRequest requestWithId:name]];
}

#pragma mark -
#pragma mark Singleton Method and Initialize

- (id)init {
    if (self = [super init]) {
        m_isSending = FALSE;
    }
    return self;
}

static NetControllerIOS *_sharedNCController;
+(NetControllerIOS*)shared {
	if(_sharedNCController == nil) {
		_sharedNCController = [[NetControllerIOS alloc] init];
	}
	return _sharedNCController;
}

+(void)dictionaryToSFSObject:(const CCDictionary*)dict obj:(SFSObject*) obj {
    if (dict==NULL || obj==nil)
        return;
    
    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(dict, pElement) {
        NSString* key=[NSString stringWithUTF8String:pElement->getStrKey()];
        const type_info &typeInfo=typeid(*pElement->getObject());
        if (typeInfo == typeid(CCInteger) ) {
            [obj putInt:key value:(dynamic_cast<CCInteger*>(pElement->getObject()))->getValue()];
        }
        else if (typeInfo == typeid(CCBoolean)) {
            [obj putBool:key value:(dynamic_cast<CCBoolean*>(pElement->getObject()))->getValue()];
        }
        else if (typeInfo == typeid(CCByte)) {
            [obj putByte:key value:(dynamic_cast<CCByte*>(pElement->getObject()))->getValue()];
        }
        else if (typeInfo == typeid(CCLong)) {
            [obj putLong:key value:[NSNumber numberWithLong:(dynamic_cast<CCLong*>(pElement->getObject()))->getValue()]];
        }
//        else if (typeInfo == typeid(CCLongLong)) {
//            [obj putLong:key value:[NSNumber numberWithLong:(dynamic_cast<CCLongLong*>(pElement->getObject()))->getValue()]];
//        }
//        else if (typeInfo == typeid(CCDouble)) {
//            [obj putDouble:key value:[NSNumber numberWithDouble:(dynamic_cast<CCDouble*>(pElement->getObject()))->getValue()]];
//        }
        else if (typeInfo == typeid(CCFloat)) {
            [obj putFloat:key value:[NSNumber numberWithFloat:(dynamic_cast<CCFloat*>(pElement->getObject()))->getValue()]];
        }
        else if (typeInfo == typeid(CCString)) {
            [obj putUtfString:key value:[NSString stringWithUTF8String:(dynamic_cast<CCString*>(pElement->getObject()))->getCString()]];
        }
        else if (typeInfo == typeid(CCDictionary)) {
            SFSObject* obj2=[[[SFSObject alloc] init] autorelease];
            [self dictionaryToSFSObject:dynamic_cast<CCDictionary*>(pElement->getObject()) obj:obj2];
            [obj putSFSObject:key value:obj2];
        }
        else if (typeInfo == typeid(CCIntArray)) {
            NSMutableArray* arr=[[[NSMutableArray alloc] init] autorelease];
            [self intArrayToNSArray:dynamic_cast<CCIntArray*>(pElement->getObject()) obj:arr];
            [obj putIntArray:key value:arr];
        }
        else if (typeInfo == typeid(CCArray)) {
             SFSArray* arr=[[[SFSArray alloc] init] autorelease];
             [self arrayToSFSObject:dynamic_cast<CCArray*>(pElement->getObject()) obj:arr];
             [obj putSFSArray:key value:arr];
        }
        else
            CCLOG("dicionaryToSFSObject: unknown object for %s", pElement->getStrKey());
    }
}

+(void)arrayToSFSObject:(const CCArray*)array obj:(SFSArray*) obj {
    if (array==NULL || obj==nil)
        return;
    
    CCObject* pElement = NULL;
    CCARRAY_FOREACH(array, pElement) {
        const type_info &typeInfo=typeid(*pElement);
        if (typeInfo == typeid(CCInteger)) {
            [obj addInt:(dynamic_cast<CCInteger*>(pElement))->getValue()];
        }
        else if (typeInfo == typeid(CCBoolean)) {
            [obj addBool:(dynamic_cast<CCBoolean*>(pElement))->getValue()];
        }
        else if (typeInfo == typeid(CCByte)) {
            [obj addByte:(dynamic_cast<CCByte*>(pElement))->getValue()];
        }
        else if (typeInfo == typeid(CCLong)) {
            [obj addLong:[NSNumber numberWithLong:(dynamic_cast<CCLong*>(pElement))->getValue()]];
        }
//        else if (typeInfo == typeid(CCLongLong)) {
//            [obj addLong:[NSNumber numberWithLong:(dynamic_cast<CCLongLong*>(pElement))->getValue()]];
//        }
//        else if (typeInfo == typeid(CCDouble)) {
//            [obj addDouble:[NSNumber numberWithDouble:(dynamic_cast<CCDouble*>(pElement))->getValue()]];
//        }
        else if (typeInfo == typeid(CCFloat)) {
            [obj addFloat:[NSNumber numberWithFloat:(dynamic_cast<CCFloat*>(pElement))->getValue()]];
        }
        else if (typeInfo == typeid(CCString)) {
            [obj addUtfString:[NSString stringWithUTF8String:(dynamic_cast<CCString*>(pElement))->getCString()]];
        }
        else if (typeInfo == typeid(CCDictionary)) {
            SFSObject* obj2=[[[SFSObject alloc] init] autorelease];
            [self dictionaryToSFSObject:dynamic_cast<CCDictionary*>(pElement) obj:obj2];
            [obj addSFSObject:obj2];
        }
        else if (typeInfo == typeid(CCIntArray)) {
            NSMutableArray* arr=[[[NSMutableArray alloc] init] autorelease];
            [self intArrayToNSArray:dynamic_cast<CCIntArray*>(pElement) obj:arr];
            [obj addIntArray:arr];
        }
        else if (typeInfo == typeid(CCArray)) {
            SFSArray* arr=[[[SFSArray alloc] init] autorelease];
            [self arrayToSFSObject:dynamic_cast<CCArray*>(pElement) obj:arr];
            [obj addSFSArray:arr];
        }
        else
            CCLOG("arrayToSFSArray: unknown object in SFSArray");
    }
}

+(void)intArrayToNSArray:(const CCIntArray*)array obj:(NSMutableArray*)obj {
    CCObject * pObj=NULL;
    CCARRAY_FOREACH(array, pObj) {
        CCInteger *value=dynamic_cast<CCInteger*>(pObj);
        if (value) {
            [obj addObject:[NSNumber numberWithInt:value->getValue()]];
        }
    }
}

-(void)dealloc{
    
    [_sfsClient release];
    [super dealloc];
}

@end
