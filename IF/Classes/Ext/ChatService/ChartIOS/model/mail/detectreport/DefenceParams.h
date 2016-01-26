//
//  DefenceParams.h
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import <Foundation/Foundation.h>

@interface DefenceParams : NSObject
@property (nonatomic,strong) NSString *total;
@property (nonatomic,strong) NSString *about;
//泛型 ArmsParams
@property (nonatomic,strong) NSMutableArray *arms;
@end
