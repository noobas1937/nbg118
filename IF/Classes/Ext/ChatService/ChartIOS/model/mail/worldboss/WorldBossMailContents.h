//
//  WorldBossMailContents.h
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import <Foundation/Foundation.h>
#import "DefParams.h"
@interface WorldBossMailContents : NSObject
@property (nonatomic,strong) DefParams *def;
//泛型 AttListParams
@property (nonatomic,strong) NSMutableArray *attList;
@property (nonatomic,strong) NSString *xy;
@end
