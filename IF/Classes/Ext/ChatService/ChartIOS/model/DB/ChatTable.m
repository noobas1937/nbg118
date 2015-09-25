//
//  ChatTable.m
//  IF
//
//  Created by mzc on 15/6/15.
//
//

#import "ChatTable.h"

@implementation ChatTable

-(instancetype)init:(NSString *)channelID :(int)type {
    self = [super init];
    if (self) {
        self.channelID = channelID;
        self.channelType = type;
    }
    return self;
}

@end
