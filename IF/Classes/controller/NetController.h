
#ifndef __IF__NetController__
#define __IF__NetController__

#include "LoginCommand.h"
#include "LogoutCommand.h"
#include "cocos2d.h"

#define NC_LOG_NET_COMMAND 1
#define SEND_TIMEOUT 8
#define LOGIN_TIMEOUT 20
#define CONNECT_TIMEOUT 15
#define kEnterBattleCancel      1   //取消进入
#define kEnterBattleOK          2   //进入战场

#define NET_CONNECTED       "NConnected"
#define NET_CONNECTION_LOST "NConnectionLost"
#define SFS_CONNECT_TIMEOUT 30 // smartfox连接timeout时间（单位：秒）
#define SFS_KICK "LoginKick"
#define MAX_SERVER_NUM 5001

#ifdef __OBJC__
#  import <SFS2XAPIIOS/SmartFox2XClient.h>

@interface NetControllerIOS : NSObject <ISFSEvents> {
	SmartFox2XClient* _sfsClient;
    bool m_isSending;
}
@property (nonatomic, retain) SmartFox2XClient* sfsClient;
+(NetControllerIOS*)shared;

+(void)dictionaryToSFSObject:(const cocos2d::CCDictionary*)dict obj:(SFSObject*)obj;
+(void)arrayToSFSObject:(const cocos2d::CCArray*)array obj:(SFSArray*)obj;
+(void)intArrayToNSArray:(const CCIntArray*)array obj:(NSMutableArray*)obj;

-(bool)connectedToNetwork;

-(void)connect;
-(void)disconnect;

-(void)doLogin:(const LoginCommand*)request;
-(void)doSend:(const CommandBase*)request;
-(void)doJoinRoom;

-(void)send:(NSString*)cmd param:(SFSObject*)param;
//-(void)cancel:(NSString*)cmd;

-(void)logoutRequest;

-(void)onConnectTimeout;
-(void)onSendTimeout;



@end
# endif // __OBJC__

class NetController: public cocos2d::CCObject {
public:
    void connect();
    void send(CommandBase* request);
    
    static NetController* shared();
    static void purgeShared();
    
    void cancelForTarget(CCObject* target);
    
    void handleRecieve(cocos2d::CCDictionary *response);
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
    void handleRecieve(CCObject* p);
#endif
    bool checkIsAlreadyInQueue(CheckType type, std::string cmd, CCDictionary *param);
    void doJoinRoom();
    void clearRequestQueue();
    void clearAllRequest();//清理所有的后台请求，用于国内平台的切换账号
    static std::string getIpByType(int type);//0 正常 1 备用
    int m_ipConnectTime;
    void checkNetWork(std::string cmd);
    void funRetry();
    void funQuitGame();
    void funFeedBack();

protected:
    NetController();
    
private:
    void onLoginFailed(CCObject * obj);
    void onConnection(CCObject* p);
    void onConnectionLost(CCObject* p);
    void onPublicMessage();
    void checkSendTimeout(float delta);
    void clickOK(CCObject* p);
    void checkReConnection(float delta);
    void doReconnection(CCObject* p);
    void onLoginFinish(CCObject* p);
    void onReLogin(CCObject* p);
    void checkSFSConnectTimeout(float delta);
    void onSendPendRequest(float delta);
    //连不上服务器先确定玩家网络状态，然后确定服务器状态
    void getServerStatus();
    void getServerStatusBack(CCHttpClient* client, CCHttpResponse* response);
    bool isNetWorkOK();
private:
    typedef list<CCSafeObject<CommandBase> > RequestQueue;
    RequestQueue m_requestSended;
    RequestQueue m_requestPending;
    bool m_isSending;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_ip, Ip);
    CC_SYNTHESIZE(int, m_port, Port);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, m_zone, Zone);
    
    CC_SYNTHESIZE(int, m_serverId, ServerId);
    
    CC_SYNTHESIZE(bool, m_isChangingServer, IsChangingServer);
    CC_SYNTHESIZE(bool, m_isLogout, IsLogout);
    CC_SYNTHESIZE(bool, m_isFirstReconnect, FirstReconnect);
    bool m_isRetry;
    int m_isReConnection;
    int m_connectTimes;
    int getRealPort();
    std::string getRealIp();
    bool isChina();
    //中国连美国服务器的设置
    bool isTecentConnectionInUse;
    bool isHKConnectionInUse;
    bool isUSConnectionInUse;
    void resetConnectionUse(bool reset);
#pragma mark -
#pragma mark Platform dependent
public:
    ~NetController();
    void disconnect();
    bool isConnected();
    bool isProcessing();
private:
    void doConnect(CCObject* p = NULL);
    void doLogin(LoginCommand *request);
    void doSend(CommandBase *request);
    void recordCmd(string cmd);
    void doLogout(LogoutCommand *request);
    void sendLog(std::string type, std::string cmd);
};

#endif /* defined(__IF__NetController__) */
