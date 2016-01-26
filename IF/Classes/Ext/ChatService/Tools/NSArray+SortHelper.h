//
//  NSArray+SortHelper.h
//  IF
//
//  Created by 张彦涛 on 15/9/11.
//
//

#import <Foundation/Foundation.h>

@interface NSArray (SortHelper)


+ (NSMutableArray *)sortingDataWithMsgItem:(NSMutableArray *)vArray andWithChannelType:(int )vType;
+ (NSMutableArray *)sortingMessageWithMsgArray:(NSMutableArray *)vArray andWithChannelType:(int )vType;
+ (NSMutableArray *)sortingDataForMessageWithMsgModelArray:(NSMutableArray *)vArray andWithChannelType:(int )vType;


+ (NSMutableArray *)mailInfoDataSortingDataSource:(NSMutableArray *)vArray;
+ (NSMutableArray *)chatChannelSortingDataSource:(NSMutableArray *)vArray;
+ (NSMutableArray *)chatMsgSortingDataSource:(NSMutableArray *)vArray andWithChannelType:(int )vType;
//+ (NSMutableArray *)chatMsgSortingDataSource:(NSMutableArray *)vArray;
-(NSArray *)vs_sortedArrayUsingComparator:(NSComparisonResult(^)(id obj1, id obj2))comparator;

+(NSMutableArray *)mailCategoryListModelSortingForIndexSeqWithArray:(NSMutableArray *)vArray;


@end
