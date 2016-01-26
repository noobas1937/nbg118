//
//  CSConfigManager.m
//  IF
//
//  Created by 马纵驰 on 15/10/30.
//
//

#import "CSConfigManager.h"
#include "LocalController.h"
#include <string.h>

static CSConfigManager * csConfigManager = nil;

@implementation CSConfigManager

+ (instancetype)csConfigManagerShared
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        csConfigManager = [[CSConfigManager alloc] init];
    });
    return csConfigManager;
}


-(instancetype)init
{
    self = [super init];
    if (self) {
        
    }
    return self;
}


+(instancetype)sharedConfigManager{
    static CSConfigManager *sharedConfigManager = nil;
    static dispatch_once_t predicate;
    dispatch_once(&predicate, ^{
        sharedConfigManager = [[self alloc] init];
    });
    return sharedConfigManager;
}
-(NSString *)gettingGameLang2IOS
{
    std::string lang=LocalController::shared()->getLanguageFileName();
    return [NSString stringWithUTF8String:lang.c_str()];
}

@end
