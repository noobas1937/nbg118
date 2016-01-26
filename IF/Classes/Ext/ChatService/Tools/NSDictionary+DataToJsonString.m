//
//  NSDictionary+DataToJsonString.m
//  IF
//
//  Created by 马纵驰 on 15/10/10.
//
//

#import "NSDictionary+DataToJsonString.h"

@implementation NSDictionary (DataToJsonString)
-(NSString*)dataTOjsonString
{
    NSString *jsonString = nil;
    NSError *error;
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:self
                                                       options:NSJSONWritingPrettyPrinted // Pass 0 if you don't care about the readability of the generated string
                                                         error:&error];
    if (! jsonData) {
        DVLog(@"Got an error: %@", error);
    } else {
        jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
    }
    return jsonString;
}
@end
