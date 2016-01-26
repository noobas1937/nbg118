//
//  MailNewUI.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "MailNewUI.h"

@implementation MailNewUI

-(instancetype)init
{
    self = [super init];
    if (self) {
        self.iosNewUIMap = [[NSMutableDictionary alloc] init];
        [self settingIosNewUIMap];
    }
    return self;
}

+ (MailNewUI *)sharedMailNewUI
{
    static MailNewUI *sharedMailNewUI = nil;
    static dispatch_once_t predicate;
    dispatch_once(&predicate, ^{
        sharedMailNewUI = [[self alloc] init];
    });
    return sharedMailNewUI;
}

-(void) settingIosNewUIMap
{
    [self iosNewUIMapKVreverseByK:@"CHANNEL_ICON_FIGHT" byV:@"mail_list_main_icon_battle"];
    [self iosNewUIMapKVreverseByK:@"CHANNEL_ICON_STUDIO" byV:@"mail_list_main_icon_studio"];
    [self iosNewUIMapKVreverseByK:@"CHANNEL_ICON_SYSTEM" byV:@"mail_list_main_icon_system"];
    [self iosNewUIMapKVreverseByK:@"CHANNEL_ICON_ALLIANCE" byV:@"mail_list_main_icon_invite"];
    [self iosNewUIMapKVreverseByK:@"CHANNEL_ICON_MOD" byV:@"mail_list_main_icon_mod"];
    [self iosNewUIMapKVreverseByK:@"CHANNEL_ICON_MESSAGE" byV:@"mail_list_main_icon_chat"];
    [self iosNewUIMapKVreverseByK:@"CHANNEL_ICON_ANNOUNCEMENT" byV:@"mail_list_main_icon_announcement"];
    [self iosNewUIMapKVreverseByK:@"CHANNEL_ICON_RESOURCE" byV:@"mail_list_main_icon_resource"];
    [self iosNewUIMapKVreverseByK:@"CHANNEL_ICON_MONSTER" byV:@"mail_list_main_icon_monster"];
    [self iosNewUIMapKVreverseByK:@"CHANNEL_ICON_EXPLORE" byV:@"mail_list_main_icon_explore"];
    [self iosNewUIMapKVreverseByK:@"CHANNEL_ICON_FAVORITE" byV:@"mail_list_main_icon_favorite"];
    [self iosNewUIMapKVreverseByK:@"CHANNEL_ICON_EVENT" byV:@"mail_list_main_icon_favorite"];
    
    
    //新邮件UI邮件ICON
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_ANNOUNCEMENT" byV:@"mail_list_icon_announcement"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_BATTLE_EXPLORE" byV:@"mail_list_icon_battle_explore"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_BATTLE_CAMP" byV:@"mail_list_icon_battle_camp"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_BATTLE_CAPTURE" byV:@"mail_list_icon_battle_capture"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_BATTLE_CITY_DEFENT_FAILURE" byV:@"mail_list_icon_battle_city_defent_failure"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_BATTLE_CITY_DEFENT_VICTORY" byV:@"mail_list_icon_battle_city_defent_victory"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_BATTLE_CITY_FAILURE" byV:@"mail_list_icon_battle_city_failure"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_BATTLE_CITY_VICTORY" byV:@"mail_list_icon_battle_city_victory"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_BATTLE_DETECT" byV:@"mail_list_icon_battle_detect"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_BATTLE_KNIGHT" byV:@"mail_list_icon_battle_knight"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_BATTLE_MONSTER" byV:@"mail_list_icon_battle_monster"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_BATTLE_REPORT" byV:@"mail_list_icon_battle_report"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_INVITE_JOIN_ALLIANCE" byV:@"mail_list_icon_invite_join_alliance"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_INVITE_JOIN" byV:@"mail_list_icon_invite_join"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_INVITE_KICKEDOUT" byV:@"mail_list_icon_invite_kickedout"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_INVITE_MOVE" byV:@"mail_list_icon_invite_move"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_INVITE_REJECTED" byV:@"mail_list_icon_invite_rejected"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_STUDIO" byV:@"mail_list_icon_studio"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_REWARD" byV:@"mail_list_icon_reward"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_SEARCH" byV:@"mail_list_icon_search"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_ID_BINGING" byV:@"mail_list_icon_id_binding"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_SYSTEM" byV:@"mail_list_icon_system_other"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_SYSTEM_VIP" byV:@"mail_list_icon_system_vip"];
    [self iosNewUIMapKVreverseByK:@"MAIL_ICON_CHAT_ROOM" byV:@"mail_pic_flag_31"];
}

-(void) iosNewUIMapKVreverseByK:(NSString*) k byV:(NSString*) v
{
    [self.iosNewUIMap setObject:v forKey:k];
}

-(NSString *)gettingIconByName:(NSString *)name
{
    if(self.iosNewUIMap && name.length > 0 && [self.iosNewUIMap objectForKey:name])
        return [self.iosNewUIMap objectForKey:name];
    return @"";
}

@end
