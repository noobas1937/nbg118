//
//  CSMessage.m
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/7.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import "CSMessage.h"

#import "LKDBHelper.h"
#import "CSMessage+Manager.h"

@implementation CSMessage


//+(CSMessage *)messageWithSelfSendString:(NSString *)vSendString andWithChannelType:(CSChannelType )vChannelType andWithMessageBodyType:(CSMessageBodyType)vMsgBodyType{
//    CSMessage *message =[[CSMessage alloc]init];
//    message.msg = vSendString;
//    message.createTime = [[NSDate date]timeIntervalSince1970];
//    message.sendLocalTime = message.createTime;
//    message.post = (int )vMsgBodyType;
//    message.channelType = vChannelType;
//    message.uid = [UserManager sharedUserManager].currentUser.uid;
//    return message;
//}

#ifdef DEBUG
// 将要插入数据库
//+(BOOL)dbWillInsert:(NSObject *)entity
//{
//    LKLog(@"will insert : %@",NSStringFromClass(self));
//    return YES;
//}
//插库
+(void)dbDidInserted:(NSObject *)entity result:(BOOL)result
{
    if (result) {
        LKLog(@"did insert Flag: 【%d】\n完成插入的数据:%@",result,[entity printAllPropertys] );
    }else{
        LKLog(@"did insert Flag: 【%d】\n插入失败的数据:%@",result,[entity printAllPropertys] );
    }
    
}

+(void)dbDidUpdated:(NSObject *)entity result:(BOOL)result{
    if (result) {
        LKLog(@"did Update Flag: 【%d】\n更新成功的数据:%@",result,[entity printAllPropertys] );
    }else{
        LKLog(@"did Update Flag: 【%d】\n 更新失败的数据:%@",result,[entity printAllPropertys] );
    }
}

#endif

#pragma mark -
#pragma mark DB Base
//更新数据模型后，需要调用
//在类 初始化的时候
+(void)initialize
{
    //remove unwant property
    //比如 getTableMapping 返回nil 的时候  会取全部属性  这时候 就可以 用这个方法  移除掉 不要的属性
    //    [self removePropertyWithColumnName:@"error"];
    
    
    //simple set a column as "LKSQL_Mapping_UserCalculate"
    //根据 属性名  来启用自己计算
    //[self setUserCalculateForCN:@"error"];
    
    
    //根据 属性类型  来启用自己计算
    //[self setUserCalculateForPTN:@"NSDictionary"];
    
    //enable own calculations
    //[self setUserCalculateForCN:@"address"];
    
    //enable the column binding property name
    //    [self setTableColumnName:@"MyAge" bindingPropertyName:@"age"];
    //    [self setTableColumnName:@"MyDate" bindingPropertyName:@"date"];
}
+(NSString *)getPrimaryKey
{
    return @"_id";
}

+(int)getTableVersion
{
    return kDBVersion;
}
+(NSDictionary *)getTableMapping{
    return @{@"TableVer"            :@"tableVer",
             @"_id"                  :@"_id"    ,
             @"SequenceID"          :@"sequenceId",
             @"MailID"              :@"mailId",
             @"UserID"              :@"uid",
             @"ChannelType"          :@"channelType",
             @"CreateTime"          :@"createTime",
             @"LocalSendTime"        :@"sendLocalTime",
             @"Type"                :@"post",
             @"Msg"                  :@"msg",
             @"Translation"          :@"translateMsg",
             @"OriginalLanguage"    :@"originalLang",
             @"TranslatedLanguage"  :@"translatedLang",
             @"Status"              :@"sendState",
             @"AttachmentId"        :@"attachmentId",
             @"Media"                :@"media"
             //             @"UserName"             :@"name",
             //             @"Asn"                  :@"asn",
             //             @"Vip"                  :@"vip",
             //             @"HeadPicVer"           :@"headPic",
             //             @"HeadPicVer"           :@"headPicVer"
             };
}
-(id)modelGetValue:(LKDBProperty *)property{
    NSString *sqlColumnname= [NSString stringWithFormat:@"%@",property.sqlColumnName];
    if([sqlColumnname isEqualToString:@"TableVer"])
    {
        return [NSString stringWithFormat:@"%d",[CSMessage getTableVersion]];
    }else{
        return  [super modelGetValue:property];
    }
    
}
+(void)columnAttributeWithProperty:(LKDBProperty *)property
{
    NSString *sqlColumnname= [NSString stringWithFormat:@"%@",property.sqlColumnName];
    if([sqlColumnname isEqualToString:@"TableVer"])
    {
        property.defaultValue = [NSString stringWithFormat:@"%d",[CSMessage getTableVersion]];
    }
    //    if([property.propertyName isEqualToString:@"date"])
    //    {
    //        property.isUnique = YES;
    //        property.checkValue = @"MyDate > '2000-01-01 00:00:00'";
    //        property.length = 30;
    //    }
}
@end
