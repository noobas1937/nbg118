//
//  AOEUtilies.h
//  aoe
//
//  Created by 岳文铎 on 13-3-1.
//
//
#include <string>
#include "FBUtilies.h"
//#import "NSString+MD5Addition.h"
#import <MessageUI/MessageUI.h>
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>
#import <FBSDKShareKit/FBSDKShareKit.h>
#include "AppController.h"
#include "CCSafeNotificationCenter.h"
#include "TypeDefinition.h"
#include <sys/sysctl.h>
#import "CCEAGLView-ios.h"
#import <FBSDKShareKit/FBSDKSharingContent.h>
#import "GameController.h"

//#import "ASIFormDataRequest.h"

#define  MSG_FBFriendsList           "MFBFriendsList"
#define MSG_FBRequestResult         "MFBRequestResult"
#define MSG_FBIviteFriends         "MFBInviteFriends"
#define MSG_FBAskForHelpResult         "MFBAskForHelpResult"
static const int REQUEST_SUCESS=1;
static const int REQUEST_ERROR=2;
static const int REQUEST_CANCEl=3;
static int fbPreRequest = 0;  //0 取绑定过的好友，1 取没有装过游戏的好友，3同1，4取邀请过自己的好友，5、feed 回调

static std::string feedName = "";
static std::string feedCaption = "";
static std::string feedLinkDescription = "";
static std::string feedLink = "";
static std::string feedPictureUrl = "";
static int feedType = 0;
static std::string feedRef = "";

USING_NS_CC;

static CGPoint convertDesignCoordToScreenCoord(const CCPoint& designCoord, bool bInRetinaMode)
{
    GLView* eglView = Director::getInstance()->getOpenGLView();// geCCEGLView::sharedOpenGLView();
    float viewH =eglView->getFrameSize().height;
    //float viewH = (float)[[EAGLView sharedEGLView] getHeight];
    
    CCPoint visiblePos = ccp(designCoord.x * eglView->getScaleX(), designCoord.y * eglView->getScaleY());
    CCPoint screenGLPos = ccpAdd(visiblePos, eglView->getViewPortRect().origin);
    
    CGPoint screenPos = CGPointMake(screenGLPos.x, viewH - screenGLPos.y);
    
    if (bInRetinaMode)
    {
        screenPos.x = screenPos.x / 2.0f;
        screenPos.y = screenPos.y / 2.0f;
    }
    
    return screenPos;
}

@interface AOEUtiliesIOS() <MFMessageComposeViewControllerDelegate,MFMailComposeViewControllerDelegate,UINavigationControllerDelegate,UIImagePickerControllerDelegate,FBSDKGameRequestDialogDelegate,FBSDKAppInviteDialogDelegate>{//
    FBSDKLikeButton *mlike;
    UIPopoverController * popoverController;
    int playerUid;
}

@property(nonatomic, retain)FBSDKLikeButton* mlike;

+(AOEUtiliesIOS*)shared;
-(void)loginFacebook;
-(void)upLoadImage:(int) srcCode Index:(int) index;
@end

// 
void FBUtilies::checkIsGameFans_FB(){
    [[AOEUtiliesIOS shared] checkIsGameFans_FB];
}

// 登陆Facebook
bool FBUtilies::fbLogin(){
    [[AOEUtiliesIOS shared] loginFacebook];
    return true;
}

// LogoutFacebook
void FBUtilies::fbLogout()
{
    CCUserDefault::sharedUserDefault()->setStringForKey("inviteFriends", "");
    CCUserDefault::sharedUserDefault()->setStringForKey("installFriendsInfo", "");
    CCUserDefault::sharedUserDefault()->flush();
    [[AOEUtiliesIOS shared] logoutFacebook];
}
// 选取Facebook上的好友
void FBUtilies::getFBFriendList(){
    [[AOEUtiliesIOS shared] getFBFriendsList];
}

void FBUtilies::fbLike(){
    [[AOEUtiliesIOS shared] addFBLike];
//    CGRect frame = [[[AOEUtiliesIOS shared] mlike] frame];
//    frame.origin = convertDesignCoordToScreenCoord(pos, [[EAGLView sharedEGLView] contentScaleFactor] == 2.0f);
//    [[[AOEUtiliesIOS shared] mlike] setFrame:frame];
    [[AOEUtiliesIOS shared] sendEvent];

}

void FBUtilies::removeLike(){
    //[[AOEUtiliesIOS shared] removeFBLike];
}

void FBUtilies::getFBInstallFriends(){
    [[AOEUtiliesIOS shared] getInstallFriends];
}

void FBUtilies::inviteFriends(){
    [[AOEUtiliesIOS shared] inviteFBFriends];
}

void FBUtilies::getAppRequestList(){
    [[AOEUtiliesIOS shared] getAppRequestFriendList];
}

void FBUtilies::appInvite(const std::string &appLink){
    [[AOEUtiliesIOS shared] fbInviteDialog:appLink];
}


// 给选取的好友发邀请
void FBUtilies::postFBSelectedFriendList(cocos2d::CCArray* arr,const std::string &msg,const std::string &ftype){
    if (!arr || strlen(msg.c_str()) == 0)
        return;
    
    NSString *selFriends = [[NSMutableString alloc] init];
    for (int i = 0; i < arr->count(); i++) {
        cocos2d::CCString *selId = dynamic_cast<cocos2d::CCString*>(arr->objectAtIndex(i));
        NSString *temp = [NSString stringWithUTF8String:selId->getCString()];
        if ([selFriends length]) {
            [selFriends appendString:@", "];
        }
        [selFriends appendString:temp];
    }
    
    const char* tempMsg = msg.c_str();
    NSString *nsmsg = [NSString stringWithUTF8String:tempMsg];
    NSString *mType = [NSString stringWithUTF8String:ftype.c_str()];

    [[AOEUtiliesIOS shared] postToFriends:selFriends Message:nsmsg MessageType:mType];
}

// 是否已经登陆Facebook
bool FBUtilies::fbIsLogin(){
    if ([FBSDKAccessToken currentAccessToken])
        return true;
    return false;
}

// 打开Facebook粉丝墙
void FBUtilies::fbShowFansWall(){
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"https://www.facebook.com/ClashOfKingsIF"]];
}

// 未使用
void FBUtilies::postFeed_FB(const std::string &msg)
{
    [[AOEUtiliesIOS shared] publishStory];
}

void FBUtilies::fbShareMessage(const std::string &msg){
    const char* tempMsg = msg.c_str();
    NSString *nsmsg = [NSString stringWithUTF8String:tempMsg];
    [[AOEUtiliesIOS shared] shareMessage:nsmsg];
}

void FBUtilies::fbSharePhotosByUrl(const std::string &url,const std::string &dialog){
    const char* tempUrl = url.c_str();
    NSString *nsUrl = [NSString stringWithUTF8String:tempUrl];
    const char* tempdialog = dialog.c_str();
    NSString *nsDialog = [NSString stringWithUTF8String:tempdialog];
    [[AOEUtiliesIOS shared] sharePhotos:nsUrl dialog:nsDialog];
}

// 通过邮件发送邀请
void FBUtilies::postFeed_email(const std::string &msg, const std::string &argSubject)
{
    const char* temp = msg.c_str();
    NSString *body = [NSString stringWithUTF8String:temp];
    
    NSString *subject = [NSString stringWithUTF8String:argSubject.c_str()];
    [[AOEUtiliesIOS shared] sendEmail:subject MessageBody:body];
    
}

// 未使用
void FBUtilies::postFeed_market()
{}


// 未使用
void FBUtilies::postFeed_sina(const std::string &msg)
{
    
}

bool FBUtilies::fbHasGranted(const std::string &permission){
    bool flag = false;
    const char* temp = permission.c_str();
    NSString *right = [NSString stringWithUTF8String:temp];
    FBSDKAccessToken* currentToken = [FBSDKAccessToken currentAccessToken];
    if (currentToken!=nil && [currentToken hasGranted:right]){//"publish_actions"
        flag = true;
    }
    return flag;
}

void FBUtilies::appEventPurchaseItem(int type,int user_level, int castle_level,int item_id,const std::string& item_name,int item_price,int item_count,int spend,const std::string entrance){
     CCLog("FBUtilies::appEventPurchaseItem ios");
    
    NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                   [NSString stringWithUTF8String:CC_ITOA(type)], @"type",
                                   [NSString stringWithUTF8String:CC_ITOA(user_level)], @"user_level",
                                   [NSString stringWithUTF8String:CC_ITOA(castle_level)], @"castle_level",
                                   [NSString stringWithUTF8String:CC_ITOA(item_id)], @"item_id",
                                   [NSString stringWithUTF8String:item_name.c_str()], @"item_name",
                                   [NSString stringWithUTF8String:CC_ITOA(item_price)], @"item_price",
                                   [NSString stringWithUTF8String:CC_ITOA(item_count)], @"item_count",
                                   [NSString stringWithUTF8String:CC_ITOA(spend)], @"spend",
                                   [NSString stringWithUTF8String:entrance.c_str()], @"entrance",
                                   nil];
    [FBSDKAppEvents logEvent:@"Purchase_Item"
                  valueToSum:spend
                  parameters:params];
}

void FBUtilies::appEventShareLog(const std::string &realm,int user_level,int castle_level,const std::string &share_content){
    CCLog("FBUtilies::fb appEventShareLog ios");
    
    NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                   [NSString stringWithUTF8String:realm.c_str()], @"realm",
                                   [NSString stringWithUTF8String:CC_ITOA(user_level)], @"user_level",
                                   [NSString stringWithUTF8String:CC_ITOA(castle_level)], @"castle_level",
                                   [NSString stringWithUTF8String:share_content.c_str()], @"share_content",
                                   nil];
    [FBSDKAppEvents logEvent:@"Share"
                  parameters:params];
}

void FBUtilies::appEventFBEntrance(const std::string &entrance){
    CCLog("FBUtilies::fb appEventFBEntrance ios");
    
    NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                   [NSString stringWithUTF8String:entrance.c_str()], @"entrance",
                                   nil];
    [FBSDKAppEvents logEvent:@"FacebookEntrance"
                  parameters:params];
}

void FBUtilies::appEventCityEffectLog(int cityLV,int playerLv){
    NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                   [NSString stringWithUTF8String:CC_ITOA(cityLV)], @"user_castle",
                                   [NSString stringWithUTF8String:CC_ITOA(playerLv)], @"user_level",
                                   nil];
    [FBSDKAppEvents logEvent:@"cityEffect"
                  parameters:params];
}

void FBUtilies::appEventSpeedUp(int user_castle,int user_level,int type,int spend){
    CCLog("FBUtilies::appEventSpeedUp ios");
    NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                   [NSString stringWithUTF8String:CC_ITOA(user_castle)], @"user_castle",
                                   [NSString stringWithUTF8String:CC_ITOA(user_level)], @"user_level",
                                   [NSString stringWithUTF8String:CC_ITOA(type)], @"type",
                                   [NSString stringWithUTF8String:CC_ITOA(spend)], @"spend",
                                   nil];
    [FBSDKAppEvents logEvent:@"SpeedUp"
                  parameters:params];
}

void FBUtilies::appEventGiftPackage(const std::string &entracnce,const std::string &name,int id,int user_castle,int user_level){
    NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                   [NSString stringWithUTF8String:entracnce.c_str()], @"package_entracnce",
                                   [NSString stringWithUTF8String:name.c_str()], @"package_name",
                                   [NSString stringWithUTF8String:CC_ITOA(id)], @"package_id",
                                   [NSString stringWithUTF8String:CC_ITOA(user_castle)], @"user_castle",
                                   [NSString stringWithUTF8String:CC_ITOA(user_level)], @"user_level",
                                   nil];
    [FBSDKAppEvents logEvent:@"GiftPackage"
                  parameters:params];
}

void FBUtilies::appEventHireWorker(int user_castle,int user_level){
    NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                   [NSString stringWithUTF8String:CC_ITOA(user_castle)], @"user_castle",
                                   [NSString stringWithUTF8String:CC_ITOA(user_level)], @"user_level",
                                   nil];
    [FBSDKAppEvents logEvent:@"HireWorker"
                  parameters:params];
}

void FBUtilies::appEventLevelUp(int user_level_up){
    NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                   [NSString stringWithUTF8String:CC_ITOA(user_level_up)], @"user_level_up",
                                   nil];
    [FBSDKAppEvents logEvent:@"LevelUp"
                  parameters:params];
}

void FBUtilies::appEventAllianceHonorExchange(const std::string &name,int id,int user_castle,int user_level,int rank){
    NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                   [NSString stringWithUTF8String:name.c_str()], @"alliance_item_name",
                                   [NSString stringWithUTF8String:CC_ITOA(id)], @"alliance_item_id",
                                   [NSString stringWithUTF8String:CC_ITOA(user_castle)], @"user_castle",
                                   [NSString stringWithUTF8String:CC_ITOA(user_level)], @"user_level",
                                   [NSString stringWithUTF8String:CC_ITOA(rank)], @"user_alliance_level",
                                   nil];
    [FBSDKAppEvents logEvent:@"AllianceHonorExchange"
                  parameters:params];
}

void FBUtilies::appEventAllianceScoreUsage(const std::string &name,int id,int rank){
    NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                   [NSString stringWithUTF8String:name.c_str()], @"alliance_item_name",
                                   [NSString stringWithUTF8String:CC_ITOA(id)], @"alliance_item_id",
                                   [NSString stringWithUTF8String:CC_ITOA(rank)], @"user_alliance_level",
                                   nil];
    [FBSDKAppEvents logEvent:@"AllianceScoreUsage"
                  parameters:params];
}

void FBUtilies::fbPublishFeedDialog(const std::string& name,const std::string& caption, const std::string& linkDescription,const std::string& link,const std::string& pictureUrl,int fType,const std::string& ref)
{
    feedName = name;
    feedCaption = caption;
    feedLinkDescription = linkDescription;
    feedLink = link;
    feedPictureUrl = pictureUrl;
    feedType = fType;
    feedRef = ref;
    if(!FBUtilies::fbIsLogin() && feedType==1){
        fbPreRequest = 5;
        FBUtilies::fbLogin();
        return ;
    }
    std::string data = "action=publish_feed";
    if (feedRef!="") {
        data.append(".");
        data.append(feedRef);
    }
    GameController::getInstance()->callXCApi(data);
    const char* tempName = name.c_str();
    NSString *nsName = [NSString stringWithUTF8String:tempName];
    NSString *nsCaption = [NSString stringWithUTF8String:caption.c_str()];
    NSString *nsLinkDescription = [NSString stringWithUTF8String:linkDescription.c_str()];
    NSString *nsLink = [NSString stringWithUTF8String:link.c_str()];
    NSString *nsUrl = [NSString stringWithUTF8String:pictureUrl.c_str()];
    NSString *nsRef = [NSString stringWithUTF8String:ref.c_str()];
    if(feedType==0){
        [[AOEUtiliesIOS shared] facebookShare:nsName
                                   MsgCaption:nsCaption
                                       MsgDes:nsLinkDescription
                                       MsgLink:nsLink
                                       MsgUrl:nsUrl
                                       MsgRef:nsRef
         ];
    }else{
        [[AOEUtiliesIOS shared] facebookPushFeed:nsName
                                   MsgCaption:nsCaption
                                       MsgDes:nsLinkDescription
                                       MsgLink:nsLink
                                       MsgUrl:nsUrl
                                       MsgRef:nsRef
         ];
    }
}

void FBUtilies::requestForMeInfo(){
    [[AOEUtiliesIOS shared] requestForMe];
}

void FBUtilies::fbPostAction(const std::string &action,const std::string &actionT,const std::string &actionObj){
    std::string tempName = "/me/clash-of-kings:";
    tempName.append(action);
    NSString *nsAction = [NSString stringWithUTF8String:tempName.c_str()];
    NSString *nsActionT = [NSString stringWithUTF8String:actionT.c_str()];
    NSString *nsActionObj = [NSString stringWithUTF8String:actionObj.c_str()];
    [[AOEUtiliesIOS shared] facebookPostAction:nsAction
                                  MsgType:nsActionT
                                      MsgObj:nsActionObj
     ];
    
}

void FBUtilies::fbPostMethod(const std::string &method,const std::string &objectType,const std::string &objectValue,const std::string &actions,const std::string &actionsValue,const std::string &ref){
    std::string tempName = "/me/";
    tempName.append(method);
    NSString *nsMethod = [NSString stringWithUTF8String:tempName.c_str()];
    NSString *nsObjectType = [NSString stringWithUTF8String:objectType.c_str()];
    NSString *nsObjectValue = [NSString stringWithUTF8String:objectValue.c_str()];
    NSString *nsActions = nil;
    if (actions!="") {
        nsActions = [NSString stringWithUTF8String:actions.c_str()];
    }
    NSString *nsActionsValue = nil;
    if (actionsValue!="") {
        nsActionsValue = [NSString stringWithUTF8String:actionsValue.c_str()];
    }
    NSString *nsRef = [NSString stringWithUTF8String:ref.c_str()];
    [[AOEUtiliesIOS shared] facebookPostMethod:nsMethod
                                        MsgType:nsObjectType
                                        MsgObj:nsObjectValue
                                        MsgActions:nsActions
                                        MsgActionsValue:nsActionsValue
                                        MsgRef:nsRef
     ];
}

#pragma mark SysUtilies
void SysUtilies::onUploadPhoto(int uid, int srcCode, int idx)
{
    if(idx<=0)
    {
        idx = 1;
    }
    [[AOEUtiliesIOS shared] upLoadImage: srcCode Index:idx Uid:uid];
}

const char* SysUtilies::getDevicePlatformInfo()
{
    size_t size;
    int nR = sysctlbyname("hw.machine", NULL, &size, NULL, 0);
    char *machine = (char *)malloc(size);
    nR = sysctlbyname("hw.machine", machine, &size, NULL, 0);
    NSString *platform = [NSString stringWithCString:machine encoding:NSUTF8StringEncoding];
    free(machine);
    
    if ([platform isEqualToString:@"iPhone1,1"])    return "iPhone 2G";
    if ([platform isEqualToString:@"iPhone1,2"])    return "iPhone 3G";
    if ([platform isEqualToString:@"iPhone2,1"])    return "iPhone 3GS";
    if ([platform isEqualToString:@"iPhone3,1"])    return "iPhone 4";
    if ([platform isEqualToString:@"iPhone3,2"])    return "iPhone 4";
    if ([platform isEqualToString:@"iPhone3,3"])    return "iPhone 4 (CDMA)";
    if ([platform isEqualToString:@"iPhone4,1"])    return "iPhone 4S";
    if ([platform isEqualToString:@"iPhone5,1"])    return "iPhone 5";
    if ([platform isEqualToString:@"iPhone5,2"])    return "iPhone 5 (GSM+CDMA)";
    
    if ([platform isEqualToString:@"iPhone5,3"])    return "iPhone 5c (GSM)";
    if ([platform isEqualToString:@"iPhone5,4"])    return "iPhone 5c (GSM+CDMA)";
    if ([platform isEqualToString:@"iPhone6,1"])    return "iPhone 5s (GSM)";
    if ([platform isEqualToString:@"iPhone6,2"])    return "iPhone 5s (GSM+CDMA)";
    
    if ([platform isEqualToString:@"iPod1,1"])      return "iPod Touch (1 Gen)";
    if ([platform isEqualToString:@"iPod2,1"])      return "iPod Touch (2 Gen)";
    if ([platform isEqualToString:@"iPod3,1"])      return "iPod Touch (3 Gen)";
    if ([platform isEqualToString:@"iPod4,1"])      return "iPod Touch (4 Gen)";
    if ([platform isEqualToString:@"iPod5,1"])      return "iPod Touch (5 Gen)";
    
    if ([platform isEqualToString:@"iPad1,1"])      return "iPad";
    if ([platform isEqualToString:@"iPad1,2"])      return "iPad 3G";
    if ([platform isEqualToString:@"iPad2,1"])      return "iPad 2 (WiFi)";
    if ([platform isEqualToString:@"iPad2,2"])      return "iPad 2";
    if ([platform isEqualToString:@"iPad2,3"])      return "iPad 2 (CDMA)";
    if ([platform isEqualToString:@"iPad2,4"])      return "iPad 2";
    if ([platform isEqualToString:@"iPad2,5"])      return "iPad Mini (WiFi)";
    if ([platform isEqualToString:@"iPad2,6"])      return "iPad Mini";
    if ([platform isEqualToString:@"iPad2,7"])      return "iPad Mini (GSM+CDMA)";
    if ([platform isEqualToString:@"iPad3,1"])      return "iPad 3 (WiFi)";
    if ([platform isEqualToString:@"iPad3,2"])      return "iPad 3 (GSM+CDMA)";
    if ([platform isEqualToString:@"iPad3,3"])      return "iPad 3";
    if ([platform isEqualToString:@"iPad3,4"])      return "iPad 4 (WiFi)";
    if ([platform isEqualToString:@"iPad3,5"])      return "iPad 4";
    if ([platform isEqualToString:@"iPad3,6"])      return "iPad 4 (GSM+CDMA)";
    if ([platform isEqualToString:@"iPad4,1"])      return "iPad Air (WiFi)";
    if ([platform isEqualToString:@"iPad4,2"])      return "iPad Air (Cellular)";
    if ([platform isEqualToString:@"iPad4,4"])      return "iPad Mini (Wifi)";
    if ([platform isEqualToString:@"iPad4,5"])      return "iPad Mini (Cellular)";
    
    if ([platform isEqualToString:@"i386"] || [platform isEqualToString:@"x86_64"])
        return "iPhone Simulator";
    
    return [platform UTF8String];
}

bool SysUtilies::isIphone()
{
    // 是否iPhone
    std::string sDevPlatformInfo = std::string(SysUtilies::getDevicePlatformInfo());
    if (std::string::npos == sDevPlatformInfo.find("iPhone",0)) {
        return false;
    }
    return true;
}

@implementation AOEUtiliesIOS

static AOEUtiliesIOS *_sharedUtilies;
+(AOEUtiliesIOS*)shared {
	if(_sharedUtilies == nil) {
		_sharedUtilies = [[AOEUtiliesIOS alloc] init];
	}
	return _sharedUtilies;
}
- (id)init {
    return self;
}

// 通过邮件发送邀请
- (IBAction)sendEmail:(NSString*)subject MessageBody:(NSString*)msgBody
{
    // 该设备可以发送邮件
    if ([MFMailComposeViewController canSendMail]) {
        //email is configured and ready to send new emails
        MFMailComposeViewController *mfMailVC = [[MFMailComposeViewController alloc]init];
        mfMailVC.mailComposeDelegate = self;
        
        [mfMailVC setToRecipients:[NSArray arrayWithObjects:nil]];
        [mfMailVC setCcRecipients:[NSArray arrayWithObjects:nil]];
        [mfMailVC setBccRecipients:[NSArray arrayWithObjects:nil]];
        
        [mfMailVC setSubject:[subject copy]];
        [mfMailVC setMessageBody:[msgBody copy] isHTML:NO];
        
        UIImage *img = [UIImage imageNamed:@"invite_share.jpg"];
        NSData *dataToEmail = UIImagePNGRepresentation(img);
        [mfMailVC addAttachmentData:dataToEmail mimeType:@"image/jpeg" fileName:@"imageOfBlackColor"];

        AppController* appDelegate = [UIApplication sharedApplication].delegate;
        [appDelegate.viewController presentViewController:mfMailVC animated:YES completion:nil];
    } else {
        //no email account configured
    }
}

// 发送邮件的回调函数
-(void)mailComposeController:(MFMailComposeViewController *)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError *)error
{
    switch (result) {
        case MFMailComposeResultCancelled:
            [controller dismissViewControllerAnimated:YES completion:nil];
            break;
            
        case MFMailComposeResultFailed:
            [controller dismissViewControllerAnimated:YES completion:nil];
            break;
            
        case MFMailComposeResultSaved:
            [controller dismissViewControllerAnimated:YES completion:nil];
            break;
            
        case MFMailComposeResultSent:
            [controller dismissViewControllerAnimated:YES completion:nil];
            break;
            
        default:
            break;
    }
}


-(void)loginFacebook
{
    [self openFBSession];
}

- (void)gameRequestDialog:(FBSDKGameRequestDialog *)gameRequest didCompleteWithResults:(NSDictionary *)results
{
    NSLog(@" gameRequestDialogresults:%@",results);
    NSString *selFriends = [[NSMutableString alloc] init];
    for (NSString<NSObject>* key in results) {
        NSString *value = [results objectForKey:key];
        if ([key isEqualToString: @"request"]) {
            cocos2d::CCString* reqID = cocos2d::CCString::create([value UTF8String]);
            CCUserDefault::sharedUserDefault()->setStringForKey(FB_RequestID, reqID->getCString());
            CCUserDefault::sharedUserDefault()->flush();
        }else{
            if ([selFriends length]) {
                [selFriends appendString:@","];
            }
            [selFriends appendString:value];
        }
    }
    NSLog(@" to send Friends:%@",selFriends);
    cocos2d::CCString* sendIds = cocos2d::CCString::create([selFriends UTF8String]);
    cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FBRequestResult,sendIds);

}

- (void)gameRequestDialog:(FBSDKGameRequestDialog *)gameRequest didFailWithError:(NSError *)error{
    NSLog(@" gameRequestDialogresults error:%@",error);
}

- (void)gameRequestDialogDidCancel:(FBSDKGameRequestDialog *)gameRequestDialog
{
     NSLog(@" gameRequestDialogDidCancel");
}

- (void)viewDidLoad{
    [super viewDidLoad];
}

-(void)logoutFacebook
{
    [self closeFBSession];
}

-(void)addFBLike
{
    if (mlike!=nil) {
        return ;
    }
    [FBSDKSettings setLimitEventAndDataUsage:YES];
    _sharedUtilies.mlike = [[FBSDKLikeButton alloc] init];
    _sharedUtilies.mlike.objectID = @"https://www.facebook.com/Clash.Of.Kings.Game";
    _sharedUtilies.mlike.userInteractionEnabled = true;
    _sharedUtilies.mlike.objectType = FBSDKLikeObjectTypeOpenGraph;
    _sharedUtilies.mlike.hidden = YES;
    [[CCEAGLView sharedEGLView] addSubview:[self mlike]];

}

-(void)removeFBLike
{
    [[self mlike] removeFromSuperview];
}

-(void)getInstallFriends
{
    if (!FBUtilies::fbIsLogin()) {//已经登录，但session关了
        fbPreRequest = 1;
        FBUtilies::fbLogin();
        return;
    }else {//已经登录，但session开着的
        FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc] initWithGraphPath:@"/me/friends?fields=id,name,devices&limit=200"
                                     parameters:nil
                                     HTTPMethod:@"GET"];
        [request startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
                                  NSArray* friends = [result objectForKey:@"data"];
                                  NSLog(@"Found: %i friends", friends.count);
                                  // 根据选取的好友信息创建数组
                                  cocos2d::CCArray *sel = cocos2d::CCArray::create();
                                  for (NSDictionary<NSObject>* user in friends) {
                                      cocos2d::CCDictionary* dic = cocos2d::CCDictionary::create();
                                      dic->setObject(cocos2d::CCBoolean::create(false), "check");
                                      dic->setObject(cocos2d::CCString::create([[user objectForKey:@"id"] UTF8String]), "id");
                                      dic->setObject(cocos2d::CCString::create([[user objectForKey:@"name"] UTF8String]), "name");
                                      NSArray* devices = [user objectForKey:@"devices"];
                                      std::string osStr = "";
                                      if(nil != devices){
                                          for (NSDictionary* dev in devices) {
                                              CCString* temp = cocos2d::CCString::create([[dev objectForKey:@"os"] UTF8String]);
                                              if(temp){
                                                  if(osStr.length()>0){
                                                      osStr.append(",");
                                                  }
                                                  osStr.append(temp->getCString());
                                              }
                                          }
                                      }
                                      if(osStr!=""){
                                          dic->setObject(cocos2d::CCString::create(osStr), "devices");
                                      }
                                      sel->addObject(dic);
                                  }
                                  cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FBFriendsList,sel);
                                  NSLog(@"Found: %i friends", friends.count);
                                  NSLog(@"Result: %@", result);
                              }];
    }

}

-(void)inviteFBFriends
{
    if (!FBUtilies::fbIsLogin()) {//已经登录，但session关了
        fbPreRequest = 2;
        FBUtilies::fbLogin();
        return;
    }else {//已经登录，但session开着的
        FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc] initWithGraphPath:@"/me/invitable_friends?limit=200"
                                     parameters:nil
                                     HTTPMethod:@"GET"];
        [request startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
                                  NSArray* friends = [result objectForKey:@"data"];
                                  NSLog(@" invitable_friends Found: %i friends", friends.count);
                                  // 根据选取的好友信息创建数组
                                  cocos2d::CCArray *sel = cocos2d::CCArray::create();
                                  NSString *selFriends = [[NSMutableString alloc] init];
                                  int countNum = 1;
                                  for (NSDictionary<NSObject>* user in friends) {
                                      NSString *temp = [NSString stringWithUTF8String:[[user objectForKey:@"id"] UTF8String]];
                                      if ([selFriends length]) {
                                          [selFriends appendString:@", "];
                                      }
                                      [selFriends appendString:temp];
                                      
                                      NSDictionary* picture = [user objectForKey:@"picture"];
                                      NSDictionary* picData = [picture objectForKey:@"data"];
                                      NSString* url = [picData objectForKey:@"url"];
                                      
                                      cocos2d::CCDictionary* dic = cocos2d::CCDictionary::create();
                                      dic->setObject(cocos2d::CCBoolean::create(false), "check");
                                      dic->setObject(cocos2d::CCString::create([[user objectForKey:@"id"] UTF8String]), "id");
                                      dic->setObject(cocos2d::CCString::create([[user objectForKey:@"name"] UTF8String]), "name");
                                      dic->setObject(cocos2d::CCString::create([url UTF8String]), "picture");
                                      dic->setObject(cocos2d::CCString::create(countNum>50?"0":"1"), "flag");
                                      sel->addObject(dic);
                                      countNum += 1;
                                  }
                                  cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FBIviteFriends,sel);
                                  const char* tempMsg = "test";
                                  //NSString *nsmsg = [NSString stringWithUTF8String:tempMsg];
                                  //[[AOEUtiliesIOS shared] postToFriends:selFriends Message:nsmsg];
                                  
                              }];
        

    }
    
}

// 选取Facebook上的好友
-(void) getFBFriendsList
{
    if (!FBUtilies::fbIsLogin()) {//已经登录，但session关了
        fbPreRequest = 3;
        FBUtilies::fbLogin();
    }else{//已经登录，但session开着的
        FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc] initWithGraphPath:@"/me/friends?fields=id,name,devices&limit=200"
                                     parameters:nil
                                     HTTPMethod:@"GET"];
        [request startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
                                  NSArray* friends = [result objectForKey:@"data"];
                                  NSLog(@"Found: %i friends", friends.count);
                                  // 根据选取的好友信息创建数组
                                  cocos2d::CCArray *sel = cocos2d::CCArray::create();
                                  for (NSDictionary<NSObject>* user in friends) {
                                      cocos2d::CCDictionary* dic = cocos2d::CCDictionary::create();
                                      dic->setObject(cocos2d::CCBoolean::create(false), "check");
                                      dic->setObject(cocos2d::CCString::create([[user objectForKey:@"id"] UTF8String]), "id");
                                      dic->setObject(cocos2d::CCString::create([[user objectForKey:@"name"] UTF8String]), "name");
                                      NSArray* devices = [user objectForKey:@"devices"];
                                      std::string osStr = "";
                                      if(nil != devices){
                                          for (NSDictionary* dev in devices) {
                                              CCString* temp = cocos2d::CCString::create([[dev objectForKey:@"os"] UTF8String]);
                                              if(temp){
                                                  if(osStr.length()>0){
                                                      osStr.append(",");
                                                  }
                                                  osStr.append(temp->getCString());
                                              }
                                          }
                                      }
                                      if(osStr!=""){
                                          dic->setObject(cocos2d::CCString::create(osStr), "devices");
                                      }
                                      CCLog("devices str=: %s",osStr.c_str());
                                      sel->addObject(dic);
                                  }
                                  NSLog(@"Result: %@", result);
                                  cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FBFriendsList,sel);
                              }];
    }

}

// 获得邀请自己的apprequest
-(void) getAppRequestFriendList
{
    if (!FBUtilies::fbIsLogin()) {//已经登录，但session关了
        fbPreRequest = 4;
        FBUtilies::fbLogin();
        return;
    }else{//已经登录，但session开着的
        FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc] initWithGraphPath:@"/me/apprequests?fields=from,action_type"
                                     parameters:nil
                                     HTTPMethod:@"GET"];
        [request startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
                                  NSArray* friends = [result objectForKey:@"data"];
                                  NSLog(@"apprequests Found: %i friends", friends.count);
                                  NSLog(@"apprequests Result: %@", result);
                                  // 根据选取的好友信息创建数组
                                  cocos2d::CCArray *sel = cocos2d::CCArray::create();
                                  for (NSDictionary* user in friends) {
                                      cocos2d::CCDictionary* dic = cocos2d::CCDictionary::create();
                                      if([user objectForKey:@"id"]){
                                          dic->setObject(cocos2d::CCString::create([[user objectForKey:@"id"] UTF8String]), "id");
                                      }
                                      if([user objectForKey:@"action_type"]){
                                          dic->setObject(cocos2d::CCString::create([[user objectForKey:@"action_type"] UTF8String]), "action_type");
                                      }
                                      if([user objectForKey:@"created_time"]){
                                          dic->setObject(cocos2d::CCString::create([[user objectForKey:@"created_time"] UTF8String]), "created_time");
                                      }
                                      NSDictionary* from = [user objectForKey:@"from"];
                                      if(nil != from){
                                          if([from objectForKey:@"id"]){
                                              dic->setObject(cocos2d::CCString::create([[from objectForKey:@"id"] UTF8String]), "fromId");
                                          }
                                          if([from objectForKey:@"name"]){
                                              dic->setObject(cocos2d::CCString::create([[from objectForKey:@"name"] UTF8String]), "fromName");
                                          }
                                      }
                                      sel->addObject(dic);
                                  }
                                  NSLog(@"apprequests parse friends num: %i", sel->count());
                                  cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(FB_APP_FriendsList,sel);
                              }];
    }
}


// 给选取的好友发邀请
-(void)postToFriends:(NSString*)selFriends Message:(NSString*)msg MessageType:(NSString*)mType
{
    
//    FBFrictionlessRecipientCache *friendCache = [[FBFrictionlessRecipientCache alloc] init];
//    [friendCache prefetchAndCacheForSession:nil];
    
    NSString *astring01 = @"0";
    BOOL result = [mType compare:astring01] == NSOrderedSame;
    
    //NSDictionary *parameters = @{@"to":[selFriends copy]};
    FBSDKGameRequestContent *gameRequestContent = [[FBSDKGameRequestContent alloc] init];
    NSArray *array = [selFriends componentsSeparatedByString:@","];
    gameRequestContent.to = array;
    gameRequestContent.message =  [msg copy];
    gameRequestContent.actionType = FBSDKGameRequestActionTypeNone;
    if(result){
        if(nil == selFriends || 0 == selFriends.length){
            gameRequestContent.filters = FBSDKGameRequestFilterAppNonUsers;
        }
    }else{
        if([mType isEqualToString: @"2"]){
            gameRequestContent.actionType = FBSDKGameRequestActionTypeAskFor;
            gameRequestContent.objectID = @"426169294209352";
        }else{
            gameRequestContent.actionType = FBSDKGameRequestActionTypeSend;
            gameRequestContent.objectID = @"1043897492291377";
        }
    }
    // Assuming self implements <FBSDKGameRequestDialogDelegate>
    [FBSDKGameRequestDialog showWithContent:gameRequestContent delegate:self];

}

-(void)closeFBSession
{
    [FBSDKProfile enableUpdatesOnAccessTokenChange:YES];
    FBSDKLoginManager *login = [[FBSDKLoginManager alloc] init];
    [login logOut];
}

- (void)openFBSession
{
    if(!FBUtilies::fbIsLogin()){
        NSArray *permissions = [[NSArray alloc] initWithObjects:
                                @"public_profile",@"user_friends",//,@"publish_actions"
                                nil];
        FBSDKLoginManager *login = [[FBSDKLoginManager alloc] init];
        login.loginBehavior = FBSDKLoginBehaviorNative;//FBSDKLoginBehaviorNative  FBSDKLoginBehaviorWeb
        [login logInWithReadPermissions:permissions handler:^(FBSDKLoginManagerLoginResult *result, NSError *error) {
            if (error) {
                fbPreRequest = 0;
                [FBSDKAccessToken setCurrentAccessToken:nil];
                [FBSDKProfile setCurrentProfile:nil];
                NSLog(@"fb login error");
            }else if (result.isCancelled) {
                fbPreRequest = 0;
                [FBSDKAccessToken setCurrentAccessToken:nil];
                [FBSDKProfile setCurrentProfile:nil];
                NSLog(@"fb login Cancell");
            }
            else {
                FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc] initWithGraphPath:@"me"
                                                                               parameters:nil];
                [request startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id meResult, NSError *m_error) {
                    if(m_error){
                        NSLog(@"Result: FBSDKGraphRequest error");
                    }else{
                        NSLog(@"Result: %@", meResult);
                        NSString *facebookId = [meResult valueForKey:@"id"];
                        std::string temp = [facebookId cStringUsingEncoding:NSUTF8StringEncoding];
                        cocos2d::CCString* strTemp = cocos2d::CCString::create(temp);
                        CCUserDefault::sharedUserDefault()->setStringForKey("tmpFaceBook_uid", strTemp->getCString());
                        NSString *facebookName = [meResult valueForKey:@"name"];
                        std::string tempName = [facebookName cStringUsingEncoding:NSUTF8StringEncoding];
                        cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("tmpFaceBook_Name", tempName);
                        cocos2d::CCUserDefault::sharedUserDefault()->flush();
                        NSString *nsmsg =  [FBSDKAccessToken currentAccessToken].tokenString;
                        NSLog(@"Result: %@", nsmsg);
                        // 发送通知
                        cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FBLoginSucess,strTemp);
                        
                        //登陆前想调用的接口
                        if(fbPreRequest == 1){
                            [self getInstallFriends];
                        }
                        if(fbPreRequest == 2){
                            [self inviteFBFriends];
                        }
                        if(fbPreRequest == 3){
                            [self getFBFriendsList];
                        }
                        if(fbPreRequest == 4){
                            [self getAppRequestFriendList];
                        }
                        if(fbPreRequest == 5){
                            if (feedType==1) {
                                if ([[FBSDKAccessToken currentAccessToken] hasGranted:@"publish_actions"]) {
                                    FBUtilies::fbPublishFeedDialog(feedName,feedCaption,feedLinkDescription,feedLink,feedPictureUrl,feedType);
                                }else{
                                    FBSDKLoginManager *lManager = [[FBSDKLoginManager alloc] init];
                                    [lManager logInWithPublishPermissions:@[@"publish_actions"] handler:^(FBSDKLoginManagerLoginResult *result, NSError *error) {
                                        CCLog("fb lManager logInWithPublishPermissions");
                                        if (!error) {
                                            CCLog("fb lManager logInWithPublishPermissions success");
                                            FBUtilies::fbPublishFeedDialog(feedName,feedCaption,feedLinkDescription,feedLink,feedPictureUrl,feedType);
                                        }else{
                                            CCLog("fb lManager error");
                                            feedType = 0;
                                            FBUtilies::fbPublishFeedDialog(feedName,feedCaption,feedLinkDescription,feedLink,feedPictureUrl,feedType);
                                            cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FBFeedDialogResult);
                                        }
                                        
                                    }];
//                                    feedType = 0;
//                                    FBUtilies::fbPublishFeedDialog(feedName,feedCaption,feedLinkDescription,feedLink,feedPictureUrl,feedType);
                                }
                            }else{
                                FBUtilies::fbPublishFeedDialog(feedName,feedCaption,feedLinkDescription,feedLink,feedPictureUrl,feedType);
                            }
                        }
                        fbPreRequest = 0;
                    }
                }];
            }
        }];
    }
}

/**
 * 是否已经成为FB上的fans
 */
- (void) checkIsGameFans_FB
{
//    // 如果已经登录了检测是否是粉丝
//    NSString *fqlQuery = @"SELECT profile_section,type,page_id FROM page_fan WHERE uid = me() and page_id=391336800999382";
//    NSDictionary *params = @{@"q":fqlQuery};
//    
//    FBRequest *request = [[FBRequest alloc] initWithSession:[FBSession activeSession]
//                                                  graphPath:@"/fql"
//                                                 parameters:params
//                                                 HTTPMethod:@"GET"];
//    [request startWithCompletionHandler: ^(FBRequestConnection *connection,
//                                           id result,
//                                           NSError *error) {
//        if (error) {
//            NSLog(@"Error: %@", [error localizedDescription]);
//        } else {
//            NSLog(@"Result: %@", result);
//            NSArray *data = (NSArray *) result[@"data"];
//            if (data.count > 0) {
//                cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("MFBFans",cocos2d::CCBoolean::create(TRUE));
//            }
//        }
//    }];
}

-(void) facebookShare:(NSString*)name
           MsgCaption:(NSString*)caption
               MsgDes:(NSString*)linkDescription
               MsgLink:(NSString*)link
               MsgUrl:(NSString*)pictureUrl
               MsgRef:(NSString*)ref
{
    FBSDKShareLinkContent *content =[[FBSDKShareLinkContent alloc] init];
    content.contentURL = [NSURL URLWithString:link];
    content.contentDescription = linkDescription;
    content.contentTitle = caption;
    content.imageURL = [NSURL URLWithString:pictureUrl];
    content.ref = ref;
    FBSDKShareDialog *shareDialog = [[FBSDKShareDialog alloc] init];
    shareDialog.shareContent = content;
    [shareDialog show];
}

-(void) facebookPushFeed:(NSString*)name
           MsgCaption:(NSString*)caption
               MsgDes:(NSString*)linkDescription
               MsgLink:(NSString*)link
               MsgUrl:(NSString*)pictureUrl
               MsgRef:(NSString*)ref
{
    
    FBSDKAccessToken* tok = [FBSDKAccessToken currentAccessToken];
    if (tok!=nil && ![tok hasGranted:@"publish_actions"]) {
        [self facebookShare:name MsgCaption:caption MsgDes:linkDescription MsgLink:link MsgUrl:pictureUrl MsgRef:ref];
        cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FBFeedDialogResult);
        return;
    }
    NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                   name, @"name",
                                   caption, @"caption",
                                   linkDescription, @"description",
                                   link, @"link",
                                   pictureUrl, @"picture",//@"http://i.imgur.com/g3Qc1HN.png"
                                   ref, @"ref",
                                   nil];
    FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc] initWithGraphPath:@"/me/feed"
                                     parameters:params
                                     HTTPMethod:@"POST"];
    [request startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
                                  if (!error) {
                                      // Link posted successfully to Facebook
                                      NSLog([NSString stringWithFormat:@"result: %@", result]);
                                      if (feedType==1) {
                                          CCUserDefault::sharedUserDefault()->setIntegerForKey(MSG_FBFeedDialogResult,REQUEST_SUCESS);
                                          cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FBFeedDialogResult);
                                      }
                                  } else {
                                      // An error occurred, we need to handle the error
                                      // See: https://developers.facebook.com/docs/ios/errors
                                      NSLog([NSString stringWithFormat:@"%@", error.description]);
                                      NSLog(@"fail result:%@",result);
                                      NSLog(@"fail feed:");
                                  }
                              }];
}

-(void) publishStory
{
    
    //    NSMutableDictionary<FBGraphObject> *object =
    //    [FBGraphObject openGraphObjectForPostWithType:@"object"
    //                                            title:@"Sample Object"
    //                                            image:@"https://s-static.ak.fbcdn.net/images/devsite/attachment_blank.png"
    //                                              url:@"http://samples.ogp.me/226075010839791"
    //                                      description:@""];;
    //
    //    [FBRequestConnection startForPostWithGraphPath:@"me/objects/object"
    //                                       graphObject:object
    //                                 completionHandler:^(FBRequestConnection *connection,
    //                                                     id result,
    //                                                     NSError *error) {
    //                                     // handle the result
    //                                 }];
    
//    NSMutableDictionary<FBGraphObject> *action = [FBGraphObject graphObject];
//    action[@"item"] = @"itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=725163645";
//    
//    [FBRequestConnection startForPostWithGraphPath:@"me/pocket:needfriends"
//                                       graphObject:actions
//                                 completionHandler:^(FBRequestConnection *connection,
//                                                     id result,
//                                                     NSError *error) {
//                                     // handle the result
//                                 }];
    
    //id<FBGraphObject> mealObject =
    //[FBGraphObject openGraphObjectForPostWithType:@"cooking-app:meal"
    //                                        title:@"Lamb Vindaloo"
    //                                        image:@"https://example.com/cooking-app/images/Lamb-Vindaloo.png"
    //                                          url:@"https://example.com/cooking-app/meal/Lamb-Vindaloo.html"
    //                                  description:@"Spicy curry of lamb and potatoes"];
    //
    //id<FBOpenGraphAction> cookAction = (id<FBOpenGraphAction>)[FBGraphObject graphObject];
    //[cookAction setObject:mealObject forKey:@"meal"];
    //
    //[FBDialogs presentShareDialogWithOpenGraphAction:cookAction
    //                                      actionType:@"cooking-app:cook"
    //                             previewPropertyName:@"meal"
    //                                         handler:^(FBAppCall *call, NSDictionary *results, NSError *error) {
    //                                             if(error) {
    //                                                 NSLog(@"Error: %@", error.description);
    //                                             } else {
    //                                                 NSLog(@"Success!");
    //                                             }
    //                                         }];
}

-(void)requestForMe
{
    FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc] initWithGraphPath:@"/me"
                                                                   parameters:nil
                                                                   HTTPMethod:@"GET"];
    [request startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
         if (!error) {
             NSString *facebookId = [result objectForKey:@"id"];
             std::string temp = [facebookId cStringUsingEncoding:NSUTF8StringEncoding];
             cocos2d::CCString* strTemp = cocos2d::CCString::create(temp);
             CCUserDefault::sharedUserDefault()->setStringForKey(FB_REQUEST_USER_ID, strTemp->getCString());
             CCUserDefault::sharedUserDefault()->flush();
             cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(
                                                                                                        FB_REQUEST_USER_ID,strTemp);
         }
     }];
}

-(void)fbInviteDialog:(NSString*) link
{
    FBSDKAppInviteContent *content =[[FBSDKAppInviteContent alloc] initWithAppLinkURL:[NSURL URLWithString:@"https://fb.me/789290781112244?feed_key=ios_appInvite"]];
    [FBSDKAppInviteDialog showWithContent:content
                                 delegate:self];
}

-(void)shareMessage:(NSString*) message
{

//    
//    [FBSDKShareDialog showFromViewController:[EAGLView sharedEGLView]
//                                 withContent:content
//                                    delegate:nil];
    
    NSLog(@"app shareMessage :%@", message);
    NSString *astring = @"fb test!";
    FBSDKShareLinkContent *content =[[FBSDKShareLinkContent alloc] init];
    content.contentURL = [NSURL URLWithString:@"https://fb.me/789290781112244?feed_key=ios_appInvite"];
    content.contentDescription = astring;
    content.contentTitle = [astring copy];
    content.imageURL = [NSURL URLWithString:@"http://cok.eleximg.com/cok/img/fb_share_icon1.png"];
    FBSDKShareDialog *shareDialog = [[FBSDKShareDialog alloc] init];
    shareDialog.shareContent = content;
    [shareDialog show];
    NSLog(@"app shareMessage :%@", message);
}

-(void)sharePhotos:(NSString*) url dialog:(NSString*)dialog
{
    BOOL isInstalled = [[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:@"fb://"]];
    if (!isInstalled) {
        NSLog(@"app sharePhotos not Install");
        if (!dialog) {
            dialog=@"facebook app not install";
        }
        UIAlertView *alter = [[UIAlertView alloc] initWithTitle:@"" message:dialog delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
        [alter show];
        return;
    }
    NSString *imageUrl = url;
    [NSURLConnection sendAsynchronousRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:imageUrl]] queue:[NSOperationQueue mainQueue] completionHandler:^(NSURLResponse *response, NSData *data, NSError *error) {
        UIImage *image = [UIImage imageWithData:data];
        FBSDKSharePhoto *sharePhoto = [FBSDKSharePhoto photoWithImage:image userGenerated:YES];
        FBSDKSharePhotoContent *pContent = [[FBSDKSharePhotoContent alloc] init];
        pContent.photos = @[sharePhoto];
        FBSDKShareDialog *shareDialog = [[FBSDKShareDialog alloc] init];
        shareDialog.shareContent = pContent;
        if(shareDialog.canShow){
            [shareDialog show];
        }else{
            NSLog(@"app sharePhotos can not show");
        }
    }];
}

-(void) facebookPostAction:(NSString*)action
              MsgType:(NSString*)actionType
                  MsgObj:(NSString*)actionObj
{
    NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                   actionObj, actionType,
                                   @"true",@"fb:explicitly_shared",
                                   nil];
    FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc] initWithGraphPath:action
                                                                   parameters:params
                                                                   HTTPMethod:@"POST"];
    [request startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
        if (!error) {
            NSLog([NSString stringWithFormat:@"fb PostActionresult: %@", result]);
        } else {
            NSLog([NSString stringWithFormat:@"%@", error.description]);
            NSLog(@" fb PostActionresult fail result:%@",result);
        }
    }];
}

-(void) facebookPostMethod:(NSString*)method
                   MsgType:(NSString*)objectType
                   MsgObj:(NSString*)objectValue
                   MsgActions:(NSString*)actions
                   MsgActionsValue:(NSString*)actionsValue
                   MsgRef:(NSString*)ref
{
    NSMutableDictionary *params = nil;
    if (actions==nil) {
        params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                  objectValue, objectType,
                  ref,@"ref",
                  @"true",@"fb:explicitly_shared",
                  nil];
    }else{
        params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                  objectValue, objectType,
                  actionsValue, actions,
                  ref,@"ref",
                  "Clash of Kings",@"caption",
                  @"true",@"fb:explicitly_shared",
                  nil];
    }
    FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc] initWithGraphPath:method
                                                                   parameters:params
                                                                   HTTPMethod:@"POST"];
    [request startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
        if (!error) {
            NSLog([NSString stringWithFormat:@"fb facebookPostMethod: %@", result]);
        } else {
            NSLog([NSString stringWithFormat:@"%@", error.description]);
            NSLog(@" fb facebookPostMethod fail result:%@",result);
        }
    }];
}

#pragma mark - FBSDKAppInviteDialogDelegate

- (void)appInviteDialog:(FBSDKAppInviteDialog *)appInviteDialog didCompleteWithResults:(NSDictionary *)results
{
    // Intentionally no-op.
    NSLog(@"app appInviteDialog results:%@", results);
}

- (void)appInviteDialog:(FBSDKAppInviteDialog *)appInviteDialog didFailWithError:(NSError *)error
{
    NSLog(@"app appInviteDialog error:%@", error);
}

#pragma mark - upLoadImage

- (void)upLoadImage:(int) srcCode Index:(int)index Uid:(int)uid {
    playerUid = uid;
    UIImagePickerControllerSourceType sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    if( [UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera])
    {
        switch (srcCode) {
            case 0: //相机
                sourceType = UIImagePickerControllerSourceTypeCamera;
                break;
            case 1: //相册
                sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
                break;
        }
    }
    else
    {
        sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    }
#if (SNS_PLATFORM_TYPE == 0) || (SNS_PLATFORM_TYPE == 2)
    AppController* appDelegate = (AppController*)[UIApplication sharedApplication].delegate;
    //        [appDelegate.viewController
    UIImagePickerController *imagePickerController = [[UIImagePickerController alloc] init];
    imagePickerController.delegate = [AOEUtiliesIOS shared];
    imagePickerController.allowsEditing = YES;
    imagePickerController.sourceType = sourceType;
    if (SysUtilies::isIphone())
    {
        if([[UIDevice currentDevice].systemVersion doubleValue] > 5.0){
            [appDelegate.viewController presentViewController:imagePickerController animated:YES completion:^{}];
        }
        else
        {
            [appDelegate.viewController presentModalViewController:imagePickerController animated:YES];
        }
    }
    else{
        /*
        if(srcCode ==1){
            popoverController = [[UIPopoverController alloc] initWithContentViewController: imagePickerController];
            [popoverController presentPopoverFromRect:CGRectMake(appDelegate.window.frame.size.width, appDelegate.window.frame.size.height/2, 0, 0) inView:appDelegate.window permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
        }
        else{
            if([[UIDevice currentDevice].systemVersion doubleValue] > 5.0){
                [appDelegate.viewController presentViewController:imagePickerController animated:YES completion:^{}];
            }
            else
            {
                [appDelegate.viewController presentModalViewController:imagePickerController animated:YES];
            }
        }
         */
        if([[UIDevice currentDevice].systemVersion doubleValue] > 5.0){
            [appDelegate.viewController presentViewController:imagePickerController animated:YES completion:^{}];
        }
        else
        {
            [appDelegate.viewController presentModalViewController:imagePickerController animated:YES];
        }
    }
    [imagePickerController release];
#endif
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker{
    if([[UIDevice currentDevice].systemVersion doubleValue] > 5.0){
        [picker dismissViewControllerAnimated:YES completion:^{}];
    }
    else
    {
        [picker dismissModalViewControllerAnimated:YES];
    }
    [UIApplication sharedApplication].statusBarHidden = YES;
    cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GetHeadImgUrl, NULL);
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info{
    if([[UIDevice currentDevice].systemVersion doubleValue] > 5.0){
        [picker dismissViewControllerAnimated:YES completion:^{}];
    }
    else
    {
        [picker dismissModalViewControllerAnimated:YES];
    }
    if(popoverController != NULL){
        [popoverController dismissPopoverAnimated:true];
        popoverController = NULL;
    }
    [UIApplication sharedApplication].statusBarHidden = YES;
    UIImage *image = [info objectForKey:UIImagePickerControllerEditedImage];
    UIImage *newImg=[self imageWithImageSimple:image scaledToSize:CGSizeMake(90, 90)];
    [self saveImage:newImg WithName:[NSString stringWithFormat:@"%i_%i%@",playerUid,1,@".jpg"]];
}

- (void)navigationController:(UINavigationController *)navigationController willShowViewController:(UIViewController *)viewController animated:(BOOL)animated
{
    [[UIApplication sharedApplication] setStatusBarHidden:YES];
}


-(UIImage *) imageWithImageSimple:(UIImage*) image scaledToSize:(CGSize) newSize{
    // newSize.height=image.size.height*(newSize.width/image.size.width);
    int w = (image.size.width<image.size.height)?90:image.size.width*(newSize.height/image.size.height);
    int h = (image.size.height<image.size.width)?90:image.size.height*(newSize.width/image.size.width);
    int mx = (image.size.width<image.size.height)?0:(90-image.size.width*(newSize.height/image.size.height))/2;
    int my = (image.size.height<image.size.width)?0:(90-image.size.height*(newSize.width/image.size.width))/2;
    UIGraphicsBeginImageContext(newSize);
    [image drawInRect:CGRectMake(mx, my, w, h)];
    UIImage *newImage=UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return  newImage;
}

- (void)saveImage:(UIImage *)tempImage WithName:(NSString *)imageName

{
    // NSLog(@"===TMP_UPLOAD_IMG_PATH===%@",TMP_UPLOAD_IMG_PATH);
    NSData* imageData = UIImagePNGRepresentation(tempImage);
    
    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES);
    
    NSString* documentsDirectory = [paths objectAtIndex:0];
    
    // Now we get the full path to the file
    
    NSString* fullPathToFile = [documentsDirectory stringByAppendingPathComponent:imageName];
    
    // and then we write it out
    NSString *TMP_UPLOAD_IMG_PATH=@"";
    TMP_UPLOAD_IMG_PATH=fullPathToFile;
    NSArray *nameAry=[TMP_UPLOAD_IMG_PATH componentsSeparatedByString:@"/"];
    NSLog(@"===new fullPathToFile===%@",fullPathToFile);
    NSLog(@"===new FileName===%@",[nameAry objectAtIndex:[nameAry count]-1]);
    
    [imageData writeToFile:fullPathToFile atomically:NO];
    cocos2d::extension::CCSafeObject<CCString> temp = CCString::create([fullPathToFile UTF8String]);
    cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GetHeadImgUrl, temp);
    
}

-(void) sendEvent
{
    //[[self mlike] sendActionsForControlEvents:UIControlEventTouchUpInside];
    [[self mlike] sendActionsForControlEvents:UIControlEventAllTouchEvents];
}

@end

