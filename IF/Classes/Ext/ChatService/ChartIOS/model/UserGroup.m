//
//  UserGroup.m
//  IF
//
//  Created by mzc on 15/6/15.
//
//

#import "UserGroup.h"

@implementation UserGroup

+(instancetype)userGroup{
    return [[UserGroup alloc]init];
    
}
-(instancetype)init{
    self = [super init];
    if (self) {
        self.memberArray =[[NSMutableArray alloc]init];
    }
    return self;
}

@end
