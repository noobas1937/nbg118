//
//  SNSUtil.h
//  IF
//
//  Created by 王智圆 on 15/7/29.
//
//

#ifndef __IF__SNSUtil__
#define __IF__SNSUtil__

#ifdef __OBJC__
#import <UIKit/UIKit.h>
#import <VKSdk/VKSdk.h>
@interface SNSUtiliesIOS:UIViewController <VKSdkDelegate>
@end
#endif

#include <string.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCTypesExt.h"
#include "CCDevice.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#   include <jni.h>
#   include "platform/android/jni/JniHelper.h"
#endif

namespace SNSUtilies{
    void doLogin(std::string pf);
//    void sendTextShare(std::string pf,std::string shareText);
};

#endif /* defined(__IF__SNSUtil__) */
