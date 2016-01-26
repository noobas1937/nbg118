//
//  CSRedEnvelopeView.m
//  IF
//
//  Created by 马纵驰 on 15/11/19.
//
//

#import "CSRedEnvelopeView.h"
#import "ServiceInterface.h"
#import "NSString+Cocos2dHelper.h"
#import "NSString+Extension.h"
#import "ServiceInterface.h"

@interface CSRedEnvelopeView()
@property (nonatomic,strong) UILabel *redEnvelopeDescribe;
@property (nonatomic,strong) UILabel *redEnvelopeContent;
@property (nonatomic,strong) UILabel *msgTime;
@property (nonatomic,strong) UIImageView *redEnvelopeImage;

@end

@implementation CSRedEnvelopeView

-(instancetype)initWithFrame:(CGRect)frame addWithIsSelf:(NSMsgItem*) msgItem{
    self  = [super initWithFrame:frame];
    if (self){
        self.msgItem = msgItem;
        [self addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onClickRedPackageMsg:)]];
        [self initBackGroundImage];
        [self initRedEnvelopeImage];
        [self initRedEnvelopeDescribe];
        [self initRedEnvelopeContent];
        [self initMsgTime];
    }
    return self;
    
}
//
//-(void)setFrame:(CGRect)frame addWithIsSelf:(NSMsgItem*) msgItem
//{
//    [super setFrame:frame];
//    
//    self.msgItem = msgItem.isSelfMsg;
//    [self initBackGroundImage];
//    [self initRedEnvelopeImage];
//    [self initRedEnvelopeDescribe];
//    [self initRedEnvelopeContent];
//    [self initMsgTime];
//}

-(void) initRedEnvelopeImage
{
    float x = 0.0;
    if (self.msgItem.isSelfMsg) {
        x = self.frame.size.width * 0.05;
    }else{
        x = self.frame.size.width * 0.12;
    }
    float y = self.frame.size.width * 0.02;
    float width = self.frame.size.width * 0.15;
    float hight = self.frame.size.height * 0.6;
    
    self.redEnvelopeImage = [[UIImageView alloc] initWithFrame:CGRectMake(x, y, width , hight)];
    self.redEnvelopeImage.image = [UIImage imageNamed:@"cs_red_package"];
    [self addSubview:self.redEnvelopeImage];
}

-(void) initRedEnvelopeDescribe
{
    NSString *describe = @"";
    if ([self.msgItem isMatchingVersion]){
        describe = self.msgItem.msg;
    }else{
        describe = [NSString stringWithMultilingualWithKey:@"105110"];
    }
    float fontSize = [ServiceInterface serviceInterfaceSharedManager].redEnvelopeFont;
    float x = self.redEnvelopeImage.right + self.frame.size.width *0.02;
    float y = self.redEnvelopeImage.top;
    float width = self.frame.size.width * 0.6;
    float height = self.frame.size.height * 0.8;
    CGSize size = [describe sizeWithFont:[UIFont systemFontOfSize:fontSize] maxSize:CGSizeMake(width, height)];
    self.redEnvelopeDescribe = [[UILabel alloc]initWithFrame:CGRectMake(x,y,size.width,size.height)];
    self.redEnvelopeDescribe.text = describe;
    self.redEnvelopeDescribe.font = [UIFont systemFontOfSize:fontSize];
    self.redEnvelopeDescribe.numberOfLines = 2;
    [self addSubview:self.redEnvelopeDescribe];
}

-(void) initRedEnvelopeContent
{
    NSString *content = @"";
    NSArray *arr = [self.msgItem.attachmentId componentsSeparatedByString:@"|"];
    if ([arr count] == 2) {
        NSString *status = arr[1];
        
        if (![self.msgItem isMatchingVersion]) {
            return ;
        }
        
        if (self.msgItem.isSelfMsg) {
            content = [NSString stringWithMultilingualWithKey:@"150291"]; //点击查看
        }else{
            content = [NSString stringWithMultilingualWithKey:@"104974"]; //点击领取
        }
        
        float fontSize = [ServiceInterface serviceInterfaceSharedManager].redEnvelopeFont2;
        
        float x = self.redEnvelopeImage.frame.origin.x;
        float y = self.frame.size.height - self.frame.size.height * [ServiceInterface serviceInterfaceSharedManager].redPackageTimePosY;
        float width = self.frame.size.width * 0.7;
        float hight = self.frame.size.height * 0.2;
        
        self.redEnvelopeContent = [[UILabel alloc]initWithFrame:CGRectMake(x,y,width,hight)];
        self.redEnvelopeContent.text = content;
        self.redEnvelopeContent.font = [UIFont systemFontOfSize:fontSize];
        self.redEnvelopeContent.textColor = [UIColor grayColor];
        self.redEnvelopeContent.backgroundColor = [UIColor clearColor];
        [self addSubview:self.redEnvelopeContent];
    }
}

-(void) initMsgTime
{
    float x = self.frame.origin.x;
    float y = self.frame.size.height - self.frame.size.height * [ServiceInterface serviceInterfaceSharedManager].redPackageTimePosY;
    float width = 0.0;
    if (self.msgItem.isSelfMsg) {
        width = self.frame.size.width * 0.65;
    }else{
        width = self.frame.size.width * 0.7;
    }
    
    float hight = self.frame.size.height * 0.2;
    float fontSize = [ServiceInterface serviceInterfaceSharedManager].redEnvelopeFont2;
    
    NSString *timeStr = [self.msgItem nsDataSwitchNSString:self.msgItem.createTime];
    timeStr = [timeStr substringWithRange:NSMakeRange(11,5)];
    
    self.msgTime = [[UILabel alloc] initWithFrame:CGRectMake(x,y,width,hight)];
    self.msgTime.text = timeStr;
    self.msgTime.font = [UIFont systemFontOfSize:fontSize];
    self.msgTime.textAlignment = UITextAlignmentRight;
    [self addSubview:self.msgTime];
}

-(void) initBackGroundImage
{
    float x = 0;
    float y = 0;
    float width = self.frame.size.width;
    float hight = self.frame.size.height;
    self.backgroundImage = [[UIImageView alloc] initWithFrame:CGRectMake(x,y,width,hight)];
    
    [self addSubview:self.backgroundImage];
    
}

-(void) onClickRedPackageMsg:(id)sender{
    if ([ServiceInterface serviceInterfaceSharedManager].cs_CountryChat_OC_Native_New) {
        if ([self.redEnvelopeViewDelegate respondsToSelector:@selector(createRedPackageViewByMsg:)]) {
            [self.redEnvelopeViewDelegate createRedPackageViewByMsg:self.msgItem];
        }
    }else{
        self.backgroundImage.image = self.downImage;
        
        if ([self.redEnvelopeViewDelegate respondsToSelector:@selector(onClickRedPackageMsg)] && [self.msgItem isMatchingVersion]) {
            DVLog(@"CSRedEnvelopeView::onClickRedPackageMsg");
            [self.redEnvelopeViewDelegate onClickRedPackageMsg];
        }
    }
}

@end
