//
//  ChatServiceController.h
//  IF
//
//  Created by mzc on 15/4/20.
//
//

#import <Foundation/Foundation.h>
#import "ChatServiceController.h"
#import "GameHostInterface.h"
#import "NSUserInfo.h"

@interface ChatServiceController : NSObject

@property (nonatomic,strong) GameHostInterface *gameHost;
+(instancetype) chatServiceControllerSharedManager;

-(void)doHostAction:(NSString *)action :(NSString *)uid :(NSString *)name :(NSString *)reportUid :(NSString *)detectReportUid :(int)equipId :(BOOL)returnToChatAfterPopup;

-(void) closekeyboard;

//获取一个喇叭所消费的金币数额
-(int) radioCount;

//判断金币是否足够发送一个喇叭
-(BOOL) isSatisfySendRadio;

//发送喇叭向后台请求
-(void) sendNotice:(NSString *) msg :(int)itemid :(BOOL)usePoint :(NSString*) sendLocalTime ;

//向c＋＋ 获取头像文件在本地的路径
-(NSString*) headPath:(NSString*)url;

//从网络下载头像
-(void) downHeadImage:(NSString*)url;

//未读邮件变成已读邮件
-(void) updateMailList;

-(void) addUser:(NSUserInfo*)userInfo;
//当前国王UID
@property (nonatomic,strong)NSString *kingUidString;
@property (nonatomic,assign) BOOL isContactMod;
@property (nonatomic,strong)NSString *banTimeString;

@property (nonatomic,strong)NSMutableDictionary *redPackgeMsgDict;

-(void)loadChatBarOnCocoas;

@end
