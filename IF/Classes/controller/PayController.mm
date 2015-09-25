//
//  PayController.mm
//  IF
//
//  Created by 邹 程 on 13-11-22.
//
//

#include "PayController.h"
#include "ShopIAPHelper.h"

void PayController::doPay() {
    NSString * productIdentifier = [NSString stringWithUTF8String : m_description.c_str()];
    std::string buyItem = m_itemId;
    if(!m_donateUID.empty()){
        buyItem.append("_" + m_donateUID);
    }
    NSString * itemId = [NSString stringWithUTF8String : buyItem.c_str()];
    NSInteger count = m_payNumber;
    [[ShopIAPHelper sharedHelper] purchaseItem:productIdentifier buyItemId:itemId buyCount:count];
}

void PayController::checkHistoryPurchase(){
    
}
string PayController::getDollarText(string dollar,string productId){
    string payCurrency = "US $";
    string returnString = payCurrency + dollar;

    if(!productId.empty() && !dollar.empty()){
        NSString * productIdentifier = [NSString stringWithUTF8String : productId.c_str()];
        NSString * currency = [[ShopIAPHelper sharedHelper] getLocalCurrency:productIdentifier];
        if (currency!= nil){
            returnString =  [currency UTF8String];
            return returnString;
        }
    }
    if(!productId.empty()){
        float value = CCUserDefault::sharedUserDefault()->getFloatForKey(productId.c_str());
        string symbole = CCUserDefault::sharedUserDefault()->getStringForKey("gp_price_symbole");
        if(value>0 && !symbole.empty()){
            NSString *nssymbole = [NSString stringWithUTF8String : symbole.c_str()];
            NSLocale *nslocale = [[NSLocale alloc] initWithLocaleIdentifier:nssymbole];
            NSNumberFormatter* numfmt = [[NSNumberFormatter alloc] init];
            [numfmt setFormatterBehavior:NSNumberFormatterBehavior10_4];
            [numfmt setNumberStyle:NSNumberFormatterCurrencyStyle];
            [numfmt setLocale:nslocale];
            
            NSDecimalNumber *price = [NSDecimalNumber numberWithFloat:value];
            NSString* fmtString = [numfmt stringFromNumber:price];
            [numfmt release];
            returnString =  [fmtString UTF8String];
            return returnString;
        }
    }
    
    if(!dollar.empty()){
        string symbole = CCUserDefault::sharedUserDefault()->getStringForKey("gp_price_symbole");
        float _exRate = CCUserDefault::sharedUserDefault()->getFloatForKey("gp_price_exchange_rate");
        if( _exRate>0 && !symbole.empty()){
            NSString *nssymbole = [NSString stringWithUTF8String : symbole.c_str()];
            NSLocale *nslocale = [[NSLocale alloc] initWithLocaleIdentifier:nssymbole];
            NSNumberFormatter* numfmt = [[NSNumberFormatter alloc] init];
            [numfmt setFormatterBehavior:NSNumberFormatterBehavior10_4];
            [numfmt setNumberStyle:NSNumberFormatterCurrencyStyle];
            [numfmt setLocale:nslocale];
            
            float payValue = atof(dollar.c_str());
            payValue *=_exRate;
            NSDecimalNumber *price = [NSDecimalNumber numberWithFloat:payValue];
            NSString* fmtString = [numfmt stringFromNumber:price];
            [numfmt release];
            returnString =  [fmtString UTF8String];
        }
    }
    return returnString;
}