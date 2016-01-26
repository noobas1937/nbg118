//
//  MailDataManager.m
//  IF
//
//  Created by 马纵驰 on 15/11/10.
//
//

#import "MailDataManager.h"
#import "ChatChannel.h"
#import "MonsterMailData.h"
#import "BattleMailData.h"
#import "DetectReportMailData.h"
#import "OcupyMailData.h"
#import "WorldExploreMailData.h"
#import "AllianceInviteMailData.h"
#import "AllianceApplyMailData.h"
#import "ResourceHelpMailData.h"
#import "InviteTeleportMailData.h"
#import "AllianceKickOutMailData.h"
#import "WorldBossMailData.h"
#import "RefuseAllReplyMailData.h"
@implementation MailDataManager

+(MailData *)mailDataWithMailInfoData:(MailInfoDataModel *)mode{
    MailData *data = nil;
    switch(mode.type)
    {
        case Mail_Battle_Report:
            data=[[BattleMailData alloc] initWithAddMailData:mode];
            break;
        case Mail_Resource:
            data=[[ResourceMailData alloc] initWithAddMailData:mode];
            break;
        case Mail_Detect_Report:
            data=[[DetectReportMailData alloc] initWithAddMailData:mode];
            break;
        case Mail_Encamp:
            data=[[OcupyMailData alloc] initWithAddMailData:mode];
            break;
        case World_New_Explore:
            data=[[WorldExploreMailData alloc] initWithAddMailData:mode];
            break;
        case Mail_Allianceinvaite:
            data=[[AllianceInviteMailData alloc] initWithAddMailData:mode];
            break;
        case Mail_Allianceapply:
            data=[[AllianceApplyMailData alloc] initWithAddMailData:mode];
            break;
        case Mail_Attackmonster:
            data=[[MonsterMailData alloc] initWithAddMailData:mode];
            break;
        case Mail_Resource_Help:
            data=[[ResourceHelpMailData alloc] initWithAddMailData:mode];
            break;
        case Mail_Invite_teleport:
            data=[[InviteTeleportMailData alloc] initWithAddMailData:mode];
            break;
        case Mail_Alliance_Kickout:
            data=[[AllianceKickOutMailData alloc] initWithAddMailData:mode];
            break;
        case Mail_World_boss:
            if([mode isWorldBossKillRewardMail]){
                data = [[MailData alloc] initWithAddMailData:mode];
                [data parseContents];
                [data parseMode2Json];
            }else
                data=[[WorldBossMailData alloc] initWithAddMailData:mode];
            break;
        case Mail_Refuse_All_Apply:
            data=[[RefuseAllReplyMailData alloc] initWithAddMailData:mode];
            break;
        default:
            data=[[MailData alloc] initWithAddMailData:mode];
            [data parseContents];
            [data parseMode2Json];
            break;
    }
    return data;
}

+(MailData *)maildataWIthMailID:(NSString *)vMailID{
    MailInfoDataModel *tempMail =  [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:[MailInfoDataModel getTableName]  andWithModelClass:[MailInfoDataModel class] where:[NSString stringWithFormat:@"ID ='%@'",vMailID] orderBy:@"_id desc"];
    if (tempMail) {
       return  [MailDataManager mailDataWithMailInfoData:tempMail];
    }else{
        return nil;
    }
}
@end
