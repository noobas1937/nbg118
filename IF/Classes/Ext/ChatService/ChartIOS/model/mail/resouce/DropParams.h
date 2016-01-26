//
//  DropParams.h
//  IF
//
//  Created by 马纵驰 on 15/9/14.
//
//

#import <Foundation/Foundation.h>
#import "DropValueParams.h"

@interface DropParams : NSObject
@property (nonatomic,strong) DropValueParams *value;
@property (nonatomic,strong) NSString *type;
@end
