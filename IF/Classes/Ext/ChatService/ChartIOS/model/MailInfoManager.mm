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
@implementation MailInfoManager

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

@end
