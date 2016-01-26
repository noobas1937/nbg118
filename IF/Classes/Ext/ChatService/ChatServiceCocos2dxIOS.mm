//
//  ChatServiceCocos2dxIOS.mm
//  IF
//
//  Created by mzc on 15/4/13.
//
//


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import "WbSocketControl.h"
#import "NSString+Extension.h"
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
#import "MailInfoDataModel.h"
#import "CSMailListViewController.h"
#import "CSMailRootCategoryListVC.h"


#import "NSString+Cocos2dHelper.h"
#import "ChatMailMsgManager.h"
#import "VersionHelper.h"
//#include "EquipInfoView.hpp" Simon
#import "NSArray+SortHelper.h"
#import "TranslateManager.h"
#import "CSChatMsgFactory.h"
#import "FriendsController.h"
#import "CSCommandManager.h"
#import "NSString+Extension.h"
#import "CSConvertViewManager.h"
 
#import "MailInfoDataModel.h"
 
#import "MailDataManager.h"
#import "MailData.h"
#import "CCJSONConverter.h"
#import "FriendsView.h"
#import "NetController.h"

#import "NSUserInfo+UserManager.h"
#import "CSMessage.h"
#import "CSMessageModel.h"
#import "ChatChannel+ChatMessage.h"
#import "CSMessageModel+ModelManager.h"
/** ios 端服务器开关
 1、 function_on3 : k8    原生聊天
 2、 chat_switch  ：k2  谷歌HTTP翻译
 3、 chat_switch  ：k4  多人聊天管理
 4、 chat_switch  : k7   0|0|0|0   DB_UserInfo|DB_ChatChannel|DB_MsgItem_switch|DB_SystemMail  (版本号)
 5、 chat_switch  ：K3   0|0   安卓|iOS  是原生邮件的开关 （iOS端废弃）
 6、 chat_switch  : k9   0|0  Mail_OC_Native_Enable|Mail_OC_UI_New_Native_Enable (版本号) ====>k9废弃
 7、 chat_switch : k6    0|0  (安卓新版邮件)|ios 新版邮件(版本号)
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
bool ChatServiceCocos2dx::m_autoTranslate=false;
bool ChatServiceCocos2dx::DB_UserInfo_switch = true ;
bool ChatServiceCocos2dx::DB_ChatChannel_switch = true;
bool ChatServiceCocos2dx::DB_MsgItem_switch = true;
bool ChatServiceCocos2dx::Mail_OC_Native_Enable = true   ;
bool ChatServiceCocos2dx::DB_SystemMail_switch = true ;
bool ChatServiceCocos2dx::CS_CountryChat_OC_Native_New = true;

bool ChatServiceCocos2dx::CS_ChatCell_OC_Native_New = true;

bool ChatServiceCocos2dx::CS_WebSocket_Msg_Get = false;
bool ChatServiceCocos2dx::CS_WebSocket_Msg_Send = false;
  bool ChatServiceCocos2dx:: CS_AllianceTask_Helped =false;

int ChatServiceCocos2dx::CS_Country_SpeakingLevelLimit = 5;
void ChatServiceCocos2dx::showChatIOSFrom2dx(){
    if(!enableNativeChat) return;
    if  (!CS_CountryChat_OC_Native_New){
        if(!enableNativeChat || ChatServiceCocos2dx::isChatShowing_fun()) return;
    }
    
    IOSScheduleObject::getInstance()->stopReturnToChat();
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
//        chatInfoArr->retain();
     __block  cocos2d::CCArray *tempChatInfoArr = chatInfoArr ;
        dispatch_async([ServiceInterface serviceInterfaceSharedManager].csChatMsgQueue, ^{
            NSString *ns_channelId = nil;
            ns_channelId = [NSString stringWithCString:channelId.c_str() encoding:4];
            if (channelType == 0) {
                //国家消息
                ns_channelId = [NSString stringWithFormat:@"%ld",[UserManager sharedUserManager].currentUser.serverId];
            }else if(channelType == 1){
                 ns_channelId = [UserManager sharedUserManager].currentUser.allianceId;
            }
                
            ChatChannel *cc = nil;
            if(![[[ChannelManager sharedChannelManager].channel_map allKeys] containsObject:ns_channelId]){
                if (channelType == 0) {
                    [[ChannelManager sharedChannelManager] createChatChannel:IOS_CHANNEL_TYPE_COUNTRY withAddChannelID:ns_channelId];
                     cc = [[ChannelManager sharedChannelManager] gettingCountryChannel];
                }else if (channelType == 1){
                    //联盟消息
//                     ns_channelId = [UserManager sharedUserManager].currentUser.allianceId;
                    [[ChannelManager sharedChannelManager] createChatChannel:IOS_CHANNEL_TYPE_ALLIANCE withAddChannelID:ns_channelId];
                     cc = [[ChannelManager sharedChannelManager] gettingAllianceChannel];
                }else if (channelType == 2){
                    if (tempChatInfoArr->count()> 0){
                        ChatMailInfo* tempMail =dynamic_cast<ChatMailInfo*>(tempChatInfoArr->objectAtIndex(0));
                        if (tempMail->post == 200){
                            
                            //mod
                            ns_channelId = [NSString stringWithUTF8String:channelId.c_str()];
                            ns_channelId = [ns_channelId stringByAppendingString:@"@mod"];
                        }else{
                            ns_channelId = [NSString stringWithUTF8String:channelId.c_str()];
                        }
                    }
                    //邮件
                    [[ChannelManager sharedChannelManager] createChatChannel:IOS_CHANNEL_TYPE_USER withAddChannelID:ns_channelId];
                        cc = [[ChannelManager sharedChannelManager] gettingChannelInfo:ns_channelId];
                    
                }else if (channelType == 3){
                    //聊天室
                    [[ChannelManager sharedChannelManager] createChatChannel:IOS_CHANNEL_TYPE_CHATROOM withAddChannelID:ns_channelId];
                    cc = [[ChannelManager sharedChannelManager] gettingChannelInfo:ns_channelId];
                }
            }else{
                if (channelType == 0) {
                    cc = [[ChannelManager sharedChannelManager] gettingCountryChannel];
                }else if (channelType == 1){
                    cc = [[ChannelManager sharedChannelManager] gettingAllianceChannel];
                }else{
                    if (tempChatInfoArr->count()> 0){
                        ChatMailInfo* tempMail =dynamic_cast<ChatMailInfo*>(tempChatInfoArr->objectAtIndex(0));
                        if (tempMail->post == 200){
                            
                            //mod
                            ns_channelId = [NSString stringWithUTF8String:channelId.c_str()];
                            ns_channelId = [ns_channelId stringByAppendingString:@"@mod"];
                        }else{
                            ns_channelId = [NSString stringWithUTF8String:channelId.c_str()];
                        }
                    }
                     cc = [[ChannelManager sharedChannelManager] gettingChannelInfo:ns_channelId];
                }
               

            }
            
            if (!cc) {
                return;
            }
            NSMutableArray *tempMsgArray =[NSMutableArray array];

            //加入耗时操作
                for(int  i = 0; i < tempChatInfoArr->count(); i++)
                {
                    ChatMailInfo* chatInfo=dynamic_cast<ChatMailInfo*>(tempChatInfoArr->objectAtIndex(i));
                    if (!chatInfo) {
                        return ;
                    }
        
                    if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New
//                         && (cc.channelType == IOS_CHANNEL_TYPE_COUNTRY ||cc.channelType == IOS_CHANNEL_TYPE_ALLIANCE)
                         ){
                        
                         [NSUserInfo userCheckWithChatMailInfo:chatInfo];
                        CSMessage *cm =[CSChatMsgFactory chatMessageWithChatMailInfo:chatInfo];
                        
                        if (ChatServiceCocos2dx::DB_MsgItem_switch) {
                            
                            [cc savingMessageToDBWithMsg:cm];
                        }
                        //更新用户信息
                        
                        
                        [tempMsgArray addObject:cm];
                        
                        if (channelType == IOS_CHANNEL_TYPE_USER){
                            if (![cm.uid isEqualToString:[UserManager sharedUserManager].currentUser.uid]){
                                cc.nameString =[NSString stringWithUTF8String:chatInfo->fromName.c_str()];
                                int headVer = chatInfo->headPicVer;
                                if  (headVer> 0){
                                    cc.faceCustemString = [NSString gettingCustomHeadPic:cm.uid intValue:headVer];
                                }
                                cc.faceImageString = [NSString stringWithUTF8String:chatInfo->headPic.c_str()];
                            }
                        }
                        
                        dispatch_async(dispatch_get_main_queue(), ^{
                            
                            //更新UI操作
                              [cc chatNewMsgPushed:@[cm]];
                            //
                        });

                    }
                    else{
                        NSMsgItem *cm =[CSChatMsgFactory chatMsgWithChatMailInfo:chatInfo];
                        
                        if (ChatServiceCocos2dx::DB_MsgItem_switch) {
                            
                            [cc savingMsgItemToDBWithMsgItem:cm];
                        }
                        
                        [cm msgItemCheckUserInfoWithMsgBeforeMsgSaveToDB];
                        
                        [tempMsgArray addObject:cm];
                        
                        if (channelType == IOS_CHANNEL_TYPE_USER){
                            if (![cm.uid isEqualToString:[UserManager sharedUserManager].currentUser.uid]){
                                cc.nameString =[NSString stringWithUTF8String:chatInfo->fromName.c_str()];
                                int headVer = chatInfo->headPicVer;
                                if  (headVer> 0){
                                    cc.faceCustemString = [NSString gettingCustomHeadPic:cm.uid intValue:headVer];
                                }
                                cc.faceImageString = [NSString stringWithUTF8String:chatInfo->headPic.c_str()];
                            }
                        }
                        
                        dispatch_async(dispatch_get_main_queue(), ^{
                            
                    //更新UI操作
                          [cc chatNewMsgPushed:@[cm]];
                    //
                        });

                    }
                    
                    
                }
            if  (tempChatInfoArr->getReferenceCount()> 0){
                tempChatInfoArr ->release();
            }
       });
        

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

//IOS 聊天相关
void ChatServiceCocos2dx::hiddenChatIosFrom2dx(){
    if(!enableNativeChat) return;
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New){
        [[CSConvertViewManager sharedMailInfoManager] closeMailVC];
        [[NSNotificationCenter defaultCenter]postNotificationName:KCLOSEKEYBOARD object:nil userInfo:nil];
    }else{
        if (ChatServiceCocos2dx::Mail_OC_Native_Enable){
            UIWindow *tempWindow=[ServiceInterface serviceInterfaceSharedManager].chatRootWindow;
            if (tempWindow.hidden == NO){
                [[CSConvertViewManager sharedMailInfoManager] closeMailVC];
            }else if([ServiceInterface serviceInterfaceSharedManager].vc_win.hidden == NO){
                [[ChatServiceController chatServiceControllerSharedManager] closekeyboard];
                [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
            }
            
        }else{
            [[ChatServiceController chatServiceControllerSharedManager] closekeyboard];
            [[ServiceInterface serviceInterfaceSharedManager] hideChatViewIOS];
        }
        
    }
}

bool ChatServiceCocos2dx::isChatShowing_fun(){
    if(!enableNativeChat) return false;
    if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New){
        UIWindow *tempWindow=[ServiceInterface serviceInterfaceSharedManager].chatRootWindow;
        if(!tempWindow){
            return false;
        }else if(tempWindow.hidden == YES){
            return false;
        }else{
            return true;
        }
    }else{
        if (ChatServiceCocos2dx::Mail_OC_Native_Enable){
            
            if([ServiceInterface serviceInterfaceSharedManager].vc_win.hidden == NO){
                return true;
            }else if([ServiceInterface serviceInterfaceSharedManager].chatRootWindow.hidden == NO){
                return true;
            }else{
                return false;
            }
        }else{
            if(![ServiceInterface serviceInterfaceSharedManager].vc_win){
                return false;
            }else if([ServiceInterface serviceInterfaceSharedManager].vc_win.hidden == YES){
                return false;
            }else{
                return true;
            }
        }
    }
}

bool ChatServiceCocos2dx::isNewMailShowing_fun(){
    if(!enableNativeChat) return false;
    if(YES == [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.hidden){
        return false;
    }else{
        return true;
    }
}

void ChatServiceCocos2dx::flyHint(std::string icon, std::string titleText, std::string contentText, float time, float dy, bool useDefaultIcon){
    if(!enableNativeChat) return;
    //有用的参数 contentText 其余的参数暂时没用
    
    NSString *nsIcon = nil;
    if (!icon.empty()) {
        nsIcon = [NSString stringWithUTF8String:icon.c_str()];
    }else{
        nsIcon = @"";
    }
    
    NSString *nsTitleText = nil;
    if (!titleText.empty()) {
        nsTitleText = [NSString stringWithUTF8String:titleText.c_str()];
    }else{
        nsTitleText = @"";
    }
    
    NSString *nsContentText = nil;
    if (!contentText.empty()) {
        nsContentText = [NSString stringWithUTF8String:contentText.c_str()];
    }else{
        nsContentText = @"";
    }
    
    [[ServiceInterface serviceInterfaceSharedManager] flyHint:@"" :@"" :nsContentText  :time :dy :useDefaultIcon];
    
}

void ChatServiceCocos2dx::flySystemUpdateHint(double countDown,bool isFlyHintLogin, bool isLogin, string tip, string icon)
{
    if(!enableNativeChat) return;
    int flyHintCount_min = (int)(countDown / 10 / 60);
    int flyHintCount_s = (int)(countDown / 10);
    
    NSString *nsTime_min = [NSString stringWithFormat:@"%d",flyHintCount_min];
    NSString *nsTime_s = [NSString stringWithFormat:@"%d",flyHintCount_s];
    NSString *nsTip =[NSString stringWithCString:tip.c_str() encoding:4];
//    NSString *nsTip = [NSString stringWithCString:tip.c_str()];
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
/*退出联盟数据重置*/
void ChatServiceCocos2dx::resetPlayerIsInAlliance() {
    ChatChannel *cc = [[ChannelManager sharedChannelManager] gettingAllianceChannel];
    if(cc)
    {
        //zyt

        [UserManager sharedUserManager].currentUser.allianceId = @"";
        [UserManager sharedUserManager].currentUser.allianceRank = -1;
        [UserManager sharedUserManager].currentUser.asn = @"";
        [cc.msgList removeAllObjects];
        [[ChannelManager sharedChannelManager] deletingChatChannel:cc];
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.dataSourceArray removeAllObjects];
        [[ServiceInterface serviceInterfaceSharedManager].chatViewController.allianceTableViewController.tableView reloadData];
    }
}
void ChatServiceCocos2dx::resetPlayerFirstJoinAlliance(){
    ChatChannel *cc = [[ChannelManager sharedChannelManager] gettingAllianceChannel];
    [cc.msgList removeAllObjects];
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
    NSArray * chatLangArray = [[ChatServiceController chatServiceControllerSharedManager].gameHost gettingChatLangArray];
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
    if (!uidString.empty())
    {
        ChatServiceCocos2dx::m_curUserInfoIndex=0;
        ChatServiceCocos2dx::resetResume();
//        ChatServiceCocos2dx::resetPlayerIsInAlliance();
 
//        [[ServiceInterface serviceInterfaceSharedManager] initIsLoadVariable];
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
    int allianceRank =GlobalData::shared()->playerInfo.allianceRank;
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
    [UserManager sharedUserManager].currentUser.allianceRank = allianceRank;
    [[UserManager sharedUserManager].currentUser updateToDBWithSelf] ; //将自己插入user表
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
        DLog(@"%d",userInfo.lastUpdateTime);
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


void ChatServiceCocos2dx::gettingSystemMsgWithString(string vCommandStr){
    ChatController::getInstance()->getNewMailMsg(vCommandStr);
}
void   ChatServiceCocos2dx::gettingGetNewMailMsgCommand(string vCommandStr){

//     ChatController::getInstance()->getNewMailMsg(vCommandStr);
}
void ChatServiceCocos2dx::removeMail2fromUid(string fromUid)
{
    if(fromUid.length() == 0)
        return;
    NSString *ns_fromUid = [NSString stringWithUTF8String:fromUid.c_str()];
    ChatChannel *cc = [[ChannelManager sharedChannelManager].channel_map objectForKey:ns_fromUid];
    [cc.msgList removeAllObjects];
}

void ChatServiceCocos2dx::stopReturnToChat()
{
    if(!enableNativeChat) return;
    IOSScheduleObject::getInstance()->stopReturnToChat();
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
    
    CCLOG("popupCloseEvent isChatDialogNeedBack = %d", ChatServiceCocos2dx::isChatDialogNeedBack);
    CCLOG("popupCloseEvent waitingForReturnToChatAfterPopup = %d", waitingForReturnToChatAfterPopup);
    CCLOG("popupCloseEvent popUpCount = %d m_curPopupWindowNum = %d", (PopupViewController::getInstance()->getCurrViewCount() +PopupViewController::getInstance()->getGoBackViewCount()), ChatServiceCocos2dx::m_curPopupWindowNum);
    
    if((PopupViewController::getInstance()->getCurrViewCount() +PopupViewController::getInstance()->getGoBackViewCount())== ChatServiceCocos2dx::m_curPopupWindowNum && waitingForReturnToChatAfterPopup && ChatServiceCocos2dx::isChatDialogNeedBack)
    {
        goBackToNewMailList();
    }
}

void IOSScheduleObject::goBackToNewMailList()
{
    if(!(waitingForReturnToChatAfterPopup && ChatServiceCocos2dx::isChatDialogNeedBack))
        return;
    ChatServiceCocos2dx::isChatDialogNeedBack = false;
    waitingForReturnToChatAfterPopup = false;
    ChatServiceCocos2dx::m_rememberPosition = true;
    
    if (ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_USER || ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_CHATROOM) {
        if (ChatServiceCocos2dx::Mail_OC_Native_Enable){
            ChatServiceCocos2dx::m_channelType=-1;
            [[ServiceInterface serviceInterfaceSharedManager] showMailListIOS];
        }
    }else{
        if(!ChatServiceCocos2dx::isChatShowing_fun())
        {
            if (actionAfterResume=="showPlayerInfo@mod") {
                //是否联系mod头像 java ChatServiceController.isContactMod = true;
                [ChatServiceController chatServiceControllerSharedManager].isContactMod = true;
            }
            [[ServiceInterface serviceInterfaceSharedManager] showChatIOSFrom2dx:ChatServiceCocos2dx::m_channelType withAddState:OpenChatIOS_Jump];
        }
    }
    
    actionAfterResume = "";
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_POPUP_CLOSE_EVENT);
}

void IOSScheduleObject::stopReturnToChat()
{
    CCLOGFUNC("stopReturnToChat 1");
    if(waitingForReturnToChatAfterPopup)
    {
        CCLOGFUNC("stopReturnToChat 2");
        waitingForReturnToChatAfterPopup = false;
        ChatServiceCocos2dx::isChatDialogNeedBack = false;
        actionAfterResume = "";
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
            //zym 2015.12.11
//            PopupViewController::getInstance()->forceClearAll(true); simon
            WorldMapView::instance()->gotoTilePoint(pt);
        }else{
            int index = WorldController::getIndexByPoint(pt);
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, worldIndex);
        }
    }else if(actionAfterResume == "checkTheEquip"){
//        PopupViewController::getInstance()->addPopupView(EquipInfoView::createWithEquipId(equipId)); simon
    }else if(actionAfterResume == "showMailPopup"){
        CCLOGFUNCF("showMailPopup uid:%s",uid.c_str());
        auto search = GlobalData::shared()->mailList.find(uid.c_str());
        bool isExistMail=(search != GlobalData::shared()->mailList.end());
        
        MailInfo* mailInfo=NULL;
        if (isExistMail) {
            mailInfo=dynamic_cast<MailInfo*>(search->second);
        }
        
        if(mailInfo!=NULL)
        {
            CCLOGFUNC("mailInfo!=NULL");
            MailController::getInstance()->showMailPopupFromAnroid(mailInfo);
        }
    }else if(actionAfterResume == "rallyMsg"){
        ChatController::getInstance()->viewRallyInfo(uid.c_str());
    }else if(actionAfterResume == "changeNickName"){
        ChatController::getInstance()->changeNickName();
    }else if(actionAfterResume == "allianceTask"){
        ChatController::getInstance()->viewAllianceTaskInfo();
    }else if(actionAfterResume == "viewRedPackage"){
        ChatController::getInstance()->viewRedPackage(uid, name,true);
    }else if(actionAfterResume == "getRedPackage"){
        ChatController::getInstance()->viewRedPackage(uid, name);
    }else if(actionAfterResume == "openFriendsView"){
        PopupViewController::getInstance()->addPopupInView(FriendsView::create());
    }
    
    if(returnToChatAfterPopup && !waitingForReturnToChatAfterPopup){
        waitingForReturnToChatAfterPopup = true;
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(IOSScheduleObject::popupCloseEvent), MSG_POPUP_CLOSE_EVENT, NULL);
    }else{
        actionAfterResume = "";
    }
    
    uid = "";
    name = "";
    attachmentId = "";
    returnToChatAfterPopup = false;

}

void IOSScheduleObject::resetResume(){
    
    actionAfterResume = "";
    uid = "";
    name = "";
    reportUid="";
    equipId = 0;
    waitingForReturnToChatAfterPopup = false;
    returnToChatAfterPopup = false;
}



void ChatServiceCocos2dx::updateChatChannelRoom(int channelType, string channelID, string uidArr, string roomOwner, string customName)
{
    
    NSString *ns_channelID = [NSString stringWithUTF8String:channelID.c_str()];
    NSString *ns_roomOwner = [NSString stringWithUTF8String:roomOwner.c_str()];
    NSString *ns_customName = [NSString stringWithUTF8String:customName.c_str()];
    NSString *ns_uidArr = [NSString stringWithUTF8String:uidArr.c_str()];
    
    [[ChannelManager sharedChannelManager] updateChatChannelRoom:channelType withAddChannelID:ns_channelID withAddUidArr:ns_uidArr withAddRoomOwner:ns_roomOwner withAddCustomName:ns_customName];
    
}



bool ChatServiceCocos2dx::isFirstOpenMailToPlayer(string fromUid)
{
    NSString *ns_fromUid = [NSString stringWithUTF8String:fromUid.c_str()];
    return [[[ChannelManager sharedChannelManager].channel_map allKeys] containsObject:ns_fromUid];
}
void ChatServiceCocos2dx::postKingUid(string kingUid){
    NSString * kingUidString = [NSString stringWithCString:kingUid.c_str() encoding:4];
    [ChatServiceController chatServiceControllerSharedManager].kingUidString = kingUidString;
}

#pragma mark -
#pragma mark 服务器的开关设置
void ChatServiceCocos2dx::settingDisableSendMsgWithServiewListKey(string listKeyStr){
    NSString *banTimeString = [NSString stringWithCString:listKeyStr.c_str() encoding:4];
    [ChatServiceController chatServiceControllerSharedManager].banTimeString = banTimeString;
}

/**是否进入巨龙战场*/
bool ChatServiceCocos2dx::isGoIntoServer_Dragon_Battle(){
//      if  (GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE){ simon
//          return true;
//      }else{
//          return false;
//      }
    return true;
}

#pragma mark -
#pragma mark 初始化channel
void ChatServiceCocos2dx::initDatabase()
{
        DVLog(@"initDatabase_________________________");
    [[ChannelManager sharedChannelManager].channel_map removeAllObjects];
     [[DBManager sharedDBManager] removeDBManager]; ;
   
    if(ChatServiceCocos2dx::DB_ChatChannel_switch){
         [[ChannelManager sharedChannelManager] loadingAllChannelWithDB];
    }
    [[ServiceInterface serviceInterfaceSharedManager] packageMsgCommand];
    
    
    if  (ChatServiceCocos2dx::CS_WebSocket_Msg_Get){
       [[WbSocketControl SingleControl] webScoketConnect];
    }
    
//    NSString *allianceID = [NSString stringWithCString:GlobalData::shared()->playerInfo.getAllianceId().c_str() encoding:4];
//    ChatChannel *chatChannel =  [[ChannelManager sharedChannelManager] gettingCountryChannel];
//    NSMsgItem *msg = nil;
//    NSMsgItem *allianLastMsg =  nil;
//    if  (chatChannel ){
//        msg =  [chatChannel gettingLastMsg];
//    }
//    if(  msg == nil){
//          [ServiceInterface serviceInterfaceSharedManager].contrySeqid =0;
//      
//    }else{
//        [ServiceInterface serviceInterfaceSharedManager].contrySeqid =msg.sequenceId;
//    }
//    if  (allianceID.length > 0){
//        ChatChannel *allChannel =  [[ChannelManager sharedChannelManager] gettingAllianceChannel];
//        NSMsgItem *allianLastMsg =  [allChannel gettingLastMsg];
//    }
//    
//    if(allianLastMsg){
//        [ServiceInterface serviceInterfaceSharedManager].allianceSeqid = allianLastMsg.sequenceId;
//    }else{
//        [ServiceInterface serviceInterfaceSharedManager].allianceSeqid = 0;
//    }
   
    
    
}

#pragma mark -
#pragma mark 第一次启动时调用
void ChatServiceCocos2dx::gettingChatMsgWithAppOpen(){
   
    if  (ChatServiceCocos2dx::CS_WebSocket_Msg_Get){
        //独立服务器数据获取
    }else{
        NSString *commandStr =[ServiceInterface serviceInterfaceSharedManager].msgCommandString;
        DVLog(@"消息请求的command :%@", commandStr);
        [[CSCommandManager sharedCommandManager] getChatMailMsgWithCommandType:0 andWithCommandString: commandStr  ];
    }
  
    
    if  (ChatServiceCocos2dx::DB_SystemMail_switch){
        //分批加载系统邮件
        [[CSCommandManager sharedCommandManager] commandStart];
    }
    
    
}

#pragma mark -
#pragma mark 联盟UID字符串
void ChatServiceCocos2dx::notifyUserUids(string uidStr,string lastUpdateTimeStr){
    
    if(uidStr == "" || lastUpdateTimeStr == "")
        return;
    
    NSString *ns_uidStr = [NSString stringWithUTF8String:uidStr.c_str()];
    DVLog(@"联盟成员UID ：%@",ns_uidStr);
    
    NSArray *uidArr=[ns_uidStr componentsSeparatedByString:@"_"];
    __block   NSString *allUidString = @"";
    NSString *ns_lastUpdateTimeStr = [NSString stringWithUTF8String:lastUpdateTimeStr.c_str()];
    NSArray *lastUpdateTimeArr=[ns_lastUpdateTimeStr componentsSeparatedByString:@"_"];
    
   __block NSMutableArray *userUidArray = [[NSMutableArray alloc]init];
    
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        for ( int  x = 0 ; x<uidArr.count ;x++){
           
            NSString *uidStr = uidArr[x];
            
            
             allUidString = [allUidString stringByAppendingString:uidStr];
            if( x<uidArr.count-1){
                allUidString = [allUidString stringByAppendingString:@"_"];
            }
 
           NSUserInfo *tempUser = [[UserManager sharedUserManager] gettingUserInfoForMemoryAndDBWithUidString:uidStr];
//            NSUserInfo *tempUser =[[UserManager sharedUserManager] gettingUser:uidStr];
//            if  (tempUser == nil){
//                tempUser = [[UserManager sharedUserManager]gettingUserInforWithUid:uidStr];
//            }
            if(tempUser == nil){
                [userUidArray addObject:uidStr];
            }else{
                long   tempCreatTime = [lastUpdateTimeArr[x] integerValue];
                if(tempUser.lastUpdateTime <tempCreatTime){
                    [userUidArray addObject:uidStr];
                }
            }
        }
        
        ChatChannel *allChannel = [[ChannelManager sharedChannelManager]gettingAllianceChannel];
        if  (allChannel && allUidString.length > 0){
            allChannel.memberUidString = allUidString;
            [allChannel  channelUpDateWithSelfChanged];
        }
        dispatch_async(dispatch_get_main_queue(), ^{
            if (userUidArray.count> 0){
                [[UserManager sharedUserManager] getMultiUserInfo:userUidArray];
            }
            
        });
        
    });
    
    
  
}

#pragma mark -
#pragma mark 获取某用户信息
void ChatServiceCocos2dx::updateUserInfoWithUidString(string uidStr){
    NSString *uidString =[NSString stringWithCString:uidStr.c_str() encoding:4];
    [[UserManager sharedUserManager]gettingServicerUser:uidString];
//    [[UserManager sharedUserManager]gettingUserInfoForMemoryAndDBWithUidString:uidString];
    
}

#pragma mark -
#pragma mark 群聊相关
void  ChatServiceCocos2dx::settingGroupChatMailVCChatChannel(){
    NSString *chatKeyString = [UserManager  sharedUserManager].currentMail.opponentUid;
    ChatChannel *chatChannel =[[ChannelManager sharedChannelManager].channel_map objectForKey:chatKeyString];
    DVLog(@"chatChannel :%@",chatChannel);
    
    [[ServiceInterface serviceInterfaceSharedManager].mailViewController.mailTableTableViewController setCurrentChatChannel:chatChannel];
    
}


  /**群聊搜索玩家*/
void ChatServiceCocos2dx::selectPlayer(cocos2d::CCArray *players)
{
    long  len=players->count();
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
        DLog(@"%ld",userInfo.lastUpdateTime);
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
    if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
        CSMessage *lastMessage  = nil;
        if  (channelType ==IOS_CHANNEL_TYPE_COUNTRY){
             ChatChannel *countryChannel =[[ChannelManager sharedChannelManager]gettingCountryChannel];
            lastMessage = [countryChannel gettingLastMessageFromDB];
        }else{
             ChatChannel *countryChannel= [[ChannelManager sharedChannelManager] gettingAllianceChannel];
             lastMessage = [countryChannel gettingLastMessageFromDB];
        }
        if (lastMessage){
            CSMessageModel * msgModel =[[CSMessageModel alloc]initWithMessage:lastMessage];
            [msgModel messageModelShowingFormat];
            if (msgModel){
                [[ChatServiceController chatServiceControllerSharedManager].gameHost refreshChatInfoWithCSMessage:msgModel ];
            }

        }
 
        
    }else{
        ChatCellFrame *ccf = nil;
        if(channelType == IOS_CHANNEL_TYPE_COUNTRY && [[[ChannelManager sharedChannelManager] gettingCountryChannel].msgList count] > 0){
            ccf = [[[ChannelManager sharedChannelManager] gettingCountryChannel].msgList lastObject];
        }else if(channelType == IOS_CHANNEL_TYPE_ALLIANCE && [[[ChannelManager sharedChannelManager] gettingAllianceChannel].msgList count] > 0){
            ccf = [[[ChannelManager sharedChannelManager] gettingAllianceChannel].msgList lastObject];
        }

        [[ChatServiceController chatServiceControllerSharedManager].gameHost refreshChatInfo:ccf.chatMessage];
    }
    
}
#pragma mark - 国家消息接收

void ChatServiceCocos2dx::csPushCountryChatMsgWithMsgArray( int commandType,cocos2d::CCArray *countryMailArray,string maxServerSeq,string  minServerSeq){
    if(countryMailArray && countryMailArray->count() > 0){
 
        NSString *maxServerSeqString = [NSString stringWithCString:maxServerSeq.c_str() encoding:4];
        long serverMaxID = [maxServerSeqString integerValue];
        NSString *minServerSeqString = [NSString stringWithCString:minServerSeq.c_str() encoding:4];
        long serverminID = [minServerSeqString integerValue];
        __block cocos2d::CCArray *tempArray =countryMailArray;
        dispatch_async([ServiceInterface serviceInterfaceSharedManager].csChatMsgQueue, ^{
            
            NSString *ns_channelId = [NSString stringWithFormat:@"%ld",[UserManager sharedUserManager].currentUser.serverId];
            
            if(![[[ChannelManager sharedChannelManager].channel_map allKeys] containsObject:ns_channelId]){
                
                [[ChannelManager sharedChannelManager] createChatChannel:IOS_CHANNEL_TYPE_COUNTRY withAddChannelID:ns_channelId];
            }
            
            ChatChannel *cc   = [[ChannelManager sharedChannelManager] gettingCountryChannel];
            cc.serverMinSeqId = serverminID;
            cc.serverMaxSeqId = serverMaxID;
            NSMutableArray *tempMsgArray =[NSMutableArray array];
            
            for(int  i = 0; i < tempArray->count(); i++)
            {
                ChatMailInfo* chatInfo=dynamic_cast<ChatMailInfo*>(tempArray->objectAtIndex(i));
                if (chatInfo) {
                    
                    if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
                        [NSUserInfo userCheckWithChatMailInfo:chatInfo];
                        CSMessage *msg = [CSChatMsgFactory chatMessageWithChatMailInfo:chatInfo];
                        if (ChatServiceCocos2dx::DB_MsgItem_switch) {
                            [cc savingMessageToDBWithMsg:msg];
                        }
                        
                    }else{
                        /**创建信息模型*/
                        NSMsgItem *cm = [CSChatMsgFactory chatMsgWithChatMailInfo:chatInfo];
                        if (ChatServiceCocos2dx::DB_MsgItem_switch) {
                            [cc savingMsgItemToDBWithMsgItem:cm];
                        }
                        [cm msgItemCheckUserInfoWithMsgBeforeMsgSaveToDB];
                        [tempMsgArray addObject:cm];

                    }
                    
                }
            }
            if (tempArray->getReferenceCount() >0){
                CCLog("引用计数count == %d",tempArray->getReferenceCount());
                tempArray->release();
            }
            dispatch_async(dispatch_get_main_queue(), ^{
                
               
                if (commandType == 0){
                    [cc chatNewMsgPushed:tempMsgArray];
                }else {
                    [cc chatmsgpushWithPullDownAfterScreenlockedMsgArray:tempMsgArray];
                }
 
            });
          
            
        });
 
    }
}
#pragma mark - 联盟消息接收
void ChatServiceCocos2dx::csPushAllianceChatMsgWithMsgArray(int commandType,cocos2d::CCArray *allianceMailArray,string  maxServerSeq,string minServerSeq){
    if(allianceMailArray && allianceMailArray->count() > 0){
        NSString *maxServerSeqString = [NSString stringWithCString:maxServerSeq.c_str() encoding:4];
        long serverMaxID = [maxServerSeqString integerValue];
        NSString *minServerSeqString = [NSString stringWithCString:minServerSeq.c_str() encoding:4];
        long serverminID = [minServerSeqString integerValue];
        
        __block cocos2d::CCArray *tempArray =allianceMailArray;
        dispatch_async([ServiceInterface serviceInterfaceSharedManager].csChatMsgQueue, ^{
            
            NSString *ns_channelId = [NSString stringWithFormat:@"%@",[UserManager sharedUserManager].currentUser.allianceId];
            
            if(![[[ChannelManager sharedChannelManager].channel_map allKeys] containsObject:ns_channelId]){
                
                [[ChannelManager sharedChannelManager] createChatChannel:IOS_CHANNEL_TYPE_ALLIANCE withAddChannelID:ns_channelId];
            }
            
            ChatChannel *cc   = [[ChannelManager sharedChannelManager] gettingAllianceChannel];
            cc.serverMinSeqId = serverminID;
            cc.serverMaxSeqId = serverMaxID;
            NSMutableArray *tempMsgArray =[NSMutableArray array];
            
            for(int  i = 0; i < tempArray->count(); i++)
            {
                ChatMailInfo* chatInfo=dynamic_cast<ChatMailInfo*>(tempArray->objectAtIndex(i));
                if (chatInfo) {
                    
                    if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
                        [NSUserInfo userCheckWithChatMailInfo:chatInfo];
                        CSMessage *msg = [CSChatMsgFactory chatMessageWithChatMailInfo:chatInfo];
                        if (ChatServiceCocos2dx::DB_MsgItem_switch) {
                             //消息插入数据库
                            [cc savingMessageToDBWithMsg:msg];
                        }
                        
                    }else{
                        /**创建信息模型*/
                        NSMsgItem *cm = [CSChatMsgFactory chatMsgWithChatMailInfo:chatInfo];
                        if (ChatServiceCocos2dx::DB_MsgItem_switch) {
                             //消息插入数据库
                            [cc savingMsgItemToDBWithMsgItem:cm];
                        }
                        [cm msgItemCheckUserInfoWithMsgBeforeMsgSaveToDB];
                        [tempMsgArray addObject:cm];
                        
                    }
                    
                }
            }
            dispatch_async(dispatch_get_main_queue(), ^{
                if (commandType == 0){
                     [cc chatNewMsgPushed:tempMsgArray];
                }else {
                     [cc chatmsgpushWithPullDownAfterScreenlockedMsgArray:tempMsgArray];
                }
               
 
            });
            if (tempArray->getReferenceCount() >0){
                tempArray->release();
            }
            
        });
    }
}
//下拉刷新接口
void ChatServiceCocos2dx::cs_get_mail_Old_AllianceChatMsgWithMsgArray(cocos2d::CCArray *allianceMailArray ,int channleType){
    if(allianceMailArray && allianceMailArray->count() > 0){
 
        __block cocos2d::CCArray *tempArray =allianceMailArray;
        dispatch_async([ServiceInterface serviceInterfaceSharedManager].csChatMsgQueue, ^{
            ChatChannel *cc  ;
            if(channleType == IOS_CHANNEL_TYPE_ALLIANCE){
                NSString *ns_channelId = [NSString stringWithFormat:@"%@",[UserManager sharedUserManager].currentUser.allianceId];
                
                if(![[[ChannelManager sharedChannelManager].channel_map allKeys] containsObject:ns_channelId]){
                    
                    [[ChannelManager sharedChannelManager] createChatChannel:IOS_CHANNEL_TYPE_ALLIANCE withAddChannelID:ns_channelId];
                }
                
                cc   = [[ChannelManager sharedChannelManager] gettingAllianceChannel];
            }else if(channleType == IOS_CHANNEL_TYPE_COUNTRY){
                 cc   = [[ChannelManager sharedChannelManager] gettingCountryChannel];
            }
            
            
            NSMutableArray *tempMsgArray =[NSMutableArray array];
            
            for(int  i = 0; i < tempArray->count(); i++)
            {
                ChatMailInfo* chatInfo=dynamic_cast<ChatMailInfo*>(tempArray->objectAtIndex(i));
                if (chatInfo) {
                    
                    if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
                        [NSUserInfo userCheckWithChatMailInfo:chatInfo];
                        CSMessage *msg = [CSChatMsgFactory chatMessageWithChatMailInfo:chatInfo];
                        if (ChatServiceCocos2dx::DB_MsgItem_switch) {
                            //消息插入数据库
                            [cc savingMessageToDBWithMsg:msg];
                        }
                        
                    }else{
                        /**创建信息模型*/
                        NSMsgItem *cm = [CSChatMsgFactory chatMsgWithChatMailInfo:chatInfo];
                        if (ChatServiceCocos2dx::DB_MsgItem_switch) {
                            //消息插入数据库
                            [cc savingMsgItemToDBWithMsgItem:cm];
                        }
                        [cm msgItemCheckUserInfoWithMsgBeforeMsgSaveToDB];
                        [tempMsgArray addObject:cm];
                        
                    }
                    
                }
            }
            dispatch_async(dispatch_get_main_queue(), ^{
                [cc chatMsgPushWithGetOldMsgArray:tempMsgArray];
 
            });
 
            if (tempArray->getReferenceCount() >0){
                tempArray->release();
            }
        });
    }
}
#pragma mark -  单人邮件
void ChatServiceCocos2dx::pushChatMailMsgWithArray(int commandType,cocos2d::CCArray *vChatArray){
    if (vChatArray->count()> 0) {
        __block cocos2d::CCArray *tempArray = vChatArray ;
        dispatch_async([ServiceInterface serviceInterfaceSharedManager].csChatMsgQueue, ^{
            
                NSMutableDictionary *mutDic =[NSMutableDictionary dictionary];
            for (int x=0; x <vChatArray->count(); x++) {
                ChatMailInfo *tempMailInfo = (ChatMailInfo *)tempArray->objectAtIndex(x);
                if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
                    [NSUserInfo userCheckWithChatMailInfo:tempMailInfo];
                    CSMessage *msg = [CSChatMsgFactory chatMessageWithChatMailInfo:tempMailInfo];
                    NSString *channelId = [NSString stringWithUTF8String:tempMailInfo->channelId.c_str()];
                    NSMutableArray *tempMutArray = nil;
                    tempMutArray = [mutDic objectForKey:channelId];
                    if  (tempMutArray == nil ){
                        tempMutArray  = [NSMutableArray array ];
                    }
                    int  chatChanneltype =  tempMailInfo->channelMsgType;
                    
                    NSString *fromName = [NSString stringWithUTF8String:tempMailInfo->fromName.c_str()];
                    NSString *faceImage =[NSString stringWithUTF8String:tempMailInfo->headPic.c_str()];
                    if(faceImage == nil || faceImage.length == 0){
                        faceImage= @"g015";
                    }
                    int headVar = tempMailInfo->headPicVer;

                    ChatChannel * channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:channelId];
                    if (channel == nil  ){
                        [[ChannelManager sharedChannelManager]createChatChannel:chatChanneltype withAddChannelID:channelId];
                        channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:channelId];
                        
                    }
                    if (ChatServiceCocos2dx::DB_MsgItem_switch) {
                        [channel savingMessageToDBWithMsg:msg];
                        
                    }
                    channel.channelType = msg.channelType;
                    channel.nameString = fromName;
                    channel.faceImageString = faceImage;
                    if (chatChanneltype == IOS_CHANNEL_TYPE_USER && headVar > 0){
                        NSString *custemUrl = [NSString gettingCustomHeadPic:channelId intValue:headVar];
                        channel.faceCustemString = custemUrl;
                    }
                    
                    [tempMutArray addObject:msg];
                    
                    [mutDic setObject:tempMutArray forKey:channelId];

                }else{
                    NSMsgItem *msgItem = [ChatMailMsgManager chatMsgWithChatMailInfo:tempMailInfo];
                    //            [msgItem msgItemCheckUserInfoWithMsgBeforeMsgSaveToDB];
                    NSString *channelId = [NSString stringWithUTF8String:tempMailInfo->channelId.c_str()];
                    NSMutableArray *tempMutArray = nil;
                    tempMutArray = [mutDic objectForKey:channelId];
                    if  (tempMutArray == nil ){
                        tempMutArray  = [NSMutableArray array ];
                    }
                    int  chatChanneltype =  tempMailInfo->channelMsgType;
                    
                    NSString *fromName = [NSString stringWithUTF8String:tempMailInfo->fromName.c_str()];
                    NSString *faceImage =[NSString stringWithUTF8String:tempMailInfo->headPic.c_str()];
                    if(faceImage == nil || faceImage.length == 0){
                        faceImage= @"g015";
                    }
                    int headVar = tempMailInfo->headPicVer;
                    
                    
                    ChatChannel * channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:channelId];
                    if (channel == nil  ){
                        [[ChannelManager sharedChannelManager]createChatChannel:chatChanneltype withAddChannelID:channelId];
                        channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:channelId];
                        
                    }
                    if (ChatServiceCocos2dx::DB_MsgItem_switch) {
                        [channel savingMsgItemToDBWithMsgItem:msgItem];
                        
                    }
                    channel.channelType = msgItem.channelType;
                    channel.nameString = fromName;
                    channel.faceImageString = faceImage;
                    if (chatChanneltype == IOS_CHANNEL_TYPE_USER && headVar > 0){
                        NSString *custemUrl = [NSString gettingCustomHeadPic:channelId intValue:headVar];
                        channel.faceCustemString = custemUrl;
                    }
                    
                    [tempMutArray addObject:msgItem];
                    
                    [mutDic setObject:tempMutArray forKey:channelId];
                }
                

 
                
            }
            NSArray *allChannelIDArray =[mutDic allKeys];
            for (NSString *tempChannelID  in allChannelIDArray) {
                ChatChannel * channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:tempChannelID];
                if (channel == nil  ){
                    [[ChannelManager sharedChannelManager]createChatChannel:IOS_CHANNEL_TYPE_USER withAddChannelID:tempChannelID];
                    channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:tempChannelID];
                    
                }
                
                if (channel ){
                    dispatch_async(dispatch_get_main_queue()  , ^{
                        NSArray *msgArray = [mutDic objectForKey:tempChannelID];
                        if  (msgArray.count > 0){
//                            if (msgArray.count> 1) {
//                                msgArray =[NSArray sortingDataWithMsgItem:msgArray andWithChannelType:IOS_CHANNEL_TYPE_USER];
//                            }
                            
                            if (commandType == 0){
                                [channel chatNewMsgPushed: msgArray];
                            }else{
                                [channel chatmsgpushWithPullDownAfterScreenlockedMsgArray: msgArray];
                            }

                        }
                        
                    });
                }
            }
            
  
            
            if (tempArray->getReferenceCount() >0){
                tempArray->release();
            }
            
        });

    }
  }


#pragma mark -  聊天室
void ChatServiceCocos2dx::pushChatRoomMsgWihtArray(int commandType,string channelIDString,string roomNameString, string memberUidString ,string founderUid,cocos2d::CCArray *vChatRoomArray){
 
    NSString * channelID  =  [NSString stringWithCString:channelIDString.c_str() encoding:4];
    NSString * memberUID = [NSString stringWithCString:memberUidString.c_str() encoding:4];
    NSString * founderUID =[NSString stringWithCString:founderUid.c_str() encoding:4];
    NSString * customName =[NSString stringWithCString:roomNameString.c_str() encoding:4];
    ChatChannel * channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:channelID];
    if (channel == nil  ){
        [[ChannelManager sharedChannelManager]createChatChannel:IOS_CHANNEL_TYPE_CHATROOM withAddChannelID:channelID];
        channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:channelID];
    }
    channel.customName =customName;
    channel.memberUidString  = memberUID;
    channel.roomOwner = founderUID;
    [channel channelUpDateWithSelfChanged];
    __block  cocos2d::CCArray *tempArray = vChatRoomArray ;
    dispatch_async([ServiceInterface serviceInterfaceSharedManager].csChatMsgQueue, ^{
        NSMutableArray *tempArr = [NSMutableArray array];
        for (int x=0; x <tempArray->count(); x++) {
            ChatMailInfo *tempMailInfo = (ChatMailInfo *)tempArray->objectAtIndex(x);
            CCLOG("%s",tempMailInfo->id.c_str());
            if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
                [NSUserInfo userCheckWithChatMailInfo:tempMailInfo];
                CSMessage *msgItem = [CSChatMsgFactory chatMessageWithChatMailInfo:tempMailInfo];
                [channel savingMessageToDBWithMsg:msgItem];
                 [tempArr addObject:msgItem];
            }else{
                 NSMsgItem *msgItem = [ChatMailMsgManager chatMsgWithChatMailInfo:tempMailInfo];
                //            [msgItem msgItemCheckUserInfoWithMsgBeforeMsgSaveToDB];
                if (ChatServiceCocos2dx::DB_MsgItem_switch) {
                    [channel savingMsgItemToDBWithMsgItem:msgItem];
                }
                 [tempArr addObject:msgItem];
            }

           
//
        }
        
         dispatch_async(dispatch_get_main_queue()  , ^{
             if (commandType == 0){
                 [channel chatNewMsgPushed:tempArr];
             }else{
                 [channel chatmsgpushWithPullDownAfterScreenlockedMsgArray:tempArr];
             }
         });
        if (tempArray->getReferenceCount() >0){
            tempArray->release();
        }
    });
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
    [chatChannel channelUpDateWithSelfChanged];
    
}
/**创建多人聊天选人界面*/
void ChatServiceCocos2dx::groupChatVCInit(){
    //c++页面弹出多人聊天管理创建页面
    
    PersonSelectVC *personSelectVC=[[PersonSelectVC alloc]initWithType:PersonSelectVCType_New];
    [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController = personSelectVC;
//    personSelectVC.ispushFrom_OC_Native = NO;
    personSelectVC.personVCOpenFrom = PersonVCOpenFrom_CocosAdd;
    [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.hidden = NO;
    
}

//创建聊天室
void ChatServiceCocos2dx::createChatChannelRoom(string groupId, string memberUids)
{
    
    NSString *ns_groupId = [NSString stringWithUTF8String:groupId.c_str()];
    NSString *ns_memberUids = [NSString stringWithUTF8String:memberUids.c_str()];
    
    [[ChannelManager sharedChannelManager] createChatChannelRoom:ns_groupId withChannelMemberUids:ns_memberUids];
    
}



///**更新聊天室*/
void ChatServiceCocos2dx::updateChannelMemberArray(int channelType,string fromUid,string uidArr,int changeStatus) {
    //changeStatus 0-加人   1-踢人  2- 退出
    NSString *fromUidString = [NSString stringWithCString:fromUid.c_str() encoding:4];
    NSString *uidArrString =[NSString stringWithCString:uidArr.c_str() encoding:4];
    NSArray *uidArray =[uidArrString componentsSeparatedByString:@"_"];
    
    if (channelType == 3){
        if  (changeStatus == 0){
            NSString *ns_fromUid =[NSString stringWithUTF8String:fromUid.c_str()];
            ChatChannel *chatChannel =  [[ChannelManager sharedChannelManager] gettingChannelInfo:fromUidString];
            if (!chatChannel) {
                return;
            }
           
//            vector<string>strVec;
//            CCCommonUtils::splitString(uidArr, "_", strVec);
            NSMutableArray *tempArray =[NSMutableArray array];
            chatChannel.memberUidArray = [NSMutableArray arrayWithArray:[chatChannel.memberUidString componentsSeparatedByString:@"_"]];
            for (NSString *uidString in uidArray) {
                if ( ![uidString stringRepeatInArray:chatChannel.memberUidArray]){
                    [tempArray addObject:uidString];
                }
            }
//            for (int x= 0; x<strVec.size(); x++) {
//                NSString *uidString =[NSString stringWithUTF8String:strVec.at(x).c_str()];
//                if ( ![uidString stringRepeatInArray:chatChannel.memberUidArray]){
//                    [tempArray addObject:uidString];
//                }
//                
//            }
            [chatChannel.memberUidArray addObjectsFromArray:tempArray];
            NSString *str =@"";
            for (NSString *string in chatChannel.memberUidArray) {
                if (str.length  > 0) {
                    str = [str stringByAppendingString:@"_"];
                }
                str = [str stringByAppendingString:string];
                
            }
            chatChannel.memberUidString = str;
            [chatChannel channelUpDateWithSelfChanged];
            DVLog(@"fromUidString %@",ns_fromUid);
            DVLog(@"uidArray :%@",chatChannel.memberUidArray);
        }
        else if(changeStatus == 1){
            
            ChatChannel *chatChannel =  [[ChannelManager sharedChannelManager] gettingChannelInfo:fromUidString];
            if (chatChannel) {
                BOOL isRemoveChannel = NO;
                chatChannel.memberUidArray = [NSMutableArray arrayWithArray:[chatChannel.memberUidString componentsSeparatedByString:@"_"]];
                NSMutableArray *tempArray =[NSMutableArray array];
//                vector<string>strVec;
//                CCCommonUtils::splitString(uidArr, "_", strVec);
                for (NSString *uidString in uidArray) {
                    if ( ![uidString stringRepeatInArray:chatChannel.memberUidArray]){
                        [tempArray addObject:uidString];
                    }
                }
                for (int x= 0; x<uidArray.count; x++) {
                    NSString *uidString =[uidArray objectAtIndex:x];
                    NSString *delectString =nil;
                    for (NSString *string in chatChannel.memberUidArray) {
                        if  ([uidString isEqualToString:string]){
                            delectString = string;
                            break;
                        }
                    }
                    if(delectString ){
                        [chatChannel.memberUidArray removeObject:delectString];
                    }
                    
                    /**移除聊天室人员是否包含自己。如果包含那么删除本地的聊天室数据*/
                    if ([uidString isEqualToString:[UserManager sharedUserManager].currentUser.uid]) {
                        isRemoveChannel = YES;
                    }
                }
                
                if (isRemoveChannel) {
                    [[ChannelManager sharedChannelManager] deletingChatChannel:chatChannel];
                }else{
                    
                    NSString *str =@"";
                    for (NSString *string in chatChannel.memberUidArray) {
                        if (str.length  > 0) {
                            str = [str stringByAppendingString:@"_"];
                        }
                        str = [str stringByAppendingString:string];
                        
                    }
                    chatChannel.memberUidString = str;
                    [chatChannel channelUpDateWithSelfChanged];
                    
                }
                DVLog(@"fromUidString %@",fromUidString);
                DVLog(@"uidArray :%@",chatChannel.memberUidArray);
            }
            
            
        }else if (changeStatus ==  2){
            //(3,groupId,operatorUid,2)
            NSString *ns_groupId =[NSString stringWithUTF8String:fromUid.c_str()];
            NSString *ns_operatorUid =[NSString stringWithUTF8String:uidArr.c_str()];
            //删除聊天室包含 uidArr 的 uid
            DVLog(@"ns_groupId :%@",ns_groupId);
            DVLog(@"ns_operatorUid :%@",ns_operatorUid);
            
            ChatChannel *chatChannel =  [[ChannelManager sharedChannelManager] gettingChannelInfo:fromUidString];
            BOOL    isRemoveChannel = NO;
   
            
            for (int x= 0; x<uidArray.count; x++) {
                NSString *uidString =[uidArray objectAtIndex:x];
                NSString *tempString = nil;
                for (NSString *sstring in chatChannel.memberUidArray) {
                    if  ([sstring isEqualToString:uidString]){
                        tempString =sstring;
                        break;
                    }
                }
                if  (tempString){
                    [chatChannel.memberUidArray removeObject:tempString];
                    if ([tempString isEqualToString:[UserManager sharedUserManager].currentUser.uid]) {
                        isRemoveChannel = YES;
                    }
                }
                
            }
            
 
            if  (isRemoveChannel){
                /**当自己退出聊天室后需要删除本地缓存或DB对应的聊天室*/
                [[ChannelManager sharedChannelManager] deletingChatChannel:chatChannel];
            }
 
        }
        
    }
    
}
#pragma mark - 系统邮件消息接受
//系统邮件在线推送
  void ChatServiceCocos2dx::pushSystemMailForOnlineStatusWithCCDic(cocos2d::CCDictionary *vDict){
//      CCDictElement * pElement;
//      CCDICT_FOREACH(vDict, pElement)
//      {
//          const char * key = pElement->getStrKey();
//          
//          CCString * value = (CCString *)pElement->getObject();
//          CCLog(key);
//          CCLog(value ->getCString());
//      }
      __block cocos2d::CCDictionary *vTempDict =  vDict ;
      dispatch_async([ServiceInterface serviceInterfaceSharedManager].csChatMsgQueue, ^{
          MailInfoDataModel *mailInfoData = [MailInfoManager maildataForOnlinePushWithCCDictionary:vDict];
          if (mailInfoData){
              DVLog(@"%@",[mailInfoData printAllPropertys]);
              
     
            //  kTimeLog(mailInfoData.mail_ID);
              if (mailInfoData.channelID.length<=0){
                  mailInfoData.channelID = [mailInfoData mailInfoDataForChannelWithSelf];
              }
              if (mailInfoData.channelID.length<=0){
                  return;
              }
              ChatChannel * channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:mailInfoData.channelID];
              if (channel == nil){
                  [[ChannelManager sharedChannelManager]createChatChannel:IOS_CHANNEL_TYPE_SYSTEMMAIL withAddChannelID:mailInfoData.channelID];
                  channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:mailInfoData.channelID];
              }
              
              if (channel.channelDelegate &&[channel.channelDelegate isKindOfClass:[UIViewController class]]){
                  if  ([channel.channelDelegate respondsToSelector:@selector(systemMail_pushNewMsgWith:andMsg:)]){
                    
                      [channel.channelDelegate systemMail_pushNewMsgWith:channel andMsg:mailInfoData];
                  }
              }
              if  (ChatServiceCocos2dx::DB_SystemMail_switch){
                  [mailInfoData upDatingForDB];
              }
              if  (channel.latestTime < mailInfoData.creatTime){
                  channel.latestTime =  mailInfoData.creatTime;
              }
              [channel channelUnreadCountcalculatedWithSystemMail];
               [channel channelUpDateWithSelfChanged];
              ChatServiceCocos2dx::updateUnreadCountOnMastermind();
             
              
              if   (mailInfoData.type == Mail_Detect_Report && [mailInfoData isGrandCastleInvestigation]){
                  NSDictionary *contentsdic =[mailInfoData.contents objectFromJSONString];
                  mailInfoData.nameText = [[contentsdic objectForKey:@"user"] objectForKey:@"name"];

                  ChatServiceCocos2dx::updateDectCityMail([mailInfoData.mail_ID UTF8String],[mailInfoData.nameText UTF8String],mailInfoData.creatTime );
              }
                  
//                  ChatServiceCocos2dx::updateDectCityMail(string mailID ,string name ,int creatTime )
              
                  [[ChannelManager sharedChannelManager] systemMailChannelUnreadManager];
                  dispatch_async(dispatch_get_main_queue(), ^{
                      [[NSNotificationCenter defaultCenter]postNotificationName:kMailChannelUnReadCountChanged object:nil userInfo:@{@"channelID":channel.channelID}];
                      
                  });
       
          }
    });
  
}
void ChatServiceCocos2dx::pushSystemMailMsgWithMsgArray(cocos2d::CCArray *vSystemMailArray ,int isHasMoreMsg){
    //isHasMoreMsg = 1为还有
//    vSystemMailArray->retain();
    if  (vSystemMailArray && vSystemMailArray->count()> 0){
        __block cocos2d::CCArray *tempArray = vSystemMailArray ;
        
        dispatch_async([ServiceInterface serviceInterfaceSharedManager].csChatMsgQueue, ^{
            NSMutableArray *tempChannelArray =[NSMutableArray array];
            for (int x = 0; x<tempArray->count(); x++) {
//                MailInfo *tempMailInfo = (MailInfo *)vSystemMailArray->objectAtIndex(x);
//                MailInfoDataModel *mailInfoData = [MailInfoManager maildataWithMailInfo:tempMailInfo];
                        MailInfoDataModel *mailInfoData = [MailInfoManager maildataWithCCDictionary:(CCDictionary *)tempArray->objectAtIndex(x)];
                ChatChannel * channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:mailInfoData.channelID];
                if (channel == nil){
                    [[ChannelManager sharedChannelManager]createChatChannel:IOS_CHANNEL_TYPE_SYSTEMMAIL withAddChannelID:mailInfoData.channelID];
                    channel = [[ChannelManager sharedChannelManager ]gettingChannelInfo:mailInfoData.channelID];
                }
             
                if  (ChatServiceCocos2dx::DB_SystemMail_switch){
                    [mailInfoData upDatingForDB];
                }
                if  (channel.latestTime < mailInfoData.creatTime){
                    channel.latestTime =  mailInfoData.creatTime;
                }
                [channel channelUpDateWithSelfChanged];
 
            }
 
          
                if (tempArray->getReferenceCount() >0){
                   tempArray->release();
                }
           
            if  (isHasMoreMsg == 1){
                [[CSCommandManager sharedCommandManager] commandStart];
            }else{
                //没有最新邮件了就刷新未读
                [[ChannelManager sharedChannelManager] systemMailChannelUnreadUpdateWithAppOpened];
                ChatServiceCocos2dx::updateUnreadCountOnMastermind();
                
                ChatServiceCocos2dx::gettingTenMinuteDectCityMailList();
                
            }
            
        });

    }else{
        //没有最新邮件了就刷新未读
        [[ChannelManager sharedChannelManager] systemMailChannelUnreadUpdateWithAppOpened];
        ChatServiceCocos2dx::updateUnreadCountOnMastermind();
        
        ChatServiceCocos2dx::gettingTenMinuteDectCityMailList();
    }
    
   
}



#pragma mark - 初始化原生邮件列表
void ChatServiceCocos2dx::creatingMailListWith_OC_Native(){
//
    IOSScheduleObject::getInstance()->stopReturnToChat();
    CSMailRootCategoryListVC *mailListVC=[[CSMailRootCategoryListVC alloc]init ];
//     CSMailListViewController *mailListVC=[[CSMailListViewController alloc]init ];
//    [[ServiceInterface serviceInterfaceSharedManager].vc_win addSubview:mailListVC.view];
//    [[ServiceInterface serviceInterfaceSharedManager].vc_win bringSubviewToFront:mailListVC.view];
    UINavigationController *nav =[[UINavigationController alloc]initWithRootViewController:mailListVC];
    mailListVC.navigationController.navigationBarHidden = YES;
    [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController = nav;
    [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.hidden = NO;
}

void ChatServiceCocos2dx::chatUnBanOrUnBlock(string palyerName, int type)
{
    NSString *ns_playerName = [NSString stringWithUTF8String:palyerName.c_str()];
    [[UserManager sharedUserManager] removeRestrictUser:ns_playerName :type];
}

#pragma mark - 系统邮件服务器回调接口
//单封邮件已领取回调
void ChatServiceCocos2dx::setMailRewardStatusToReadWithMailID(string MailID){
    NSString *mailIDString =[NSString stringWithCString:MailID.c_str() encoding:4];
    [MailInfoManager settingRewardStatusToReadWithMailIDArray:@[mailIDString]];
    [[CSConvertViewManager sharedMailInfoManager]refresh_RewardStatusWithMailID:mailIDString andWithStatusFlag:YES];
   
}
//批量邮件已领取回调
void  ChatServiceCocos2dx::setMailRewardStatusForBatchToReadWithMailIDs(string MailID,int rewardStatus){
    if (rewardStatus == 1){
        //已经领取过
        NSString *mailIDsString =[NSString stringWithCString:MailID.c_str() encoding:4];
        NSArray *mailIdArray =[mailIDsString componentsSeparatedByString:@","];
        [MailInfoManager settingRewardStatusToReadWithMailIDArray:mailIdArray];
    }else{
        //本次领取的
        [[CSConvertViewManager sharedMailInfoManager]closeMailVC] ;
        NSString *mailIDsString =[NSString stringWithCString:MailID.c_str() encoding:4];
        NSArray *mailIdArray =[mailIDsString componentsSeparatedByString:@","];
        [MailInfoManager settingRewardStatusToReadWithMailIDArray:mailIdArray];

    }
    [[ChannelManager sharedChannelManager]systemMailChannelUnreadUpdateWithAppOpened ];
    ChatServiceCocos2dx::updateUnreadCountOnMastermind();
    
}
//批量领取，返回领取的内容
void ChatServiceCocos2dx::settingMailRewadToReadFor_batchWithRewardString(string MailIDs,string rewardString,int rewardStatus){
    if (rewardStatus == 0){
        // 奖励当次领取
        NSString *rewardStr  =[NSString stringWithCString:rewardString.c_str() encoding:4];
        DVLog(@"%@",rewardStr);
        id result ;
        result =[rewardStr JSONObject];
        if (result){
            DVLog(@"%@",result);
        }
        [[NSNotificationCenter defaultCenter]postNotificationName:kMailRewardBanchForClaim
                                                           object:nil
                                                         userInfo:@{@"rewardStatus":@"0",@"rewardDic":result}];
       

    }else{
        //奖励以前领取过
        [[NSNotificationCenter defaultCenter]postNotificationName:kMailRewardBanchForClaim object:nil
                                                         userInfo:@{@"rewardStatus":@"1",@"rewardDic":@""}];
    }
    //更新DB
    NSString *mailIDsString =[NSString stringWithCString:MailIDs.c_str() encoding:4];
    NSArray *mailIdArray =[mailIDsString componentsSeparatedByString:@","];
    [MailInfoManager settingRewardStatusToReadWithMailIDArray:mailIdArray];
    if (mailIdArray.count > 0){
        MailInfoDataModel *tempModel = [MailInfoManager mailInfoModelFromDBWithMailID:mailIdArray[0]];
        ChatChannel *channel =[[ChannelManager sharedChannelManager]gettingChannelInfo:tempModel.channelID];
        [channel channelUnreadCountcalculatedWithSystemMail];
        [channel channelUpDateWithSelfChanged];
        [[NSNotificationCenter defaultCenter]postNotificationName:kMailChannelUnReadCountChanged object:nil userInfo:@{@"channelID":channel.channelID} ];
        ChatServiceCocos2dx::updateUnreadCountOnMastermind();
    }
    
    
}
//单封邮件已读回调
void ChatServiceCocos2dx::settingUnReadStatusToReadWithMailID(string MailID){
    NSString *mailIDString =[NSString stringWithCString:MailID.c_str() encoding:4];
    [MailInfoManager settingUnReadStatusToReadWithMailIDArray:@[mailIDString]];
    [[CSConvertViewManager sharedMailInfoManager]refresh_ReadStatusWithMailID:mailIDString andWithStatusFlag:YES];
    
     [[ChannelManager sharedChannelManager]systemMailChannelUnreadUpdateWithAppOpened ];
    ChatServiceCocos2dx::updateUnreadCountOnMastermind();
}
//一个类型的邮件已读回调
void ChatServiceCocos2dx::settingUnReadStatusToreadWith_mailType(int mailType ,bool readStatus){
    NSString * channelID = [MailInfoDataModel channelStringWithMailType:mailType];
    [MailInfoManager settingUnReadStatusToReadWith_MailType:mailType];
     [[NSNotificationCenter defaultCenter]postNotificationName:kMailChannelUnReadCountChanged object:nil userInfo:@{@"channelID":channelID} ];
    
    ChatServiceCocos2dx::updateUnreadCountOnMastermind();
}
//一个人聊天未读回调
void ChatServiceCocos2dx::settingUnReadStatusToReadWIth_ChatUserUid(string uid,bool readstatus){
    NSString *channelIDString =[NSString stringWithCString:uid.c_str() encoding:4];
    ChatChannel *channel =[[ChannelManager sharedChannelManager]gettingChannelInfo:channelIDString];
    channel.unreadCount = 0;
    [channel channelUpDateWithSelfChanged];
      
    
    ChatServiceCocos2dx::updateUnreadCountOnMastermind();
}
//uid 批量删除回调
void  ChatServiceCocos2dx::deleteingbanchMailsWithMailID(string uid){
    
}
//黑骑士删除
void ChatServiceCocos2dx::deleteingKnightMail(){
    NSString *knightChannelStr = Mail_ChannelID_Knight;
//    ChatChannel *channel = [[ChannelManager sharedChannelManager]gettingChannelInfo:knightChannelStr];
//    if  (channel){
//        channel.unreadCount = 0;
//    }
    [MailInfoManager banchDeleteMailInfoWIthMailChannelID:knightChannelStr];
    
    [[NSNotificationCenter defaultCenter]postNotificationName:kMailChannelUnReadCountChanged object:nil userInfo:@{@"channelID":knightChannelStr} ];
}
#pragma mark - loading完成
void ChatServiceCocos2dx::loadingFinishedSingel(){
    
    //获取所有联盟成员Uid
    MailController::getInstance()->getAllianceMember();
//   [[ChatServiceController chatServiceControllerSharedManager]loadChatBarOnCocoas];

}
#pragma mark - 设置翻译禁用
void ChatServiceCocos2dx::setDisableLang(std::string disableLang)
{
  
    ChatChannel *countryCC = [[ChannelManager sharedChannelManager]gettingCountryChannel];
    ChatChannel *allianceCC =[[ChannelManager sharedChannelManager]gettingAllianceChannel ] ;
    [countryCC.msgList removeAllObjects];
    [allianceCC.msgList removeAllObjects];
    
    NSString* nsDisableLang = [NSString stringWithUTF8String:disableLang.c_str()];
    [TranslateManager sharedTranslateManagerInstance].disableLang = nsDisableLang;
}

#pragma mark - app进入后台回到前台代理调用
//进入前台
void ChatServiceCocos2dx::CSApplicationDidBecomeActiveLessThanSixtySec(){
    NSString * countryID =[NSString stringWithFormat:@"%d",[UserManager sharedUserManager].currentUser.serverId] ;
    NSString *  commandStr =  @"";
    NSString * countrySqe = [NSString stringWithFormat:@"%@",[ServiceInterface serviceInterfaceSharedManager].countrySeqIDWithPause] ;
    
    NSString * allianceSqe = [NSString stringWithFormat:@"%@",[ServiceInterface serviceInterfaceSharedManager].allianceSeqIDWithPause]  ;
    NSString * mailIDSqe = [NSString stringWithFormat:@"%@",[ServiceInterface serviceInterfaceSharedManager].mailIDWithPause] ;
    if ([ServiceInterface serviceInterfaceSharedManager].allianceSeqIDWithPause.length > 0) {
//        std::string allianceId = GlobalData::shared()->playerInfo.getAllianceId();
        NSString *allianceIDString =[NSString stringWithCString:GlobalData::shared()->playerInfo.getAllianceId().c_str() encoding:4];
        
        commandStr =[commandStr stringByAppendingFormat:@"%@|%@|0,%@|%@|1,0|%@|2",
                     countryID,countrySqe,
                     allianceIDString,allianceSqe,
                     mailIDSqe];
//        commandStr = countryID + "|"+countrySqe + "|0,"+allianceId+"|"+allianceSqe + "|1,0|"+mailIDSqe+"|2" ;
    }else{
        commandStr =[commandStr stringByAppendingFormat:@"0|%@|0,0|%@|2",
                     countryID,countrySqe,
                     mailIDSqe];
//        commandStr = "0|"+countrySqe + "|0,0|"+mailIDSqe+"|2" ;
    }
    commandStr = [[commandStr stringByAppendingString:@","] stringByAppendingString:[ServiceInterface serviceInterfaceSharedManager].chatRoomCommandString];
//    commandStr = commandStr + ","+[[ServiceInterface serviceInterfaceSharedManager].chatRoomCommandString UTF8String];
//    DVLog(@"自己uid :%@",[[UserManager sharedUserManager] currentUser ].uid);
//    DVLog(@"获取缓存请求的command %@",commandStr );
    
   dispatch_async([ServiceInterface serviceInterfaceSharedManager].csChatMsgQueue, ^{
        while ([[CSCommandManager sharedCommandManager] isConnected] == NO) {
            sleep(1);
        }
        [[CSCommandManager sharedCommandManager] getChatMailMsgWithCommandType:1 andWithCommandString: commandStr  ];
        [[CSCommandManager sharedCommandManager] commandStart];
    
   });
    
//    ChatServiceCocos2dx::gettingGetNewMailMsgCommand(commandStr);
    
    if (!ChatServiceCocos2dx::CS_CountryChat_OC_Native_New){
        //未开新版原生聊天
        if (ChatServiceCocos2dx::m_channelType ==IOS_CHANNEL_TYPE_USER ||  ChatServiceCocos2dx::m_channelType ==IOS_CHANNEL_TYPE_CHATROOM) {
            
            //关闭键盘
            [[ServiceInterface serviceInterfaceSharedManager].mailViewController endEditWithTextField];
            //隐藏原生邮件
            [ServiceInterface serviceInterfaceSharedManager].vc_win.hidden  =YES;
            
        }else if (ChatServiceCocos2dx::m_channelType ==IOS_CHANNEL_TYPE_COUNTRY ||  ChatServiceCocos2dx::m_channelType ==IOS_CHANNEL_TYPE_ALLIANCE){
            [[ServiceInterface serviceInterfaceSharedManager].chatViewController chatVCEndingEdit];
            [ServiceInterface serviceInterfaceSharedManager].vc_win.hidden  =YES;
        }
        UIWindow *tempWindow =[ServiceInterface serviceInterfaceSharedManager].chatRootWindow;
        if  (tempWindow.rootViewController){
            if  ([tempWindow.rootViewController isKindOfClass:[UINavigationController class]]){
                //邮件打开着
                tempWindow.rootViewController = nil;
                tempWindow.hidden = YES;
            }
        }
    }else{
 
            [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.rootViewController = nil;
            [ServiceInterface serviceInterfaceSharedManager].chatRootWindow.hidden = YES;
      
    }
    
  
    
    [[CSCommandManager sharedCommandManager] systemMailForLockScreenCommandStartWithMailID:[ServiceInterface serviceInterfaceSharedManager].mail_system_LastMailID andWithMailCreateTime:[ServiceInterface serviceInterfaceSharedManager].mail_system_lastCreateTime];
        CCDirector::sharedDirector()->setVisitFlag(true);
}

//进入后台
void ChatServiceCocos2dx::CSApplicationWillDidGotoBackGround(){
    
    ChatChannel *chatChannel =  [[ChannelManager sharedChannelManager] gettingCountryChannel];
    if  (chatChannel ){
        [ServiceInterface serviceInterfaceSharedManager].countrySeqIDWithPause =  [NSString stringWithFormat:@"%d",[(NSMsgItem *)[chatChannel gettingLastMsg] sequenceId]];
    }
    ChatChannel *allChannel =  [[ChannelManager sharedChannelManager] gettingAllianceChannel];
    if  (allChannel ){
        [ServiceInterface serviceInterfaceSharedManager].allianceSeqIDWithPause =  [NSString stringWithFormat:@"%d",[(NSMsgItem *)[allChannel gettingLastMsg] sequenceId]];
    }else{
        [ServiceInterface serviceInterfaceSharedManager].allianceSeqIDWithPause = @"0";
    }
    ChatChannel *mailChannel =   [[ChannelManager sharedChannelManager] gettingMailChannelWithLastChanged];
    [ServiceInterface serviceInterfaceSharedManager].chatRoomCommandString =[[ChannelManager sharedChannelManager] stringForChatRoomWithAllRoomChannel];
    NSString * mailIDString = @"0";
    if (mailChannel != nil) {
        NSMsgItem *msgItem =  [mailChannel gettingLastMsg];
        mailIDString= [NSString stringWithFormat:@"%@",msgItem.mailId] ;
    }
    [ServiceInterface serviceInterfaceSharedManager].mailIDWithPause =mailIDString;
    MailInfoDataModel *tempMailInfo =[MailInfoDataModel gettingLastSystemMailFromDB];
    if  (tempMailInfo){
        [ServiceInterface serviceInterfaceSharedManager].mail_system_LastMailID = tempMailInfo.mail_ID;
        [ServiceInterface serviceInterfaceSharedManager].mail_system_lastCreateTime = tempMailInfo.creatTime;
    }else{
        [ServiceInterface serviceInterfaceSharedManager].mail_system_LastMailID = @"0";
        [ServiceInterface serviceInterfaceSharedManager].mail_system_lastCreateTime = 0;
    }
    
}

#pragma mark - 比较系统版本号
bool ChatServiceCocos2dx::compareVersionWithKeyVersionString( string vKeyString){
    return  [VersionHelper compareVersionWithKeyVersionStr:[NSString stringWithUTF8String:vKeyString.c_str()]];
}
#pragma mark - 更新主城未读数
void ChatServiceCocos2dx::updateUnreadCountOnMastermind(){
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        return;
    UIComponent::getInstance()->updateMailNum(NULL);
}
//返回未读数
int  ChatServiceCocos2dx::settingMailUnreadCount(){
    return  [[ChannelManager sharedChannelManager]chechAllUnreadCount];
   
}
 

void ChatServiceCocos2dx::settingOC_Native_MailViewShowFromCocos(PopupBaseView *popView){
    [[CSConvertViewManager sharedMailInfoManager] ocNativeFormCocos2dWithPopUpView:popView isShowAgain:YES];
}

#pragma mark - C++端要的城堡侦查邮件相关

void ChatServiceCocos2dx::gettingTenMinuteDectCityMailList(){
    
    NSDate* dat = [NSDate dateWithTimeIntervalSinceNow:-1*60*10];
    
    long nowTimes =  (long)[dat timeIntervalSince1970];
    NSArray *investArray = [MailInfoManager mainCityInvestigativeNewsAfterCreatTime:nowTimes];
    NSMutableDictionary *dic =[NSMutableDictionary dictionary];
    for (MailInfoDataModel *mailInfo in investArray) {
        NSDictionary *contentsdic =[mailInfo.contents objectFromJSONString];
        mailInfo.nameText = [[contentsdic objectForKey:@"user"] objectForKey:@"name"];
        
        MailInfoDataModel *tempModel =[dic objectForKey:mailInfo.nameText];
        
        if  (tempModel){
            if (tempModel.creatTime> mailInfo.creatTime){
                [dic setObject:tempModel forKey:mailInfo.nameText];
            }
        }else{
            [dic setObject:mailInfo forKey:mailInfo.nameText];
        }
    }
    NSArray *allInvestArray =[dic allValues];
    DVLog(@"%@",allInvestArray);
    
    map<string,pair<string,int>> m_detectMailMap;//name uid createTime
    for (MailInfoDataModel *model   in allInvestArray) {
        string name = [model.nameText UTF8String];
        string mailUid = [model.mail_ID UTF8String];
        int createTime = model.creatTime;
        m_detectMailMap.insert(make_pair(name, make_pair(mailUid, createTime)));
    }
#if DEBUG
    auto tempIt =  m_detectMailMap.begin();
    while (tempIt !=  m_detectMailMap.end()) {
        string name =(*tempIt).first;
        string uid = (*tempIt).second.first;
        int createTime = (*tempIt).second.second;
        CCLOG("whilelog=====name: %s,uid:  %s,createtime  %d",name.c_str(),uid.c_str(),createTime);
        tempIt++;
    }
#endif

    MailController::getInstance()->initAllMail_ios(m_detectMailMap);
    
}
#pragma mark - 返回好友列表的最后一条消息
  map<string,string>  ChatServiceCocos2dx::lastMsgWithFriendList(std::vector<std::string> friendUidVec){
      NSMutableArray *tempArr=[NSMutableArray array];
       NSString *tempString =@"";
         map<string,string>friendMap;
      for(int i=0;i<friendUidVec.size();i++)
      {
          if(friendUidVec[i]!="")
          {
              tempString = [NSString stringWithUTF8String:friendUidVec[i].c_str()];
              DLog(@"好友UID ：%@",tempString);
              [tempArr addObject:tempString];
          }
      }
    NSDictionary * dic =   [ChannelManager lastMessageForUserUidArray:tempArr];
      DLog(@"%@",dic);
      NSArray *allkeys = [dic allKeys];
      for (NSString *keyStirng in allkeys) {
          NSString  *valueString = [dic objectForKey:keyStirng];
          
          friendMap.insert(make_pair([keyStirng UTF8String],[valueString UTF8String]));
      }
      return friendMap;
}
#pragma mark - 原生关闭回调
void ChatServiceCocos2dx::nativeUIClosed(){
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ReturnFrom_ChatOrMail); simon
}
void ChatServiceCocos2dx::updateDectCityMail(string mailID ,string name ,int creatTime ){
    map<string,pair<string,int>> m_detectMailMap;//name uid createTime
 
        m_detectMailMap.insert(make_pair(name, make_pair(mailID, creatTime)));
    
    MailController::getInstance()->updateDectMail_ios(m_detectMailMap);
}
void ChatServiceCocos2dx::deleteDectCityMail(map<string,pair<string,int>> mailDectMap ){
    
    MailController::getInstance()->DeleteDectMail_ios(mailDectMap);
}
#pragma mark - 获取解析后的邮件

void ChatServiceCocos2dx::popUpNewMail(string uid)
{
    if (!uid.empty())
    {
        MailData *data = [MailDataManager maildataWIthMailID:[NSString stringWithUTF8String:uid.c_str()]];
        
        if (!data)
        {
            return ;
        }
        
        CCDictionary *dic = CCJSONConverter::sharedConverter()->dictionaryFrom([data.jsonStr UTF8String]);
        
        MailController::getInstance()->addMailFromAndroidToList(dic, true);
        
        auto search = GlobalData::shared()->mailList.find(uid.c_str());
        bool isExistMail=(search != GlobalData::shared()->mailList.end());
        
        MailInfo* mailInfo=NULL;
        if (isExistMail) {
            mailInfo=dynamic_cast<MailInfo*>(search->second);
        }
        
        if(mailInfo!=NULL)
        {
            CCLOGFUNC("mailInfo!=NULL");
            MailController::getInstance()->showMailPopupFromAnroid(mailInfo);
        }
        
    }
}

void ChatServiceCocos2dx::showChatView2IOS()
{
    if(!enableNativeChat) return;
    [[ServiceInterface serviceInterfaceSharedManager] showChatViewIOS];
}

void ChatServiceCocos2dx::postRedPackageGotUids(string uids)
{
    CCLog("%s",uids.c_str());
    
    //0-以领取  1- 未领取 2-红包被抢光 3-红包到期
    //7b0ba3781b7845d588e74834fc204e4c_1|1
    NSString *uidStr = [NSString stringWithUTF8String:uids.c_str()];
    NSArray *uidArr = [uidStr componentsSeparatedByString:@","];
    for (NSString *redPackage in uidArr){
        NSArray *arr = [redPackage componentsSeparatedByString:@"|"];
        if (arr.count>1){
            NSString *redPackageKey = arr[0];
            NSString *attachmentString =[redPackageKey stringByAppendingFormat:@"|%@",arr[1]];
            NSMsgItem *tempMsg =[[ChatServiceController chatServiceControllerSharedManager].redPackgeMsgDict objectForKey:redPackageKey];
            if (tempMsg) {
                tempMsg.attachmentId = attachmentString;
            }
            
        }
    }
//    for (NSString *redPackage in uidArr){
//        NSArray *arr = [redPackage componentsSeparatedByString:@"|"];
//        NSString *key = (NSString*)arr[0];
//        NSString *redPacketStatus = (NSString*)arr[1];
//        
//        
////        [[ChannelManager sharedChannelManager].redPackageUnHandleMap setObject:val forKey:key];
//    }
}

void ChatServiceCocos2dx::postNotify_NewMailChatCreat_OC_Native( int sucessed,string name,string uid ){
    if (sucessed == 1){
        //邮件创建成功
        NSString *namestring =[NSString stringWithCString:name.c_str() encoding:4];
        NSString *uidString =[NSString stringWithCString:uid.c_str() encoding:4];
        [[NSNotificationCenter defaultCenter] postNotificationName:kMailChatCreatByOCNative object:nil userInfo:@{@"creatStatus":@"1",@"name":namestring,@"uid":uidString}];

    }else{
        NSString *namestring =[NSString stringWithCString:name.c_str() encoding:4];
        NSString *uidString =[NSString stringWithCString:uid.c_str() encoding:4];
        [[NSNotificationCenter defaultCenter] postNotificationName:kMailChatCreatByOCNative object:nil userInfo:@{@"creatStatus":@"0",@"name":namestring,@"uid":uidString}];

    }
}
 
void ChatServiceCocos2dx::loadMoreMailFromIOS(string channelId)
{
    if(!ChatServiceCocos2dx::Mail_OC_Native_Enable)
        return;
    ChatChannel *cc = [[ChannelManager sharedChannelManager] gettingChannelInfo:[NSString stringWithUTF8String:channelId.c_str()]];
    if (cc && cc.msgList && [cc.msgList count] > 0)
    {
        MailInfoDataModel *mode = [cc.msgList lastObject];
        if (mode){
          [[ChannelManager sharedChannelManager] loadMoreSysMailFromDBByChannel:cc withAddId:mode];
        }
    }
    
}

void ChatServiceCocos2dx::postMailDealStatus(string mailUid)
{
    if(ChatServiceCocos2dx::Mail_OC_Native_Enable)
    {
        [[ChannelManager sharedChannelManager] dealMailFrom2dxByMailId:[NSString stringWithUTF8String:mailUid.c_str()]];
    }
}

void ChatServiceCocos2dx::postRedPackageStatus(string uids, int status){
    
    NSString *attachmentId = [NSString stringWithUTF8String:uids.c_str()];
    NSString *statusStr = [NSString stringWithFormat:@"%d",status];
    
    ChatChannel *cc = nil;
    if (ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_COUNTRY){
        cc = [[ChannelManager sharedChannelManager] gettingCountryChannel];
    }else if(ChatServiceCocos2dx::m_channelType == IOS_CHANNEL_TYPE_COUNTRY){
        cc = [[ChannelManager sharedChannelManager] gettingAllianceChannel];
    }
    if (cc){
        NSMsgItem *mi = [cc redPackageSearchWithAttachmentId:attachmentId];
        if(mi)
            mi.attachmentId = [NSString stringWithFormat:@"%@|%@",attachmentId,statusStr];
        [cc redPacketMsgSaveToDBWithMsgItem:mi];
    }
    
    [[NSNotificationCenter defaultCenter]postNotificationName:kRefreshRedPackage object:nil userInfo:@{@"attachmentId":attachmentId,@"status":statusStr}];
}

void ChatServiceCocos2dx::postRedPackageDuringTime(int time){
    [MsgMessage msgMessageShared].redPackageTime = time;
}
//邮件标星回掉
void ChatServiceCocos2dx::setMailSave(string mailId,int saveFlag)
{
    NSString *mail_ID = [NSString stringWithUTF8String:mailId.c_str()];
    MailInfoDataModel *mailInfo = [MailInfoManager mailInfoModelFromDBWithMailID:mail_ID];
    [[CSConvertViewManager sharedMailInfoManager]mailStarWithMailID:mail_ID andWithStarStatus:saveFlag];
    if (mailInfo){
        if (mailInfo.saveFlag != saveFlag){
            mailInfo.saveFlag = saveFlag;
            [mailInfo upDatingForDB];
        }
    }
}
void ChatServiceCocos2dx::deleteOneMailWithMailID(string mailID,int type){
    dispatch_async([ServiceInterface serviceInterfaceSharedManager].csChatMsgQueue, ^{
        NSString * mailIDstring =[NSString stringWithCString:mailID.c_str() encoding:4];
        [[CSConvertViewManager sharedMailInfoManager]deleteMailAtCocosViewWithMailID:mailIDstring];
        
    });
}
//多封合一封邮件删除
void ChatServiceCocos2dx::deleteMail(string mailId,int channelType,int type)
{
    DVLog(@"%@",@"删除邮件");
     [MailInfoManager banchDeleteMailInfoWithMailType:type];
    NSString *channelIDString = [MailInfoDataModel channelStringWithMailType:type];
   ChatChannel *channel =  [[ChannelManager sharedChannelManager] gettingChannelInfo:channelIDString];
    if(channel){
        [[NSNotificationCenter defaultCenter]postNotificationName:kMailChannelUnReadCountChanged object:nil userInfo:@{@"channelID":channel.channelID}];
    }
}

void ChatServiceCocos2dx::initBanList(cocos2d::CCArray *shieldList){
    DVLog(@"initBanList");
    cocos2d::CCObject* obj;
    CCARRAY_FOREACH(shieldList, obj){
        ShieldInfo* info = dynamic_cast<ShieldInfo*>(obj);
        NSString *blockUID = [NSString stringWithUTF8String:info->uid.c_str()];
        [[UserManager sharedUserManager] addRestrictUser:blockUID :1];
    }
}

void ChatServiceCocos2dx::exitChatActivityFrom2dx(bool needRemeberActivityStack){
//    ChatServiceCocos2dx::hiddenChatIosFrom2dx();
//    NetController::shared()->checkGameConnection(); simon
}

void ChatServiceCocos2dx::changeLanguage(){
    [ServiceInterface serviceInterfaceSharedManager].isChangeLanguage = TRUE;
}
#endif