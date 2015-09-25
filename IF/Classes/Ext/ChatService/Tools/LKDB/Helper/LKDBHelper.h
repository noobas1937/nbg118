//
//  LKDBHelper.h
//  upin
//
//  Created by Fanhuan on 12-12-6.
//  Copyright (c) 2012年 linggan. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "FMDatabaseQueue.h"
#import "FMDatabase.h"

#import "LKDBUtils.h"

#import "LKDB+Manager.h"
#import "LKDB+Mapping.h"

#import "NSObject+LKModel.h"
#import "NSObject+LKDBHelper.h"

#ifdef DEBUG
#   define LKLog(fmt, ...) {NSLog((@"\n#LKDBHelper : %s [Line %d] \n" fmt), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__);}
#else
#   define LKLog(...)
#endif

@interface LKDBHelper : NSObject
// you can use [LKDBHelper getUsingLKDBHelper]
//+(LKDBHelper*)sharedDBHelper;

-(id)initWithDBName:(NSString*)dbname;
-(instancetype)initWithCustomNameDB;

/**
 *	@brief  change database , filepath the use of : "documents/db/" + fileName + ".db"
 */
-(void)setDBName:(NSString*)fileName;

/**
 *	@brief  execute database operations synchronously,not afraid of recursive deadlock  同步执行数据库操作 可递归调用
 */
-(void)executeDB:(void (^)(FMDatabase *db))block;

-(BOOL)executeSQL:(NSString *)sql arguments:(NSArray *)args;
-(NSString *)executeScalarWithSQL:(NSString *)sql arguments:(NSArray *)args;
@end

@interface LKDBHelper(DatabaseManager)

//create table with entity class
-(BOOL)createTableWithModelClass:(Class)model;
-(BOOL)createTableWithModelClassTableName:(NSString *)vTableName andModelClass:(Class)vModelClass;

//drop all table
-(void)dropAllTable;

//drop table with entity class
-(BOOL)dropTableWithClass:(Class)modelClass;


@end

@interface LKDBHelper(DatabaseExecute)
/**
 *	@brief	The number of rows query table
 *
 *	@param 	modelClass      entity class
 *	@param 	where           can use NSString or NSDictionary or nil
 *
 *	@return	rows number
 */
-(int)rowCount:(Class)modelClass where:(id)where;
/** zyt扩展 添加传入表名 */
-(int)rowCountWithModelClassName:(NSString *)vTableName where:(id)where;

-(void)rowCount:(Class)modelClass where:(id)where callback:(void(^)(int rowCount))callback;
/** zyt扩展 添加传入表名 */
-(void)rowCountWithModelClassName:(NSString *)vTableName where:(id)where callback:(void (^)(int))callback;
/**
 *	@brief	query table
 *
 *	@param 	modelClass      entity class
 *	@param 	where           can use NSString or NSDictionary or nil
 
 *	@param 	orderBy         The Sort: Ascending "name asc",Descending "name desc"
                            For example: @"rowid desc"  or @"rowid asc"
 
 *	@param 	offset          Skip how many rows
 *	@param 	count           Limit the number
 *
 *	@return	query finished result is an array(model instance collection)
 */
-(NSMutableArray*)search:(Class)modelClass where:(id)where orderBy:(NSString*)orderBy offset:(int)offset count:(int)count;
/** zyt扩展 添加传入表名 */
-(NSMutableArray *)searchWithModelTableName:(NSString *)vTableNameString andWithModelClass:(Class)vModelClass where:(id)where orderBy:(NSString *)orderBy offset:(int)offset count:(int)count;

//return first model or nil
-(id)searchSingle:(Class)modelClass where:(id)where orderBy:(NSString*)orderBy;
/** zyt扩展 添加传入表名 */
-(id)searchSingleWithModelTableName:(NSString *)vTableNameString
                  andWithModelClass:(Class)vModelClass
                              where:(id)where
                            orderBy:(NSString *)orderBy;

-(void)search:(Class)modelClass where:(id)where orderBy:(NSString*)orderBy offset:(int)offset count:(int)count callback:(void(^)(NSMutableArray* array))block;
/** zyt扩展 添加传入表名 */
-(void)searchWithModelTableName:(NSString *)vTableNameString andWithModelClass:(Class)vModelClass where:(id)where orderBy:(NSString *)orderBy offset:(int)offset count:(int)count callback:(void (^)(NSMutableArray *))block;

/**
 *	@brief	insert table
 *
 *	@param 	model 	you want to insert the entity
 *
 *	@return	the inserted was successful
 */
-(BOOL)insertToDB:(NSObject*)model;
/** zyt扩展 添加传入表名 */
-(BOOL)insertToDBWithModelTableName:(NSString *)vTableName andWithModel:(NSObject *)model;

-(void)insertToDB:(NSObject*)model callback:(void(^)(BOOL result))block;
/** zyt扩展 添加传入表名 */
-(void)insertToDBWithModelTableName:(NSString *)vTableName andWithModel:(NSObject *)model callback:(void (^)(BOOL))block;
/**
 *	@brief	insert when the entity primary key does not exist
 *
 *	@param 	model 	you want to insert the entity
 *
 *	@return	the inserted was successful
 */
-(BOOL)insertWhenNotExists:(NSObject*)model;
-(void)insertWhenNotExists:(NSObject*)model callback:(void(^)(BOOL result))block;

/**
 *	@brief update table
 *
 *	@param 	model 	you want to update the entity
 *	@param 	where 	can use NSString or NSDictionary or nil
                    when "where" is nil : update the value based on rowid colume or primary key colume
 *
 *	@return	the updated was successful
 */
-(BOOL)updateToDB:(NSObject *)model where:(id)where;
/** zyt扩展 添加传入表名 */
-(BOOL)updateToDBWithModelTableName:(NSString *)vTableName
                       andWithModel:(NSObject *)vModel
                              where:(id)where;

-(void)updateToDB:(NSObject *)model where:(id)where callback:(void (^)(BOOL result))block;
/** zyt扩展 添加传入表名 */
-(void)updateToDBWithModelTableName:(NSString *)vTableName
                       andWithModel:(NSObject *)vModel
                              where:(id)where
                           callback:(void (^)(BOOL))block;

-(BOOL)updateToDB:(Class)modelClass set:(NSString*)sets where:(id)where;
/**
 *	@brief	delete table
 *
 *	@param 	model 	you want to delete entity
                    when entity property "rowid" == 0  based on the primary key to delete
 *
 *	@return	the deleted was successful
 */
-(BOOL)deleteToDB:(NSObject*)model;
/** zyt扩展 添加传入表名 */
-(BOOL)deleteToDBWithModelTableName:(NSString *)vTableName
                       andWithModel:(NSObject *)vModel;

-(void)deleteToDB:(NSObject*)model callback:(void(^)(BOOL result))block;
/** zyt扩展 添加传入表名 */
-(void)deleteToDBWithModelTableName:(NSString *)vTableName
                       andWithModel:(NSObject *)vModel
                           callback:(void (^)(BOOL))block;

/**
 *	@brief	delete table with "where" constraint
 *
 *	@param 	modelClass      entity class
 *	@param 	where           can use NSString or NSDictionary,  can not is nil
 *
 *	@return	the deleted was successful
 */
-(BOOL)deleteWithClass:(Class)modelClass where:(id)where;
-(void)deleteWithClass:(Class)modelClass where:(id)where callback:(void (^)(BOOL result))block;

/**
 *	@brief   entity exists?
 *           for primary key colume 
            （if rowid > 0 would certainly exist so we do not rowid judgment）
 *	@param 	model 	entity
 *
 *	@return	YES: entity presence , NO: entity not exist
 */
-(BOOL)isExistsModel:(NSObject*)model;
-(BOOL)isExistsClass:(Class)modelClass where:(id)where;


/**
 *	@brief	Clear data based on the entity class
 *
 *	@param 	modelClass 	entity class
 */
+(void)clearTableData:(Class)modelClass;
   
/**
 *	@brief	Clear Unused Data File
            if you property has UIImage or NSData, will save their data in the (documents dir)
 *
 *	@param 	modelClass      entity class
 *	@param 	columes         UIImage or NSData Colume Name
 */
+(void)clearNoneImage:(Class)modelClass columes:(NSArray*)columes;
+(void)clearNoneData:(Class)modelClass columes:(NSArray*)columes;

@end