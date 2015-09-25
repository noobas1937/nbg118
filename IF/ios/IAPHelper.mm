//
//  IAPHelper.mm
//  IF
//
//  Created by 邹 程 on 13-11-26.
//
//

#import "IAPHelper.h"
#import "AppController.h"
#import "RootViewController.h"
#import "GTMBase64.h"

#include "PayController.h"
#include "base/CCUserDefault.h"
#include <string>



NSString* iapProductName[IF_IAP_PRODUCT_COUNT] = {
    @"gold_1"
    ,@"gold_2"
    ,@"gold_3"
    ,@"gold_4"
    ,@"gold_5"
    ,@"gold_6"
    ,@"gold_8"
};

@implementation IAPHelper

@synthesize productIds;
@synthesize products;
@synthesize request;

@synthesize hud;


static IAPHelper* _sharedhelper = nil;
+ (IAPHelper*) sharedHelper {
    if (_sharedhelper == nil) {
        _sharedhelper = [[IAPHelper alloc] init];
    }
    return _sharedhelper;
}

- (void) dealloc {
    [self.hud release];
    [self.products release];
    [self.productIds release];
    [self.request release];
    [super dealloc];
}

- (id) init {
    if ((self = [super init])) {
        self.productIds = [NSSet setWithObjects:iapProductName count:IF_IAP_PRODUCT_COUNT];
        [self requestProducts];
    }
    return self;
}

- (void) requestProducts {
    NSLog(@"$IAP$ Request : %@",self.productIds);
    SKProductsRequest* pRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:self.productIds];
    self.request = pRequest;
    [pRequest release];
    self.request.delegate = self;
    [self.request start];
}

- (void) productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
    self.products = response.products;
    self.request = nil;
    
    NSLog(@"$IAP$ Result:");
    int i = 1;
    for (SKProduct* p in response.products) {
        NSLog(@"$IAP$ Valid product %d",i++);
        NSLog(@"    ProductId: %@",p.productIdentifier);
        NSNumberFormatter* numfmt = [[NSNumberFormatter alloc] init];
        [numfmt setFormatterBehavior:NSNumberFormatterBehavior10_4];
        [numfmt setNumberStyle:NSNumberFormatterCurrencyStyle];
        NSString* fmtString = [numfmt stringFromNumber:p.price];
        [numfmt release];
        NSLog(@"    Price: %@",fmtString);
        NSLog(@"    LocalizedTitle: %@",p.localizedTitle);
        NSLog(@"    LocalizedDescription: %@",p.localizedDescription);
        
        CCUserDefault::sharedUserDefault()->setFloatForKey([p.productIdentifier UTF8String],[p.price floatValue]);
        if([iapProductName[0] isEqualToString:p.productIdentifier]){
            float rate = [p.price floatValue]/4.99;
            CCUserDefault::sharedUserDefault()->setFloatForKey("gp_price_exchange_rate",rate);
            NSString *locale = [p.priceLocale localeIdentifier];
            CCUserDefault::sharedUserDefault()->setStringForKey("gp_price_symbole",[locale UTF8String]);
        }
    }
    CCUserDefault::sharedUserDefault()->flush();
    
    i = 1;
    for (NSString* p in response.invalidProductIdentifiers) {
        NSLog(@"$IAP$ Invalid product %d",i++);
        NSLog(@"    ProductId: %@",p);
    }
}
- (NSString*) getLocalCurrency:(NSString *)productIdentifier{
    for (SKProduct* p in self.products) {
        if([productIdentifier isEqualToString:p.productIdentifier]){
            NSNumberFormatter* numfmt = [[NSNumberFormatter alloc] init];
            [numfmt setFormatterBehavior:NSNumberFormatterBehavior10_4];
            [numfmt setNumberStyle:NSNumberFormatterCurrencyStyle];
            [numfmt setLocale:p.priceLocale];
            
            NSString* fmtString = [numfmt stringFromNumber:p.price];
            [numfmt release];
            return fmtString;
        }
    }
    return nil;
}
- (NSDecimalNumber *) getLocalPrice:(NSString *)productIdentifier{
    for (SKProduct* p in self.products) {
        if([productIdentifier isEqualToString:p.productIdentifier]){
            return p.price;
        }
    }
    return nil;
}

- (NSString*) getLocalCustomPrice:(NSDecimalNumber *) customPrice{
    if(self.products.count>0){
        SKProduct* p = self.products[0];
        NSNumberFormatter* numfmt = [[NSNumberFormatter alloc] init];
        [numfmt setFormatterBehavior:NSNumberFormatterBehavior10_4];
        [numfmt setNumberStyle:NSNumberFormatterCurrencyStyle];
        [numfmt setLocale:p.priceLocale];
        NSString* fmtString = [numfmt stringFromNumber:customPrice];
        [numfmt release];
        return fmtString;
    }
    return nil;
}
- (void) buyProductIdentifier:(NSString *)productIdentifier buyItemId:(NSString *)productItemId buyCount:(NSInteger)productCount{
    for (SKProduct* p in self.products) {
        if([productIdentifier isEqualToString:p.productIdentifier]){
            SKMutablePayment* payment = [SKMutablePayment paymentWithProduct:p];
            payment.requestData = [NSData dataWithBytes:productItemId.UTF8String length:productItemId.length];
            payment.quantity = productCount;
            [[SKPaymentQueue defaultQueue] addPayment:payment];
            break;
        }
    }
}

- (void) paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions {
    for (SKPaymentTransaction* transaction in transactions) {
        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchased:
				[self completeTransaction:transaction];
				break;
			case SKPaymentTransactionStateFailed:
				[self failedTransaction:transaction];
				break;
			case SKPaymentTransactionStateRestored:
				[self restoreTransaction:transaction];
			default:
                NSLog(@"$IAP$ unknown payment transaction state: %d",transaction.transactionState);
				break;
        }
    }
}

- (void)completeTransaction:(SKPaymentTransaction *)transaction {
	[self recordTransaction: transaction];
	[self provideContent:transaction.payment.productIdentifier requestData:transaction.payment.requestData myData:transaction.transactionReceipt transactionId:transaction.transactionIdentifier];
	[[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction {
	[self purchaseFailed:transaction.error];
	[[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)restoreTransaction:(SKPaymentTransaction *)transaction {
	[self recordTransaction: transaction];
	[self provideContent: transaction.originalTransaction.payment.productIdentifier requestData:transaction.payment.requestData myData:transaction.transactionReceipt transactionId:transaction.transactionIdentifier];
	[[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)recordTransaction:(SKPaymentTransaction *)transaction {
	// todo: Record the transaction on the server side
}

- (void)provideContent:(NSString *)productIdentifier requestData:(NSData*)iapItemId myData:(NSData*)productData transactionId:(NSString*) transactionId {
    // todo: give player gold
    [self hideHUD];
}

-(void)purchaseFailed:(NSError*)error {
    // todo : parse error
    [self hideHUD];
}


-(void)showHUD {
	[self hideHUD];
    MBProgressHUD* h = [[MBProgressHUD alloc]  initWithView:((AppController*)[UIApplication sharedApplication].delegate).viewController.view];
	self.hud = h;
	[h release];
	self.hud.removeFromSuperViewOnHide = TRUE;
	[((AppController*)[UIApplication sharedApplication].delegate).viewController.view addSubview:self.hud];
	[self performSelector:@selector(hideHUD) withObject:nil afterDelay:120.0];
	[self.hud show:YES];
}

- (void)hideHUD {
	[NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(hideHUD) object:nil];
	if (self.hud != nil) {
		[self.hud hide:YES];
	}
}



@end