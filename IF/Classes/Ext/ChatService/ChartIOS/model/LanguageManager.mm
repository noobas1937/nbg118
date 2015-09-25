//
//  LanguageManager.m
//  IF
//
//  Created by mzc on 15/4/20.
//
//

#import "LanguageManager.h"

@implementation LanguageItem
-(instancetype)init:(NSString*)key :(NSString*)langValue
{
    self = [super init];
    if (self) {
        self.key=key;
        self.langValue=langValue;
    }
    return self;
}
@end

@implementation LanguageManager
static LanguagePack * languagePack_chatText = nil;

//@property (nonatomic, strong) LanguagePack *chatText;

+(NSString*) languageManager_getLangByKey:(NSString*) key
{
    return languagePack_chatText != nil ? [languagePack_chatText getTextByKey:key] : key;
}

+(NSString*) languageManager_getLangByKey_replace:(NSString *)key :(NSString *)replaceStr
{
    return languagePack_chatText != nil ? [languagePack_chatText getTextByKey_replace:key :replaceStr] : key;
}

+(void) languageManager_initChatLanguage:(NSArray*) chatLangArray
{
    languagePack_chatText = [[LanguagePack alloc] init:chatLangArray];
}
@end


@interface LanguagePack()
@property (nonatomic, strong) NSMutableDictionary *textMap;
@end

@implementation LanguagePack
-(instancetype) init:(NSArray*) chatLangArray
{
    self = [super init];
    if (self) {
        self.textMap = [[NSMutableDictionary alloc]init];
        
        for (LanguageItem *li in chatLangArray)
        {
            [self.textMap setObject:li.langValue forKey:li.key];
        }
    }
    return self;
}

-(NSString*) getTextByKey:(NSString*) key
{
    if (self.textMap == nil || [self.textMap count] == 0)
    {
        return @"not initialized";
    }
    else if ((NSString*)[self.textMap objectForKey:key] == nil || [(NSString*)[self.textMap objectForKey:key] isEqualToString:@""])
    {
        return key;
    }
    else
    {
        return (NSString*)[self.textMap objectForKey:key];
    }
}

-(NSString*) getTextByKey_replace:(NSString *)key :(NSString *)replaceStr
{

    NSString *s = [self getTextByKey:key];
    
    NSRange range = [s rangeOfString:@"{0}"];
    
    if (range.length > 0)
    {
        s = [s stringByReplacingCharactersInRange:range withString:replaceStr];
    }
    return s;
}
@end