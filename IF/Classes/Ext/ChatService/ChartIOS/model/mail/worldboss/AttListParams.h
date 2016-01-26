//
//  AttListParams.h
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import <Foundation/Foundation.h>
#import "AttParams.h"
@interface AttListParams : NSObject
@property (nonatomic,strong) NSString *picVer;
@property (nonatomic,strong) NSString * uid;
@property (nonatomic,strong) NSString * name;
@property (nonatomic,strong) NSString * leader;
@property (nonatomic,strong) AttParams *att;
@property (nonatomic,strong) NSString * pic;
@end
