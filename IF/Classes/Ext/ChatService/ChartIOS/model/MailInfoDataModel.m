//
//  MailInfoDataModel.m
//  IF
//
//  Created by 张彦涛 on 15/8/17.
//
//

#import "MailInfoDataModel.h"
#import "UserManager.h"
 

@implementation MailInfoDataModel

-(void)mailInfoDataSettingTabType{
    if (self.type == Mail_Battle_Report || self.type == Mail_Detect || self.type == Mail_Detect_Report || self.type == Mail_Encamp || self.type == Mail_World_boss
        ) {
        self.tabType = Mail_BigType_Fight;
    }else if (self.type == All_Service ){
        self.tabType = Mail_BigType_Studio;
    }else if (self.type == Mail_Fresher || self.type == Mail_Sysnotice || self.type == Mail_Sysupdate ){
        self.tabType = Mail_BigType_Notice;
    }else if (self.type <= Mail_User || self.type == Mail_Alliance_All || self.type == Chat_Room){
        self.tabType = Mail_BigType_User;
    }else if (self.type == Mail_Mod_Send || self.type == Mail_Mod_Personal){
        if ([UserManager sharedUserManager].currentUser.mGmod == 2 || [UserManager sharedUserManager].currentUser.mGmod == 5) {
            self.tabType = Mail_BigType_Mod;
        }else{
            self.tabType = Mail_BigType_User;
        }
    }
    else{
        self.tabType = Mail_BigType_System;
    }
 
}


-(void)mailInfoDataSettingChannelID{
    int switchKey = self.tabType;
    switch (switchKey) {
        case Mail_BigType_User:{
            
            if (self.type == Mail_Mod_Personal) {
                self.channelID = [NSString stringWithFormat:@"%@@mod",self.fromUserUid];
                
            }else{
                self.channelID = self.fromUserUid;
            }
        }
            break;
        case Mail_BigType_System:{
            self.channelID = Mail_ChannelID_System;
        }
            break;
        case Mail_BigType_Notice:{
            self.channelID = Mail_ChannelID_Notice;
        }
            break;
        case Mail_BigType_Studio:{
            self.channelID = Mail_ChannelID_Studio;
        }
            break;
        case Mail_BigType_Fight:{
            self.channelID = Mail_ChannelID_Fight;
        }
            break;
        case Mail_BigType_Mod:{
            self.channelID = Mail_ChannelID_Mod;
        }
            break;
        default:
            break;
    }
 
    if (self.type == Mail_Resource) {
        self.channelID = Mail_ChannelID_Resource;
    }else if (self.type == Mail_Attackmonster){
        self.channelID = Mail_ChannelID_Monster;
    }else if (self.type == Mail_Resource_Help){
        self.channelID = Mail_ChannelID_Resourcehelp;
    }
}
@end
