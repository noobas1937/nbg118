//
//  DetectReportMailContents.h
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import <Foundation/Foundation.h>
#import "DefenceParams.h"
#import "ResourceParams.h"
#import "ReinAboutParams.h"
#import "FortParams.h"
#import "UserInfoParams.h"
@interface DetectReportMailContents : NSObject

//泛型 ReinDetailParams
@property (nonatomic,strong) NSMutableArray *rein_detail;
//泛型 TowerParams
@property (nonatomic,strong) NSMutableArray * tower;
@property (nonatomic,strong) DefenceParams *defence;
@property (nonatomic,strong) NSString *name;
@property (nonatomic,strong) ResourceParams *resource;
@property (nonatomic,strong) NSString *pointId;
//结构 List<List<ReinAboutDetailParams>> 泛型 ReinAboutDetailParams
@property (nonatomic,strong) NSMutableArray *rein_about_detail;
@property (nonatomic,strong) ReinAboutParams *rein_about;
@property (nonatomic,strong) FortParams *fort;
@property (nonatomic,strong) UserInfoParams *user;
//泛型 ScienceParams
@property (nonatomic,strong) NSMutableArray *science;
@property (nonatomic,strong) NSString *pointType;
@property (nonatomic,strong) NSString *contents;
//泛型 string
@property (nonatomic,strong) NSMutableArray *ability;
@property (nonatomic,strong) NSString *ckf;
@end
