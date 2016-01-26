//
//  MailNewUI.h
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import <Foundation/Foundation.h>

@interface MailNewUI : NSObject
+ (MailNewUI *)sharedMailNewUI;

-(NSString*) gettingIconByName:(NSString*) name;
@property (nonatomic,strong) NSMutableDictionary *iosNewUIMap;
@end
