//
//  MailManagerKey.h
//  IF
//
//  Created by 张彦涛 on 15/8/17.
//
//

#ifndef MailManagerKey_h
#define MailManagerKey_h

 
//channelID 字符串
#define   Mail_ChannelID_System         @"system"
#define   Mail_ChannelID_Studio         @"studio"
#define   Mail_ChannelID_Fight          @"fight"
#define   Mail_ChannelID_Mod            @"mod"
#define   Mail_ChannelID_Resource       @"resource"
#define   Mail_ChannelID_Monster        @"monster"
#define   Mail_ChannelID_Resourcehelp   @"resourcehelp"
#define   Mail_ChannelID_Alliance       @"alliance"
#define   Mail_ChannelID_Message        @"message"//邮件和聊天室集合
#define   Mail_ChannelID_Event          @"event"
#define   Mail_ChannelID_Knight         @"knight"//黑骑士
 

//邮件大类
#define  Mail_BigType_User        0 //玩家邮件，包括个人邮件、聊天室、联盟邮件
#define  Mail_BigType_System      1 //系统邮件，对应后台的"per_sys"
#define  Mail_BigType_Notice      2 //公告邮件，包括更新、公告
#define  Mail_BigType_Studio      3 //COK工作室邮件
#define  Mail_BigType_Fight       4 //战斗邮件
#define  Mail_BigType_Mod         5 //mod邮件
#define  Mail_BigType_Resource    6//资源

//邮件类型

#define  Mail_Self_Send         0 
#define  Mail_User              1 
#define  Mail_System            2 
#define  Mail_Server_Battle_Field   2
#define  Mail_Service           3 
#define  Mail_Battle_Report     4 
#define  Mail_Resource          5 
#define  Mail_Detect            6 
#define  Mail_General_Train     7 
#define  Mail_Detect_Report     8 
#define  Mail_Encamp            9 
#define  Mail_Fresher           10 
#define  Mail_Wounded           11  //收治伤兵
#define  Mail_Digong            12 
#define  All_Service            13 
#define  World_New_Explore      14 
#define  Mail_Sysnotice         15 
#define  Mail_Sysupdate         16 
#define  Mail_Allianceinvaite   17 
#define  Mail_Attackmonster     18 
#define  World_Monster_Special   19 
#define  Mail_Alliance_All      20 
#define  Mail_Resource_Help     21 
#define  Mail_Personal          22 
#define  Mail_Mod_Personal      23 
#define  Mail_Mod_Send          24 
#define  Mail_Allianceapply     25 
#define  Mail_Invite_teleport   26 
#define  Mail_Alliance_Kickout  27 
#define  Mail_Gift              28 
#define  Mail_Donate            29 
#define  Mail_World_boss        30 
#define  Chat_Room              31 
#define  Mail_Activity          32 
#define  Mail_Refuse_All_Apply  33 
#define  Mail_Allinace_Package  34  //联盟礼包
#define  Mail_Allinace_RankChange	 35//联盟等级变化
#define  MAIL_TYPE_COUNT    36   //邮件类型总数,增加邮件需修改

// 世界建筑
#define  Mail_OriginTile   0   
#define  Mail_CityTile   1   
#define  Mail_CampTile   2    // 扎营地
#define  Mail_ResourceTile   3    // 资源
#define  Mail_KingTile   4    // 遗迹
#define  Mail_BattleTile   5    // 塔
#define  Mail_MonsterTile   6    // 地宫
#define  Mail_MonsterRange   7   
#define  Mail_CityRange   8    // 玩家周边
#define  Mail_FieldMonster   9    // 野怪
#define  Mail_Throne   10   // 王座
#define  Mail_ThroneRange   11   // 王座周边
#define  Mail_Trebuchet   12   // 投石机
#define  Mail_TrebuchetRange   13   // 投石机周边
#define  Mail_Tile_allianceArea   14   
#define  Mail_ActBossTile   15   // 活动怪物boss
#define  Mail_Tile_allianceRange   16   // 领地周边16
#define  Mail_ActBossTileRange   17   
#define  Mail_tile_superMine   18   
#define  Mail_tile_superMineRange   19   
#define  Mail_tile_tower   20   
#define  Mail_tile_wareHouse   21   
#define  Mail_tile_wareHouseRange   22   
#define  Mail_tile_banner   23    //联盟国旗

#endif /* MailManagerKey_h */


