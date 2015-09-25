//
//  GameHost.m
//  IF
//
//  Created by mzc on 15/4/20.
//
//

#import "GameHostDemo.h"
//#import "LocalController.h"
#import "LanguageManager.h"
#include "ChatServiceCocos2dx.h"
//#include "ChatController.h"
#import "ChatServiceController.h"
#import "BanPopUpController.h"
#import "TipPopUpController.h"
#import "ServiceInterface.h"
//#include "WorldController.h"
//#include "SceneController.h"
//#include "WorldMapView.h"
#import "LanguageKeys.h"
//#include "ChatController.h"

@implementation GameHostDemo

-(NSArray*) getChatLangArray
{
    const char* chatLang[] = {"E100068","115020","105207","105209","105210","105300","105302","105304","105307","105308"
        ,"105309","105312","105313","105315","105316","105321","105322","105502","105602","108584","115922","115923"
        ,"115925","115926","115929","115181","115182","115281","115282","115168","115169","115170","105326","105327"
        ,"105328","105324","105325","115068","confirm","cancel_btn_label","114110","104932","105564","101205","105329"
        ,"105522","105591","105330","105332","105333","104371","104912","105331"};
    
    int len=sizeof(chatLang)/sizeof(char *);
    
    NSMutableArray *chatLangArray = [[NSMutableArray alloc]init];
    
//    for(int  i = 0; i < len; i++)
//    {
//        
//        NSString *key = [NSString stringWithUTF8String:chatLang[i]];
//        std::string str = _lang(chatLang[i]);
//        NSString *langValue = [[NSString alloc]initWithUTF8String:str.c_str()];
//        LanguageItem *languageItem = [[LanguageItem alloc]init:key :langValue];
//        [chatLangArray addObject:languageItem];
//        
//    }
    
    return [NSArray arrayWithArray:chatLangArray];
    
}

-(void) onResume:(int)chatType
{
//    ChatServiceCocos2dx::m_curChatType=chatType;
//    ChatServiceCocos2dx::isChatShowing=false;
//    ChatServiceCocos2dx::isForumShowing=false;
//    
//    if(IOSScheduleObject::getInstance()->actionAfterResume != "") {
//        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(IOSScheduleObject::handleResume), IOSScheduleObject::getInstance(), 0.0f, 0, 0.01f, false);
//    }
//    if(ChatServiceCocos2dx::m_isInMailDialog)
//        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(IOSScheduleObject::updateMailCell), IOSScheduleObject::getInstance(), 0.0f, 0, 1.0f, false);
//    else
//        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(IOSScheduleObject::showLatestMessage), IOSScheduleObject::getInstance(), 0.0f, 0, 1.0f, false);
}

-(void) setActionAfterResume:(NSString *)action :(NSString *)uid :(NSString *)name :(NSString *)reportUid :(NSString *)detectReportUid :(BOOL)returnToChatAfterPopup
{
//    IOSScheduleObject::getInstance()->actionAfterResume = [action UTF8String];
//    IOSScheduleObject::getInstance()->uid = [uid UTF8String];
//    IOSScheduleObject::getInstance()->name = [name UTF8String];
//    IOSScheduleObject::getInstance()->reportUid = [reportUid UTF8String];
//    IOSScheduleObject::getInstance()->detectReportUid = [detectReportUid UTF8String];
//    IOSScheduleObject::getInstance()->returnToChatAfterPopup = returnToChatAfterPopup;
}

//解除屏蔽玩家
-(void) unBlock:(NSString *)uid :(NSString *)name
{
//    ChatController::getInstance()->unShieldPlayer([uid UTF8String],[name UTF8String]);
}

//解除禁言玩家
-(void) unBan:(NSString *)uid
{
//    ChatController::getInstance()->unBanPlayer([uid UTF8String]);
}

//禁言玩家
-(void) ban:(ChatMessage*) chatMessage
{
    NSLog(@"禁言");
    BanPopUpController *banPopUpController = [[BanPopUpController alloc] initWithNibName:@"BanPopUpController" bundle:nil];
    
    [banPopUpController setInitData:chatMessage];
    
    UIViewController *recentView = [ServiceInterface serviceInterfaceSharedManager].chatViewController;
    
    while (recentView.parentViewController != nil) {
        recentView = recentView.parentViewController;
    }
    [recentView.view.window addSubview:banPopUpController.view];
}

//立即加入
-(void) joinAlliance:(NSString *)allianceId :(NSString*)name
{
    int type = ChatServiceCocos2dx::m_curChatType;
    
    [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"joinAlliance" :allianceId :name :@"" :@"" :TRUE ];
    [self onResume:type];
}

//跳转坐标
-(void) skipPos:(NSString *)posX :(NSString *)posY
{
//    CCPoint pt = ccp(atoi([posX UTF8String]), atoi([posY UTF8String]));
//    int worldIndex = WorldController::getIndexByPoint(pt);
//    WorldController::getInstance()->openTargetIndex = worldIndex;
//    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
//        WorldMapView::instance()->gotoTilePoint(pt);
//    }else{
//        int index = WorldController::getIndexByPoint(pt);
//        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, worldIndex);
//    }
}
//发送信息
-(void) sendText:(NSString *)msg :(int)m_curChatType
{
//    ChatController::getInstance()->sendCountryChat([msg UTF8String], m_curChatType);
}

//邀请加入联盟
-(void) invitejoin:(NSString *)uid :(NSString *)name
{
    int type = ChatServiceCocos2dx::m_curChatType;
    
    [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"inviteJoinAlliance" :uid :name :@"" :@"" :TRUE ];
    [self onResume:type];
}

- (void)battleMsg:(NSString *)uid :(NSString *)reportUid
{
    NSLog(@"查看战报");
    
    int type = ChatServiceCocos2dx::m_curChatType;
    
    [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"viewBattleReport" :uid :reportUid :@"" :@"" :TRUE ];
    [ServiceInterface serviceInterfaceSharedManager].vc_win.hidden = YES;
    [self onResume:type];
    
}

-(void) detectMsg:(NSString *)uid :(NSString *)detectReportUid
{
    NSLog(@"查看侦查战报");
    
    int type = ChatServiceCocos2dx::m_curChatType;
    
    [ServiceInterface serviceInterfaceSharedManager].vc_win.hidden = YES;
    [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"viewDetectReport" :uid :detectReportUid :@"" :@"" :TRUE ];
    [self onResume:type];
}

-(void) showPlayerInfo:(NSString *)uid :(NSString *)name
{
    
    int type = ChatServiceCocos2dx::m_curChatType;
    
    [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"showPlayerInfo" :uid :name :@"" :@"" :TRUE ];
    [ServiceInterface serviceInterfaceSharedManager].vc_win.hidden = YES;
    [self onResume:type];
}

-(void) resetSend:(ChatCellIOS *)cell
{
    
    [[ChatServiceController chatServiceControllerSharedManager] closekeyboard];
    
    TipPopUpController *tipPopUpController = [[TipPopUpController alloc] initWithNibName:@"TipPopUpController" bundle:nil];
    
    tipPopUpController.cell = cell;
    
    tipPopUpController.tipType = RESENDTYPE;
    
    UIViewController *recentView = [ServiceInterface serviceInterfaceSharedManager].chatViewController;
    
    while (recentView.parentViewController != nil) {
        recentView = recentView.parentViewController;
    }
    
    [recentView.view.window addSubview:tipPopUpController.view];
    
}

-(void) sendRadio:(ChatMessage *)chatMessage
{
    
    [[ChatServiceController chatServiceControllerSharedManager] closekeyboard];
    
    //判断喇叭是否足够  count=ChatController::getInstance()->getNoticePrice();
//    CCLOG("Java_com_elex_chatservice_host_GameHost_isHornEnough  itemid %d",ITEM_SEND_NOTICE);
//    auto& info = ToolController::getInstance()->getToolInfoById(ITEM_SEND_NOTICE);
    
//    if (info.getCNT()>0) {
//
//        TipPopUpController *tipPopUpController = [[TipPopUpController alloc] initWithNibName:@"TipPopUpController" bundle:nil];
//        
//        tipPopUpController.chatMessage = chatMessage;
//        
//        tipPopUpController.tipType = RADIOTYPE;
//        
//        UIViewController *recentView = [ServiceInterface serviceInterfaceSharedManager].chatViewController;
//        
//        while (recentView.parentViewController != nil) {
//            recentView = recentView.parentViewController;
//        }
//        [recentView.view.window addSubview:tipPopUpController.view];
//        
//    //弹出消费喇叭的一个提示
//    }else{
//        TipPopUpController *tipPopUpController = [[TipPopUpController alloc] initWithNibName:@"TipPopUpController" bundle:nil];
//        
//        tipPopUpController.chatMessage = chatMessage;
//        
//        tipPopUpController.tipType = NOHORN;
//        
//        UIViewController *recentView = [ServiceInterface serviceInterfaceSharedManager].chatViewController;
//        
//        while (recentView.parentViewController != nil) {
//            recentView = recentView.parentViewController;
//        }
//        [recentView.view.window addSubview:tipPopUpController.view];
//    }
}

-(void) directlySendMsg:(ChatMessage *)chatMessage
{
    chatMessage.sendState = 0;
    //刷新显示
    ChatCellFrame *cellFrame=[[ChatCellFrame alloc]init:chatMessage];
    //国家
    if (chatMessage.sessionType == 0 && chatMessage.post == 0) {
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController refreshDisplay:cellFrame];
        [self sendText:chatMessage.msg :0];
    //联盟
    }else if(chatMessage.sessionType == 2){
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController refreshDisplay:cellFrame];
        [self sendText:chatMessage.msg :2];
    //邮件
    }else if(chatMessage.sessionType == 3){
        [[ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController refreshDisplay:cellFrame];
        [self sendText:chatMessage.msg :3];
    //喇叭
    }else if(chatMessage.post == 6 && chatMessage.sessionType == 0){
        //判断是否能够发出喇叭 喇叭功能没有先上屏功能 广播走单独的接口
        [self sendRadio:chatMessage];
    }
    
}

-(void) initChatToHistory
{
//    CCArray* chatInfoArr=CCArray::create();
//    for(int i=ChatController::getInstance()->m_chatAlliancePool.size()-1;i>=0;i--){
//        ChatInfo chat = ChatController::getInstance()->m_chatAlliancePool[i];
//        ChatMailInfo* chatInfo=ChatMailInfo::create(chat,false);
//        chatInfoArr->addObject(chatInfo);
//    }
//    ChatServiceCocos2dx::handleChatPush(chatInfoArr,CHAT_ALLIANCE);
}

-(int) radioCount
{
//    return ChatController::getInstance()->getNoticePrice();
    return 1;
}

-(void) shieldPlayer:(NSString *)uid
{
//    ChatController::getInstance()->shieldPlayer([uid UTF8String]);
}

-(BOOL) isSatisfySendRadio
{
//    return CCCommonUtils::isEnoughResourceByType(Gold, ChatController::getInstance()->getNoticePrice());
    return true;
}

-(void) sendNotice:(NSString *)msg :(int)itemid :(BOOL)usePoint :(NSString *)sendLocalTime
{
//    ChatController::getInstance()->sendNotice([msg UTF8String],200011,true,[sendLocalTime UTF8String]);
}

-(void) banPlayer:(NSString *)uid :(int)banTime
{
    //ChatController::getInstance()->banPlayer([uid UTF8String],banTime);
}

-(void) sendRequestChatFromAndroid:(int)type
{
//    ChatController::getInstance()->sendRequestChatFromAndroid(type);
}

-(NSString*) headPath:(NSString *)url
{
    //string customHeadPicPath = HFUrlImageCache::shared()->getCachePath([url UTF8String]);
    return @"1";
}

-(void) downHeadImage:(NSString *)url
{
 //   HFUrlImageCache::shared()->downHeadImage([url UTF8String]);
}

-(void) initChatToHistory_allance{}

-(void) initChatToHistory_countries{}

@end
