//
//  ShopIAPHelper.h
//  GOE
//
//  Created by Limin on 12-4-1.
//  Copyright 2011年 Elex. All rights reserved.
//

#import "ShopIAPHelper.h"
#import "GTMBase64.h"

#import "AppController.h"
#import "RootViewController.h"
#import "PayController.h"

#include <string>

@implementation ShopIAPHelper

@synthesize hud = _hud;

static ShopIAPHelper * _sharedHelper;

+ (ShopIAPHelper *) sharedHelper
{
	if (_sharedHelper == nil) {
		_sharedHelper = [[ShopIAPHelper alloc] init];
	}
	return _sharedHelper;    
}

- (id)init
{
	if ((self = [super init]))
	{}
	return self;    
}

-(void)dealloc 
{
	[_hud release];
	[super dealloc];
}

#pragma mark - 购买

-(void) purchaseItem:(NSString *)productIdentifier buyItemId:(NSString *)itemId buyCount:(NSInteger)count
{
    [[ShopIAPHelper sharedHelper] buyProductIdentifier:productIdentifier buyItemId:itemId buyCount:count];
	[self showHUD];
}

// 购买成功
- (void)provideContent:(NSString*)productIdentifier requestData:(NSData*)iapItemId myData:(NSData*)productData transactionId:(NSString*) transactionId
{
//	NSLog(@"PurchaseInfo:%@", productData);

	[self hideHUD];

	// 发送给服务器请求验证
    std::string orderId = [transactionId UTF8String];
    std::string sSignedData = "";
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 9.0)
        sSignedData = [[GTMBase64 stringByEncodingData:productData] UTF8String];
    else{
        //9.0以后读取方式改变
        NSData *receipt = [NSData dataWithContentsOfURL:[[NSBundle mainBundle] appStoreReceiptURL]];
        if(receipt) {
            sSignedData = [[receipt base64EncodedStringWithOptions:0] UTF8String];
        }
    }
    std::string productId = [productIdentifier UTF8String];
    std::string itemId = [[[NSString alloc] initWithData:iapItemId encoding:NSUTF8StringEncoding] UTF8String];
    PayController::getInstance()->callPaymentIOSSendGoods(orderId, sSignedData, productId, itemId);
}

// 购买失败
-(void)purchaseFailed:(NSError*)error
{
	[self hideHUD];
	if (error.code != SKErrorPaymentCancelled) {
		UIAlertView *alert = [[[UIAlertView alloc] initWithTitle:@"Error!" message:error.localizedDescription delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil] autorelease];
		[alert show];
    }else{
        PayController::getInstance()->onPayCancel();
    }
}

#pragma  mark - HUD

-(void)showHUD
{
	[self hideHUD];
	
	MBProgressHUD* h = [[MBProgressHUD alloc]  initWithView:((AppController*)[UIApplication sharedApplication].delegate).viewController.view];
	self.hud = h;
	[h release];
	
	_hud.removeFromSuperViewOnHide = TRUE;
	[((AppController*)[UIApplication sharedApplication].delegate).viewController.view addSubview:_hud];
//	_hud.labelText = NSLocalizedString(@"购买中...", @"");
	[self performSelector:@selector(hideHUD) withObject:nil afterDelay:120.0];
	[_hud show:YES];
}

- (void)hideHUD
{
	[NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(hideHUD) object:nil];
	if (_hud != nil) {
		[_hud hide:YES];
	}
}

@end
