//
//  UpdateParam.h
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import <Foundation/Foundation.h>

@interface UpdateParam : NSObject
//泛型 nsstring
@property (nonatomic,strong) NSMutableArray *Delete;
//泛型 UpdateParam
@property (nonatomic,strong) NSMutableArray *update;
@end
