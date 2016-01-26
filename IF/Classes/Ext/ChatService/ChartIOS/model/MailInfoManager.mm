//
//  MailInfoManager.m
//  IF
//
//  Created by 张彦涛 on 15/8/17.
//
//

#import "MailInfoManager.h"
#import <string.h>
#import "GlobalData.h"
#import "ResourceMailData.h"
#import "ChatServiceController.h"
#import "BattleMailData.h"
#import "MailController.h"
#import "ChatServiceCocos2dx.h"
#import "MailNewUI.h"
 
#import "DetectReportMailData.h"
#import "OcupyMailData.h"
#import "WorldExploreMailData.h"
#import "AllianceInviteMailData.h"
#import "AllianceApplyMailData.h"
#import "MonsterMailData.h"
#import "ResourceHelpMailData.h"
#import "InviteTeleportMailData.h"
#import "AllianceKickOutMailData.h"
#import "WorldBossMailData.h"
#import "RefuseAllReplyMailData.h"
#import "CSCommandManager.h"


#define cString(a) [NSString stringWithUTF8String:a]
 
//#import "ResourceHelpMailData.h"
 

@implementation MailInfoManager

-(instancetype)init
{
    self = [super init];
    if (self) {

    }
    return self;
}

+ (MailInfoManager *)sharedMailInfoManager
{
    static MailInfoManager *sharedMailInfoManager = nil;
    static dispatch_once_t predicate;
    dispatch_once(&predicate, ^{
        sharedMailInfoManager = [[self alloc] init];
    });
    return sharedMailInfoManager;
}

-(NSString *)gettingMailIconByName:(NSString *)name
{
 
    return [[MailNewUI sharedMailNewUI] gettingIconByName:name];
 
}

+(MailInfoDataModel *)maildataForOnlinePushWithCCDictionary:(CCDictionary *)vDic{
    //    CCDictElement * pElement;
    //    CCDICT_FOREACH(vDic, pElement)
    //
    //    {
    //
    //        const char * key = pElement->getStrKey();
    //
    //        CCString * value = (CCString *)pElement->getObject();
    //
    //        CCLog(key);
    //
    //        CCLog(value ->getCString());
    //        
    //    }
    MailInfoDataModel *maildata= [[MailInfoDataModel alloc]init];
    maildata.mail_ID = cString(vDic -> valueForKey("uid")->getCString());
    
    maildata.type  = vDic->valueForKey("type")->intValue();
    //    maildata.fromUserUid = cString(vDic->valueForKey("formUid")->getCString());
    maildata.fromName = cString(vDic ->valueForKey("fromName")->getCString());
    maildata.fromUserUid = cString(vDic ->valueForKey("fromUser")->getCString());
    maildata.title = cString(vDic ->valueForKey("title")->getCString()) ;
    maildata.contents   = cString(vDic ->valueForKey("contentsLocal")->getCString());
    
    maildata.rewardId   = cString(vDic ->valueForKey("rewardId")->getCString());
    maildata.itemIdFlag = vDic->valueForKey("itemIdFlag")->intValue();
    maildata.status  = vDic->valueForKey("status")->intValue();;
    
    maildata.rewardStatus =  vDic->valueForKey("rewardStatus")->intValue();
    maildata.saveFlag = vDic->valueForKey("save")->intValue();
    maildata.creatTime = vDic->valueForKey("createTime")->doubleValue() / 1000;
    maildata.reply = vDic->valueForKey("reply")->intValue() ;
    maildata.channelID =  [maildata mailInfoDataForChannelWithSelf];
    
        return maildata;
}
+(MailInfoDataModel *)maildataWithCCDictionary:(CCDictionary *)vDic{
    
//    CCDictElement * pElement;
//    CCDICT_FOREACH(vDic, pElement)
//    
//    {
//        
//        const char * key = pElement->getStrKey();
//        
//        CCString * value = (CCString *)pElement->getObject();
//        
//        CCLog(key);
//        
//        CCLog(value ->getCString());
//        
//    }
//    
 

     MailInfoDataModel *maildata= [[MailInfoDataModel alloc]init];
    maildata.mail_ID = cString(vDic -> valueForKey("uid")->getCString());
  
     maildata.type  = vDic->valueForKey("type")->intValue();
//    maildata.fromUserUid = cString(vDic->valueForKey("formUid")->getCString());
    maildata.fromName = cString(vDic ->valueForKey("fromName")->getCString());
    maildata.fromUserUid = cString(vDic ->valueForKey("fromUser")->getCString());
    maildata.title = cString(vDic ->valueForKey("title")->getCString()) ;
    maildata.contents   = cString(vDic ->valueForKey("contents")->getCString());

    maildata.rewardId   = cString(vDic ->valueForKey("rewardId")->getCString());
    maildata.itemIdFlag = vDic->valueForKey("itemIdFlag")->intValue();
    maildata.status  = vDic->valueForKey("status")->intValue();;
   
    maildata.rewardStatus =  vDic->valueForKey("rewardStatus")->intValue();
    maildata.saveFlag = vDic->valueForKey("save")->intValue();
    maildata.creatTime = vDic->valueForKey("createTime")->doubleValue() / 1000;
    maildata.reply = vDic->valueForKey("reply")->intValue() ;
    maildata.channelID =  [maildata mailInfoDataForChannelWithSelf];
//    [maildata mailInfoDataSettingTabType];
//    [maildata mailInfoDataSettingChannelID];
    
//    CCArray *tempContentArr = vDic ->valueForKey("contentsArr");
    
    /**
     *  解析系统邮件
    
//    MailData *mail = nil;
//    switch(maildata.type)
//    {
//        case Mail_Battle_Report:
//            mail=[[BattleMailData alloc] initWithAddMailData:maildata];
//            break;
//        case Mail_Resource:
//            mail=[[ResourceMailData alloc] initWithAddMailData:maildata];
//            break;
//        case Mail_Detect_Report:
//            mail=[[DetectReportMailData alloc] initWithAddMailData:maildata];
//            break;
//        case Mail_Encamp:
//            mail=[[OcupyMailData alloc] initWithAddMailData:maildata];
//            break;
//        case World_New_Explore:
//            mail=[[WorldExploreMailData alloc] initWithAddMailData:maildata];
//            break;
//        case Mail_Allianceinvaite:
//            mail=[[AllianceInviteMailData alloc] initWithAddMailData:maildata];
//            break;
//        case Mail_Allianceapply:
//            mail=[[AllianceApplyMailData alloc] initWithAddMailData:maildata];
//            break;
//        case Mail_Attackmonster:
//            mail=[[MonsterMailData alloc] initWithAddMailData:maildata];
//            break;
//        case Mail_Resource_Help:
//            mail=[[ResourceHelpMailData alloc] initWithAddMailData:maildata];
//            break;
//        case Mail_Invite_teleport:
//            mail=[[InviteTeleportMailData alloc] initWithAddMailData:maildata];
//            break;
//        case Mail_Alliance_Kickout:
//            mail=[[AllianceKickOutMailData alloc] initWithAddMailData:maildata];
//            break;
//        case Mail_World_boss:
//            if([maildata isWorldBossKillRewardMail])
//                mail = [[MailData alloc] initWithAddMailData:maildata];
//            else
//                mail=[[WorldBossMailData alloc] initWithAddMailData:maildata];
//            break;
//        case Mail_Refuse_All_Apply:
//            mail=[[RefuseAllReplyMailData alloc] initWithAddMailData:maildata];
//            break;
//        default:
//            mail=[[MailData alloc] initWithAddMailData:maildata];
//            break;
//    }
     //    maildata.maildata = mail;
      */
    

    
    return maildata;
}


+(MailInfoDataModel *)maildataWithMailInfo:(MailInfo *)vMailInfo{
    MailInfoDataModel *maildata= [[MailInfoDataModel alloc]init];
    maildata.mail_ID = [NSString stringWithUTF8String:vMailInfo->uid.c_str()];
    maildata.rewardId = [NSString stringWithUTF8String:vMailInfo->rewardId.c_str()];
    maildata.fromUserUid = [NSString stringWithUTF8String:vMailInfo->fromUid.c_str()];
    maildata.fromName = [NSString stringWithUTF8String:vMailInfo->fromName.c_str()];
    maildata.title = [NSString stringWithUTF8String:vMailInfo->title.c_str()];
    maildata.creatTime =  vMailInfo->createTime;
    maildata.contents = [NSString stringWithUTF8String:vMailInfo->showContent.c_str()];
    maildata.language = [NSString stringWithUTF8String:vMailInfo->modLanguage.c_str()];
    maildata.contentText = [NSString stringWithUTF8String:vMailInfo->showContent.c_str()];
    maildata.type = vMailInfo->type;
    maildata.reply = vMailInfo->reply;
    maildata.itemIdFlag = vMailInfo->itemIdFlag;
    maildata.saveFlag = vMailInfo->save;
    maildata.rewardStatus = vMailInfo->rewardStatus;
    
    maildata.picString = [MailInfoManager gettingPicStringWithType:maildata.type andWithMailInfo:vMailInfo];
    
//    maildata.contents = [NSString stringWithUTF8String:vMailInfo->title.c_str()];
    
    //两句代码顺序不能颠倒
    [maildata mailInfoDataSettingTabType];
    [maildata mailInfoDataSettingChannelID];
    
    /**
     *  解析系统邮件
     */
//    if(false){
//        if (maildata.type == 5) {
//            
//            cocos2d::CCDictionary *item = dynamic_cast<cocos2d::CCDictionary*>(MailController::getInstance()->m_mailDataDicIOS->objectForKey(vMailInfo->uid));
//            
//            CCString *contents = dynamic_cast<CCString*>(item->objectForKey("contents"));
//            NSString *resultStr = [NSString stringWithUTF8String:contents->getCString()];
//            
//            maildata.contents = resultStr;
//            
//            maildata.maildata = [[ResourceMailData alloc] initWithAddMailData:maildata];
//        }else if (maildata.type == 4){
//            maildata.maildata = [[BattleMailData alloc] initWithAddMailData:maildata];
//        }else if (maildata.type == 21){
////            maildata.maildata = [[ResourceHelpMailData alloc] initWithAddMailData:maildata];
//        }
//    }

    return maildata;
}

+(NSString *)gettingPicStringWithType:(int )vType
                      andWithMailInfo:(MailInfo *)vMailInfo{
    NSString * path;
    switch (vType) {
        case 0:
            break;
        case 1:
            path= [NSString stringWithUTF8String:vMailInfo->pic.c_str()];
            break;
        case 2:
            path=@"mail_pic_flag_2";
            break;
        case 3:
            path=[NSString stringWithUTF8String:vMailInfo->pic.c_str()];
          
            break;
        case 4:{
            bool isAtt = false;
            std::string attUid;
            std::string attName;
            std::string defName ;
            if (vMailInfo->attUser ) {
                attUid = vMailInfo->attUser->valueForKey("uid")->getCString();
                 attName = vMailInfo->attUser->valueForKey("name")->getCString();
               defName = vMailInfo->defUser->valueForKey("name")->getCString();
            }
            
            
            if(vMailInfo->atkHelper!=NULL){
                cocos2d::CCObject* obj;
                CCARRAY_FOREACH(vMailInfo->atkHelper, obj){
                    string helpUid = dynamic_cast<CCString*>(obj)->getCString();
                    if(helpUid==GlobalData::shared()->playerInfo.uid){
                        isAtt = true;
                        break;
                    }
                }
            }
            
            if(GlobalData::shared()->playerInfo.uid==attUid){
                isAtt = true;
            }

            if(isAtt){
                path=@"mail_pic_flag_4_1";
            }else{
                path=@"mail_pic_flag_4";
            }
            
            break;
        }
        case 5:
            path=@"mail_pic_flag_5";
            break;
        case 6:
            path=@"mail_pic_flag_8";
            break;
        case 7:
            path=[NSString stringWithUTF8String:vMailInfo->pic.c_str()];
         
            break;
        case 8:
            path=@"mail_pic_flag_8";
            break;
        case 9:
            path=@"mail_pic_flag_9";
            break;
        case 10:
            path=@"mail_pic_flag_2";
            break;
        case 11:
            path=[NSString stringWithUTF8String:vMailInfo->pic.c_str()];
         
            break;
        case 12:
            path=[NSString stringWithUTF8String:vMailInfo->pic.c_str()];
     
            break;
        case 13:
            path=@"mail_pic_flag_2";
            break;
        case 14:
            path=@"mail_pic_flag_14";
            break;
        case 15:
            path=@"mail_pic_flag_2";
            break;
        case 16:
            path=@"mail_pic_flag_16";
            break;
        case 17:
            path=[NSString stringWithUTF8String:vMailInfo->pic.c_str()];
        
            break;
        case 18:
            path=@"mail_pic_flag_18";
            break;
        case 19:
            path=[NSString stringWithUTF8String:vMailInfo->pic.c_str()];
      
            break;
        case 20:
            path=[NSString stringWithUTF8String:vMailInfo->pic.c_str()];
    
            break;
        case 26:
            path =[NSString stringWithUTF8String:vMailInfo->pic.c_str()];
         
            break;
        case 27:
        case 33:
            path = [NSString stringWithUTF8String:vMailInfo->pic.c_str()];
         
            break;
        case 31:
            path=@"mail_pic_flag_31";
            break;
        default:
            break;
    }
    return path;
}
#pragma mark -
#pragma mark  系统邮件删除管理
+(void)mailInfoDataFor_deleteWithMailID:(NSString *)vMailIdString{
    [[DBManager sharedDBManager].dbHelper deleteToDBWithModelTableName:[MailInfoDataModel getTableName] where:[NSString stringWithFormat:@"(ID ='%@')",vMailIdString]];
    [[CSCommandManager sharedCommandManager]deletingMailDataWithMailID:vMailIdString];
}

#pragma mark -
#pragma mark  服务器接口返回状态回调
//奖励领取
+(void)settingRewardStatusToReadWithMailIDArray:(NSArray *)vMailIdArray{
    
    
    if (vMailIdArray.count> 0) {
        for (NSString *mailId in vMailIdArray) {
            MailInfoDataModel *tempmodel =   [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:[MailInfoDataModel getTableName] andWithModelClass:[MailInfoDataModel class] where:[NSString stringWithFormat:@"(ID = '%@')", mailId] orderBy:@"_id desc"];
            if (tempmodel) {
                tempmodel.rewardStatus = 1;
                tempmodel.status = 1;
                [[DBManager sharedDBManager].dbHelper updateToDBWithModelTableName:[MailInfoDataModel getTableName] andWithModel:tempmodel where:[NSString stringWithFormat:@"(_id = %ld)",tempmodel._id]];
            }
        }
    }
}

//邮件已读
+(void)settingUnReadStatusToReadWithMailIDArray:(NSArray *)vMailIdArray{
    if (vMailIdArray.count> 0) {
        for (NSString *mailId in vMailIdArray) {
            MailInfoDataModel *tempmodel =   [[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:[MailInfoDataModel getTableName] andWithModelClass:[MailInfoDataModel class] where:[NSString stringWithFormat:@"(ID = '%@')", mailId] orderBy:@"_id desc"];
            if (tempmodel) {
                tempmodel.status = 1;
                [[DBManager sharedDBManager].dbHelper updateToDBWithModelTableName:[MailInfoDataModel getTableName] andWithModel:tempmodel where:[NSString stringWithFormat:@"(_id = %ld)",tempmodel._id]];
            }
        }
    }
}
+(void)settingUnReadStatusToReadWith_MailType:(int )vMailType{
    int count =[[DBManager sharedDBManager].dbHelper rowCountWithModelClassName:[MailInfoDataModel getTableName] withModelClass:[MailInfoDataModel class] where:nil];
    NSArray *tempArray =[[DBManager sharedDBManager].dbHelper searchWithModelTableName:[MailInfoDataModel getTableName] andWithModelClass:[MailInfoDataModel class] where:[NSString stringWithFormat:@"(Type = %d)",vMailType] orderBy:@"_id desc" offset:0 count:count];
    if (tempArray.count> 0){
        for (MailInfoDataModel *mailModel  in tempArray) {
            mailModel.status = 1;
            [[DBManager sharedDBManager].dbHelper updateToDBWithModelTableName:[MailInfoDataModel getTableName] andWithModel:mailModel where:[NSString stringWithFormat:@"(_id = %ld)",mailModel._id]];
        }
    }
}
 
//批量删除系统邮件
+(void)mailInfoDataFor_deleteWithmailArray:(NSArray *)vMailArray{
    for (MailInfoDataModel *model in vMailArray) {
         [[DBManager sharedDBManager].dbHelper deleteToDBWithModelTableName:[MailInfoDataModel getTableName] where:[NSString stringWithFormat:@"(ID ='%@')",model.mail_ID]];
        
    }
}
//系统邮件数据库更新
+(void)mailInfoDataUpdateToDBWithMailDate:(MailInfoDataModel *)vMailInfoData{
    if (vMailInfoData){
       MailInfoDataModel *tempModel =[[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:[MailInfoDataModel getTableName] andWithModelClass:[MailInfoDataModel class] where:[NSString stringWithFormat:@"(ID = '%@')",vMailInfoData.mail_ID] orderBy:@"_id desc"];
        if (tempModel) {
            tempModel.contents = vMailInfoData.contents;
            [[DBManager sharedDBManager].dbHelper updateToDBWithModelTableName:[MailInfoDataModel getTableName] andWithModel:tempModel where:[NSString stringWithFormat:@"(ID = '%@')",tempModel.mail_ID]];
        }
    }
}
//根据mailID 返回一个mailmodel
+(MailInfoDataModel *)mailInfoModelFromDBWithMailID:(NSString *)vMailIdString{
    if (vMailIdString.length> 0){
        MailInfoDataModel *tempModel =[[DBManager sharedDBManager].dbHelper searchSingleWithModelTableName:[MailInfoDataModel getTableName] andWithModelClass:[MailInfoDataModel class] where:[NSString stringWithFormat:@"(ID = '%@')",vMailIdString] orderBy:@"_id desc"];
        if (tempModel){
            return tempModel;
        }
    }
    return nil;
}
//更具邮件类型批量删除邮件
+(void)banchDeleteMailInfoWithMailType:(int )vType{
    if (vType> 0){
        [[DBManager sharedDBManager].dbHelper deleteToDBWithModelTableName:[MailInfoDataModel getTableName] where:[NSString stringWithFormat:@"Type = %d",vType]];
    }
}
//更具邮件channelID批量删除邮件
+(void)banchDeleteMailInfoWIthMailChannelID:(NSString *)vChannelID{
    if (vChannelID.length > 0){
        [[DBManager sharedDBManager].dbHelper deleteToDBWithModelTableName:[MailInfoDataModel getTableName] where:[DBManager dbSqlWithChannelID:vChannelID]];
    }
}
//获取所有的主城战报战报
+(NSArray *)mainCityInvestigativeNewsAfterCreatTime:(int)vCreatTime{
    NSArray *array = nil;
     NSString *investCitySqlString = @"Type = 8 AND (Contents LIKE '%\"pointType\":1%') ";
 
    if (vCreatTime> 0){
        NSString *where = [NSString stringWithFormat:@"%@ AND CreateTime >= %ld",investCitySqlString,vCreatTime];
        array = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:[MailInfoDataModel getTableName] andWithModelClass:[MailInfoDataModel class] where:where orderBy:@"CreateTime desc" offset:0 count:0];

    }else{
        NSString *where = investCitySqlString;
        array = [[DBManager sharedDBManager].dbHelper searchWithModelTableName:[MailInfoDataModel getTableName] andWithModelClass:[MailInfoDataModel class] where:where orderBy:@"CreateTime desc" offset:0 count:0];

    }
    
        return array;
 
}

//获取未读系统邮件根据channelID
+(NSArray *)gettingUnReadSystemMailTypeWithChannelID:(NSString *)vChannelID{
    
    NSString *mailSeqString =[DBManager dbSqlWithChannelID:vChannelID];
    mailSeqString =[NSString stringWithFormat:@"(%@) and  Status = 0 ",mailSeqString];
    NSArray *channelArr =[[DBManager sharedDBManager].dbHelper searchWithModelTableName:[MailInfoDataModel getTableName] andWithModelClass:[MailInfoDataModel class] where:mailSeqString orderBy:@"_id desc" offset:0 count:0 ];
    
    if  (channelArr.count > 0){
        return channelArr;
    }else{
        return nil;
    }
    
}


@end
