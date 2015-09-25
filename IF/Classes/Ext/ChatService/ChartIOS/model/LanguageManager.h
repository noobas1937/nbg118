//
//  LanguageManager.h
//  IF
//
//  Created by mzc on 15/4/20.
//
//

#import <Foundation/Foundation.h>

@interface LanguageItem : NSObject
@property (nonatomic, strong) NSString *key;
@property (nonatomic, strong) NSString *langValue;
-(instancetype)init:(NSString*)key :(NSString*)langValue;
@end

@interface LanguagePack : NSObject
-(NSString*) getTextByKey:(NSString*) key;
-(NSString*) getTextByKey_replace:(NSString*)key :(NSString*) replaceStr;
-(instancetype) init:(NSArray*) chatLangArray;
@end

@interface LanguageManager : NSObject
+(NSString*) languageManager_getLangByKey:(NSString*) key;
+(NSString*) languageManager_getLangByKey_replace:(NSString*)key :(NSString*) replaceStr;
+(void) languageManager_initChatLanguage:(NSArray*) chatLangArray;
@end


