/*
 *    HelpshiftCampaigns.h
 *    SDK Version 5.2.2
 *
 *    Get the documentation at http://www.helpshift.com/docs
 *
 */

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import "HelpshiftCore.h"

@interface HelpshiftCampaigns : NSObject <HsApiProvider>
{
    BOOL isInitialized;
}

/**
 *  Return the shared instance object of the HelpshiftCampaigns class
 *
 *  @return object of HelpshiftCampaigns class
 */
+ (HelpshiftCampaigns *) sharedInstance;

/**
 *  Add an integer property for current user
 *
 *  @param key   name of the key with which you want to associate data
 *  @param value integer value of the key
 *
 *  @return YES if property key is valid, NO otherwise
 */
+ (BOOL) addProperty:(NSString *)key withInteger:(NSInteger)value;


/**
 *  Add a string property for current user
 *
 *  @param key   name of the key with which you want to associate data
 *  @param value string value of the key
 *
 *  @return YES if property key is valid, NO otherwise
 */
+ (BOOL) addProperty:(NSString *)key withString:(NSString *)value;

/**
 *  Add a boolean property for current user
 *
 *  @param key   name of the key with which you want to associate data
 *  @param value BOOL value of the key
 *
 *  @return YES if property key is valid, NO otherwise
 */
+ (BOOL) addProperty:(NSString *)key withBoolean:(BOOL)value;

/**
 *  Add a Date property for current user
 *
 *  @param key   name of the key with which you want to associate data
 *  @param value NSDate value of the key
 *
 *  @return YES if property key is valid, NO otherwise
 */
+ (BOOL) addProperty:(NSString *)key withDate:(NSDate *)value;

/**
 *  Add multiple properties at once for the current user
 *
 *  @param keyValues A dictionary of keys and values which represent properties to be added for the current user
 *
 *  @return Array of strings which represent valid keys.
 */
+ (NSArray *) addProperties:(NSDictionary *)keyValues;
@end
