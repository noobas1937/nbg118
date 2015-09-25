//
//  NSUserInfo.h
//  IF
//
//  Created by mzc on 15/6/25.
//
//

#import <Foundation/Foundation.h>

typedef enum : NSUInteger {
    ChooseState_normal = 0,
    ChooseState_pressed = 1,
    ChooseState_selected = 2,
    
} ChooseState;


@interface NSUserInfo : NSObject

/**数据库对应*/

@property (nonatomic,assign) int        tableVer;
@property (nonatomic,strong) NSString   *uid;                               // uid，群聊时才会存数据库
@property (nonatomic,strong) NSString   *userName;
@property (nonatomic,strong) NSString   *allianceId;
@property (nonatomic,strong) NSString   *asn;                               // 联盟简称，显示在()里面的
@property (nonatomic,assign) int        allianceRank;                       // 决定禁言权限，只有本玩家有
@property (nonatomic,assign) int        serverId;                           // 国家号，只有本玩家有
@property (nonatomic,assign) int        crossFightSrcServerId;              // 跨服战时的原服id，若为-1表示没有跨服
@property (nonatomic,assign) int        type;                               // 玩家类型，尚未使用；会被C++初始化且不赋初值
@property (nonatomic,strong) NSString   *headPic;
@property (nonatomic,assign) int        headPicVer;                         // 头像id
/*
 *
 *当为mod时名字颜色为(65,119,41)，如果为gm时名字颜色为(33,111,169),否则名字颜色为(111,26,0)
 *mod为"2"、"4"、"5"时对应的图片分别为mod.png、tmod.png、smod.png，gm为3时对应的图片为gm.png，其他的不显示mod和gm
 *gm和mod信息，如果为"2"、"4"、"5"表示为mod，如果为“3”表示为gm
 *
 */
@property (nonatomic,assign) int        mGmod;
@property (nonatomic,assign) int        vipLevel;                           // vip等级，至少为1，由vip points决定，只升不降
@property (nonatomic,strong) NSString   *vip;
@property (nonatomic,assign) int        vipEndTime;                         // vip时间，单位为s，有时区，过期则vip暂时失效（等级保留）
@property (nonatomic,assign) int        lastUpdateTime;                     // 上次更新时间
@property (nonatomic,assign) int        lastChatTime;                       // 上次聊天时间

//运行时
@property (nonatomic,assign) BOOL isSelected;
@property (nonatomic,assign) BOOL isDummy;
@property (nonatomic,assign) ChooseState chooseState;                       //当前的勾选状态
@property (nonatomic,assign) int sex;
 

-(instancetype) init:(NSString*) usdStr;
-(NSString*) gettingVipInfo;

-(void) initNullField;

/**创建联盟数据缓存*/
-(void) settingAllianceId:(NSString*) allianceId;

-(int) gettingHeadPicVer;
/**自定义头像是否存在*/
-(BOOL) isCustomHeadImage;
/**获取自定义头像url*/
-(NSString*) gettingCustomHeadPicUrl;
/**获取自定义头像地址*/
-(NSString*) gettingCustomHeadPicPath;
/**自定义头像是否在本地存在*/
-(BOOL) isCustomHeadPicExist_fun;
-(NSString *)gettingVipString;//组合vip字符串信息

@end
