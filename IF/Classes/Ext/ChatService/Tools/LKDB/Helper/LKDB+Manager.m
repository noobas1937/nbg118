//
//  NSObject+TableManager.m
//  LKDBHelper
//
//  Created by upin on 13-6-20.
//  Copyright (c) 2013年 ljh. All rights reserved.
//

#import "LKDB+Manager.h"
#import "LKDBHelper.h"

@implementation NSObject (TableManager)
+(int)getTableVersion
{
    return 1;
}
+(LKTableUpdateType)tableUpdateForOldVersion:(int)oldVersion newVersion:(int)newVersion
{
    return LKTableUpdateTypeDefault;
}
+(LKTableUpdateType)tableUpdateWithModelTableName:(NSString *)vTableName
                                 andForOldVersion:(int)oldVersion
                                       newVersion:(int)newVersion
{
    return LKTableUpdateTypeDefault;
}

+(void)tableUpdateAddColumeWithPN:(NSString*)propertyName
{
    LKModelInfos* infos = [self getModelInfos];
    LKDBProperty* property = [infos objectWithPropertyName:propertyName];
    
    NSAssert(property, @"#error %@ add colume name, not exists property name %@",NSStringFromClass(self),propertyName);
    
    [self tableUpdateAddColumeWithName:property.sqlColumeName sqliteType:property.sqlColumeType];
}
+(void)tableUpdateAddColumeWithModelTableName:(NSString *)vTableName andWithPN:(NSString*)propertyName
{
    LKModelInfos* infos = [self getModelInfos];
    LKDBProperty* property = [infos objectWithPropertyName:propertyName];
    
    NSAssert(property, @"#error %@ add colume name, not exists property name %@",NSStringFromClass(self),propertyName);
    
//    [self tableUpdateAddColumeWithName:property.sqlColumeName sqliteType:property.sqlColumeType];
    [self tableUpdateAddColumeWithModelTableName:vTableName andWithName:property.sqlColumeName sqliteType:property.sqlColumeType];
}

+(void)tableUpdateAddColumeWithName:(NSString*)columeName sqliteType:(NSString*)sqliteType
{
    NSString* alertSQL =[NSString stringWithFormat:@"alter table %@ add column %@ %@ ",[self getTableName],columeName,sqliteType];
    
    [[self getUsingLKDBHelper] executeDB:^(FMDatabase *db) {
        [db executeUpdate:alertSQL];
    }];
}
+(void)tableUpdateAddColumeWithModelTableName:(NSString *)vTableName andWithName:(NSString*)columeName sqliteType:(NSString*)sqliteType
{
    NSString* alertSQL =[NSString stringWithFormat:@"alter table %@ add column %@ %@ ",vTableName,columeName,sqliteType];
    
    [[self getUsingLKDBHelper] executeDB:^(FMDatabase *db) {
        [db executeUpdate:alertSQL];
    }];
}


@end

@interface LKTableManager()
@property(strong,nonatomic)NSMutableDictionary* tableInfos;
@property(unsafe_unretained,nonatomic)LKDBHelper* dbhelper;
@end
@implementation LKTableManager
- (id)initWithLKDBHelper:(LKDBHelper *)helper
{
    self = [super init];
    if (self) {
        
        self.dbhelper = helper;
        self.tableInfos = [NSMutableDictionary dictionaryWithCapacity:0];
        [helper executeDB:^(FMDatabase *db) {
            
            [db executeUpdate:@"CREATE TABLE IF NOT EXISTS LKTableManager(table_name text primary key,version integer)"];
            
            FMResultSet* set = [db executeQuery:@"select table_name,version from LKTableManager"];
            
            while ([set next]) {
                [_tableInfos setObject:[NSNumber numberWithInt:[set intForColumnIndex:1]] forKey:[set stringForColumnIndex:0]];
            }
            
            [set close];
        }];
    }
    return self;
}
-(int)versionWithName:(NSString *)name
{
    return [[_tableInfos objectForKey:name] intValue];
}
-(void)setTableName:(NSString *)name version:(int)version
{
   
    [_dbhelper executeDB:^(FMDatabase *db) {
        NSString* replaceSQL = [NSString stringWithFormat:@"replace into LKTableManager(table_name,version) values('%@',%d)",name,version];
        LKLog(@"更新表结构SQL ：%@",replaceSQL);
       BOOL isOK =  [db executeUpdate:replaceSQL];
        if (isOK) {
             [_tableInfos setObject:[NSNumber numberWithInt:version] forKey:name];
        }
    }];
}
-(void)clearTableInfos
{
    [_dbhelper executeDB:^(FMDatabase *db) {

        [db executeUpdate:@"delete from LKTableManager"];
        
        [db executeUpdate:@"CREATE TABLE IF NOT EXISTS LKTableManager(table_name text primary key,version integer)"];
    }];
    [self.tableInfos removeAllObjects];
}
@end