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
#define   Mail_ChannelID_Notice         @"notice"
#define   Mail_ChannelID_Studio         @"studio"
#define   Mail_ChannelID_Fight          @"fight"
#define   Mail_ChannelID_Mod            @"mod"
#define   Mail_ChannelID_Resource       @"resource"
#define   Mail_ChannelID_Monster        @"monster"
#define   Mail_ChannelID_Resourcehelp   @"resourcehelp"

//邮件大类
#define  Mail_BigType_User        0 //玩家邮件，包括个人邮件、聊天室、联盟邮件
#define  Mail_BigType_System      1 //系统邮件，对应后台的"per_sys"
#define  Mail_BigType_Notice      2 //公告邮件，包括更新、公告
#define  Mail_BigType_Studio      3 //COK工作室邮件
#define  Mail_BigType_Fight       4 //战斗邮件
#define  Mail_BigType_Mod         5 //mod邮件


//邮件类型

#define  Mail_Self_Send         0 
#define  Mail_User              1 
#define  Mail_System            2 
#define  Mail_Service           3 
#define  Mail_Battle_Report     4 
#define  Mail_Resource          5 
#define  Mail_Detect            6 
#define  Mail_General_Train     7 
#define  Mail_Detect_Report     8 
#define  Mail_Encamp            9 
#define  Mail_Fresher           10 
#define  Mail_Wounded           11 
#define  Mail_Digong            12 
#define  All_Service            13 
#define  World_New_Explore      14 
#define  Mail_Sysnotice         15 
#define  Mail_Sysupdate         16 
#define  Mail_Allianceinvaite   17 
#define  Mail_Attackmonster     18 
#define  Mail_Monster_Special   19 
#define  Mail_Alliance_All      20 
#define  Mail_Resource_Help     21 
#define  Mail_Personal          22 
#define  Mail_Mod_Personal      23 
#define  Mail_Mod_Send          24 
#define  Mail_Allianceapply     25 
#define  Mail_Invite_teleport   26 
#define  Mail_Alliance_kickout  27 
#define  Mail_Gift              28 
#define  Mail_Donate            29 
#define  Mail_World_boss        30 
#define  Chat_Room              31 
#define  Mail_Activity          32 
#define  Mail_Refuse_All_Apply  33 


#define  MAIL_TYPE_COUNT    34 



#endif /* MailManagerKey_h */


