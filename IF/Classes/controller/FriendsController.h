//
//  FriendsController.h
//  IF
//
//  Created by xxrdsg on 15-9-29.
//
//

#ifndef __IF__FriendsController__
#define __IF__FriendsController__

#include "CommonInclude.h"

//about local
#define APPLYING_UID_SEEN "APPLYING_UID_SEEN"
#define FRIEND_I_APPLYING_GROUP "FRIEND_I_APPLYING_GROUP"
#define FRIEND_APPLYING_ME_GROUP "FRIEND_APPLYING_ME_GROUP"
#define FRIEND_STAR_GROUP "FRIEND_STAR_GROUP"
#define FRIEND_PUTONG_GROUP "FRIEND_PUTONG_GROUP"

#define HAS_NEW_FRIEND_APPLY "HAS_NEW_FRIEND_APPLY"
#define NEW_FRIENDS_TIPS_SHOW "NEW_FRIENDS_TIPS_SHOW"

#define DELETE_FRIEND_SUC "DELETE_FRIEND_SUC"
#define ACCEPT_FRIEND_APPLY_SUC "ACCEPT_FRIEND_APPLY_SUC"
#define GET_FRIEND_DATA_SUC "GET_FRIEND_DATA_SUC"
#define APPLY_FRIEND_SEND_SUC "APPLY_FRIEND_SEND_SUC"
#define FRIEND_DESC_CHANGE_SUC "FRIEND_DESC_CHANGE_SUC"
#define UP_FRIEND_STAR_SUC "UP_FRIEND_STAR_SUC"
#define DOWN_FRIEND_NORMAL_SUC "DOWN_FRIEND_NORMAL_SUC"
#define REJECT_APPLY_SUC "REJECT_APPLY_SUC"

#define RECEIVE_MY_APPLY_ACCEPTED_PUSH "RECEIVE_MY_APPLY_ACCEPTED_PUSH"
#define RECEIVE_APPLY_PUSH "RECEIVE_APPLY_PUSH"
#define RECEIVE_ME_DELETED_PUSH "RECEIVE_ME_DELETED_PUSH"

enum FriendRelation{
    FRIEND_NONE, //啥都不是 0
    FRIEND_FRIEND, //好友关系 1
    FRIEND_FAVO, //星标好友（单向）2
    FRIEND_APPLYING, //申请中 3 我请求加他为好友
    FRIEND_AUDITING, //审核申请中 4 请求加我为好友
    FRIEND_FACEBOOK, //facebook好友 5
};

class FriendInfo: public CCObject
{
public:
//    NONE, //啥都不是 0
//    FRIEND, //好友关系 1
//    FAVO, //星标好友（单向）2
//    APPLYING, //申请中 3
//    AUDITING; //审核申请中 4

    void updateInfo(CCDictionary* dict);
    static FriendInfo* create();
    static FriendInfo* create(CCDictionary* dict);
    FriendInfo(CCDictionary* dict);
    FriendInfo(){};
    
    string pic;
    int picVer;
    int serverId;
    int relation;
    double vipEndTime;
    string allianceId;
    int vipLevel;
    string uid;
    int crossFightSrcServerId;
    double offLineTime;
    int gmFlag;
    string name;
    int online;
    int rank;
    double power;
    string abbr;
    string lang;
    int mainBuildingLevel;
    string description;
    
    string fbUid;
    string fbName;
    bool isNew;
};

#pragma mark FriendSearchInfo
class FriendSearchInfo: public CCObject
{
public:    
    void updateInfo(CCDictionary* dict,int type,bool open =false,int num =0,int onNum=0);
    static FriendSearchInfo* create(CCDictionary* dict,int type,bool open =false,int num =0,int onNum=0);
    FriendSearchInfo(CCDictionary* dict,int type,bool open =false,int num =0,int onNum=0);
    FriendSearchInfo(){};
    
    string pic;
    int picVer;
    int serverId;
    int relation;
    double vipEndTime;
    string allianceId;
    int vipLevel;
    string uid;
    int crossFightSrcServerId;
    double offLineTime;
    int gmFlag;
    string name;
    int online;
    int rank;
    double power;
    string abbr;
    string lang;
    int mainBuildingLevel;//mainCityLv
    string description;
    
    //alliance
    double jointime;
    int armyKill;
    int pointId;
    int fight;
    //search
    string alliancename;
    int level;
    
    int cellType;
    bool cellOpen;
    int cellNum;
    int cellOnNum;
};

class FriendsController: public CCObject
{
public:
    static void purgeData();
    static FriendsController* getInstance();
    FriendsController();
    ~FriendsController(){};
    
    void handleResponse(CCDictionary* dic);
    
    void startGetFriendsData();
    void endGetFriendsData(CCDictionary * dic);
    
    void startApplyFriend(string uid);
    void endApplyFriend(CCDictionary* dic);
    
    void startDeleteFriend(string uid);
    void endDeleteFriend(CCDictionary * dic);
    
//    void startGetApplyingIds();
//    void endGetApplyingIds(CCDictionary* dict);
    
    void startAcceptApply(string uid);
    void endAcceptApply(CCDictionary* dict);
    
    void startUpToStarFriend(string uid);
    void endUpToStarFriend(CCDictionary* dic);
    
    void startFBUpToStarFriend(string uid,string fbUid,string fbName);
    void endFBUpToStarFriend(CCDictionary* dic);
    
    void startDownToNormalFriend(string uid);
    void endDownToNormalFriend(CCDictionary* dic);
    
    void startChangeDescription(string uid, string des);
    void endChangeDescription(CCDictionary* dic);
    
    void startRejectApply(string uid);
    void endRejectApply(CCDictionary* dic);
    //按uid搜索Facebook好友
    void startGetFbFriends(string uids);
    void endGetFbFriends(CCDictionary* dict);
    
    void delInfoFromApplySeenLocal(string uid);
    void addInfoToApplySeenLocal(string uid);
    bool bHasInfoInApplySeenLocal(string uid);
    
    void delInfoFromApplyByMeLocal(string uid);
    void addInfoToApplyByMeLocal(string uid);
    bool bHasInfoInApplyByMeLocal(string uid);
    
    bool isMyFriend(string uid);
    bool isMyPutongFriend(string uid);
    bool isMyPutongFBFriend(string uid);
    bool isMyStarFriend(string uid);
    bool isApplyingMe(string uid);
    bool isBeingAppliedByMe(string uid);
    int getContactState(string uid);
    bool isStranger(string uid);
    
    void checkHasNewApplyForMe();
    void resetLocalSeenApply();
    
    void getNumByGroup(map< int, pair<int, int> >& mp); // <int, int> 在线 总数
    bool isfbUidAddToFamily(string fbuid);
    void DelFBPutongFriendToFB();
    
    
    CCSafeObject<CCDictionary> m_data;
    CCSafeObject<CCDictionary> m_fbData;
    CCSafeObject<CCDictionary> m_appliedData;
    
    bool m_isHasFriendsViewPop;
    bool m_isDataInit;
    bool m_bOpen;
    
    bool isFaceBookOpen();
    string fbSwitch;
    map<string, string> fbInfo;
    
    map<string, string> lastMailMap;
protected:
private:
};

#endif /* defined(__IF__FriendsController__) */
