//
//  ChatTable.h
//  IF
//
//  Created by mzc on 15/6/15.
//
//

#import <Foundation/Foundation.h>

@interface ChatTable : NSObject
@property (nonatomic,assign)    int                    channelType;//0为国家 1为联盟 2为聊天室
@property (nonatomic, strong)   NSString               *channelID;
- (instancetype)init:(NSString*) channelID :(int) type;
-(NSString*) getChannelName;
-(NSString*) getTableNameAndCreate;
-(NSString*) getTableName;
@end
