/**
 * Copyright (c) 2014-2015 by Helpshift, Inc. All Rights Reserved.
 *
 */

#include "HelpshiftCocos2dx.h"
#include <string>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>


using namespace std;

static void (*alertToRateAppListener) (int result);
static void (*didReceiveNotificationCount) (int result);

static void (*helpshiftSessionBeganListener) () = NULL;
static void (*helpshiftSessionEndedListener) () = NULL;
static void (*newConversationStartedListener) (const char *message) = NULL;
static void (*userRepliedToConversationListener) (const char *message) = NULL;
static void (*userCompletedCustomerSatisfactionSurveyListener) (int rating, const char *message) = NULL;

/*! \brief Parses the tags array
 *         from the metaData and creates a Java ArrayList
 *
 *  \param env The JNIEnv
 *  \param metaData The metaData dictionary which contains the tags array
 */
jobject parseTagsArray (JNIEnv *env, cocos2d::CCDictionary* metaData) {
    if(metaData == NULL) {
        return NULL;
    }
    const char* arraylist_class_name = "java/util/ArrayList";
    jclass clsArrayList = env->FindClass(arraylist_class_name);
    jmethodID arrayConstructorID = env->GetMethodID (clsArrayList, "<init>", "()V");
    jobject jarrayobj = env->NewObject(clsArrayList, arrayConstructorID);
    jmethodID array_add_method = 0;
    array_add_method = env->GetMethodID(clsArrayList, "add", "(Ljava/lang/Object;)Z");

    cocos2d::CCArray* tags = dynamic_cast<cocos2d::CCArray*> (metaData->objectForKey(HS_TAGS_KEY));
    int j = 0;
    for (j = 0; j < tags->count(); j++) {
        cocos2d::CCString *valStr = dynamic_cast<cocos2d::CCString*> (tags->objectAtIndex(j));
        if (valStr != NULL && valStr->length() > 0) {
            jstring value = env->NewStringUTF(valStr->getCString());
            env->CallBooleanMethod(jarrayobj, array_add_method, value);
        }
    }
    return jarrayobj;
}


/*! \brief Parses the config dictionary
 *         and created a Java HashMap to pass to the native layer
 *
 *  \param env The JNIEnv
 *  \param config The config dictionary which contains various config options which accepted by the SDK
 */
jobject parseConfigDictionary (JNIEnv* env, cocos2d::CCDictionary *config) {
    if(config == NULL) {
        return NULL;
    }
    const char* hashmap_class_name = "java/util/HashMap";
    jclass clsHashMap = env->FindClass(hashmap_class_name);
    jmethodID constructorID = env->GetMethodID (clsHashMap, "<init>", "()V");

    jobject jmapobj = env->NewObject(clsHashMap, constructorID);

    jmethodID map_put_method = 0;
    map_put_method = env->GetMethodID(clsHashMap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    jstring key = env->NewStringUTF(HS_ENABLE_IN_APP_NOTIFICATION);
    jstring data;

    cocos2d::CCString* enableInAppStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_ENABLE_IN_APP_NOTIFICATION));
    if (enableInAppStr != NULL && enableInAppStr->length() > 0) {
        data = env->NewStringUTF(enableInAppStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_ENABLE_CONTACT_US);
    cocos2d::CCString* enableContactUsStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_ENABLE_CONTACT_US));
    if (enableContactUsStr != NULL && enableContactUsStr->length() > 0) {
        data = env->NewStringUTF(enableContactUsStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_GOTO_CONVERSATION_AFTER_CONTACT_US);
    cocos2d::CCString* gotoConvStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_GOTO_CONVERSATION_AFTER_CONTACT_US));
    if (gotoConvStr != NULL && gotoConvStr->length() > 0) {
        data = env->NewStringUTF(gotoConvStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_REQUIRE_EMAIL);
    cocos2d::CCString* requireEmailStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_REQUIRE_EMAIL));
    if (requireEmailStr != NULL && requireEmailStr->length() > 0) {
        data = env->NewStringUTF(requireEmailStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_HIDE_NAME_AND_EMAIL);
    cocos2d::CCString* hideNameEmailStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_HIDE_NAME_AND_EMAIL));
    if (hideNameEmailStr != NULL && hideNameEmailStr->length() > 0) {
        data = env->NewStringUTF(hideNameEmailStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_META_DATA_KEY);
    cocos2d::CCDictionary* metaData = dynamic_cast<cocos2d::CCDictionary*>(config->objectForKey(HS_META_DATA_KEY));
    if (metaData!= NULL) {
        jobject metaMap = env->NewObject(clsHashMap, constructorID);
        cocos2d::CCArray *keys = metaData->allKeys();
        int i = 0;
        for(i = 0; i < keys->count(); i++) {
            cocos2d::CCString* keyStr = dynamic_cast<cocos2d::CCString*> (keys->objectAtIndex(i));
            if (keyStr != NULL && keyStr->length() > 0) {
                jstring key = env->NewStringUTF(keyStr->getCString());
                cocos2d::CCString *valStr = dynamic_cast<cocos2d::CCString*> (metaData->objectForKey(keyStr->getCString()));
                if(valStr != NULL && valStr->length() > 0) {
                    jstring value = env->NewStringUTF(valStr->getCString());
                    env->CallObjectMethod(metaMap, map_put_method, key, value);
                } else if (valStr == NULL) {
                    env->CallObjectMethod(metaMap, map_put_method, key, parseTagsArray(env, metaData));
                }
            }
        }
        env->CallObjectMethod(jmapobj, map_put_method, key, metaMap);
    }

    key = env->NewStringUTF(HS_CONVERSATION_PREFILL_TEXT);
    cocos2d::CCString* prefillStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_CONVERSATION_PREFILL_TEXT));
    if (prefillStr != NULL && prefillStr->length() > 0) {
        data = env->NewStringUTF(prefillStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_SHOW_SEARCH_ON_NEW_CONVERSATION);
    cocos2d::CCString* showSearchOnConversationStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_SHOW_SEARCH_ON_NEW_CONVERSATION));
    if (showSearchOnConversationStr != NULL && showSearchOnConversationStr->length() > 0) {
        data = env->NewStringUTF(showSearchOnConversationStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_ENABLE_FULL_PRIVACY);
    cocos2d::CCString* fullPrivacyStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_ENABLE_FULL_PRIVACY));
    if (fullPrivacyStr != NULL && fullPrivacyStr->length() > 0) {
        data = env->NewStringUTF(fullPrivacyStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }

    key = env->NewStringUTF(HS_ENABLE_DIALOG_UI_FOR_TABLETS);
    cocos2d::CCString* dialogUIStr = dynamic_cast<cocos2d::CCString*>(config->objectForKey(HS_ENABLE_DIALOG_UI_FOR_TABLETS));
    if (dialogUIStr != NULL && dialogUIStr->length() > 0) {
        data = env->NewStringUTF(dialogUIStr->getCString());
        env->CallObjectMethod(jmapobj, map_put_method, key, data);
    }


    return jmapobj;
}


/*! \brief You can use this api call to provide a way
 *         for the user to send feedback or start a new conversation with you.
 *
 */
void HelpshiftCocos2dx::showConversation(void) {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showConversation",
                                                             "()V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}


/*! \brief You can use this api call to provide a way
 *         for the user to send feedback or start a new conversation with you.
 * There is one flag supported in this config -
 * gotoConversationAfterContactUs (Default : "no")
 * If set to "yes". Helpshift SDK will land on conversation activity after starting a new conversation. If set to "no" Helpshift SDK will land on last activity before starting a new conversation.
 *
 * \param config Extra config
 */
void HelpshiftCocos2dx::showConversation(cocos2d::CCDictionary *config) {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showConversation",
                                                             "(Ljava/util/HashMap;)V");
    if (hasMethod) {
        jobject hashMap = parseConfigDictionary(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, hashMap);
    }
}


/*! \brief Shows FAQ section activity.
 * This will show a FAQ section view with list of questions in that section. The search inside this view will be limited to the specified section. You can specify a section using publish ID of that section.
 *
 * \param sectionPublishId id specifying a section
 */
void HelpshiftCocos2dx::showFAQSection(const char *sectionPublishId) {
    if(sectionPublishId == NULL || strlen(sectionPublishId) == 0) {
        return;
    }
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showFAQSection",
                                                             "(Ljava/lang/String;)V");
    if (hasMethod) {
        jstring sectionPubId = minfo.env->NewStringUTF(sectionPublishId);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        sectionPubId);
    }
}


/*! \brief Shows FAQ section activity.
 * This will show a FAQ section view with list of questions in that section. The search inside this view will be limited to the specified section. You can specify a section using publish ID of that section.
 *
 * There are two flags supported in this config -
 * gotoConversationAfterContactUs (Default : "no") If set to "yes". Helpshift SDK will land on conversation activity after starting a new conversation. If set to "no" Helpshift SDK will land on last activity before starting a new conversation.
 * enableContactUs (Default : "yes") The enableContactUs flag will determine whether the Contact Us button is shown.
 * requireEmail (Default : "no" ) If requireEmail flag is set to "yes", an e-mail address is required while starting a new conversation.
 * \param sectionPublishId id specifying a section
 * \param config Additional config
 */
void HelpshiftCocos2dx::showFAQSection(const char *sectionPublishId, cocos2d::CCDictionary *config) {
    if(sectionPublishId == NULL || strlen(sectionPublishId) == 0) {
        return;
    }

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showFAQSection",
                                                             "(Ljava/lang/String;Ljava/util/HashMap;)V");
    if (hasMethod) {
        jstring sectionPubId = minfo.env->NewStringUTF(sectionPublishId);
        jobject hashMap = parseConfigDictionary(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        sectionPubId,
                                        hashMap);
    }
}


/*! \brief Shows question activity.
 *         This provides show question view provided a publish id of that question.
 *
 * \param publishId id specifying a question
 *
 */
void HelpshiftCocos2dx::showSingleFAQ(const char *publishId) {
    if(publishId == NULL || strlen(publishId) == 0) {
        return;
    }

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showSingleFAQ",
                                                             "(Ljava/lang/String;)V");
    if (hasMethod) {
        jstring pubId = minfo.env->NewStringUTF(publishId);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, pubId);
    }
}


/*! \brief Shows question activity.
 *         This provides show question view provided a publish id of that question.
 *
 * There are two flags supported in this config -
 * gotoConversationAfterContactUs (Default : "no") If set to "yes". Helpshift SDK will land on conversation activity after starting a new conversation. If set to "no" Helpshift SDK will land on last activity before starting a new conversation.
 * enableContactUs (Default : "yes") The enableContactUs flag will determine whether the Contact Us button is shown.
 * requireEmail (Default : "no" ) If requireEmail flag is set to "yes", an e-mail address is required while starting a new conversation.
 *
 * \param publishId id specifying a question
 * \param config Additional config
 */
void HelpshiftCocos2dx::showSingleFAQ(const char *publishId, cocos2d::CCDictionary *config) {
    if(publishId == NULL || strlen(publishId) == 0) {
        return;
    }

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showSingleFAQ",
                                                             "(Ljava/lang/String;Ljava/util/HashMap;)V");
    if (hasMethod) {
        jstring pubId = minfo.env->NewStringUTF(publishId);
        jobject hashMap = parseConfigDictionary(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        pubId,
                                        hashMap);
    }
}


/*! \brief Shows faqs activity. This will show list of sections with search.
 *
 */
void HelpshiftCocos2dx::showFAQs() {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showFAQs",
                                                             "()V");
    if (hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}


/*! \brief Shows faqs activity. This will show list of sections with search.
 *
 * There are two flags supported in this config -
 * gotoConversationAfterContactUs (Default : "no") If set to "yes". Helpshift SDK will land on conversation activity after starting a new conversation. If set to "no" Helpshift SDK will land on last activity before starting a new conversation.
 * enableContactUs (Default : "yes") The enableContactUs flag will determine whether the Contact Us button is shown.
 * requireEmail (Default : "no" ) If requireEmail flag is set to "yes", an e-mail address is required while starting a new conversation.
 *
 * \param config Additional config
 */
void HelpshiftCocos2dx::showFAQs(cocos2d::CCDictionary *config) {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showFAQs",
                                                             "(Ljava/util/HashMap;)V");
    if (hasMethod) {
        jobject hashMap = parseConfigDictionary(minfo.env, config);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, hashMap);
    }
}


/*! \brief API to login a user to the Help section.
 * \param identifier The unique id for the login
 * \param name User name
 * \param email User email
 */
void HelpshiftCocos2dx::login(const char *identifier, const char *name, const char *email) {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "login",
                                                             "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if(hasMethod) {
        jstring idStr = minfo.env->NewStringUTF(identifier);
        jstring nameStr = minfo.env->NewStringUTF(name);
        jstring emailStr = minfo.env->NewStringUTF(email);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        idStr,
                                        nameStr,
                                        emailStr);
    }
}

/*! \brief API to logout the current user
 */
void HelpshiftCocos2dx::logout() {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "logout",
                                                             "()V");
    if(hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID);
    }
}

/*! \brief You can specify the name and email for your User.
 * If you want to reset previously set values, please provide NULL for both params
 * \param name User name
 * \param email User email
 */
void HelpshiftCocos2dx::setNameAndEmail(const char *name, const char *email) {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "setNameAndEmail",
                                                             "(Ljava/lang/String;Ljava/lang/String;)V");
    if(hasMethod) {
        jstring nameStr = minfo.env->NewStringUTF(name);
        jstring emailStr = minfo.env->NewStringUTF(email);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        nameStr,
                                        emailStr);
    }
}


/*! \brief If you already have indentification for your users, you can specify that as well.
 *
 * \param userIdentifier A custom user identifier
 *
 */
void HelpshiftCocos2dx::setUserIdentifier(const char *userIdentifier) {
    if(userIdentifier == NULL || strlen(userIdentifier) == 0) {
        return;
    }
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "setUserIdentifier",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        jstring idStr = minfo.env->NewStringUTF(userIdentifier);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, idStr);
    }
}


/*! \brief If you are using GCM push or Urban Airship and if you want to enable Push Notification in the Helpshift Android SDK, set the Android Push ID (APID) using this method
 *
 * \param deviceToken The Android Push Id
 */
void HelpshiftCocos2dx::registerDeviceToken(const char *deviceToken) {
    if(deviceToken == NULL || strlen(deviceToken) == 0) {
        return;
    }

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "registerDeviceToken",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        jstring devTokenStr = minfo.env->NewStringUTF(deviceToken);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        devTokenStr);
    }
}


/*! \brief Add additonal debugging information in your code.
 * You can add additional debugging statements to your code,
 * and see exactly what the user was doing right before they started a new conversation.
 *
 * \param breadCrumb Action/Message to add to bread-crumbs list.
 */
void HelpshiftCocos2dx::leaveBreadCrumb(const char *breadCrumb) {
    if(breadCrumb == NULL || strlen(breadCrumb) == 0) {
        return;
    }

    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "leaveBreadCrumb",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        jstring breadCrumbStr = minfo.env->NewStringUTF(breadCrumb);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        breadCrumbStr);
    }
}


/*! \brief Show an alert to let the user rate your app
 *
 * \param url The url of your app on the play store
 * \param action A callback function which takes a HSAlertToRateAppAction param
 * and returns void. This callback function will be called to let the developer
 * know what the user's response was to the Alert.
 */
void HelpshiftCocos2dx::showAlertToRateApp(const char *url,
                                           void (*action) (int result)) {
    if(url == NULL || strlen(url) == 0) {
        return;
    }
    if (action) {
        alertToRateAppListener = action;
    }
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "showAlertToRateApp",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        jstring idStr = minfo.env->NewStringUTF(url);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, idStr);
    }
}


/*! \brief Clears Breadcrumbs list. Breadcrumbs list stores upto 100 latest actions. You'll receive those in every Issue. But if for reason you want to clear previous messages (On app load, for eg), you can do that by calling this api.
 *
 */
void HelpshiftCocos2dx::clearBreadCrumbs() {
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "clearBreadCrumbs",
                                                             "()V");
    if(hasMethod) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}


/*! \brief Gets notification count
 *
 * \return The count of new notifications for updates to the conversation
 *
 */
int HelpshiftCocos2dx::getNotificationCount(bool isAsync, void (*receiver) (int result)) {
    cocos2d::JniMethodInfo minfo;
    if(receiver) {
        didReceiveNotificationCount = receiver;
    }

    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "getNotificationCount",
                                                             "(Z)I");
    if(hasMethod) {
        return minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID, isAsync);
    }
}


/*! \brief If you receive a push notification from the Helpshift server, the "origin" field of the notification will be set to "helpshift". In such a case, you can forward the notification to Helpshift so that the relevant issue chat screen is opened. To forward the push notification to the Helpshift sdk, please use this api.
 *
 * \param issueId The "issue_id" field of the received notification dictionary
 */
void HelpshiftCocos2dx::handlePush(const char *issueId) {
    if (issueId == NULL || strlen(issueId) == 0) {
        return;
    }
    cocos2d::JniMethodInfo minfo;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/HelpshiftBridge",
                                                             "handlePush",
                                                             "(Ljava/lang/String;)V");
    if(hasMethod) {
        jstring issueIdStr = minfo.env->NewStringUTF(issueId);
        minfo.env->CallStaticVoidMethod(minfo.classID,
                                        minfo.methodID,
                                        issueIdStr);
    }
}

void HelpshiftCocos2dx::registerSessionDelegates (void (*sessionBeganListener)(),
                                                  void (*sessionEndedListener)()) {
    if (sessionBeganListener != NULL) {
        helpshiftSessionBeganListener = sessionBeganListener;
    }
    if (sessionEndedListener != NULL) {
        helpshiftSessionEndedListener = sessionEndedListener;
    }
}

void HelpshiftCocos2dx::registerConversationDelegates (void (*newConversationStartedListenerArg)(const char *message),
                                                       void (*userRepliedToConversationListenerArg)(const char *message),
                                                       void (*userCompletedCustomerSatisfactionSurveyListenerArg)(int rating, const char *feedback)) {
    if(newConversationStartedListenerArg != NULL) {
        newConversationStartedListener = newConversationStartedListenerArg;
    }

    if(userRepliedToConversationListenerArg != NULL) {
        userRepliedToConversationListener = userRepliedToConversationListenerArg;
    }

    if(userCompletedCustomerSatisfactionSurveyListenerArg != NULL) {
        userCompletedCustomerSatisfactionSurveyListener = userCompletedCustomerSatisfactionSurveyListenerArg;
    }
}


/*! \brief Calls the Helpshift log class with provided arguments
 *
 * \param logFunction corresponds to the log level
 * \param tagString the debug tag to use
 * \param logString the log message
 */
int logger (const char* logFunction, const char *tagString, const char* logString) {
    if(tagString == NULL || logString == NULL ||
       strlen(logString) == 0) {
        return -1;
    }

    cocos2d::JniMethodInfo minfo;
    int retVal;
    bool hasMethod = cocos2d::JniHelper::getStaticMethodInfo(minfo,
                                                             "com/helpshift/Log",
                                                             logFunction,
                                                             "(Ljava/lang/String;Ljava/lang/String;)I");
    if(hasMethod) {
        jstring logStr = minfo.env->NewStringUTF(logString);
        jstring tagStr = minfo.env->NewStringUTF(tagString);
        retVal = minfo.env->CallStaticIntMethod(minfo.classID,
                                                minfo.methodID,
                                                tagStr,
                                                logStr);
    }
    return retVal;
}


/*! \brief Adds logs with debug level
 */
int HelpshiftCocos2dx::logd(const char *tag, const char *format, ...) {
    va_list arg;
    va_start (arg, format);
    char *logString = NULL;
    vasprintf (&logString, format, arg);
    va_end (arg);
    return logger("d", tag, logString);
}


/*! \brief Adds logs with info level
 */
int HelpshiftCocos2dx::logi(const char *tag, const char *format, ...) {
    va_list arg;
    va_start (arg, format);
    char *logString = NULL;
    vasprintf (&logString, format, arg);
    va_end (arg);
    return logger("i", tag, logString);
}

/*! \brief Adds logs with warn level
 */
int HelpshiftCocos2dx::logw(const char *tag, const char *format, ...) {
    va_list arg;
    va_start (arg, format);
    char *logString = NULL;
    vasprintf (&logString, format, arg);
    va_end (arg);
    return logger("w", tag, logString);
}

/*! \brief Adds logs with verbose level
 */
int HelpshiftCocos2dx::logv(const char *tag, const char *format, ...) {
    va_list arg;
    va_start (arg, format);
    char *logString = NULL;
    vasprintf (&logString, format, arg);
    va_end (arg);
    return logger("v", tag, logString);
}


extern "C" {
    void Java_com_helpshift_HelpshiftBridge_alertToRateAppAction (JNIEnv *env,
                                                                  jobject object,
                                                                  int message) {
        if(alertToRateAppListener) {
            alertToRateAppListener(message);
        }
    }

    void Java_com_helpshift_HelpshiftBridge_didReceiveNotificationCount (JNIEnv *env,
                                                                         jobject object,
                                                                         int message) {
        if(didReceiveNotificationCount) {
            didReceiveNotificationCount(message);
        }
    }

    void Java_com_helpshift_HelpshiftBridge_helpshiftSessionBegan (JNIEnv *env,
                                                                   jobject object) {
        if(helpshiftSessionBeganListener) {
            helpshiftSessionBeganListener();
        }
    }

    void Java_com_helpshift_HelpshiftBridge_helpshiftSessionEnded (JNIEnv *env,
                                                                   jobject object) {
        if(helpshiftSessionEndedListener) {
            helpshiftSessionEndedListener();
        }
    }

    void Java_com_helpshift_HelpshiftBridge_newConversationStarted (JNIEnv *env,
                                                                    jobject object,
                                                                    jstring messageString) {
        const char *message  = env->GetStringUTFChars(messageString, NULL);
        if(newConversationStartedListener) {
            newConversationStartedListener(message);
        }
    }

    void Java_com_helpshift_HelpshiftBridge_userRepliedToConversation (JNIEnv *env,
                                                                       jobject object,
                                                                       jstring messageString) {
        const char *message  = env->GetStringUTFChars(messageString, NULL);
        if(userRepliedToConversationListener) {
            userRepliedToConversationListener(message);
        }
    }

    void Java_com_helpshift_HelpshiftBridge_userCompletedCustomerSatisfactionSurvey (JNIEnv *env,
                                                                                     jobject object,
                                                                                     int rating,
                                                                                     jstring feedbackString) {
        const char *feedback = env->GetStringUTFChars(feedbackString, NULL);
        if(userCompletedCustomerSatisfactionSurveyListener) {
            userCompletedCustomerSatisfactionSurveyListener(rating, feedback);
        }
    }

}
#endif
