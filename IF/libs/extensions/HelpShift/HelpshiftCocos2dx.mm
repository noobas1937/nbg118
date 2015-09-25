//
//  HelpshiftCocos2dx.mm
//  Helpshift Cocos2dx iOS SDK
//

#include "HelpshiftCocos2dx.h"
#include "Helpshift.h"

#include "HelpshiftCocos2dxDelegate.h"

static NSString* hsParseCString(const char *cstring);
static NSMutableDictionary *hsParseConfig(cocos2d::CCDictionary* dict);
static void hsAddCCObjectToNSDict(const char*key, cocos2d::CCObject* object, NSMutableDictionary *dict);
static void (*alertToRateAppAction) (int result);

/*! \brief Initialize helpshift support
 *
 * When initializing Helpshift you must pass these three tokens. You initialize Helpshift by adding the following lines in the implementation file for your app delegate, ideally at the top of AppDelegate::applicationDidFinishLaunching(). If you use this api to initialize helpshift support, in-app notifications will be enabled by default.
 *
 * \param apiKey This is your developer API Key
 * \param domainName This is your domain name without any http:// or forward slashes
 * \param appID This is the unique ID assigned to your app
 */
void HelpshiftCocos2dx::install(const char *apiKey,
                                        const char *domainName,
                                        const char *appID) {

    [Helpshift installForApiKey:hsParseCString(apiKey)
                     domainName:hsParseCString(domainName)
                          appID:hsParseCString(appID)];
    [[Helpshift sharedInstance] setDelegate:[HelpshiftCocos2dxDelegate sharedInstance]];
}

/*! \brief Initialize helpshift support
 *
 * When initializing Helpshift you must pass these three tokens. You initialize Helpshift by adding the following lines in the implementation file for your app delegate, ideally at the top of AppDelegate::applicationDidFinishLaunching().
 *
 * \param apiKey This is your developer API Key
 * \param domainName This is your domain name without any http:// or forward slashes
 * \param appID This is the unique ID assigned to your app
 * \param config This is the dictionary which contains additional configuration options for the HelpshiftSDK. Currently we support the "enableInAppNotification" as the only available option. Possible values are <"yes"/"no">. If you set the flag to "yes", the helpshift SDK will show notifications similar to the banner notifications supported by Apple Push notifications. These notifications will alert the user of any updates to ongoing conversations. If you set the flag to "no", the in-app notifications will be disabled.
 */
void HelpshiftCocos2dx::install(const char *apiKey,
                                        const char *domainName,
                                        const char *appID,
                                        cocos2d::CCDictionary *config) {

    [Helpshift installForApiKey:hsParseCString(apiKey)
                     domainName:hsParseCString(domainName)
                          appID:hsParseCString(appID)
                    withOptions:hsParseConfig(config)];
    [[Helpshift sharedInstance] setDelegate:[HelpshiftCocos2dxDelegate sharedInstance]];
}

/*! \brief Shows faqs screen. This will show list of sections with search.
 *
 */
void HelpshiftCocos2dx::showFAQs() {
    [[Helpshift sharedInstance] showFAQs:[UIApplication sharedApplication].keyWindow.rootViewController
                             withOptions:nil];
}

/*! \brief Shows faqs screen. This will show list of sections with search.
 *
 * There are 2 flags supported in this config -
 * gotoConversationAfterContactUs (Default : "no") If set to "yes". Helpshift SDK will land on conversation activity after starting a new conversation. If set to "no" Helpshift SDK will land on last activity before starting a new conversation.
 * enableContactUs (Default : "yes") The enableContactUs flag will determine whether the Contact Us button is shown.
 *
 * \param config Additional config
 */
void HelpshiftCocos2dx::showFAQs(cocos2d::CCDictionary *config) {

    [[Helpshift sharedInstance] showFAQs:[UIApplication sharedApplication].keyWindow.rootViewController
                             withOptions:hsParseConfig(config)];
}

/*! \brief You can use this api call to provide a way
 *         for the user to send feedback or start a new conversation with you.
 *
 */
void HelpshiftCocos2dx::showConversation() {

   [[Helpshift sharedInstance] showConversation:[UIApplication sharedApplication].keyWindow.rootViewController
                                    withOptions:nil];

}

/*! \brief You can use this api call to provide a way
 *         for the user to send feedback or start a new conversation with you.
 * There is one flag supported in this config -
 * gotoConversationAfterContactUs (Default : "no")
 * If set to "yes". Helpshift SDK will land on conversation activity after starting a new conversation. If set to "no" Helpshift SDK will land on last activity before starting a new conversation.
 *
 * \param config Extra config
 */
void HelpshiftCocos2dx::showConversation(cocos2d::CCDictionary* config) {

    [[Helpshift sharedInstance] showConversation:[UIApplication sharedApplication].keyWindow.rootViewController
                                     withOptions:hsParseConfig(config)];

}

/*! \brief Shows FAQ section screen.
 * This will show a FAQ section view with list of questions in that section. The search inside this view will be limited to the specified section. You can specify a section using publish ID of that section.
 *
 * \param sectionPublishId id specifying a section
 */
void HelpshiftCocos2dx::showFAQSection(const char *sectionPublishId) {

    [[Helpshift sharedInstance] showFAQSection:hsParseCString(sectionPublishId)
                                withController:[UIApplication sharedApplication].keyWindow.rootViewController
                                   withOptions:nil];

}

/*! \brief Shows FAQ section screen.
 * This will show a FAQ section view with list of questions in that section. The search inside this view will be limited to the specified section. You can specify a section using publish ID of that section.
 *
 * There are 2 flags supported in this config -
 * gotoConversationAfterContactUs (Default : "no") If set to "yes". Helpshift SDK will land on conversation activity after starting a new conversation. If set to "no" Helpshift SDK will land on last activity before starting a new conversation.
 * enableContactUs (Default : "yes") The enableContactUs flag will determine whether the Contact Us button is shown.
 *
 * \param sectionPublishId id specifying a section
 * \param config Additional config
 */
void HelpshiftCocos2dx::showFAQSection(const char *sectionPublishId, cocos2d::CCDictionary *config) {

    [[Helpshift sharedInstance] showFAQSection:hsParseCString(sectionPublishId)
                                withController:[UIApplication sharedApplication].keyWindow.rootViewController
                                   withOptions:hsParseConfig(config)];

}

/*! \brief Shows question screen.
 *         This provides show question view provided a publish id of that question.
 *
 * \param publishId id specifying a question
 *
 */
void HelpshiftCocos2dx::showSingleFAQ(const char *publishId) {

    [[Helpshift sharedInstance] showSingleFAQ:hsParseCString(publishId)
                                withController:[UIApplication sharedApplication].keyWindow.rootViewController
                                   withOptions:nil];

}

/*! \brief Shows question screen.
 *         This provides show question view provided a publish id of that question.
 *
 * There are 2 flags supported in this config -
 * gotoConversationAfterContactUs (Default : "no") If set to "yes". Helpshift SDK will land on conversation activity after starting a new conversation. If set to "no" Helpshift SDK will land on last activity before starting a new conversation.
 * enableContactUs (Default : "yes") The enableContactUs flag will determine whether the Contact Us button is shown.
 *
 * \param publishId id specifying a question
 * \param config Additional config
 */
void HelpshiftCocos2dx::showSingleFAQ(const char *publishId, cocos2d::CCDictionary *config) {

    [[Helpshift sharedInstance] showSingleFAQ:hsParseCString(publishId)
                               withController:[UIApplication sharedApplication].keyWindow.rootViewController
                                  withOptions:hsParseConfig(config)];

}

/*! \brief To pause and restart the display of inapp notification
 *         This provides show question view provided a publish id of that question.
 *
 * When this method is called with boolean value true, inapp notifications are paused and not displayed.
 * To restart displaying inapp notifications pass the boolean value false.
 *
 * \param pauseInApp the boolean value to pause/restart inapp nofitications
 */
void HelpshiftCocos2dx::pauseDisplayOfInAppNotification(bool pauseInApp) {

    [Helpshift pauseDisplayOfInAppNotification:pauseInApp];

}

/*! \brief Login a user with a given identifier
 *
 * This api introduces support for multiple logins in Helpshift.
 * The identifier uniquely identifies the user. Name and email are optional.
 * If any Helpshift session is active, then any login attempt is ignored.
 *
 * \param identifier The unique identifier for the use
 * \param name The name of the user
 * \param email The email of the user
 *
 * @available Available in SDK version 4.10.0 or later
 *
 */

void HelpshiftCocos2dx::login(const char *identifier, const char *name, const char *email) {
    [Helpshift loginWithIdentifier:hsParseCString(identifier) withName:hsParseCString(name) andEmail:hsParseCString(email)];
}

void HelpshiftCocos2dx::logout() {
    [Helpshift logout];
}

/*! \brief You can specify the name and email for your User.
 * To reset the name and email values set previously, you can pass null for both params
 *
 * \param name User name
 * \param email User email
 */
void HelpshiftCocos2dx::setNameAndEmail(const char *name, const char *email) {

    [Helpshift setName:hsParseCString(name)
              andEmail:hsParseCString(email)];

}

/*! \brief If you already have indentification for your users, you can specify that as well.
 *
 * \param userIdentifier A custom user identifier
 *
 */
void HelpshiftCocos2dx::setUserIdentifier(const char *userIdentifier) {

    [Helpshift setUserIdentifier:hsParseCString(userIdentifier)];

}

/*! \brief Adds additonal debugging information in your code. You can add additional debugging statements to your code, and see exactly what the user was doing right before they started a new conversation.
 *
 * \param breadCrumb Action/Message to add to bread-crumbs list.
 */
void HelpshiftCocos2dx::leaveBreadCrumb(const char *breadCrumb) {

    [Helpshift leaveBreadCrumb:hsParseCString(breadCrumb)];

}

/*! \brief Clears Breadcrumbs list. Breadcrumbs list stores upto 100 latest actions. You'll receive those in every Issue. But if for reason you want to clear previous messages (On app load, for eg), you can do that by calling this api.
 *
 */
void HelpshiftCocos2dx::clearBreadCrumbs() {

    [[Helpshift sharedInstance] clearBreadCrumbs];

}

/*! \brief Gets notification count
 *
 * \return The count of new notifications for updates to the conversation
 *
 */
int HelpshiftCocos2dx::getNotificationCountFromRemote(bool isRemote, void (*notificationHandler) (int count)) {
    [HelpshiftCocos2dxDelegate sharedInstance].notificationCountHandler = notificationHandler;
    return [[Helpshift sharedInstance] getNotificationCountFromRemote:isRemote];
}

/*! \brief Set the inApp notification handler
 *
 *
 */
void HelpshiftCocos2dx::setInAppNotificationHandler(void (*inAppNotificationHandler) (int count)) {
    [HelpshiftCocos2dxDelegate sharedInstance].inAppNotificationCountHandler = inAppNotificationHandler;
}


/*! \brief Set the session delegates
 *
 *
 */
void HelpshiftCocos2dx::registerSessionDelegates(void (*sessionBeganHandler) (void), void (*sessionEndedHandler) (void)) {
    [HelpshiftCocos2dxDelegate sharedInstance].sessionBeganHandler = sessionBeganHandler;
    [HelpshiftCocos2dxDelegate sharedInstance].sessionEndedHandler = sessionEndedHandler;
}


void HelpshiftCocos2dx::registerConversationDelegates(void (*newConversationHandler) (const char *), void (*newMessageHandler) (const char *), void (*csatHandler) (int, const char *)) {
    [HelpshiftCocos2dxDelegate sharedInstance].newConversationHandler = newConversationHandler;
    [HelpshiftCocos2dxDelegate sharedInstance].newConversationMessageHandler = newMessageHandler;
    [HelpshiftCocos2dxDelegate sharedInstance].csatCompletedHandler = csatHandler;
}

/*! \brief Register the device token with Helpshift for getting Push notifications
 *
 * \param deviceToken The deviceToken for the device
 *
 */
void HelpshiftCocos2dx::registerDeviceToken(const char *deviceToken) {

    [[Helpshift sharedInstance] registerDeviceToken:[hsParseCString(deviceToken) dataUsingEncoding:NSUTF8StringEncoding]];

}

/*! \brief Forward the local notification information to Helpshift for opening the conversation
 *
 * \param issueId The issueId for which a new local notification was received
 *
 */
void HelpshiftCocos2dx::handleLocalNotification(const char *issueId) {

    NSString *issueID = hsParseCString(issueId);
    UILocalNotification *localNotif = [[[UILocalNotification alloc] init] autorelease];
    localNotif.userInfo = @{ @"issue_id": issueID };
    [[Helpshift sharedInstance] handleLocalNotification:localNotif
                                         withController:[UIApplication sharedApplication].keyWindow.rootViewController];

}

/*! \brief Forward the push notification information to Helpshift for opening the conversation
 *
 * \param notification The notification dictionary which was received
 *
 */
void HelpshiftCocos2dx::handleRemoteNotification(cocos2d::CCDictionary *notification) {

    [[Helpshift sharedInstance] handleRemoteNotification:hsParseConfig(notification)
                                          withController:[UIApplication sharedApplication].keyWindow.rootViewController];

}

/*! \brief Show alert for app rating
 *
 * To manually show an alert for app rating, you need automated reviews disabled in admin.
 * Also, if there is an ongoing conversation, the review alert will not show up.
 * The result of the interaction will be available through the callback which is supplied as param.
 *
 *  \param url The url of the app in the app store
 *  \param action Pointer to a callback function which takes and int and returns void.
 */

void HelpshiftCocos2dx::showAlertToRateApp(const char *url, void (*action) (int result)) {
    alertToRateAppAction = action;

    [Helpshift showAlertToRateAppWithURL:hsParseCString(url)
                     withCompletionBlock:^(HSAlertToRateAppAction action) {
                         if (alertToRateAppAction != nil) {
                             alertToRateAppAction(action);
                         }
                     }];
}


static NSString* hsParseCString(const char *cstring) {
    if (cstring == NULL) {
        return NULL;
    }
    NSString * nsstring = [[NSString alloc] initWithBytes:cstring
                                                   length:strlen(cstring)
                                                 encoding:NSUTF8StringEncoding];
    return [nsstring autorelease];
}

static NSMutableDictionary *hsParseConfig(cocos2d::CCDictionary* dict) {
    NSMutableDictionary *nsDict = [NSMutableDictionary dictionary];

#if ((COCOS2D_VERSION & 0x00FF0000) > 0x20000)
    cocos2d::DictElement *element = NULL;
#else
    cocos2d::CCDictElement *element = NULL;
#endif
    cocos2d::CCDICT_FOREACH(dict, element)
    {
        CCLOG("%s", element->getStrKey());
        hsAddCCObjectToNSDict(element->getStrKey(), element->getObject(), nsDict);
    }
    return nsDict;
}

static void hsAddCCObjectToNSArray(cocos2d::CCObject *object, NSMutableArray *array)
{
    // add string into array
    if (cocos2d::CCString *ccString = dynamic_cast<cocos2d::CCString *>(object)) {
        NSString *strElement = [NSString stringWithCString:ccString->getCString() encoding:NSUTF8StringEncoding];
        [array addObject:strElement];
        return;
    }

    // add array into array
    if (cocos2d::CCArray *ccArray = dynamic_cast<cocos2d::CCArray *>(object)) {
        NSMutableArray *arrElement = [NSMutableArray array];
        cocos2d::CCObject *element = NULL;
        CCARRAY_FOREACH(ccArray, element){
            hsAddCCObjectToNSArray(element, arrElement);
        }
        [array addObject:arrElement];
        return;
    }

    // add dictionary value into array
    if (cocos2d::CCDictionary *ccDict = dynamic_cast<cocos2d::CCDictionary *>(object)) {
        NSMutableDictionary *dictElement = [NSMutableDictionary dictionary];
#if ((COCOS2D_VERSION & 0x00FF0000) > 0x20000)
        cocos2d::DictElement *element = NULL;
#else
        cocos2d::CCDictElement *element = NULL;
#endif
        cocos2d::CCDICT_FOREACH(ccDict, element)
        {
            hsAddCCObjectToNSDict(element->getStrKey(), element->getObject(), dictElement);
        }
        [array addObject:dictElement];
    }

}

static void hsAddCCObjectToNSDict(const char * key, cocos2d::CCObject* object, NSMutableDictionary *dict)
{
    NSString *NSkey = [NSString stringWithCString:key encoding:NSUTF8StringEncoding];

    // the object is a CCDictionary
    if (cocos2d::CCDictionary *ccDict = dynamic_cast<cocos2d::CCDictionary *>(object)) {
        NSMutableDictionary *dictElement = [NSMutableDictionary dictionary];
#if ((COCOS2D_VERSION & 0x00FF0000) > 0x20000)
        cocos2d::DictElement *element = NULL;
#else
        cocos2d::CCDictElement *element = NULL;
#endif
        cocos2d::CCDICT_FOREACH(ccDict, element)
        {
            hsAddCCObjectToNSDict(element->getStrKey(), element->getObject(), dictElement);
        }
        [dict setObject:dictElement forKey:NSkey];
        return;
    }

    // the object is a CCString
    if (cocos2d::CCString *element = dynamic_cast<cocos2d::CCString *>(object)) {
        NSString *strElement = [NSString stringWithCString:element->getCString() encoding:NSUTF8StringEncoding];
        [dict setObject:strElement forKey:NSkey];
        return;
    }

    // the object is a CCArray
    if (cocos2d::CCArray *ccArray = dynamic_cast<cocos2d::CCArray *>(object)) {
        NSMutableArray *arrElement = [NSMutableArray array];
        cocos2d::CCObject *element = NULL;
        CCARRAY_FOREACH(ccArray, element){
            hsAddCCObjectToNSArray(element, arrElement);
        }
        [dict setObject:arrElement forKey:NSkey];
        return;
    }
}
