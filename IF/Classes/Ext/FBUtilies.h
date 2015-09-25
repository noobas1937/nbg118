
#ifndef __FBUtilies__
#define __FBUtilies__

#ifdef __OBJC__
#import <UIKit/UIKit.h>
@interface AOEUtiliesIOS:UIViewController
@end
#endif

#include <string.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCTypesExt.h"
#include "CCDevice.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#   include <jni.h>
#   include "platform/android/jni/JniHelper.h"
#endif
/** 收藏信息的分割符 */
static const char* AOE_COLLECTION_DELIMITER = "~^";
namespace FBUtilies{
#pragma mark Platform independent

    // 取得FACEBOOK奖励的key
    const char* getFacebookRewardKey();
    void getFriendsData();
#pragma mark - Platform depends
    
#pragma mark Both
    //facebook
    bool fbLogin();//改为有返回，是为了判断android 3.0以 下 login fb能不能登录的问题
    bool fbIsLogin();
    void fbLogout();
    void getFBFriendList();
    void postFBSelectedFriendList(cocos2d::CCArray* arr,const std::string &msg,const std::string &ftype="0");// 0 request 1 bule 2 为askfor
    void fbShowFansWall();
    
    void postFeed_FB(const std::string &msg);
    void postFeed_email(const std::string &msg, const std::string &subject);
    void postFeed_sina(const std::string &msg);
    void postFeed_market();
    void fbPublishFeedDialog(const std::string& name,const std::string& caption, const std::string& linkDescription,const std::string& link,const std::string& pictureUrl,int feedType=0,const std::string& ref="");
    void fbLike();
    void removeLike();
    void inviteFriends();
    void getFBInstallFriends();
    void requestForMeInfo();//取自己的信息，防止app 和绑定的号不一致
    void getAppRequestList();//取得邀请自己的好友信息，用于建立好友关系
    void appInvite(const std::string &appLink);// app request dialog
    void fbShareMessage(const std::string &msg);
    void fbSharePhotosByUrl(const std::string &url,const std::string &dialog);//分享图片
    bool fbHasGranted(const std::string &permission);//判断是否有权限
    void checkIsGameFans_FB();
    void appEventPurchaseItem(int type,int user_level, int castle_level,int item_id,const std::string& item_name,int item_price,int item_count,int spend,const std::string entrance);
    void fbPostAction(const std::string &action,const std::string &actionT,const std::string &actionObj);// post a action
    void appEventShareLog(const std::string &realm,int user_level,int castle_level,const std::string &share_content);
    void appEventFBEntrance(const std::string &entrance);
    void appEventSpeedUp(int user_castle,int user_level,int type,int spend);
    //entracnce 弹窗，点击限时抢购，点击金币  name礼包名称（金币礼包，限时礼包，月度礼包）
    void appEventGiftPackage(const std::string &entracnce,const std::string &name,int id,int user_castle,int user_level);
    void appEventHireWorker(int user_castle,int user_level);
    void appEventLevelUp(int user_level_up);
    void appEventAllianceHonorExchange(const std::string &name,int id,int user_castle,int user_level,int rank);
    void appEventAllianceScoreUsage(const std::string &name,int id,int rank);
    void appEventCityEffectLog(int cityLV,int playerLv);
    
    //FBUtilies::fbPostMethod("feed", "link", "http://cok.elex.com/opengraph/opengraph.php?id=treasure_help&feedback=treasure_help", "actions", "{ \"name\":\"hi test\", \"link\":\"http://cok.elex.com/opengraph/opengraph.php?id=common&feedback=call_for_help\"}", "cal_for_hp");
    void fbPostMethod(const std::string &method,const std::string &objectType,const std::string &objectValue,const std::string &actions,const std::string &actionsValue,const std::string &ref);// 动态调用facebook graph里的方法
};
namespace SysUtilies
{
    void onUploadPhoto(int uid, int srcCode, int idx);
    
    bool isIphone();
    const char* getDevicePlatformInfo();
//    int playerUid;
};

#endif 
