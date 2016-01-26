//
//  NSString+JSONCategories.m
//  IF
//
//  Created by 马纵驰 on 15/10/8.
//
//

#import "NSString+JSONCategories.h"

@implementation NSString (JSONCategories)
-(id)JSONValue
{
    NSData* data = [self dataUsingEncoding:NSUTF8StringEncoding];
    __autoreleasing NSError* error = nil;
    id result = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&error];
    if (error != nil) return nil;
    return result;
}
@end
