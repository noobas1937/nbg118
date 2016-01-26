//
//  CSShowTimeTool.m
//  IF
//
//  Created by 张彦涛 on 15/10/16.
//
//

#import "CSShowTimeTool.h"
#import "NSDateFormatter+Category.h"

@implementation CSShowTimeTool
+(void)saveLogToFile:(NSString *)vString{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSArray *directoryPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentDirectory = [directoryPaths objectAtIndex:0];
    
    NSString *filePath = [documentDirectory stringByAppendingPathComponent:@"CSTimeLOG.txt"];
    if ( ![fileManager fileExistsAtPath:filePath]) {
        
        [fileManager createFileAtPath:filePath contents:nil attributes:nil];
        
    }
    //    NSString *s =[NSString stringWithFormat:@"%s" ,__FILE__,__LINE__ ,__LINE__];
    NSString *fileString =[[NSDateFormatter custemDebugDateFormatter] stringFromDate:[NSDate date]];
    fileString = [fileString stringByAppendingString:@"       "];
    fileString  = [fileString stringByAppendingString:vString ];
    fileString = [fileString stringByAppendingString:@"\n"];
    
    
    
    NSFileHandle * fh = [NSFileHandle fileHandleForWritingAtPath:filePath];
    [fh seekToEndOfFile];
    [fh writeData:[fileString dataUsingEncoding:NSUTF8StringEncoding]];
    
}
@end
