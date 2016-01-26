//
//  GameHost.m
//  IF
//
//  Created by mzc on 15/4/20.
//
//

#import "GameHost.h"
#import "LocalController.h"
#import "LanguageManager.h"
#include "ChatServiceCocos2dx.h"
#include "ChatController.h"
#import "ChatServiceController.h"
#import "BanPopUpController.h"
#import "TipPopUpController.h"
#import "ServiceInterface.h"
#include "WorldController.h"
#include "SceneController.h"
#include "WorldMapView.h"
#import "LanguageKeys.h"
#include "ChatController.h"
#include "HFUrlImageCache.h"
#include "MailController.h"
#include "UserManager.h"
#import "ServiceInterface.h"
#include "HFUrlImageCache.h"
#include "UIComponent.h"
#import "UserGroup.h"
#import "ChannelManager.h"
#import "MsgMessage.h"
#include "GetMsgBySeqIdCommand.h"
#import "MailPopUpView.h"
#include "EquipmentBagView.h"
#include "RewardController.h"
#import "MailInfoDataModel.h"
#import "RewardParams.h"
#import "DropParams.h"
//#import "CCFileUtilsIOS.h"
#import "ResourceHelpMailData.h"
#import "ResourceMailData.h"
//#import "CCJSONConverter.h"
#import "BattleMailData.h"
#include "CCJSONConverter.h"
#import "MailInfoManager.h"
#import "JSONKit.h"
#import "FriendsController.h"
#import "YesNoDialog.h"

#import "NSString+Cocos2dHelper.h"
#import "CSMessage.h"
#import "CSMessageModel.h"
void addValueToCCDict(id key, id value, CCDictionary* pDict);

@interface GameHost()
@property (nonatomic,strong) TipPopUpController *tipPopUpController;
@end

@implementation GameHost

-(NSArray*) gettingChatLangArray
{
    const char* chatLang[] = {"E100068","115020","105207","105209","105210","105300","105302","105304","105307","105308"
        ,"105309","105312","105313","105315","105316","105321","105322","105502","105602","108584","115922","115923"
        ,"115925","115926","115929","115181","115182","115281","115282","115168","115169","115170","105326","105327"
        ,"105328","105324","105325","115068","confirm","cancel_btn_label","114110","104932","105564","101205","105329"
        ,"105522","105591","105330","105332","105333","104371","104912","105331","115100","115101","115102","115103"
        ,"115104","103000","105348","105349","105350","105351","105352","105353","105354","105355","105344","119004"
        ,"113907","105356","105357","111660","111665","105777"};
    
    int len=sizeof(chatLang)/sizeof(char *);
    
    NSMutableArray *chatLangArray = [[NSMutableArray alloc]init];
    
    for(int  i = 0; i < len; i++)
    {
        if (chatLang[i] == "115104") {
            CCLog("115104");
        }
        NSString *key = [NSString stringWithUTF8String:chatLang[i]];
        std::string str = _lang(chatLang[i]);
        NSString *langValue = [[NSString alloc]initWithUTF8String:str.c_str()];
        LanguageItem *languageItem = [[LanguageItem alloc]init:key :langValue];
        [chatLangArray addObject:languageItem];
        
    }
    
    return [NSArray arrayWithArray:chatLangArray];
    
}

-(void) onResume:(int)chatType
{

    ChatServiceCocos2dx::m_channelType=chatType;

    ChatServiceCocos2dx::isChatShowing=false;
    ChatServiceCocos2dx::isForumShowing=false;
    
    if(IOSScheduleObject::getInstance()->actionAfterResume != "") {
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(IOSScheduleObject::handleResume), IOSScheduleObject::getInstance(), 0.0f, 0, 0.01f, false);
    }
    if(ChatServiceCocos2dx::m_isInMailDialog)
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(IOSScheduleObject::updateMailCell), IOSScheduleObject::getInstance(), 0.0f, 0, 1.0f, false);
    else
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(IOSScheduleObject::showLatestMessage), IOSScheduleObject::getInstance(), 0.0f, 0, 1.0f, false);
}

-(void) setActionAfterResume:(NSString*) action :(NSString*) uid :(NSString*) name :(NSString*) reportUid :(NSString*) detectReportUid :(int)equipId :(BOOL) returnToChatAfterPopup
{
    if (action.length > 0 ) {
        IOSScheduleObject::getInstance()->actionAfterResume = [action UTF8String];
    }else{
        IOSScheduleObject::getInstance()->actionAfterResume = "";
    }
    
    if (uid.length > 0 ) {
        IOSScheduleObject::getInstance()->uid = [uid UTF8String];
    }else{
        IOSScheduleObject::getInstance()->uid = "";
    }
    
    if (name.length > 0 ) {
        IOSScheduleObject::getInstance()->name = [name UTF8String];
    }else{
        IOSScheduleObject::getInstance()->name = "";
    }
    
    if (reportUid.length > 0 ) {
        IOSScheduleObject::getInstance()->reportUid = [reportUid UTF8String];
    }else{
        IOSScheduleObject::getInstance()->reportUid = "";
    }
    
    if (detectReportUid.length > 0 ) {
        IOSScheduleObject::getInstance()->detectReportUid = [detectReportUid UTF8String];
    }else{
        IOSScheduleObject::getInstance()->detectReportUid = "";
    }
    
    if (equipId > 0) {
        IOSScheduleObject::getInstance()->equipId = equipId;
    }else{
        IOSScheduleObject::getInstance()->equipId = 0;
    }
    
    IOSScheduleObject::getInstance()->returnToChatAfterPopup = returnToChatAfterPopup;
}

//解除屏蔽玩家
-(void) unBlock:(NSString *)uid :(NSString *)name
{
    [[UserManager sharedUserManager] removeRestrictUser:uid :1];
    ChatController::getInstance()->unShieldPlayer([uid UTF8String],[name UTF8String]);
}

//屏蔽玩家
-(void) block:(NSString*) uid :(NSString*)name
{
    [[UserManager sharedUserManager] addRestrictUser:uid :1];
    ChatController::getInstance()->shieldPlayer([uid UTF8String]);
}

//解除禁言玩家
-(void) unBan:(NSString*) uid :(NSString*)name
{
    [[UserManager sharedUserManager] removeRestrictUser:uid :2];
    ChatController::getInstance()->unBanPlayer([uid UTF8String]);
}

//禁言玩家
-(void) ban:(NSString*) uid :(NSString*)name :(int) banTime
{
    DVLog(@"禁言  ");
    DVLog(@"禁言uid： %@ \n 禁言名字:%@ \n 禁言时间等级:%d",uid,name,banTime);
    [[UserManager sharedUserManager] addRestrictUser:uid :2];
    ChatController::getInstance()->banPlayer([uid UTF8String],banTime);
}
-(void)banPlayerNoticeWithUserUid:(NSString *)vUidString andWithBanTimeLever:(int)vTimeLever{
    DVLog(@"喇叭消息禁言uid： %@  \n 禁言时间等级:%d",vUidString,vTimeLever);
    [[UserManager sharedUserManager] addRestrictUser:vUidString :2];
    ChatController::getInstance()->banPlayerNotice([vUidString UTF8String],vTimeLever);
}
-(void)closeBanPlayNoticeWithUserUid:(NSString *)vUidString{
    [[UserManager sharedUserManager] removeRestrictUser:vUidString :2];
    ChatController::getInstance()->unBanPlayerNotice([vUidString UTF8String]);
}
//立即加入
-(void) joinAlliance:(NSString *)allianceId :(NSString*)name
{
    int type = ChatServiceCocos2dx::m_channelType;
    
    [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"joinAlliance" :allianceId :name :@"" :@"" :0:TRUE ];
    [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:type];
}

//跳转坐标
-(void) skipPos:(NSString *)posX :(NSString *)posY
{
    CCPoint pt = ccp(atoi([posX UTF8String]), atoi([posY UTF8String]));
    int worldIndex = WorldController::getIndexByPoint(pt);
    WorldController::getInstance()->openTargetIndex = worldIndex;
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        //zym 2015.12.11
//        PopupViewController::getInstance()->forceClearAll(true); test
        WorldMapView::instance()->gotoTilePoint(pt);
    }else{
        int index = WorldController::getIndexByPoint(pt);
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, worldIndex);
    }
}
#pragma mark -
#pragma mark 发送消息
//发送信息
-(void) sendText:(NSMsgItem *)cm :(int)m_curChatType
{
    //组装发送数据
    string msg = [cm.msg UTF8String];
    int msgType = m_curChatType;
    int post = cm.post;
    NSString *sendLocalTime =  [NSString stringWithFormat:@"%ld",cm.sendLocalTime];
    std::string createTime = [sendLocalTime UTF8String];
    ChatController::getInstance()->sendCountryChat(msg.c_str(), msgType,post,createTime);
}

-(void)sendTextWithChatMessage:(CSMessage *)vMsg andWithSendMsgType:(int )vSendType{
    //组装发送数据
    string msg = [vMsg.msg UTF8String];
    int msgType = vSendType;
    int post = (int )vMsg.post;
    NSString *sendLocalTime =  [NSString stringWithFormat:@"%ld",vMsg.sendLocalTime];
    std::string createTime = [sendLocalTime UTF8String];
    ChatController::getInstance()->sendCountryChat(msg.c_str(), msgType,post,createTime);
}

-(void) sendMail:(NSMsgItem *)mi
{
    NSString *toNameStr = nil;
    NSString *allianceUidStr = nil;
    
    if ([[UserManager sharedUserManager].currentMail.opponentUid isEqualToString:[UserManager sharedUserManager].currentUser.uid]) {
        toNameStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_ALLIANCE];
        allianceUidStr = [UserManager sharedUserManager].currentUser.allianceId;
    }else{
        toNameStr = [UserManager sharedUserManager].currentMail.opponentName;
        allianceUidStr = @"";
    }
    
    NSString *contentStr = mi.msg;
    NSString *mailUid = [UserManager sharedUserManager].currentMail.opponentUid;
    NSString *sendLocalTime = [NSString stringWithFormat:@"%ld",mi.sendLocalTime];
    BOOL isflag = [UserManager sharedUserManager].currentMail.isCurChannelFirstVisit;
    int type = [UserManager sharedUserManager].currentMail.type;
    
    //    MailController::getInstance()->sendMail([toNameStr UTF8String], "", [contentStr UTF8String],[allianceUidStr UTF8String],[mailUid UTF8String],(bool)isflag,(int)type);
    
    //    std::string toName, std::string title, std::string contents, std::string sendLocalTime, std::string allianceId,std::string mailUid,bool isflag, int type,std::string targetUid,std::string thxMail,bool showTip
    
    MailController::getInstance()->sendMailToServer([toNameStr UTF8String], "", [contentStr UTF8String],[sendLocalTime UTF8String],[allianceUidStr UTF8String],[mailUid UTF8String],(bool)isflag,(int)type,[mailUid UTF8String]);
    
}
-(void) sendMailWithMessage:(CSMessage *)mi
{
    NSString *toNameStr = nil;
    NSString *allianceUidStr = nil;
    
    if ([[UserManager sharedUserManager].currentMail.opponentUid isEqualToString:[UserManager sharedUserManager].currentUser.uid]) {
        //发送全体联盟邮件
        toNameStr =[NSString stringWithMultilingualWithKey:@"105564"];//    "105564"=全体联盟成员
//        toNameStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_ALLIANCE];
        allianceUidStr = [UserManager sharedUserManager].currentUser.allianceId;
    }else{
        toNameStr = [UserManager sharedUserManager].currentMail.opponentName;
        allianceUidStr = @"";
    }
    
    NSString *contentStr = mi.msg;
    NSString *mailUid = [UserManager sharedUserManager].currentMail.opponentUid;
    NSString *sendLocalTime = [NSString stringWithFormat:@"%ld",mi.sendLocalTime];
    BOOL isflag = [UserManager sharedUserManager].currentMail.isCurChannelFirstVisit;
    int type = [UserManager sharedUserManager].currentMail.type;
    
    
    MailController::getInstance()->sendMailToServer([toNameStr UTF8String], "", [contentStr UTF8String],[sendLocalTime UTF8String],[allianceUidStr UTF8String],[mailUid UTF8String],(bool)isflag,(int)type,[mailUid UTF8String]);
    
}

-(void) sendChatRoomMsg:(NSString*) msg :(NSString*) groupId sendLocalTime:(NSString*)sendLocalTime{
    DVLog(@"sendChatRoomMsg 聊天室发送消息");
    std::string ccMsg = [msg UTF8String];
    std::string ccGroupId = [groupId UTF8String];
    std::string ccSendLocalTime = [sendLocalTime UTF8String];
    MailController::getInstance()->sendChatRoomMsg(ccMsg, ccGroupId,ccSendLocalTime);
}
 

//邀请加入联盟
-(void) invitejoin:(NSString *)uid :(NSString *)name
{
    int type = ChatServiceCocos2dx::m_channelType;
    
    [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"inviteJoinAlliance" :uid :name :@"" :@"" :0 :TRUE ];
    [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:type];
}

- (void)battleMsg:(NSString *)uid :(NSString *)reportUid
{
    DVLog(@"查看战报");
    
    if (uid.length > 0 && reportUid.length > 0){
        int type = ChatServiceCocos2dx::m_channelType;
        [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"viewBattleReport" :uid :@"":reportUid :@"" :0 :TRUE ];
        [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:type];
    }
}

-(void) detectMsg:(NSString *)uid :(NSString *)detectReportUid
{
    DVLog(@"查看侦查战报");
    
    int type = ChatServiceCocos2dx::m_channelType;
    
    if (uid.length > 0 && detectReportUid.length > 0) {
        [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"viewDetectReport" :uid :@"" :@"" :detectReportUid :0 :TRUE ];
        [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:type];
    }
}

-(void) showPlayerInfo:(NSString *)uid :(NSString *)name
{
    
//    int type = ChatServiceCocos2dx::m_channelType;
    
    [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"showPlayerInfo" :uid :name :@"" :@"" :0 :TRUE ];
    
//    if (type == IOS_CHANNEL_TYPE_COUNTRY || type == IOS_CHANNEL_TYPE_ALLIANCE) {
//        [self onResume:type];
//    }else{
//        [self onResume:IOS_CHANNEL_TYPE_MAILLIST];
//    }
    
}

-(void) resetSend:(ChatCellIOS *)cell
{
    
    [[ChatServiceController chatServiceControllerSharedManager] closekeyboard];
    
    if(cell.cellFrame.chatMessage.post == 6 && cell.cellFrame.chatMessage.channelType == 0){
        
        [[ChatServiceController chatServiceControllerSharedManager].gameHost sendRadio:cell.cellFrame.chatMessage];
    }else{
        [cell exitResetSend];
    }
    
}

-(void) sendRadio:(NSMsgItem *)chatMessage
{
    
    CCLOG("Java_com_elex_chatservice_host_GameHost_isHornEnough  itemid %d",ITEM_SEND_NOTICE);
    
    [[ChatServiceController chatServiceControllerSharedManager] closekeyboard];
    
    BOOL radioCountFlag = [self isPopUpRadioConsumptionTip];
    BOOL radioMoneyFlag = [self isPopUpMoneyConsumptionTip];
    BOOL firstDeductCount = [ServiceInterface serviceInterfaceSharedManager].isFirstDeductRadioCount;
    BOOL firstDeductMoney = [ServiceInterface serviceInterfaceSharedManager].isFirstDeductRadioMoney;
    
    if (radioCountFlag && !firstDeductCount)
        [self sendRadioConsumptionType:FALSE msg:chatMessage];
    else if (radioMoneyFlag && !firstDeductMoney)
        [self sendRadioConsumptionType:TRUE msg:chatMessage];
    else if (radioCountFlag && firstDeductCount) {
        [self popUpConsumptionTip:RADIOTYPE msg:chatMessage];
    }else{
        [self popUpConsumptionTip:NOHORN msg:chatMessage];
    }
    
}
/**
 *  发送喇叭使用道具类型
 *
 *  @param flag true 使用金币 false 使用喇叭
 *  @param mi 发送信息
 */
-(void) sendRadioConsumptionType:(BOOL)flag msg:(NSMsgItem*)mi
{
    
    //    ChatCellFrame *cellFrame=[[ChatCellFrame alloc]init:mi];
    
    //    [[MsgMessage msgMessageShared] addChatMsgList:mi];
    //    [[ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController refreshDisplay:cellFrame];
    //自己发的消息先上屏新逻辑
    [[ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.currentChannel  savingMsgForSelfSendToServerWithMsgItem:mi];
    [[ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.currentChannel chatNewMsgPushed:@[mi]];

    
     NSString *sendLoaclTime = [NSString stringWithFormat:@"%ld",mi.sendLocalTime];
    //给服务器发送喇叭消息
    [[ChatServiceController chatServiceControllerSharedManager] sendNotice:mi.msg :200011 :flag :sendLoaclTime];
}

/**
 *  判断喇叭道具数量是否大于0个
 *
 *  @return
 */
-(BOOL) isPopUpRadioConsumptionTip
{
    auto& info = ToolController::getInstance()->getToolInfoById(ITEM_SEND_NOTICE);
    BOOL flag = info.getCNT() > 0;
    return flag;
}
/**
 *  判断使用喇叭金币是否足够
 *
 *  @return
 */
-(BOOL) isPopUpMoneyConsumptionTip
{
    return [[ChatServiceController chatServiceControllerSharedManager] isSatisfySendRadio];
}
/**
 *  发送喇叭弹出提示框
 *
 *  @param type 弹出提示框类型
 *  @param msg  弹出提示框显示数据
 */
-(void) popUpConsumptionTip:(TipType) type msg:(NSMsgItem*)msg
{
    if (!self.tipPopUpController){
         self.tipPopUpController = [[TipPopUpController alloc] initWithNibName:@"TipPopUpController" bundle:nil];
    }
   
    self.tipPopUpController.chatMessage = msg;
    
    self.tipPopUpController.tipType = type;
    
    UIViewController *recentView = [ServiceInterface serviceInterfaceSharedManager].chatViewController;
    
    while (recentView.parentViewController != nil) {
        recentView = recentView.parentViewController;
    }
    [recentView.view addSubview:self.tipPopUpController.view];
}

//消息发送接口
-(void) directlySendMsg:(NSMsgItem *)chatMessage
{
    chatMessage.sendState = 0;
    //刷新显示
    ChatCellFrame *cellFrame=[[ChatCellFrame alloc]init:chatMessage];
    //国家
    if (chatMessage.channelType == IOS_CHANNEL_TYPE_COUNTRY && chatMessage.post == 0) {
         chatMessage.sendState = 1;
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.currentChannel savingMsgForSelfSendToServerWithMsgItem:chatMessage];
        chatMessage.sendState = 0;
        
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.currentChannel chatNewMsgPushed:@[chatMessage]];
 
        [self sendText:chatMessage :IOS_CHANNEL_TYPE_COUNTRY];
        //联盟
    }else if(chatMessage.channelType == IOS_CHANNEL_TYPE_ALLIANCE && chatMessage.post == 0){
        //保存联盟频道的个人发送数据
//        [[MsgMessage msgMessageShared] addChatMsgList:chatMessage];
        chatMessage.sendState = 1;
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.currentChannel savingMsgForSelfSendToServerWithMsgItem:chatMessage];
        chatMessage.sendState = 0;   
         [[ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.currentChannel chatNewMsgPushed:@[chatMessage]];
 
        [self sendText:chatMessage :2];
        //邮件
    }else if(chatMessage.channelType == IOS_CHANNEL_TYPE_USER){
        //用户自己的邮件信息数据存在临时容器中一份
//        [[MsgMessage msgMessageShared] addChatMsgList:chatMessage];
        
        NSString *channelID = nil;
        if ([UserManager sharedUserManager].currentMail.type == 23 || [UserManager sharedUserManager].currentMail.type == 24) {
            channelID = [UserManager sharedUserManager].currentMail.opponentUid;
            channelID = [channelID stringByAppendingString:@"@mod"];
        }else{
            channelID = [UserManager sharedUserManager].currentMail.opponentUid;
        }
        chatMessage.sendState = 1;
        [[ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController.currentChatChannel savingMsgForSelfSendToServerWithMsgItem:chatMessage];
        chatMessage.sendState = 0;
        [[ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController refreshDisplay:cellFrame :channelID];
        
        ChatChannel *cc = [[ChannelManager sharedChannelManager] gettingChannelInfo:channelID];
        chatMessage.sendState = 1;
        [cc savingMsgItemToDBWithMsgItem:chatMessage];
        chatMessage.sendState = 0;
        [self sendMail:chatMessage];
        //喇叭
    }else if(chatMessage.post == 6 && chatMessage.channelType == IOS_CHANNEL_TYPE_COUNTRY){
        //判断是否能够发出喇叭 喇叭功能没有先上屏功能 广播走单独的接口
        [self sendRadio:chatMessage];
        //群聊
    }else if(chatMessage.channelType == IOS_CHANNEL_TYPE_CHATROOM){
//        [[MsgMessage msgMessageShared] addChatMsgList:chatMessage];
        ChatChannel *cc = [[ChannelManager sharedChannelManager] gettingChannelInfo:[UserManager sharedUserManager].currentMail.opponentUid];
        chatMessage.sendState = 1;
        [cc savingMsgItemToDBWithMsgItem:chatMessage];
        chatMessage.sendState = 0;
        [[ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController refreshDisplay:cellFrame :[UserManager sharedUserManager].currentMail.opponentUid];
        NSString *sendLoaclTime = [NSString stringWithFormat:@"%ld",chatMessage.sendLocalTime];
        [self sendChatRoomMsg:chatMessage.msg :[UserManager sharedUserManager].currentMail.opponentUid sendLocalTime:sendLoaclTime];
    }
    
}


-(int) radioCount
{
    return ChatController::getInstance()->getNoticePrice();
}

-(BOOL) isSatisfySendRadio
{
    return CCCommonUtils::isEnoughResourceByType(Gold, ChatController::getInstance()->getNoticePrice());
}

-(void) sendNotice:(NSString *)msg :(int)itemid :(BOOL)usePoint :(NSString *)sendLocalTime
{
    ChatController::getInstance()->sendNotice([msg UTF8String],200011,usePoint,[sendLocalTime UTF8String]);
}

-(NSString*) headPath:(NSString *)url
{
    string customHeadPicPath = HFUrlImageCache::shared()->getCachePath([url UTF8String]);
    return [NSString stringWithUTF8String:customHeadPicPath.c_str()];
}

-(void) downHeadImage:(NSString *)url
{
    if(url)
        HFUrlImageCache::shared()->downHeadImage([url UTF8String]);
}

-(void) updateMailList
{
    MailController::getInstance()->updateMailList();
    UIComponent::getInstance()->refreshUIComponent();
    MailController::getInstance()->m_mailInfoSendDic->removeAllObjects();
}

-(void) openMailDialogViewFirst:(NSString *)fromUid :(NSString *)fromName
{
    MailController::getInstance()->openMailDialogViewFirst([fromName UTF8String], [fromUid UTF8String]);
}

-(void) updateChatInfo:(int)type
{
    if (type == 0) {
        UIComponent::getInstance()->showCountryIcon(true);
    }else{
        UIComponent::getInstance()->showCountryIcon(false);
    }
    
}

//-(void) requestCountriesData
//{
//    [ServiceInterface serviceInterfaceSharedManager].isLoadCountriesData = FALSE;
//    if ([self isfromServiceOrLocal_courtries]) {
//        [self initChatToHistory_countries];
//    }else{
//        CCCommonUtils::getGameDataAfterInit();
//    }
//
//}
//
//-(void) requestAllanceData
//{
//    [ServiceInterface serviceInterfaceSharedManager].isLoadAllanceData = FALSE;
//    if ([self isfromServiceOrLocal_allance]) {
//        [self initChatToHistory_allance];
//    }else{
//        CCCommonUtils::getGameDataAfterInit();
//    }
//
//}

//-(void) getGameDataAfterInit
//{
//    if(ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_ALLIANCE){
//        [ServiceInterface serviceInterfaceSharedManager].isLoadAllanceData = FALSE;
//    }else if(ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_COUNTRY){
//        [ServiceInterface serviceInterfaceSharedManager].isLoadCountriesData = FALSE;
//    }
//    CCCommonUtils::getGameDataAfterInit();
//}

-(BOOL) isfromServiceOrLocal_courtries
{
    int i = ChatController::getInstance()->m_chatCountryPool.size();
    if (i > 0) {
        return TRUE;
    }else{
        return FALSE;
    }
    
}

-(BOOL) isfromServiceOrLocal_allance
{
    int i = ChatController::getInstance()->m_chatAlliancePool.size();
    if (i > 0) {
        return TRUE;
    }else{
        return FALSE;
    }
}

-(void) joinAllianceBtnClick
{
    
    [[ServiceInterface serviceInterfaceSharedManager].chatViewController.topUIView selectCoun];
    
    int type = ChatServiceCocos2dx::m_channelType;
    
    NSString *ns_uid = [UserManager sharedUserManager].currentUser.uid;
    NSString *ns_name = [UserManager sharedUserManager].currentUser.userName;
    
    [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"joinAllianceBtnClick" :ns_uid :ns_name :@"" :@"" :0:TRUE ];
    [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:type];
    [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
    
}

-(void) changeCountry
{
    
    ChatServiceCocos2dx::m_channelType = IOS_CHANNEL_TYPE_COUNTRY;
    
}

-(NSMutableArray*) getUserInfoArray:(int)index
{
    //    NSMutableArray *rank1=[[NSMutableArray alloc]init];
    //    NSMutableArray *rank2=[[NSMutableArray alloc]init];
    //    NSMutableArray *rank3=[[NSMutableArray alloc]init];
    //    NSMutableArray *rank4=[[NSMutableArray alloc]init];
    //    NSMutableArray *rank5 =[[NSMutableArray alloc]init];
    //    NSArray *arr=@[rank1,rank2,rank3,rank4,rank5];
    
    //    CCLOG("Java_com_elex_chatservice_host_GameHost_getUserInfoArray");
    CCArray* memberArray=dynamic_cast<CCArray*>(ChatController::getInstance()->m_userInfoDic->objectForKey((int)index)) ;
    int len=memberArray->count();
    CCLOG("len: %i", memberArray->count());
    NSMutableArray *userInfos = [[NSMutableArray alloc]init];
    //给每一个实例的变量赋值，并且将实例作为一个object，添加到objcet数组中
    for(int  i = 0; i < len; i++)
    {
        CCDictionary* dic=dynamic_cast<CCDictionary*>(memberArray->objectAtIndex(i));
        NSUserInfo *userInfo = [[NSUserInfo alloc]init];
        NSString *ns_uid = [NSString stringWithUTF8String:dic->valueForKey("uid")->getCString()];
        NSString *ns_name = [NSString stringWithUTF8String:dic->valueForKey("userName")->getCString()];
        NSString *ns_headPicStr = [NSString stringWithUTF8String:dic->valueForKey("headPic")->getCString()];
        userInfo.uid = ns_uid;
        userInfo.userName = ns_name;
        userInfo.headPic = ns_headPicStr;
        
        if(dic->objectForKey("allianceRank")){
            int allianceRank=(dynamic_cast<CCInteger*>(dic->objectForKey("allianceRank")))->getValue();
            CCLOG("allianceRank: %i", allianceRank);
            userInfo.allianceRank = allianceRank;
        }
        if(dic->objectForKey("allianceId")){
            NSString *ns_allianceId = [NSString stringWithUTF8String:dic->valueForKey("allianceId")->getCString()];
            userInfo.allianceId = ns_allianceId;
        }
        
        if(dic->objectForKey("asn")){
            NSString *ns_asnStr = [NSString stringWithUTF8String:dic->valueForKey("asn")->getCString()];
            userInfo.asn = ns_asnStr;
        }
        
        
        userInfo.headPicVer = (dynamic_cast<CCInteger*>(dic->objectForKey("headPicVer")))->getValue();
        userInfo.mGmod = (dynamic_cast<CCInteger*>(dic->objectForKey("mGmod")))->getValue();
        userInfo.lastUpdateTime = (dynamic_cast<CCInteger*>(dic->objectForKey("lastUpdateTime")))->getValue();
        userInfo.vipLevel = (dynamic_cast<CCInteger*>(dic->objectForKey("vipLevel")))->getValue();
        if (dic->objectForKey("vipEndTime")) {
            int vipEndTime=dynamic_cast<CCInteger*>(dic->objectForKey("vipEndTime"))->getValue();
            userInfo.vipEndTime = vipEndTime;
        }
        
        userInfo.serverId = (dynamic_cast<CCInteger*>(dic->objectForKey("serverId")))->getValue();
        
        if(dic->objectForKey("crossFightSrcServerId")){
            userInfo.crossFightSrcServerId = (dynamic_cast<CCInteger*>(dic->objectForKey("crossFightSrcServerId")))->getValue();
        }
        
        if  ([userInfo.uid isEqualToString:[UserManager sharedUserManager].currentUser.uid]){
            userInfo.chooseState = ChooseState_selected;
        }
        
        //添加到objcet数组中
        
        //        if (userInfo.allianceRank == 1) {
        //            [rank1 addObject:userInfo];
        //        }else if (userInfo.allianceRank == 2){
        //            [rank2 addObject:userInfo];
        //        }else if (userInfo.allianceRank == 3){
        //            [rank3 addObject:userInfo];
        //        }else if ( userInfo.allianceRank == 4){
        //            [rank4 addObject:userInfo];
        //        }else{
        //            [rank5 addObject:userInfo];
        //        }
        [userInfos addObject:userInfo];
        
    }
    
    //    for (NSArray *tempArr in arr) {
    //        if (tempArr.count>0) {
    //            UserGroup *tempGroup =[UserGroup userGroup];
    //            tempGroup.grade = [(UserInfo *)[tempArr objectAtIndex:0]allianceRank];
    //            tempGroup.open = YES;
    //            tempGroup.groupNameString= [[UserManager sharedUserManager]getRankLang:tempGroup.grade];
    //            tempGroup.memberArray = tempArr;
    //            [userInfos addObject:tempGroup];
    //        }
    //    }
    
    
    return userInfos;
}

-(void) getMultiUserInfo:(NSString*)ns_uidsStr
{
    DVLog(@"%@",ns_uidsStr);
    
    vector<string> strVec;
    vector<std::string> *uids = new vector<std::string>();
    std::string uidsStr = [ns_uidsStr UTF8String];
    CCCommonUtils::splitString(uidsStr, "|", strVec);
    CCLOGFUNCF("strVec.size() :%d",strVec.size());
    
    for (int i = 0; i<strVec.size(); i++)
    {
        CCLOGFUNCF("strVec.at(i) :%s",strVec.at(i).c_str());
        uids->push_back(strVec.at(i));
    }
    
    ChatController::getInstance()->getMultiUserInfo(uids);
}

-(void) selectChatRoomMember:(NSString *)roomName :(NSString *)memberName :(NSString *)memberUid
{
    std::string ccRoomName = [roomName UTF8String];
    std::string ccRoomMemberName = [memberName UTF8String];
    std::string ccRoomMemberUid = [memberUid UTF8String];
    
    MailController::getInstance()->setChatRoomInfo(ccRoomName,ccRoomMemberName,ccRoomMemberUid);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CHAT_ROOM_NAME_REFRESH);
}
/**
 *  群组加人
 *
 *  @param memberName     组ID
 *  @param memberUidArray 成员uid 数组
 *  @param groupId        成员名称
 *
 *  (std::string memberName,CCArray* memberUidArray,std::string groupId)
 */
-(void)addGroupChatMemberWithGroupID:(NSString *)vGroupID
               andWithMemberUidArray:(NSArray  *)vMemberUidArray
                   andWithMemberName:(NSString *)vMemberName
{
    DVLog(@"添加的UID :%@",vMemberUidArray);
    const char * ccRoomMemberName = [vMemberName UTF8String];
    const char * groupIdCCString = [vGroupID UTF8String];
    CCArray *uidCCArr =CCArray::create();
    for (NSString *uidString in vMemberUidArray) {
        CCString *uidCCString =CCString::create([uidString UTF8String]);
        uidCCArr->addObject(uidCCString);
    }
    
    MailController::getInstance()->inviteChatRoomMembers(ccRoomMemberName ,uidCCArr,groupIdCCString);
    
}

/**
 *  群组减人
 *
 *  @param memberName     组ID
 *  @param memberUidArray 成员uid 数组
 *  @param groupId        成员名称
 *
 * std::string memberName,CCArray* memberUidArray,std::string groupId
 */
-(void)subGroupChatMemberWithGroupID:(NSString *)vGroupID
               andWithMemberUidArray:(NSArray  *)vMemberUidArray
                   andWithMemberName:(NSString *)vMemberName
{
    std::string ccRoomMemberName = [vMemberName UTF8String];
    std::string groupIdCCString = [vGroupID UTF8String];
    CCArray *uidCCArr =CCArray::create();
    for (NSString *uidString in vMemberUidArray) {
        CCString *uidCCString =CCString::create([uidString UTF8String]);
        uidCCArr->addObject(uidCCString);
    }
    
    MailController::getInstance()->kickChatRoomMembers(ccRoomMemberName ,uidCCArr,groupIdCCString);
    
}

/**
 *  群组改名
 *
 *  @param vGroupID    群组id
 *  @param vNameString 名字
 modifyChatRoomName(std::string name,std::string groupId)
 */
-(void)reNameGroupChatTitleWithGroupID:(NSString *)vGroupID andWithGroupName:(NSString *)vNameString{
    std::string groupIdCCString =[vGroupID UTF8String];
    std::string groupNameCCString =[vNameString UTF8String];
    MailController::getInstance()->modifyChatRoomName(groupNameCCString, groupIdCCString);
}
/**
 *  退出群组
 *
 *  quitChatRoom(std::string groupId)
 *  @param groupId 群组ID
 */
-(void)quitGroupChatWithGroupID:(NSString *)vGroupId{
    std::string groupIdCCString =[vGroupId UTF8String];
    MailController::getInstance()->quitChatRoom(groupIdCCString);
}



-(NSArray*) getChatInfoArray:(int)chatInfoNo :(NSString *)msgTypeStr
{
    
    
    //    if(chatInfoNo<0 || [msgTypeStr isEqualToString:@""])
    //    {
    //        DVLog(@"(int)chatInfoNo<0");
    //        return NULL;
    //    }
    //    CCArray* chatInfoArr=NULL;
    //    if([msgTypeStr isEqualToString:@"0"]){
    //        chatInfoArr=dynamic_cast<CCArray*>(ChatController::getInstance()->m_chatInfoSendDic->objectForKey((int)chatInfoNo)) ;
    //    }else{
    //        chatInfoArr=dynamic_cast<CCArray*>(MailController::getInstance()->m_mailInfoSendDic->objectForKey((int)chatInfoNo)) ;
    //    }
    //
    //    if(chatInfoArr && chatInfoArr->count()>0)
    //    {
    //        int len=chatInfoArr->count();
    //        CCLOG("chatInfoLen:%d",len);
    //
    //        //新建chatInfo数组
    //        NSMutableArray *ns_chatInfoArr = [[NSMutableArray alloc]init];
    //
    //        for(int  i = 0; i < len; i++)
    //        {
    //            ChatMailInfo* chatInfo=dynamic_cast<ChatMailInfo*>(chatInfoArr->objectAtIndex(i));
    //            if (!chatInfo) {
    //                return NULL;
    //            }
    //
    //
    //            NSString * ns_vip = [NSString stringWithUTF8String:chatInfo->vip.c_str()];
    //            NSString * ns_uid = [NSString stringWithUTF8String:chatInfo->uid.c_str()];
    //            NSString * ns_name = [NSString stringWithUTF8String:chatInfo->name.c_str()];
    //            NSString * ns_asn = [NSString stringWithUTF8String:chatInfo->asn.c_str()];
    //
    //            NSString * ns_msg = [NSString stringWithUTF8String:chatInfo->msg.c_str()];
    //            NSString * ns_translateMsg = [NSString stringWithUTF8String:chatInfo->translateMsg.c_str()];
    //            NSString * ns_headPic = [NSString stringWithUTF8String:chatInfo->headPic.c_str()];
    //            NSString * ns_attachmentId = [NSString stringWithUTF8String:chatInfo->attachmentId.c_str()];
    //            NSString * ns_originalLang = [NSString stringWithUTF8String:chatInfo->originalLang.c_str()];
    //
    //            NSInteger ns_createTime = chatInfo->createTime;
    //            bool isNewMsg = chatInfo->isNewMsg;
    //            bool isSelfMsg = chatInfo->isSelfMsg;
    //            int channelType = chatInfo->channelType;
    //            int gmod = chatInfo->gmod;
    //            int post = chatInfo->post;
    //            int headPicVer = chatInfo->headPicVer;
    //            int sequenceId =  chatInfo->sequenceId;
    //            int lastUpdateTime = chatInfo->lastUpdateTime;
    //            long createTime = chatInfo->createTime;
    //
    //            /**创建信息模型*/
    //            NSMsgItem *cm = [[NSMsgItem alloc]init];
    //
    //            cm.vip = ns_vip;
    //            cm.createTime = ns_createTime;
    //            cm.uid = ns_uid;
    //            cm.name = ns_name;
    //            cm.asn = ns_asn;
    //            cm.msg = ns_msg;
    //            cm.translateMsg = ns_translateMsg;
    //            cm.headPic = ns_headPic;
    //
    //            cm.originalLang = ns_originalLang;
    //
    //            cm.isNewMsg = isNewMsg;
    //            cm.isSelfMsg = isSelfMsg;
    //            cm.channelType = channelType;
    //            cm.gmod = gmod;
    //            cm.post = post;
    //            cm.headPicVer = headPicVer;
    //            cm.sequenceId = sequenceId;
    //            cm.lastUpdateTime = lastUpdateTime;
    //            cm.createTime = createTime;
    //
    //            [cm packedMsg];
    //
    //            cm.sendState = 2;
    //
    //            [ns_chatInfoArr addObject:cm];
    //        }
    //
    //
    //        if([msgTypeStr intValue] == IOS_CHANNEL_TYPE_COUNTRY)
    //            ChatController::getInstance()->m_chatInfoSendDic->removeObjectForKey((int)chatInfoNo);
    //        else if([msgTypeStr intValue] == IOS_CHANNEL_TYPE_ALLIANCE)
    //            MailController::getInstance()->m_mailInfoSendDic->removeObjectForKey((int)chatInfoNo);
    //
    //        return (NSArray *)ns_chatInfoArr;
    //    }
    return NULL;
}

-(void) saveInitChatMsg:(NSArray*) chatMessages :(NSString*)chatMessageId
{
    //    //根据key获取频道 判断是否是第一次打开。如果是第一次。需要加载历史数据
    //    if (![[[ChannelManager sharedChannelManager].channel_map allKeys] containsObject:chatMessageId]) {
    //        //创建一个新的容器 保存玩家的邮件信息
    //        ChatChannel *cc = [[ChatChannel alloc]init];
    //        for(NSMsgItem *chatMessage in chatMessages){
    //            ChatCellFrame *cellFrame=[[ChatCellFrame alloc]init:chatMessage];
    //            [cc.msgList insertObject:cellFrame atIndex:0];
    //        }
    //
    //        [[ChannelManager sharedChannelManager].channel_map setObject:cc forKey:chatMessageId];
    //    }
}

-(ChatChannel*) gettingChatChannel:(NSString*)fromUid
{
    ChatChannel *cc =[[ChannelManager sharedChannelManager] gettingChannelInfo:fromUid];
    return cc;
}

-(void) requestActionHistoryMsg:(NSString*) channelId
{
    ChatChannel *cc = [[ChannelManager sharedChannelManager].channel_map objectForKey:channelId];
    
    int start = cc.dbMinSeqId - 20;
    int end = cc.dbMaxSeqId;
    int channelType = cc.channelType;
    std::string cc_channelId = [cc.channelID UTF8String];
    
    GetMsgBySeqIdCommand* cmd = new GetMsgBySeqIdCommand(start,end,channelType,cc_channelId);
    cmd->sendAndRelease();
}

-(NSString *)gettingCustomHeadPic:(NSString *)uid int:(int)picVer
{
    std::string cc_uid = [uid UTF8String];
    std::string cc_result = CCCommonUtils::getCustomPicUrl(cc_uid,picVer);
    return [NSString stringWithUTF8String:cc_result.c_str()];
}


-(void)gettingUsersWithSearchString:(NSString *)vSearchString {
    std::string  searchString = [vSearchString UTF8String];
    MailController::getInstance()->searchPlayer(searchString);
}


-(NSString *)gettingLocalLanString
{
    return [NSString stringWithUTF8String:LocalController::shared()->getLanguageFileName().c_str()] ;
}

-(void)close2MailList
{
    MailPopUpView* mailView = dynamic_cast<MailPopUpView*>(PopupViewController::getInstance()->getCurrentPopupView());
    if(mailView){
        PopupViewController::getInstance()->removeAllPopupView();
    }
}

-(void)refreshChatInfo:(NSMsgItem *)mi
{
    if(!mi)
    {
        return;
    }
    
    int type = mi.channelType;
    int vip = [mi.vip intValue];
    int post = mi.post;
    if (mi.name.length == 0){
        mi.name = mi.uid;
    }
    string name = [mi.name UTF8String];
    string asn = "";
    if (mi.asn) {
        asn = [mi.asn UTF8String];
    }
    
    string msg = [mi.msg UTF8String];
    
    ChatInfo info = ChatInfo();
    
    info.type = type;
    info.vip = vip;
    info.name = name;
    info.asn = asn;
    info.post = post;
    
    if (![mi isMatchingVersion]){
        info.msg =  _lang("105110");
    }else if (mi.post == 7) {
        vector<string> strVec;
        CCCommonUtils::splitString(msg, "|", strVec);
        if (strVec.size() == 2) {
            std::string equipName = _lang(strVec[1].c_str());
            info.msg =  _lang_1("111660",equipName.c_str());
        }
    }else{
        info.msg = msg;
    }
    
    UIComponent::getInstance()->refreshChatInfoIOS(info);
}
-(void)refreshChatInfoWithCSMessage:(CSMessageModel*)mi
{
    if(!mi)
    {
        return;
    }
    
    int type = mi.message.channelType;
    NSUserInfo *user = [[UserManager sharedUserManager] gettingUserInfoForMemoryAndDBWithUidString:mi.message.uid];
     ChatInfo info = ChatInfo();
     info.type = type;
     info.post = mi.message.post;
    if  (user){
       
        info.vip = [user.vip intValue];
        info.name = [user.userName UTF8String];
        if (user.asn){
             info.asn = [user.asn UTF8String];
        }else{
             info.asn = "";
        }
    }else{
        info.name = [mi.message.uid UTF8String];
        info.asn = "";
        info.vip = 0;
    }
 
    string msg = [ mi.showContentsString UTF8String];
    
    if (mi.messageBodyType == CSMessageBodyType_NotCanParse){
        info.msg =  _lang("105110");
    }else  {
        NSString *showString = mi.showContentsString;
        showString =   [showString stringByReplacingOccurrencesOfString:@"[sysChat]" withString:@""];
        showString = [showString stringByReplacingOccurrencesOfString:@"[equip_share]" withString:@""];
        showString = [showString stringByReplacingOccurrencesOfString:@"[battlereport]" withString:@""];
      
        info.msg = [showString UTF8String];
    } 
    
    DVLog(@"c++消息条：============= %@",[NSString stringWithCString:info.msg.c_str() encoding:4] );
 
    
    UIComponent::getInstance()->refreshChatInfoIOS(info);
}
-(void) gettingChatRoomInfo:(NSString *)chatRoomUid
{
    string groupId = [chatRoomUid UTF8String];
    MailController::getInstance()->getChatRoomMember(groupId);
}

-(void)gettingServiceChannelNewMsg:(ChatChannel *)cc
{
    NSMsgItem *tempMsgItem =  [cc gettingLastMsg];
    int channelType = cc.channelType;
    
    string id = [cc.channelID UTF8String];
    NSString *seqId ;
    if (cc.channelType == IOS_CHANNEL_TYPE_USER){
        seqId = tempMsgItem.mailId;
    }else{
        seqId = [NSString stringWithFormat:@"%ld",tempMsgItem.sequenceId];
    }
    string seqIdStr = [[NSString stringWithFormat:@"%@",seqId] UTF8String];
    string channelTypeStr = [[NSString stringWithFormat:@"%d",channelType] UTF8String];
    
    string param = id + "|" + seqIdStr + "|" + channelTypeStr;
    ChatController::getInstance()->getNewMsg(param);
}

-(BOOL)gettingServiceChannelOldMsg:(ChatChannel *)cc
{
    if (cc.channelType == IOS_CHANNEL_TYPE_USER) {
        NSString *ns_fromUid =cc.channelID;
        
        NSString *ns_mailUid = [UserManager sharedUserManager].currentUser.uid;
        std::string fromUid = [ns_fromUid UTF8String];
        std::string mailUid = [ns_mailUid UTF8String];
        int count = [[self gettingChatChannel:ns_fromUid].msgList count];
        DVLog(@"当前对话邮件条目数 == %d",count);
        MailController::getInstance()->requestMoreMail(fromUid, mailUid, count);
        return TRUE;
    }else{
        int minSeq = [cc gettingChatChannelMinSeqId];
        int maxSeq = [cc gettingChatChannelMaxSeqId];
        DVLog(@"channel：%@  最小seq%d 最大seq%d",cc.channelID,minSeq,maxSeq);
        if ((maxSeq <= cc.serverMaxSeqId && minSeq >= cc.serverMinSeqId) || minSeq != 0) {
            GetMsgBySeqIdCommand* cmd = new GetMsgBySeqIdCommand(minSeq,maxSeq,cc.channelType,[cc.channelID UTF8String]);
            cmd->sendAndRelease();
            return TRUE;
        }else{
            return FALSE;
        }
    }
    
}

-(void)viewEquipment:(NSString *)equipId
{
    if (equipId.length > 0) {
        int id = [equipId intValue];
        if (id > 0) {
            DVLog(@"查看战报");
            int type = ChatServiceCocos2dx::m_channelType;
            [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"checkTheEquip" :@"" :@"":@"" :@"" :id:TRUE ];
            [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:type];
        }
    }
}

-(void)reportCustomHeadPic:(NSString*) uid
{
    ChatController::getInstance()->reportCustomHeadPic([uid UTF8String]);
}

-(void) popUpCocosMailfByData:(MailData *)mailData
{
    
    CCDictionary *dic = CCJSONConverter::sharedConverter()->dictionaryFrom([mailData.jsonStr UTF8String]);

    MailController::getInstance()->addMailFromAndroidToList(dic, true);
     [[ServiceInterface serviceInterfaceSharedManager] hideMailListIOS];
    
    [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"showMailPopup" :mailData.uid :@"":@"" :@"" :0 :TRUE ];
    
    [self onResume:[mailData.type intValue]];
   
//    [self openCocosMailUI2MailId:mailData.uid];

}

-(void) openCocosMailUI2MailId:(NSString*) mailId
{
    std:string cc_uid = [mailId UTF8String];
    
    CCLOGFUNCF("showMailPopup uid:%s",cc_uid.c_str());
    auto search = GlobalData::shared()->mailList.find(cc_uid.c_str());
    bool isExistMail=(search != GlobalData::shared()->mailList.end());
    
    MailInfo* mailInfo=NULL;
    if (isExistMail) {
        mailInfo=dynamic_cast<MailInfo*>(search->second);
    }
    
    if(mailInfo!=NULL)
    {
        CCLOGFUNC("mailInfo!=NULL");
        [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"showMailPopup" :@"" :@"":@"" :@"" :0 :TRUE ];

        MailController::getInstance()->showMailPopupFromAnroid(mailInfo);
        [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:IOS_CHANNEL_TYPE_MAILLIST];
    }
}

-(NSDictionary *)result2JsonWithAddUid:(NSString *)uid
{
    cocos2d::CCDictionary *item = dynamic_cast<cocos2d::CCDictionary*>(MailController::getInstance()->m_mailDataDicIOS->objectForKey([uid UTF8String]));
    
    CCString *contents = dynamic_cast<CCString*>(item->objectForKey("contents"));
    NSString *resultStr = [NSString stringWithUTF8String:contents->getCString()];
    DVLog(@"resultStr == %@",resultStr);
    NSDictionary *resultDict = [resultStr objectFromJSONString];
    
    return resultDict;
}

-(NSString *)gettingPicByType:(int)type withVal:(int)val
{
    std::string iconStr = RewardController::getInstance()->getPicByType(type, val);
    return [NSString stringWithUTF8String:iconStr.c_str()];
}

-(NSString*) int2NSStringWithAddInt:(int) i
{
    return [NSString stringWithFormat:@"%d",i];
}




///**
// *  点击mailListcell 弹出cocos层 不需解析json
// *
// *  @param uid 根据uid取c++端获取dic
// */
//-(void) clickMailListCellWithAddUid:(NSString*) uid
//{
//    [[ServiceInterface serviceInterfaceSharedManager] hideMailListIOS];
//
//    cocos2d::CCDictionary *item = dynamic_cast<cocos2d::CCDictionary*>(MailController::getInstance()->m_mailDataDicIOS->objectForKey([uid UTF8String]));
//    
//    MailController::getInstance()->addMailFromAndroidToList(item, true);
//    
//  std:string cc_uid = [uid UTF8String];
//    
//    CCLOGFUNCF("showMailPopup uid:%s",cc_uid.c_str());
//    auto search = GlobalData::shared()->mailList.find(cc_uid.c_str());
//    bool isExistMail=(search != GlobalData::shared()->mailList.end());
//    
//    MailInfo* mailInfo=NULL;
//    if (isExistMail) {
//        mailInfo=dynamic_cast<MailInfo*>(search->second);
//    }
//    
//    if(mailInfo!=NULL)
//    {
//        CCLOGFUNC("mailInfo!=NULL");
//        MailController::getInstance()->showMailPopupFromAnroid(mailInfo);
//    }
//    
//}

-(void) contentofReportWithUid:(NSString*)uid withContent:(NSString*)content
{
    ChatController::getInstance()->reportPlayerChatContent([uid UTF8String], [content UTF8String]);
}

-(BOOL)isAutoTranslate
{
    return ChatServiceCocos2dx::m_autoTranslate;
}

-(NSString *)gettingNameById:(NSString *)defId
{
    string name=CCCommonUtils::getNameById([defId UTF8String]);
    return [NSString stringWithUTF8String:name.c_str()];
    
}

-(NSString *)gettingPropById:(NSString *)defId ByLeve:(NSString *)defName
{
    string proValue=CCCommonUtils::getPropById([defId UTF8String],[defName UTF8String]);
    return [NSString stringWithUTF8String:proValue.c_str()];
}

-(NSString *)gettingMailIconByName:(NSString *)name
{
    return [[MailInfoManager sharedMailInfoManager] gettingMailIconByName:name];
}

-(NSString *)gettingPointByOccupyIdx:(int)occupyId
{
    string pointStr = MailController::getInstance()->getPointByOccupyIdx(occupyId);
    return [NSString stringWithUTF8String:pointStr.c_str()];
}

-(NSString *)gettingPointByMapIndex:(int)occupyPointId ByType:(int)serverType
{
    string pointStr = MailController::getInstance()->getPointByMapTypeAndIndex(occupyPointId,serverType);
    return [NSString stringWithUTF8String:pointStr.c_str()];
}

/**
 *  集结信息接口
 */
-(void) viewRallyInfoByStr:(NSString*) teamUid
{
     if (teamUid.length> 0){
         ChatController::getInstance()->viewRallyInfo([teamUid UTF8String]);
     }
}

/**
 *  轮盘分享
 */
-(void) viewLotteryInfoById:(NSString*) lotteryInfo;
{
    if (lotteryInfo.length> 0){
        ChatController::getInstance()->viewLotteryInfoFromIOS([lotteryInfo UTF8String]);
    }
}

-(void)changeNickName
{
    
    [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"changeNickName" :@"" :@"":@"" :@"" :0 :TRUE ];
    
    [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:IOS_CHANNEL_TYPE_COUNTRY];
    
}

-(void) gettingFriendsList
{
    if ([[UserManager sharedUserManager].friends_ordinary count] > 0 )
    {
        [[UserManager sharedUserManager].friends_ordinary removeAllObjects];
    }
    
    if ([[UserManager sharedUserManager].friends_FAVO count] > 0 )
    {
        [[UserManager sharedUserManager].friends_FAVO removeAllObjects];
    }
    
    CCDictionary * pDict = FriendsController::getInstance()->m_data;
    
    CCDictElement * pElement;
    
    CCDICT_FOREACH(pDict, pElement)
    
    {
        FriendInfo *friendInfo = (FriendInfo *)pElement->getObject();
        
        if (friendInfo->pic.empty() && friendInfo->allianceId.empty() && friendInfo->uid.empty() && friendInfo->name.empty() && friendInfo->abbr.empty() && friendInfo->lang.empty() && friendInfo->description.empty()) {
            continue;
        }
        
        NSUserInfo *user = [[NSUserInfo alloc] initFriendByPic:[NSString stringWithUTF8String:friendInfo->pic.c_str()] withPicVer:friendInfo->picVer withServerId:friendInfo->serverId withVipEndTime:friendInfo->vipEndTime withAllianceId:[NSString stringWithUTF8String:friendInfo->allianceId.c_str()] withVipLevel:friendInfo->vipLevel withUid:[NSString stringWithUTF8String:friendInfo->uid.c_str()] withCrossFightSrcServerId:friendInfo->crossFightSrcServerId withGmFlag:friendInfo->gmFlag withName:[NSString stringWithUTF8String:friendInfo->name.c_str()] withRank:friendInfo->rank withfriendsType:friendInfo->relation];
        
        if (user.relation == FRIEND_FRIEND || user.relation == FRIEND_FAVO)
        {
            [[UserManager sharedUserManager].friends_ordinary addObject:user];
        }
        
//        [[UserManager sharedUserManager].friends_ordinary addObject:user];
        
//        if (user.relation == FRIEND_FRIEND) {
//            [[UserManager sharedUserManager].friends_ordinary addObject:user];
//        }else if(user.relation == FRIEND_FAVO){
//            [[UserManager sharedUserManager].friends_FAVO addObject:user];
//        }

    }
}

-(BOOL)checkVersionByVersion:(NSString *)version
{
    return CCCommonUtils::checkVersion([version UTF8String]);
}

-(void)openFriendsView
{
    
    [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
    
    [[ChatServiceController chatServiceControllerSharedManager] doHostAction:@"openFriendsView" :@"" :@"":@"" :@"" :0 :TRUE ];
    
    int type = ChatServiceCocos2dx::m_channelType;
    
    [[ChatServiceController chatServiceControllerSharedManager].gameHost onResume:type];
    
}

-(void)gettingRedPackageStatusById:(NSString *)redPackageUid withServerId:(NSString *)serverId
{
    ChatController::getInstance()->getRedPackageStatus([redPackageUid UTF8String],[serverId UTF8String]);
}

-(int)gettingPlayerLevel{
    return GlobalData::shared()->playerInfo.level;
}

-(void)showYesDialogByStr:(NSString *)dialog{
    if (dialog.length > 0){
        std::string c_dialog = (_lang([dialog UTF8String])).c_str();
        if (!c_dialog.empty()) {
            YesNoDialog::showYesDialog((_lang([dialog UTF8String])).c_str());
        }else{
            YesNoDialog::showYesDialog([dialog UTF8String]);
        }
    }
}
@end

