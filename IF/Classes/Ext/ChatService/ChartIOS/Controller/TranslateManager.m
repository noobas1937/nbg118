//
//  TranslateManager.m
//  IF
//
//  Created by 马纵驰 on 15/10/9.
//
//

#import "TranslateManager.h"

@implementation TranslateManager

-(instancetype)init
{
    self = [super init];
    if (self) {
        self.disableLang = @"";
    }
    return self;
}

+ (TranslateManager *)sharedTranslateManagerInstance
{
    static TranslateManager *sharedTranslateManagerInstance = nil;
    static dispatch_once_t predicate;
    dispatch_once(&predicate, ^{
        sharedTranslateManagerInstance = [[self alloc] init];
    });
    return sharedTranslateManagerInstance;
}

@end
