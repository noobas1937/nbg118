//
//  NSArray+SortHelper.m
//  IF
//
//  Created by 张彦涛 on 15/9/11.
//
//

#import "NSArray+SortHelper.h"
#import "ServiceInterface.h"

#import "MailInfoDataModel.h"
#import "ChatChannel.h"
#import "ChatCellFrame.h"
#import "CSMailCategoryListModel.h"
#import "CSMessage.h"
#import "CSMessageModel.h"
@implementation NSArray (SortHelper)


+ (NSMutableArray *)sortingDataWithMsgItem:(NSMutableArray *)vArray andWithChannelType:(int )vType
{
    NSMutableArray *ret = nil;
    //获取当前登录用户的会话列表
    NSArray *conversations = vArray;
    NSArray* sorte = [conversations sortedArrayUsingComparator:
                      ^(NSMsgItem *obj1, NSMsgItem* obj2){
                          
                          
                          
                          
                          if(vType == IOS_CHANNEL_TYPE_USER){
                              if(obj1.createTime < obj2.createTime) {
                                  //升序
                                  return(NSComparisonResult)NSOrderedAscending;
                              }else {
                                  //降序
                                  return(NSComparisonResult)NSOrderedDescending;
                              }
                          }else{
                              if(obj1.sequenceId < obj2.sequenceId) {
                                  return(NSComparisonResult)NSOrderedAscending;
                              }else {
                                  return(NSComparisonResult)NSOrderedDescending;
                              }
                          }
                      }];
    ret = [[NSMutableArray alloc] initWithArray:sorte];
    return ret;
}
+ (NSMutableArray *)sortingMessageWithMsgArray:(NSMutableArray *)vArray andWithChannelType:(int )vType{
    NSMutableArray *ret = nil;
    //获取当前登录用户的会话列表
    NSArray *conversations = vArray;
    NSArray* sorte = [conversations sortedArrayUsingComparator:
                      ^(CSMessage    *msg1, CSMessage* msg2){
 
                        
                          if(msg1.createTime < msg2.createTime) {
                              //升序
                              return(NSComparisonResult)NSOrderedAscending;
                          }else if(msg1.createTime == msg2.createTime) {
                              if (vType != IOS_CHANNEL_TYPE_USER) {
                                  if (msg1.sequenceId< msg2.sequenceId) {
                                      
                                      return(NSComparisonResult)NSOrderedAscending;
                                  }else{
                                      return(NSComparisonResult)NSOrderedDescending;
                                  }
                              }else{
                                  return (NSComparisonResult)NSOrderedAscending;
                              }
                              
                          }else{
                              //降序
                              return(NSComparisonResult)NSOrderedDescending;
                          }
                          
                          
                          
                      }];
    ret = [[NSMutableArray alloc] initWithArray:sorte];
    return ret;

}

+ (NSMutableArray *)sortingDataForMessageWithMsgModelArray:(NSMutableArray *)vArray andWithChannelType:(int )vType
{
    NSMutableArray *ret = nil;
    //获取当前登录用户的会话列表
    NSArray *conversations = vArray;
    NSArray* sorte = [conversations sortedArrayUsingComparator:
                      ^(CSMessageModel   *obj1, CSMessageModel* obj2){
                          //                          NSNumber *msg1= [NSNumber numberWithLong:obj1.chatMessage.createTime ];
                          //                          NSNumber *msg2=[NSNumber numberWithLong:obj2.chatMessage.createTime ];
                          //                          return [msg1 compare:msg2];
                          CSMessage  *msg1 = [obj1 message];
                          CSMessage  *msg2 = [obj2 message];
                          //                          if (vType == IOS_CHANNEL_TYPE_USER) {
                          if(msg1.createTime < msg2.createTime) {
                              //升序
                              return(NSComparisonResult)NSOrderedAscending;
                          }else if(msg1.createTime == msg2.createTime) {
                              if (vType != IOS_CHANNEL_TYPE_USER) {
                                  if (msg1.sequenceId< msg2.sequenceId) {
                                      
                                      return(NSComparisonResult)NSOrderedAscending;
                                  }else{
                                      return(NSComparisonResult)NSOrderedDescending;
                                  }
                              }else{
                                  return (NSComparisonResult)NSOrderedAscending;
                              }
                              
                          }else{
                              //降序
                              return(NSComparisonResult)NSOrderedDescending;
                          }
                          
                          
                          
                      }];
    ret = [[NSMutableArray alloc] initWithArray:sorte];
    return ret;
}

/**
 *  从大到小排序
 *
 *  @param vArray 传入的array
 *
 *  @return 返回的array
 */
+ (NSMutableArray *)mailInfoDataSortingDataSource:(NSMutableArray *)vArray
{
    NSMutableArray *ret = nil;
    //获取当前登录用户的会话列表
    NSArray *conversations = vArray;
    NSArray* sorte = [conversations sortedArrayUsingComparator:
                      ^(MailInfoDataModel *obj1, MailInfoDataModel* obj2){
                          
                          
                              if(obj1.creatTime > obj2.creatTime) {
                                  //升序
                                  return(NSComparisonResult)NSOrderedAscending;
                              }else {
                                  //降序
                                  return(NSComparisonResult)NSOrderedDescending;
                              }
                        }];
    ret = [[NSMutableArray alloc] initWithArray:sorte];
    return ret;
}
+ (NSMutableArray *)chatChannelSortingDataSource:(NSMutableArray *)vArray
{
    NSMutableArray *ret = nil;
    //获取当前登录用户的会话列表
    NSArray *conversations = vArray;
    NSArray* sorte = [conversations sortedArrayUsingComparator:
                      ^(ChatChannel   *obj1, ChatChannel* obj2){
                          
                          
                          if(obj1.latestTime > obj2.latestTime) {
                              //升序
                              return(NSComparisonResult)NSOrderedAscending;
                          }else {
                              //降序
                              return(NSComparisonResult)NSOrderedDescending;
                          }
                      }];
    ret = [[NSMutableArray alloc] initWithArray:sorte];
    return ret;
}

+ (NSMutableArray *)chatMsgSortingDataSource:(NSMutableArray *)vArray andWithChannelType:(int )vType
{
    NSMutableArray *ret = nil;
    //获取当前登录用户的会话列表
    NSArray *conversations = vArray;
    NSArray* sorte = [conversations sortedArrayUsingComparator:
                      ^(ChatCellFrame   *obj1, ChatCellFrame* obj2){
//                          NSNumber *msg1= [NSNumber numberWithLong:obj1.chatMessage.createTime ];
//                          NSNumber *msg2=[NSNumber numberWithLong:obj2.chatMessage.createTime ];
//                          return [msg1 compare:msg2];
                          NSMsgItem *msg1 = [obj1 chatMessage];
                          NSMsgItem *msg2 = [obj2 chatMessage];
//                          if (vType == IOS_CHANNEL_TYPE_USER) {
                              if(msg1.createTime < msg2.createTime) {
                                  //升序
                                  return(NSComparisonResult)NSOrderedAscending;
                              }else if(msg1.createTime == msg2.createTime) {
                                  if (vType != IOS_CHANNEL_TYPE_USER) {
                                      if (msg1.sequenceId< msg2.sequenceId) {
                                          
                                          return(NSComparisonResult)NSOrderedAscending;
                                      }else{
                                          return(NSComparisonResult)NSOrderedDescending;
                                      }
                                  }else{
                                      return (NSComparisonResult)NSOrderedAscending;
                                  }
                                 
                              }else{
                                  //降序
                                  return(NSComparisonResult)NSOrderedDescending;
                              }

 
                          
                       }];
    ret = [[NSMutableArray alloc] initWithArray:sorte];
    return ret;
}
-(NSArray *)vs_sortedArrayUsingComparator:(NSComparisonResult(^)(id obj1, id obj2))comparator
{
    NSMutableArray *array = [self mutableCopy];
    if ([array count] > 1) {
        for (NSUInteger i = 0; i < [array count] - 1; ++i) {
            for (NSUInteger j  = i; j < [array count]; ++j) {
                if (comparator(array[i], array[j]) == NSOrderedDescending) {
                    [array exchangeObjectAtIndex:i withObjectAtIndex:j];
                }
            }
        }
    }
    return [NSMutableArray arrayWithArray:array] ;
}

+(NSMutableArray *)mailCategoryListModelSortingForIndexSeqWithArray:(NSMutableArray *)vArray{
    NSMutableArray *ret = nil;
    //获取当前登录用户的会话列表
    NSArray *conversations = vArray;
    NSArray* sorte = [conversations sortedArrayUsingComparator:
                      ^(CSMailCategoryListModel   *obj1, CSMailCategoryListModel* obj2){
  
                          
                          if(obj1.showIndexSequence < obj2.showIndexSequence) {
                              //升序
                              return(NSComparisonResult)NSOrderedAscending;
                          }else {
                              //降序
                              return(NSComparisonResult)NSOrderedDescending;
                          }
                      }];
    ret = [[NSMutableArray alloc] initWithArray:sorte];
    return ret;
}
@end
