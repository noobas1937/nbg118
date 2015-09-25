//
//  IAPHelper.h
//  IF
//
//  Created by 邹 程 on 13-11-26.
//
//

#import <StoreKit/StoreKit.h>
#import "MBProgressHUD.h"

#define IF_IAP_PRODUCT_COUNT    7

extern NSString* iapProductName[IF_IAP_PRODUCT_COUNT];

@interface IAPHelper : NSObject <SKProductsRequestDelegate,SKPaymentTransactionObserver>

@property (nonatomic,retain) NSSet*              productIds;
@property (nonatomic,retain) NSArray*            products;
@property (nonatomic,retain) SKProductsRequest*  request;

@property (nonatomic,retain) MBProgressHUD*      hud;


- (void) requestProducts;
- (void) buyProductIdentifier:(NSString *)productIdentifier buyItemId:(NSString *)itemId buyCount:(NSInteger)productCount;
- (NSString*) getLocalCurrency:(NSString *)productIdentifier;
- (NSDecimalNumber *) getLocalPrice:(NSString *)productIdentifier;
- (NSString*) getLocalCustomPrice:(NSDecimalNumber *) customPrice;

+ (IAPHelper*) sharedHelper;

- (void) showHUD;
- (void) hideHUD;

@end