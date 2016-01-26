//
//  ResourceHelpMailContents.h
//  IF
//
//  Created by 马纵驰 on 15/9/26.
//
//

#import <Foundation/Foundation.h>

@interface ResourceHelpMailContents : NSObject

@property (nonatomic,strong) NSString *uid;
@property (nonatomic,strong) NSString *fromName;
@property (nonatomic,strong) NSString *pic;
@property (nonatomic,strong) NSString *alliance;
@property (nonatomic,strong) NSString *level;
@property (nonatomic,strong) NSString *traderAlliance;
@property (nonatomic,strong) NSString *pointId;
@property (nonatomic,strong) NSString *createTime;

/**
 *  泛型 RewardParams
 */
@property (nonatomic,strong) NSMutableArray *reward;


@end
