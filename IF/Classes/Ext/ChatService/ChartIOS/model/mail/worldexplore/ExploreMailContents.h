//
//  ExploreMailContents.h
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import <Foundation/Foundation.h>

@interface ExploreMailContents : NSObject
@property (nonatomic,strong) NSString *point;
@property (nonatomic,strong) NSString *exploredTime;
//泛型 ExporeRewardParams
@property (nonatomic,strong) NSMutableArray *reward;
@property (nonatomic,strong) NSDictionary *trap;
@end
