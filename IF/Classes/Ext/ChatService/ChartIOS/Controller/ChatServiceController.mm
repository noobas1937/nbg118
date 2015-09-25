//
//  ChatServiceController.m
//  IF
//
//  Created by mzc on 15/4/20.
//
//

#import "ChatServiceController.h"
#import "ChatServiceCocos2dx.h"
#include "ServiceInterface.h"
#import "GameHost.h"

static ChatServiceController * chatServiceController = nil;

@implementation ChatServiceController

-(void)doHostAction:(NSString *)action :(NSString *)uid :(NSString *)name :(NSString *)reportUid :(NSString *)detectReportUid :(BOOL)returnToChatAfterPopup
{
    [self.gameHost setActionAfterResume:action :uid :name :reportUid :detectReportUid :returnToChatAfterPopup];
}

-(void) closekeyboard
{
    int channel = ChatServiceCocos2dx::m_channelType;
    
    if (channel == IOS_CHANNEL_TYPE_COUNTRY || channel == IOS_CHANNEL_TYPE_ALLIANCE) {
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController.keyBordView.chatViewTextField resignFirstResponder];
    }else{
        [[ServiceInterface serviceInterfaceSharedManager].mailViewController.keyBordView.chatViewTextField resignFirstResponder];
    }
}

-(int) radioCount
{
    return [self.gameHost radioCount];
}

-(BOOL) isSatisfySendRadio
{
    return [self.gameHost isSatisfySendRadio];
}

-(void) sendNotice:(NSString *)msg :(int)itemid :(BOOL)usePoint :(NSString *)sendLocalTime
{
    [self.gameHost sendNotice:msg :itemid :usePoint :sendLocalTime];
}


-(NSString*) headPath:(NSString *)url
{
    return [self.gameHost headPath:url];
}

-(void) downHeadImage:(NSString *)url
{
    [self.gameHost downHeadImage:url];
}

-(void) updateMailList
{
    [self.gameHost updateMailList];
}

/**华丽的分割线－－－－－－－－－－－－－－单例－－－－－－－－－－－－－－－－－－－－－－－*/

+ (instancetype)chatServiceControllerSharedManager
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        chatServiceController = [[ChatServiceController alloc] init];
    });
    return chatServiceController;
}

-(instancetype)init{
    self = [super init];
    if (self){
        self.gameHost = [[GameHost alloc]init];
    }
    return self;
}

//为了确保有且只有一个实例，有必要覆盖一下+ (id)allocWithZone:(NSZone *)zone 这个方法，如果不小心调用了alloc方法，则又会创建一个实例
+ (id)allocWithZone:(NSZone *)zone
{
    //调用dispatch_once保证在多线程中也只被实例化一次
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        chatServiceController = [super allocWithZone:zone];
    });
    return chatServiceController;
}

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}


@end
