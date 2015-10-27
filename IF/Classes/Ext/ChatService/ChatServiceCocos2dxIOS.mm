//
//  ChatServiceCocos2dxIOS.mm
//  IF
//
//  Created by mzc on 15/4/13.
//
//


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ChatServiceCocos2dx.h"
#include "ChatController.h"
#include "GlobalData.h"
#include "CCDirector.h"
#include "CCSafeNotificationCenter.h"
#include "PopupViewController.h"
#include "LocalController.h"
#include "WorldController.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "UIComponent.h"
#include "MailController.h"
#include "LocalController.h"
#include "VipUtil.h"
#include "ChatContentView.h"
#include "ChatViewController.h"
#include "ServiceInterface.h"
#include "LanguageManager.h"
#include "ChatServiceController.h"
#import "MsgMessage.h"
#import "UserManager.h"
#include "CCCommonUtils.h"
#import "GameHost.h"
#include <stdlib.h>
#import "LanguageKeys.h"
#import "PersonSelectVC.h"
#import "ChatChannel.h"
#import "ChannelManager.h"
#import "ChatMailTableTableViewController.h"

#import "MailInfoManager.h"
#import "ZYTMailListViewController.h"
#import "NSString+Cocos2dHelper.h"
#import "ChatMailMsgManager.h"
#import "VersionHelper.h"
/** ios 端服务器开关
 1、 function_on3 : k8    原生聊天
 2、 chat_switch  ：k2  谷歌HTTP翻译
 3、 chat_switch  ：k4  多人聊天管理
 4、 chat_switch  : k5   0|0|0   DB_UserInfo|DB_ChatChannel|DB_MsgItem_switch
 5、 chat_switch  ：K3   0|0   安卓|iOS  是原生邮件的开关
 */
static void (*alertToRateAppListener) (int result);
bool ChatServiceCocos2dx::isChatShowing = false;
bool ChatServiceCocos2dx::isForumShowing = false;
bool ChatServiceCocos2dx::enableNativeChat = true;
int ChatServiceCocos2dx::m_channelType=IOS_CHANNEL_TYPE_COUNTRY;
bool ChatServiceCocos2dx::m_isInMailDialog=false;
bool ChatServiceCocos2dx::enableNativeMail=false;
bool ChatServiceCocos2dx::isChatDialogNeedBack=false;
bool ChatServiceCocos2dx::m_rememberPosition=false;
int ChatServiceCocos2dx::m_curPopupWindowNum=0;
bool ChatServiceCocos2dx::enableChatRoom=true;
std::string ChatServiceCocos2dx::m_curMailFromUid="";
int ChatServiceCocos2dx::m_curUserInfoIndex=0;
int ChatServiceCocos2dx::m_curSendMailIndex=0;
int ChatServiceCocos2dx::m_curSendChatIndex=0;
bool ChatServiceCocos2dx::m_isNoticItemUsed=false;
bool ChatServiceCocos2dx::m_autoTranslateWithGoogle=false;

bool ChatServiceCocos2dx::DB_UserInfo_switch = true;
bool ChatServiceCocos2dx::DB_ChatChannel_switch = true;
bool ChatServiceCocos2dx::DB_MsgItem_switch = true;
bool ChatServiceCocos2dx::Mail_OC_Native_Enable = false  ;

void ChatServiceCocos2dx::showChatIOSFrom2dx(){
    if(!enableNativeChat) return;
    if(!enableNativeChat || ChatServiceCocos2dx::isChatShowing) return;
    
    ChatServiceCocos2dx::initPlayerInfo();
    [[ServiceInterface serviceInterfaceSharedManager] showChatIOSFrom2dx:m_channelType withAddState:OpenChatIOS_Normal];
//    [[ServiceInterface serviceInterfaceSharedManager]loadingChatVC];
}

void ChatServiceCocos2dx::openBBS()
{
    [[ServiceInterface serviceInterfaceSharedManager] openBBS];
}

void ChatServiceCocos2dx::handleChatPush(cocos2d::CCArray *chatInfoArr,int channelType,string channelId){
    if(!enableNativeChat) return;
    
    if(chatInfoArr && chatInfoArr->count() > 0){
        
        NSString *ns_channelId = nil;
        
        if (channelId == "1")
        {
            ns_channelId = [UserManager sharedUserManager].currentUser.allianceId;
        }else if(channelId == "0"){
            ns_channelId = [NSString stringWithFormat:@"%d",[UserManager sharedUserManager].currentUser.serverId];
        }else{
            ns_channelId = [NSString stringWithUTF8String:channelId.c_str()];
        }

        
        if(![[[ChannelManager sharedChannelManager].channel_map allKeys] containsObject:ns_channelId]){
            if (channelType == 0) {
                [[ChannelManager sharedChannelManager] createChatChannel:IOS_CHANNEL_TYPE_COUNTRY withAddChannelID:ns_channelId];
            }else if (channelType == 1){
                [[ChannelManager sharedChannelManager] createChatChannel:IOS_CHANNEL_TYPE_ALLIANCE withAddChannelID:ns_channelId];
            }else if (channelType == 2){
                [[ChannelManager sharedChannelManager] createChatChannel:IOS_CHANNEL_TYPE_USER withAddChannelID:ns_channelId];
            }else if (channelType == 3){
                [[ChannelManager sharedChannelManager] createChatChannel:IOS_CHANNEL_TYPE_CHATROOM withAddChannelID:ns_channelId];
            }
        }
        
        ChatChannel *cc = nil;
        
        if(channelType == 0){
            cc = [[ChannelManager sharedChannelManager] gettingCountryChannel];
        }else if(channelType == 1){
            cc = [[ChannelManager sharedChannelManager] gettingAllianceChannel];
        }else{
            cc = [[ChannelManager sharedChannelManager] gettingChannelInfo:ns_channelId];
            
        }
        
        if (!cc) {
            return;
        }
        NSMutableArray *tempMsgArray =[NSMutableArray array];
        
        for(int  i = 0; i < chatInfoArr->count(); i++)
        {
            ChatMailInfo* chatInfo=dynamic_cast<ChatMailInfo*>(chatInfoArr->objectAtIndex(i));
            if (!chatInfo) {
                return ;
            }
            NSString * ns_uid = [NSString stringWithUTF8String:chatInfo->uid.c_str()];
 
            if (channelType == IOS_CHANNEL_TYPE_USER){
                if (![ns_uid isEqualToString:[UserManager sharedUserManager].currentUser.uid]){
                    cc.nameString =[NSString stringWithUTF8String:chatInfo->fromName.c_str()];
                    int headVer = chatInfo->headPicVer;
                    if  (headVer> 0){
                       cc.faceCustemString = [NSString gettingCustomHeadPic:ns_uid int:headVer];
                    }
                        cc.faceImageString = [NSString stringWithUTF8String:chatInfo->headPic.c_str()];
                }
            }
            
            NSString * ns_vip = [NSString stringWithUTF8String:chatInfo->vip.c_str()];
            if (chatInfo->channelMsgType == IOS_CHANNEL_TYPE_CHATROOM) {
                ns_vip = [@"VIP" stringByAppendingString:ns_vip];
            }
            
            NSString * ns_name = [NSString stringWithUTF8String:chatInfo->name.c_str()];
            NSString * ns_asn = [NSString stringWithUTF8String:chatInfo->asn.c_str()];
            
            NSString * ns_msg = [NSString stringWithUTF8String:chatInfo->msg.c_str()];
            NSString * ns_translateMsg = [NSString stringWithUTF8String:chatInfo->translateMsg.c_str()];
            ns_msg= [ns_msg stringByReplacingOccurrencesOfString:@"\n" withString:@" "];
            ns_translateMsg = [ns_translateMsg stringByReplacingOccurrencesOfString:@"\n" withString:@" "];
            NSString * ns_headPic = [NSString stringWithUTF8String:chatInfo->headPic.c_str()];
            NSString * ns_sendLocalTime = [NSString stringWithUTF8String:chatInfo->sendLocalTime.c_str()];
            NSString * ns_originalLang = [NSString stringWithUTF8String:chatInfo->originalLang.c_str()];
            NSString * ns_mailID = [NSString stringWithUTF8String:chatInfo->id.c_str()];
            
            
            NSInteger ns_createTime = chatInfo->createTime;
            bool isNewMsg = chatInfo->isNewMsg;
            bool isSelfMsg = chatInfo->isSelfMsg;
            int ns_channelType = chatInfo->channelMsgType;
            int gmod = chatInfo->gmod;
            int post = chatInfo->post;
            int headPicVer = chatInfo->headPicVer;
            int sequenceId =  chatInfo->sequenceId;
            NSLog(@"sequenceId = %d",sequenceId);
            NSInteger lastUpdateTime = chatInfo->lastUpdateTime;
            long createTime = chatInfo->createTime;
            
            NSString *ns_attachmentId = nil;
            if(post == 3){
                ns_attachmentId = [NSString stringWithUTF8String:chatInfo->allianceId.c_str()];
            }else if(post == 4){
                ns_attachmentId = [NSString stringWithUTF8String:chatInfo->reportUid.c_str()] ;
            }else if(post == 5){
                ns_attachmentId = [NSString stringWithUTF8String:chatInfo->detectReportUid.c_str()];
            }

            
            /**创建信息模型*/
            NSMsgItem *cm = [[NSMsgItem alloc]init];
            
            cm.uid = ns_uid;
            cm.vip = ns_vip;
            cm.createTime = ns_createTime;
            cm.name = ns_name;
            cm.asn = ns_asn;
            cm.msg = ns_msg;
            cm.translateMsg = ns_translateMsg;
            cm.headPic = ns_headPic;
            if (channelType == IOS_CHANNEL_TYPE_USER && ns_headPic.length== 0){
                cm.headPic = @"g015";
            }
            cm.originalLang = ns_originalLang;
            cm.attachmentId = ns_attachmentId;
            cm.isNewMsg = isNewMsg;
            cm.isSelfMsg = isSelfMsg;
            cm.channelType = ns_channelType;
            cm.gmod = gmod;
            cm.post = post;
            cm.headPicVer = headPicVer;
            cm.sequenceId = sequenceId;
            cm.mailId = ns_mailID;
            cm.lastUpdateTime = lastUpdateTime;
            cm.createTime = createTime;
            cm.sendLocalTime = [ns_sendLocalTime intValue];
            
            [cm packedMsg];
            [cm initUserForReceivedMsg];
            [tempMsgArray addObject:cm];
            if(i == 0){
            }
 
        }
 
        int count = tempMsgArray.count;
        if(count > 0){
            NSMsgItem *msg=[tempMsgArray lastObject];
            
            if([cc.msgList count] == 0){
                [cc chatMsgPushArray:tempMsgArray andMsgType:ResponseMsgTypeInitHistoryMSG];
//                [ServiceInterface serviceInterfaceSharedManager].rmt = InitHistoryMSG;
            }else if(count > 1 && msg.isNewMsg == false){
                [cc chatMsgPushArray:tempMsgArray andMsgType:ResponseMsgTypeActionMSG];
//                [ServiceInterface serviceInterfaceSharedManager].rmt = ActionMSG;
            }else if(count == 1 && msg.isNewMsg == true){
                [cc chatMsgPushArray:tempMsgArray andMsgType:ResponseMsgTypeOneMSG];
//                [ServiceInterface serviceInterfaceSharedManager].rmt = OneMSG;
            }

        }
 
    }
}

void ChatServiceCocos2dx::setPlayerAllianceInfo(const char *asn, const char *allianceId, int allianceRank, bool isFirstJoinAlliance)
{
    if(!enableNativeChat) return;
    if (asn==NULL || allianceId==NULL || allianceRank<0 ) {
        CCLOG("setPlayerAllianceInfo error");
        return;
    }
    
    NSString *ns_asn = [NSString stringWithUTF8String:asn];
    NSString *ns_allianceId = [NSString stringWithUTF8String:allianceId];
    if([ns_asn isEqualToString:@""] || [ns_allianceId isEqualToString:@""] || allianceId<0 || ns_asn == nil || ns_allianceId == nil){
        CCLog("没有拿到联盟信息");
        return;
    }
    
    [[ServiceInterface serviceInterfaceSharedManager] setPlayerAllianceInfo:ns_asn :ns_allianceId :allianceRank :isFirstJoinAlliance ];
}

void ChatServiceCocos2dx::setPlayerInfo(int gmod, int headPicVer, const char *customHeadImageUrl, const char *name, const char *uid, const char *headPic, const char *vipInfo) {
    if(!enableNativeChat) return;
    
    NSString *ns_customHeadImageUrl = [NSString stringWithUTF8String:customHeadImageUrl];
    NSString *ns_name = [NSString stringWithUTF8String:name];
    NSString *ns_uid = [NSString stringWithUTF8String:uid];
    NSString *ns_headPic = [NSString stringWithUTF8String:headPic];
    NSString * ns_vipInfo = [NSString stringWithUTF8String:vipInfo];
    
    [[ServiceInterface serviceInterfaceSharedManager] setPlayerInfo:gmod headPicVer:headPicVer customHeadImageUrl:ns_customHeadImageUrl name:ns_name uidStr:ns_uid picStr:ns_headPic vipStr:ns_vipInfo];
}

//android 邮件相关，打开邮件窗口时传递
void ChatServiceCocos2dx::setMailInfo(const char* mailInfoFromUid,const char* mailInfoUid,const char* mailInfoName,int mailInfoType)
{
    if(!enableNativeChat) return;
    
    if (mailInfoFromUid==NULL || mailInfoUid==NULL || mailInfoName==NULL) {
        CCLOG("setMailInfo error");
        return;
    }
    
    NSString * ns_mailInfoFromUid = [NSString stringWithUTF8String:mailInfoFromUid];
    NSString * ns_mailInfoUid = [NSString stringWithUTF8String:mailInfoUid];
    NSString * ns_mailInfoName = [NSString stringWithUTF8String:mailInfoName];
    [[ServiceInterface serviceInterfaceSharedManager] setMailInfo:ns_mailInfoFromUid :ns_mailInfoUid :ns_mailInfoName :mailInfoType];
    [[ServiceInterface serviceInterfaceSharedManager] settingChatRoomName:ns_mailInfoName];
}

//字符串替换函数
std::string& ChatServiceCocos2dx::StringReplace(std::string& strBig, const std::string& strsrc, const std::string& strdst)
{
    std::string::size_type pos = 0;
    std::string::size_type srclen = strsrc.size();
    std::string::size_type dstlen = strdst.size();
    while( (pos = strBig.find(strsrc, pos)) != std::string::npos)
    {
        strBig.replace(pos, srclen, strdst);
        pos += dstlen;
    }
    return strBig;
}

void ChatServiceCocos2dx::selectAsnORCountries()
{
    if(!enableNativeChat) return;
    [[ServiceInterface serviceInterfaceSharedManager] selectAsnORCountries];
}

//android 聊天相关
void ChatServiceCocos2dx::hiddenChatIosFrom2dx(){
    if(!enableNativeChat) return;
    [[ChatServiceController chatServiceControllerSharedManager] closekeyboard];
    [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
}

bool ChatServiceCocos2dx::isChatShowing_fun(){
    if(!enableNativeChat) return false;
    if(![ServiceInterface serviceInterfaceSharedManager].vc_win){
        return false;
    }else if([ServiceInterface serviceInterfaceSharedManager].vc_win.hidden == YES){
        return false;
    }else{
        return true;
    }
}

void ChatServiceCocos2dx::flyHint(std::string icon, std::string titleText, std::string contentText, float time, float dy, bool useDefaultIcon){
    if(!enableNativeChat) return;
    //有用的参数 contentText 其余的参数暂时没用
    NSString *nsIcon = [NSString stringWithUTF8String:icon.c_str()];
    NSString *nsTitleText = [NSString stringWithUTF8String:titleText.c_str()];;
    NSString *nsContentText = [NSString stringWithUTF8String:contentText.c_str()];
    
    [[ServiceInterface serviceInterfaceSharedManager] flyHint:@"" :@"" :nsContentText  :time :dy :useDefaultIcon];
    
}

void ChatServiceCocos2dx::flySystemUpdateHint(double countDown,bool isFlyHintLogin, bool isLogin, string tip, string icon)
{
    if(!enableNativeChat) return;
    int flyHintCount_min = (int)(countDown / 10 / 60);
    int flyHintCount_s = (int)(countDown / 10);
    
    NSString *nsTime_min = [NSString stringWithFormat:@"%d",flyHintCount_min];
    NSString *nsTime_s = [NSString stringWithFormat:@"%d",flyHintCount_s];
    
    NSString *nsTip = [NSString stringWithCString:tip.c_str()];
    NSString *nsShowText = @"";
    
    if (flyHintCount_min > 0){
        NSString *timeContentText = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].FLYHINT_DOWN_MIN :nsTime_min];
        nsShowText = [nsTip stringByAppendingString:@"\n"];
        nsShowText = [nsTip stringByAppendingString:timeContentText];
    }else{
        NSString *timeContentText = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].FLYHINT_DOWN_SECOND :nsTime_s];
        nsShowText = [nsTip stringByAppendingString:@"\n"];
        nsShowText = [nsTip stringByAppendingString:timeContentText];
    }
    
    [[ServiceInterface serviceInterfaceSharedManager] flyHint:@"" :@"" :nsShowText  :0 :0 :true];
    
}

void ChatServiceCocos2dx::resetPlayerIsInAlliance() {
    ChatChannel *cc = [[ChannelManager sharedChannelManager] gettingAllianceChannel];
    if(cc)
    {
        [cc.msgList removeAllObjects];
        [[ChannelManager sharedChannelManager] deletingChatChannel:cc];
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.tableView reloadData];
        [UserManager sharedUserManager].currentUser.allianceId = @"";
        [UserManager sharedUserManager].currentUser.allianceRank = -1;
        [UserManager sharedUserManager].currentUser.asn = @"";
    }
}

void ChatServiceCocos2dx::clearCountryMsg() {
    if(!enableNativeChat) return;
    ChatChannel *cc = [[ChannelManager sharedChannelManager] gettingCountryChannel];
    [cc.msgList removeAllObjects];
    [[ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController.tableView reloadData];
}

void ChatServiceCocos2dx::clearChatData()
{
    [[ChannelManager sharedChannelManager].channel_map removeAllObjects];
}

void ChatServiceCocos2dx::notifyChangeLanguage(){
    if(!enableNativeChat) return;
    //获取本地化数组
    NSArray * chatLangArray = [[ChatServiceController chatServiceControllerSharedManager].gameHost getChatLangArray];
    //初始化本地化
    [LanguageManager languageManager_initChatLanguage:chatLangArray];
}

void ChatServiceCocos2dx::unInstallWin()
{
    if(!enableNativeChat) return;
    UIWindow *vc_win = [ServiceInterface serviceInterfaceSharedManager].vc_win;
    
    if(vc_win){
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController removeFromParentViewController];
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController.view removeFromSuperview];
        
        [[ServiceInterface serviceInterfaceSharedManager].mailViewController removeFromParentViewController];
        [[ServiceInterface serviceInterfaceSharedManager].mailViewController.view removeFromSuperview];
        
        
        [ServiceInterface serviceInterfaceSharedManager].mailViewController = [[MailViewController alloc]init];
        [ServiceInterface serviceInterfaceSharedManager].chatViewController = [[ChatViewController alloc]init];
        
        [[ServiceInterface serviceInterfaceSharedManager].vc_win addSubview:[ServiceInterface serviceInterfaceSharedManager].mailViewController.view];
        [[ServiceInterface serviceInterfaceSharedManager].vc_win addSubview:[ServiceInterface serviceInterfaceSharedManager].chatViewController.view];
        
        [[ServiceInterface serviceInterfaceSharedManager] initIsLoadVariable];
        
//        [ServiceInterface serviceInterfaceSharedManager].isFirstOpenIOS = TRUE;
        
        //[ServiceInterface serviceInterfaceSharedManager].isLockScreen = FALSE;
    }
}

void ChatServiceCocos2dx::deleteUserMail(string fromUid)
{
    if(!enableNativeChat) return;
//    NSString *ns_fromUid= [NSString stringWithCString:fromUid.c_str() encoding:[NSString defaultCStringEncoding]];
//    NSMutableArray *mail_value = [[ServiceInterface serviceInterfaceSharedManager].channel_map objectForKey:ns_fromUid];
//    [mail_value removeAllObjects];
}

void ChatServiceCocos2dx::resetResume()
{
    if(!enableNativeChat) return;
    IOSScheduleObject::getInstance()->resetResume();
}


/**创建多人聊天选人界面*/
void ChatServiceCocos2dx::groupChatVCInit(){
    

    PersonSelectVC *personSelectVC=[[PersonSelectVC alloc]initWithType:PersonSelectVCType_New];
    [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController = personSelectVC;
    
    [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.hidden = NO;

}
//
///**更新聊天室*/
void ChatServiceCocos2dx::updateChannelMemberArray(int channelType,string fromUid,string uidArr,int changeStatus) {
//changeStatus 0-创建   1-踢人  2- 退出
    if (channelType == 3){
        if  (changeStatus == 0){
            NSString *ns_fromUid =[NSString stringWithUTF8String:fromUid.c_str()];
            ChatChannel *chatChannel =  [[ChannelManager sharedChannelManager] gettingChannelInfo:ns_fromUid];
            if (!chatChannel) {
                return;
            }
            [chatChannel.memberUidArray removeAllObjects];
            vector<string>strVec;
            CCCommonUtils::splitString(uidArr, "_", strVec);
            for (int x= 0; x<strVec.size(); x++) {
                NSString *uidString =[NSString stringWithUTF8String:strVec.at(x).c_str()];
                [chatChannel.memberUidArray addObject:uidString];
            }
            DVLog(@"fromUidString %@",ns_fromUid);
            DVLog(@"uidArray :%@",chatChannel.memberUidArray);
        }else if(changeStatus == 1){
            NSString *ns_fromUid =[NSString stringWithUTF8String:fromUid.c_str()];
            
            ChatChannel *chatChannel =  [[ChannelManager sharedChannelManager] gettingChannelInfo:ns_fromUid];
            if (!chatChannel) {
                return;
            }
            BOOL isRemoveChannel = FALSE;
            vector<string>strVec;
            CCCommonUtils::splitString(uidArr, "_", strVec);
            for (int x= 0; x<strVec.size(); x++) {
                NSString *uidString =[NSString stringWithUTF8String:strVec.at(x).c_str()];
                [chatChannel.memberUidArray removeObject:uidString];
                /**移除聊天室人员是否包含自己。如果包含那么删除本地的聊天室数据*/
                if ([uidString isEqualToString:[UserManager sharedUserManager].currentUser.uid]) {
                    isRemoveChannel = TRUE;
                }
            }
            
            if (isRemoveChannel) {
                [[ChannelManager sharedChannelManager] deletingChatChannel:chatChannel];
//                [[ChannelManager sharedChannelManager].channel_map removeObjectForKey:ns_fromUid];
            }
            DVLog(@"fromUidString %@",ns_fromUid);
            DVLog(@"uidArray :%@",chatChannel.memberUidArray);

        }else if (changeStatus ==  2){
            //(3,groupId,operatorUid,2)
             NSString *ns_groupId =[NSString stringWithUTF8String:fromUid.c_str()];
             NSString *ns_operatorUid =[NSString stringWithUTF8String:uidArr.c_str()];
            //删除聊天室包含 uidArr 的 uid
            DVLog(@"ns_groupId :%@",ns_groupId);
            DVLog(@"ns_operatorUid :%@",ns_operatorUid);

            ChatChannel *chatChannel =  [[ChannelManager sharedChannelManager] gettingChannelInfo:ns_groupId];
 
            NSMutableArray *tempArr=[NSMutableArray array];
            vector<string>strVec;
            CCCommonUtils::splitString(uidArr, "_", strVec);
            for (int x= 0; x<strVec.size(); x++) {
                NSString *uidString =[NSString stringWithUTF8String:strVec.at(x).c_str()];
                [chatChannel.memberUidArray removeObject:uidString];
            }
            /**当自己退出聊天室后需要删除本地缓存或DB对应的聊天室*/
            [[ChannelManager sharedChannelManager] deletingChatChannel:chatChannel];
//            [[ChannelManager sharedChannelManager].channel_map removeObjectForKey:ns_groupId];
        }
       
    }
   
}

void ChatServiceCocos2dx::updateChatChannelInfo(string newMsgInfo)
{
    if(!newMsgInfo.empty())
        [[ChannelManager sharedChannelManager] updateChatChannelInfo:[NSString stringWithUTF8String:newMsgInfo.c_str()]];
}


void ChatServiceCocos2dx::resetIOS()
{
//判断是否是第一次加载，第一次加载就跳过
    if(!enableNativeChat) return;
    string uidString = GlobalData::shared()->playerInfo.uid;
    if (!uidString.empty()){
        ChatServiceCocos2dx::m_curUserInfoIndex=0;
        ChatServiceCocos2dx::resetResume();
        ChatServiceCocos2dx::resetPlayerIsInAlliance();
        ChatServiceCocos2dx::clearChatData();
        [[ServiceInterface serviceInterfaceSharedManager] initIsLoadVariable];
    }
}

void ChatServiceCocos2dx::setCurrentUserId(string uid)
{
    if(!enableNativeChat) return;
    if (uid != "" || uid.length() > 0){
        NSString *ns_uid = [NSString stringWithUTF8String:uid.c_str()];
        [UserManager sharedUserManager].currentUser.uid = ns_uid;
    }
}

void ChatServiceCocos2dx::setPlayerInfo() {
    if(!enableNativeChat) return;
    
    int selfServerId = GlobalData::shared()->playerInfo.selfServerId;
    int timeStamp = GlobalData::shared()->getTimeStamp();
    int vipLevel = VipUtil::getVipLevel(GlobalData::shared()->playerInfo.vipPoints);
    int vipEndTime = GlobalData::shared()->playerInfo.vipEndTime;
    int lastUpdateTime = GlobalData::shared()->playerInfo.lastUpdateTime;
    int gmFlag = GlobalData::shared()->playerInfo.gmFlag;
    int headPicVer=GlobalData::shared()->playerInfo.picVer;
    NSString *ns_name = [NSString stringWithUTF8String:GlobalData::shared()->playerInfo.name.c_str()];
    NSString *ns_uid = [NSString stringWithUTF8String:GlobalData::shared()->playerInfo.uid.c_str()];
    NSString *ns_headPic = [NSString stringWithUTF8String:GlobalData::shared()->playerInfo.pic.c_str()];
    int crossFightSrcServerId = GlobalData::shared()->playerInfo.crossFightSrcServerId;
    
    [UserManager sharedUserManager].currentUser.uid = ns_uid;
    [UserManager sharedUserManager].currentUser.serverId = selfServerId;
    [UserManager sharedUserManager].currentUser.crossFightSrcServerId = crossFightSrcServerId;
    [UserManager sharedUserManager].currentUser.headPicVer = headPicVer;
    [UserManager sharedUserManager].currentUser.mGmod = gmFlag;
    [UserManager sharedUserManager].currentUser.userName = ns_name;
    [UserManager sharedUserManager].currentUser.headPic = ns_headPic;
    [UserManager sharedUserManager].currentUser.vipLevel = vipLevel;
    [UserManager sharedUserManager].currentUser.vipEndTime = vipEndTime;
    [UserManager sharedUserManager].currentUser.lastUpdateTime = lastUpdateTime;
}

void ChatServiceCocos2dx::notifyUserInfo(cocos2d::CCArray *memberArr){
    if(!enableNativeChat) return;
    
    ChatServiceCocos2dx::initPlayerInfo();

    int len=memberArr->count();
    CCLOG("len: %i", memberArr->count());
    NSMutableArray *userInfos = [[NSMutableArray alloc]init];
    //给每一个实例的变量赋值，并且将实例作为一个object，添加到objcet数组中
    for(int  i = 0; i < len; i++)
    {
        CCDictionary* dic=dynamic_cast<CCDictionary*>(memberArr->objectAtIndex(i));
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
        NSLog(@"%d",userInfo.lastUpdateTime);
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
        
        [userInfos addObject:userInfo];
        
    }
    
    [[UserManager sharedUserManager] onReceiveUserInfo:userInfos];
}

void ChatServiceCocos2dx::initPlayerInfo()
{
    m_curPopupWindowNum=PopupViewController::getInstance()->getCurrViewCount()+PopupViewController::getInstance()->getGoBackViewCount();
    
    setPlayerAllianceInfo(GlobalData::shared()->playerInfo.allianceInfo.shortName.c_str(),GlobalData::shared()->playerInfo.getAllianceId().c_str(),GlobalData::shared()->playerInfo.allianceInfo.rank ,GlobalData::shared()->playerInfo.isfirstJoin);
    
    int vipLv = VipUtil::getVipLevel(GlobalData::shared()->playerInfo.vipPoints);
    int dtime = GlobalData::shared()->playerInfo.vipEndTime - GlobalData::shared()->getWorldTime();
    if(dtime>0)
    setPlayerInfo(GlobalData::shared()->playerInfo.gmFlag,GlobalData::shared()->playerInfo.picVer,GlobalData::shared()->playerInfo.getCustomPicUrl().c_str(),GlobalData::shared()->playerInfo.name.c_str(),GlobalData::shared()->playerInfo.uid.c_str(),GlobalData::shared()->playerInfo.pic.c_str(),_lang_1("103001", CC_ITOA(vipLv)));
    else
    setPlayerInfo(GlobalData::shared()->playerInfo.gmFlag,GlobalData::shared()->playerInfo.picVer,GlobalData::shared()->playerInfo.getCustomPicUrl().c_str(),GlobalData::shared()->playerInfo.name.c_str(),GlobalData::shared()->playerInfo.uid.c_str(),GlobalData::shared()->playerInfo.pic.c_str(),_lang_1("103001", CC_ITOA(vipLv)));
}


void ChatServiceCocos2dx::notifyChatRoomNameChanged(string name)
{
    [[ServiceInterface serviceInterfaceSharedManager] settingChatRoomName:[NSString stringWithUTF8String:name.c_str()]];
}
/**
 *  设置聊天室房主uid
 *
 *  @param groupId    聊天室ID
 *  @param founderUid 房主uid
 */
void ChatServiceCocos2dx::setChatRoomFounder(string groupId,string founderUid) {
    NSString* groupIdString  = [NSString stringWithUTF8String:groupId.c_str()];
    NSString *founderUidString =[NSString stringWithUTF8String:founderUid.c_str()];
    DVLog(@"groupId %s",groupIdString);
    DVLog(@"founderUid %s",founderUidString);
     ChatChannel *chatChannel =  [[ChannelManager sharedChannelManager] gettingChannelInfo:groupIdString];
    chatChannel.roomOwner = founderUidString;

}

string ChatServiceCocos2dx::getChannelInfo()
{
    if  (!ChatServiceCocos2dx::DB_MsgItem_switch){
        std::string allianceId = GlobalData::shared()->playerInfo.getAllianceId();
        if(allianceId.empty()){
            return "0|0|0,0|0|2";
        }else{
           return "0|0|0," + allianceId + "|0|1," + "0|0|2";
        }
        
    }else{
        //数据库开关打开
        std::string allianceId = GlobalData::shared()->playerInfo.getAllianceId();
     
        ChatChannel *chatChannel =  [[ChannelManager sharedChannelManager] gettingCountryChannel];
        if (chatChannel == nil){
            
            if(allianceId.empty()){
                return "0|0|0,0|0|2";
            }else{
                return "0|0|0," + allianceId + "|0|1," + "0|0|2";
            }
         
        }else{
            NSMsgItem *msg =  [chatChannel gettingLastMsg];
            if  (msg == nil){
                if(allianceId.empty()){
                    return "0|0|0,0|0|2";
                }else{
                    return "0|0|0," + allianceId + "|0|1," + "0|0|2";
                }
                
            }else{
                
                    string seqId = [[NSString stringWithFormat:@"%d",msg.sequenceId] UTF8String];
                    if(allianceId.empty()){
                        return   "0|"+seqId + "|0";;
                    }else{
                        return "0|"+seqId + "|0," + allianceId + "|0|1" ;
                    }
            }
        }
    }
    
    
//    "id|seqid|channelType,id|seqid|channelType,id|seqid|channelType"
   
    
}

void ChatServiceCocos2dx::removeMail2fromUid(string fromUid)
{
    if(fromUid.length() == 0)
        return;
    NSString *ns_fromUid = [NSString stringWithUTF8String:fromUid.c_str()];
    ChatChannel *cc = [[ChannelManager sharedChannelManager].channel_map objectForKey:ns_fromUid];
    [cc.msgList removeAllObjects];
}

/**华丽的分割线----------------------------------------------IOSScheduleObject--------------------------------------------------*/

static IOSScheduleObject* _scheduleObjectInstance = NULL;

IOSScheduleObject::IOSScheduleObject()
{
    this->resetResume();
}

void IOSScheduleObject::popupCloseEvent(CCObject *params)
{
    //PopupViewController::getInstance()->getCurrViewCount() +PopupViewController::getInstance()->getGoBackViewCount())== ChatServiceCocos2dx::m_curPopupWindowNum 判断堆栈中的窗口数 ＋ 外面显示的窗口数 ＝＝ 跳转时窗口的初始值
    //waitingForReturnToChatAfterPopup 判断 是否需要跳回聊天窗口 例如会退按钮就不用在跳回聊天界面
    CCLOG("popupCloseEvent isChatDialogNeedBack=%d",ChatServiceCocos2dx::isChatDialogNeedBack);
    if((PopupViewController::getInstance()->getCurrViewCount() +PopupViewController::getInstance()->getGoBackViewCount())== ChatServiceCocos2dx::m_curPopupWindowNum && waitingForReturnToChatAfterPopup && ChatServiceCocos2dx::isChatDialogNeedBack)
    {
        if(!ChatServiceCocos2dx::isChatShowing_fun())
        {
            ChatServiceCocos2dx::isChatDialogNeedBack = false;
            waitingForReturnToChatAfterPopup = false;
            ChatServiceCocos2dx::m_rememberPosition = true;
            [ServiceInterface serviceInterfaceSharedManager].isSrollNewMsg = FALSE;
            [[ServiceInterface serviceInterfaceSharedManager] showChatIOSFrom2dx:ChatServiceCocos2dx::m_channelType withAddState:OpenChatIOS_Jump];
            CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_POPUP_CLOSE_EVENT);
        }
    }
}

void IOSScheduleObject::stopReturnToChat()
{
    if(waitingForReturnToChatAfterPopup)
    {
        waitingForReturnToChatAfterPopup = false;
        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_POPUP_CLOSE_EVENT);
    }
}

IOSScheduleObject::~IOSScheduleObject()
{
}
IOSScheduleObject* IOSScheduleObject::getInstance()
{
    if (_scheduleObjectInstance == NULL) {
        _scheduleObjectInstance = new IOSScheduleObject();
    }
    
    return _scheduleObjectInstance;
}

void IOSScheduleObject::showLatestMessage(float time){
    CCLOG("IOSScheduleObject::showLatestMessage");
    
    if (ChatServiceCocos2dx::m_channelType==IOS_CHANNEL_TYPE_COUNTRY) {
        
        UIComponent::getInstance()->showCountryIcon(true);
    }
    else if (ChatServiceCocos2dx::m_channelType==IOS_CHANNEL_TYPE_ALLIANCE)
    {
        UIComponent::getInstance()->showCountryIcon(false);
    }
    
    int type = 0;
    
    if(ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_COUNTRY)
    {
        type = 0;
    }else if(ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_ALLIANCE){
        type = 2;
    }
    
    ChatController::getInstance()->showLatestMessage(type);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(IOSScheduleObject::showLatestMessage), this);
}

void IOSScheduleObject::updateMailCell(float time){
    CCLOG("updateMailCell");
    MailController::getInstance()->updateMailList();
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(IOSScheduleObject::updateMailCell), this);
    if(!ChatServiceCocos2dx::isChatDialogNeedBack)
    {
        ChatServiceCocos2dx::m_isInMailDialog=false;
        ChatServiceCocos2dx::m_curMailFromUid="";
    }
}


void IOSScheduleObject::handleResume(float time){
    CCLOG("IOSScheduleObject::handleResume %s", actionAfterResume.c_str());
    
    ChatServiceCocos2dx::isChatDialogNeedBack=true;
    
    if(actionAfterResume == "joinAlliance"){
        ChatController::getInstance()->joinAlliance(uid);
    }else if(actionAfterResume == "sendMail"){
        if(ChatServiceCocos2dx::enableNativeChat)
            returnToChatAfterPopup = false;
        ChatController::getInstance()->sendMail(name,uid);
    }else if(actionAfterResume == "showPlayerInfo"){
        ChatController::getInstance()->showPlayerInfo(uid);
    }else if(actionAfterResume == "joinAllianceBtnClick"){
        ChatController::getInstance()->joinAllianceBtnClick();
    }
    else if(actionAfterResume == "viewBattleReport"){
        CCLOG("IOSScheduleObject viewBattleReport");
        ChatController::getInstance()->viewBattleReport(uid,reportUid);
    }
    else if(actionAfterResume == "viewDetectReport"){
        CCLOG("IOSScheduleObject viewDetectReport");
        ChatController::getInstance()->viewDetectReport(uid,detectReportUid);
    }
    else if(actionAfterResume == "banPlayer"){
        ChatController::getInstance()->shieldPlayer(uid);
        ChatController::getInstance()->inviteJoinAlliance(uid,name);
    }else if(actionAfterResume == "gotoCoordinate"){
        CCPoint pt = ccp(atoi(uid.c_str()), atoi(name.c_str()));
        int worldIndex = WorldController::getIndexByPoint(pt);
        WorldController::getInstance()->openTargetIndex = worldIndex;
        if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
            WorldMapView::instance()->gotoTilePoint(pt);
        }else{
            int index = WorldController::getIndexByPoint(pt);
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, worldIndex);
        }
    }else{
        return;
    }
    
    if(returnToChatAfterPopup && !waitingForReturnToChatAfterPopup){
        waitingForReturnToChatAfterPopup = true;
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(IOSScheduleObject::popupCloseEvent), MSG_POPUP_CLOSE_EVENT, NULL);
    }
    
    actionAfterResume = "";
    uid = "";
    name = "";
    reportUid = "";
    detectReportUid="";
    returnToChatAfterPopup = false;

}

void IOSScheduleObject::resetResume(){
    
    actionAfterResume = "";
    uid = "";
    name = "";
    reportUid="";
    waitingForReturnToChatAfterPopup = false;
    returnToChatAfterPopup = false;
}

void ChatServiceCocos2dx::notifyUserUids(string uidStr,string lastUpdateTimeStr){
    
    if(uidStr == "" || lastUpdateTimeStr == "")
        return;
    
    NSString *ns_uidStr = [NSString stringWithUTF8String:uidStr.c_str()];
    NSArray *uidArr=[ns_uidStr componentsSeparatedByString:@"_"];
    
    NSString *ns_lastUpdateTimeStr = [NSString stringWithUTF8String:lastUpdateTimeStr.c_str()];
    NSArray *lastUpdateTimeArr=[ns_lastUpdateTimeStr componentsSeparatedByString:@"_"];
    
    NSMutableArray *userUidArray = [[NSMutableArray alloc]init];

    [[UserManager sharedUserManager] clearAllianceMember];
    
    int i = 0;
    for(NSString *uid in uidArr)
    {
        if(![uid isEqualToString:@""])
        {
            NSUserInfo *user = [[UserManager sharedUserManager] gettingUser:uid];
            if(user!=nil)
            {
                int lastUpdateTime=[(NSString*)lastUpdateTimeArr[i] isEqualToString:@""]?1:[(NSString*)lastUpdateTimeArr[i] intValue];
                if(lastUpdateTime>user.lastUpdateTime)
                    [userUidArray addObject:(NSString*)uidArr[i]];
                [[UserManager sharedUserManager] putChatRoomMemberInMap:user];
            }
            else
            {
                user = [[NSUserInfo alloc]init:uid];
                [[UserManager sharedUserManager] addUser:user];
                [userUidArray addObject:(NSString*)uidArr[i]];
            }
            
            
        }
        i++;
    }
    
    [[UserManager sharedUserManager] userInfoArraySwitchGroupArray];
    
    if([userUidArray count] > 0)
        [[UserManager sharedUserManager] getMultiUserInfo:userUidArray];
    
}


void ChatServiceCocos2dx::updateChatChannelRoom(int channelType, string channelID, string uidArr, string roomOwner, string customName)
{
    
    NSString *ns_channelID = [NSString stringWithUTF8String:channelID.c_str()];
    NSString *ns_roomOwner = [NSString stringWithUTF8String:roomOwner.c_str()];
    NSString *ns_customName = [NSString stringWithUTF8String:customName.c_str()];
    NSString *ns_uidArr = [NSString stringWithUTF8String:uidArr.c_str()];
    
    [[ChannelManager sharedChannelManager] updateChatChannelRoom:channelType withAddChannelID:ns_channelID withAddUidArr:ns_uidArr withAddRoomOwner:ns_roomOwner withAddCustomName:ns_customName];
    
}

void ChatServiceCocos2dx::createChatChannelRoom(string groupId, string memberUids)
{
    
    NSString *ns_groupId = [NSString stringWithUTF8String:groupId.c_str()];
    NSString *ns_memberUids = [NSString stringWithUTF8String:memberUids.c_str()];
    
    [[ChannelManager sharedChannelManager] createChatChannelRoom:ns_groupId withChannelMemberUids:ns_memberUids];
    
}

bool ChatServiceCocos2dx::isFirstOpenMailToPlayer(string fromUid)
{
    NSString *ns_fromUid = [NSString stringWithUTF8String:fromUid.c_str()];
    return [[[ChannelManager sharedChannelManager].channel_map allKeys] containsObject:ns_fromUid];
}

#pragma mark -
#pragma mark 群聊相关
void  ChatServiceCocos2dx::settingGroupChatMailVCChatChannel(){
    NSString *chatKeyString = [UserManager  sharedUserManager].currentMail.opponentUid;
    ChatChannel *chatChannel =[[ChannelManager sharedChannelManager].channel_map objectForKey:chatKeyString];
    DVLog(@"chatChannel :%@",chatChannel);
    
    [[ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController setCurrentChatChannel:chatChannel];
    
}

void ChatServiceCocos2dx::initDatabase()
{
    [[ChannelManager sharedChannelManager].channel_map removeAllObjects];
}

void ChatServiceCocos2dx::selectPlayer(cocos2d::CCArray *players)
{
    int len=players->count();
    CCLOG("len: %i", players->count());
    NSMutableArray *userInfos = [[NSMutableArray alloc]init];
    //给每一个实例的变量赋值，并且将实例作为一个object，添加到objcet数组中
    for(int  i = 0; i < len; i++)
    {
        CCDictionary* dic=dynamic_cast<CCDictionary*>(players->objectAtIndex(i));
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
        NSLog(@"%d",userInfo.lastUpdateTime);
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
        
        [userInfos addObject:userInfo];
        
    }
    
    [[UserManager sharedUserManager] onReceiveUserInfo:userInfos];
        [[NSNotificationCenter defaultCenter]postNotificationName:kSearchUserWithNameString object:nil
                                                         userInfo:@{@"users":userInfos}];
}

void ChatServiceCocos2dx::refreshChatInfo(int channelType)
{
    ChatCellFrame *ccf = nil;
    if(channelType == IOS_CHANNEL_TYPE_COUNTRY && [[[ChannelManager sharedChannelManager] gettingCountryChannel].msgList count] > 0){
        ccf = [[[ChannelManager sharedChannelManager] gettingCountryChannel].msgList lastObject];
    }else if(channelType == IOS_CHANNEL_TYPE_ALLIANCE && [[[ChannelManager sharedChannelManager] gettingAllianceChannel].msgList count] > 0){
        ccf = [[[ChannelManager sharedChannelManager] gettingAllianceChannel].msgList lastObject];
    }
    
    [[ChatServiceController chatServiceControllerSharedManager].gameHost refreshChatInfo:ccf.chatMessage];
}

#pragma mark - 系统邮件消息接受
void ChatServiceCocos2dx::pushSystemMailMsgWithMsgArray(cocos2d::CCArray *vSystemMailArray){
    NSMutableArray *tempArray =[[NSMutableArray alloc]init];
    for (int x = 0; x<vSystemMailArray->count(); x++) {
        MailInfo *tempMailInfo = (MailInfo *)vSystemMailArray->objectAtIndex(x);
         MailInfoDataModel *mailInfoData = [MailInfoManager maildataWithMailInfo:tempMailInfo];
        
        int  tempmailType =  tempMailInfo->type;
        
        
        if(mailInfoData.tabType == Mail_BigType_System){
            ChatChannel * channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:mailInfoData.channelID];
            if (channel == nil){
                [[ChannelManager sharedChannelManager]createChatChannel:IOS_CHANNEL_TYPE_SYSTEMMAIL withAddChannelID:mailInfoData.channelID];
                channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:mailInfoData.channelID];
            }
            channel.channelType = IOS_CHANNEL_TYPE_SYSTEMMAIL;
            [channel.msgList addObject:mailInfoData];
            if ([mailInfoData.channelID isEqualToString:@"909504798000489"]){
                DVLog(@"%@",[mailInfoData printAllPropertys]);
            }

        }else{
            if (tempmailType == MAIL_USER || tempmailType ==MAIL_SELF_SEND ){
                ChatChannel * channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:mailInfoData.channelID];
                if (channel == nil){
                    [[ChannelManager sharedChannelManager]createChatChannel:IOS_CHANNEL_TYPE_USER withAddChannelID:mailInfoData.channelID];
                    channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:mailInfoData.channelID];
                }
                channel.channelType = IOS_CHANNEL_TYPE_USER;
                channel.nameString = mailInfoData.fromName;
                channel.faceImageString = mailInfoData.picString;
                channel.contentsString = mailInfoData.title;
            }else if(tempmailType == CHAT_ROOM ){
                ChatChannel * channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:mailInfoData.channelID];
                if (channel == nil){
                    [[ChannelManager sharedChannelManager]createChatChannel:IOS_CHANNEL_TYPE_CHATROOM withAddChannelID:mailInfoData.channelID];
                    channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:mailInfoData.channelID];
                }
                channel.channelType = IOS_CHANNEL_TYPE_CHATROOM;
            }
        }
        
 
        
        
    }
 
//    for (ChatChannel *channel  in tempArray) {
//        channel.msgList = [channel sortingMailWithCreateTimeDataSource:channel.msgList];
//         MailInfoDataModel *maildata =  [channel.msgList lastObject];
//        if (channel.channelType == IOS_CHANNEL_TYPE_SYSTEMMAIL) {
//           
//            channel.timeString = [NSString stringWithTimeFormatWithCreateTime:maildata.creatTime];
//            channel.contentsString = maildata.title;
//            
//            channel.faceImageString = maildata.picString;
//            
//            if(maildata.tabType == Mail_BigType_System){
//                channel.nameString = @"系统";
//            }else if ( maildata.tabType == Mail_BigType_Notice){
//                channel.nameString = @"公告";
//            }else if ( maildata.tabType == Mail_BigType_Studio){
//                channel.nameString = @"龙族部落游戏工作室";
//            }else if ( maildata.tabType == Mail_BigType_Fight){
//                channel.nameString = @"战斗报告";
//            }else if ( maildata.tabType == Mail_BigType_Mod){
//                channel.nameString = maildata.fromName;
//            }
//
//        }        
//    }
    

}

void ChatServiceCocos2dx::pushChatMailMsgWithArray(cocos2d::CCArray *vChatArray){
    for (int x = 0; x <vChatArray->count(); x++) {
        ChatMailInfo *tempMailInfo = (ChatMailInfo *)vChatArray->objectAtIndex(x);
        CCLOG("%s",tempMailInfo->id.c_str());
        
        NSMsgItem *msgItem = [ChatMailMsgManager chatMsgWithChatMailInfo:tempMailInfo];
        NSString *channelId = [NSString stringWithUTF8String:tempMailInfo->channelId.c_str()];
        if (channelId.length == 0 ){
            channelId = [NSString stringWithUTF8String:tempMailInfo->uid.c_str()];
        }
        NSString *fromName = [NSString stringWithUTF8String:tempMailInfo->fromName.c_str()];
        NSString *faceImage =[NSString stringWithUTF8String:tempMailInfo->headPic.c_str()];
        int headVar = tempMailInfo->headPicVer;
 
        NSString *custemUrl = [NSString gettingCustomHeadPic:channelId int:headVar];
        ChatChannel * channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:channelId];
        if (channel == nil){
            [[ChannelManager sharedChannelManager]createChatChannel:IOS_CHANNEL_TYPE_USER withAddChannelID:channelId];
            channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:channelId];
 
        }
        channel.channelType = msgItem.channelType;
        channel.nameString = fromName;
        channel.faceImageString = faceImage;
        channel.faceCustemString = custemUrl;
        [channel chatMsgPushArray:@[msgItem] andMsgType:ResponseMsgTypeInitHistoryMSG];

    }
}

bool ChatServiceCocos2dx::compareVersionWithKeyVersionString( string vKeyString){
   return  [VersionHelper compareVersionWithKeyVersionStr:[NSString stringWithUTF8String:vKeyString.c_str()]];
}


#pragma mark - 初始化原生邮件列表
void ChatServiceCocos2dx::creatingMailListWith_OC_Native(){
    
    ZYTMailListViewController *mailListVC=[[ZYTMailListViewController alloc]init ];
    UINavigationController *nav =[[UINavigationController alloc]initWithRootViewController:mailListVC];
    mailListVC.navigationController.navigationBarHidden = YES;
    [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController = nav;
    
    [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.hidden = NO;
//
    
}
#endif