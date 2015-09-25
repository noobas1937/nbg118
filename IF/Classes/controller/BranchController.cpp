//
//  BranchController.cpp
//  IF
//
//  Created by liruiqi on 15/7/16.
//
//

#include "BranchController.h"
#include "YesNoDialog.h"
#include "AppLibHelper.h"
#include "ApplyAllianceCommand.h"
#include "InviteForGiftCommand.h"
#include "../Ext/CCDevice.h"
#include "FunBuildController.h"

static BranchController *_instance = NULL;

BranchController* BranchController::getInstance(){
    if(!_instance){
        _instance = new BranchController();
    }
    return _instance;
}

void BranchController::sendToBranch(std::string cmd, std::string value){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    doSendToBranchAndroid(cmd, value);
#else
    doSendToBranchIOS(cmd, value);
#endif
}

void BranchController::redeemCredit(int creditNum){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    redeemCreditAndriod(creditNum);
#else
    redeemCreditIOS(creditNum);
#endif
}

int BranchController::getCredit(){
    int result = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    result = getCreditAndroid();
#else
    result = getCreditIOS();
#endif
    return result;
}

void BranchController::addToBranchData(std::string cmd, std::string value){
    m_branchData[cmd] = value;
}

void BranchController::addToBranchData(CCDictionary *dict){
    CCDictElement *ele;
    CCDICT_FOREACH(dict, ele) {
        std::string key = ele->getStrKey();
        std::string value = dict->valueForKey(key)->getCString();
        addToBranchData(key, value);
    }
}

void BranchController::excute(std::string cmd){
    if(m_branchData.find(cmd) == m_branchData.end()){
        return;
    }
    auto value = m_branchData[cmd];
    std::vector<std::string> m_strs;
    if(value == ""){
        return;
    }
    CCCommonUtils::splitString(value, "_", m_strs);
    if(cmd == "send"){
        std::string str = m_strs[0] + "邀请加入" + m_strs[1];
        ApplyAllianceCommand *cmd = new ApplyAllianceCommand(m_strs[2]);
        cmd->sendAndRelease();
        CCCommonUtils::flyHint("", "", str);
    }
    if(cmd == "InviteForGift"){
        CCCommonUtils::flyHint("", "", value);
        CCLog("InviteForGift|excute value=%s",value.c_str());
        //接受邀请
        int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_MAIN);
        int lv = GlobalData::shared()->imperialInfo[bid].level;
        if (lv>=6) {
            CCLog("lv>=6");
            return;
        }
        std::vector<std::string> values;
        CCCommonUtils::splitString(value, "|", values);
        if (values.size()==2) {
            string uid = cocos2d::extension::CCDevice::getDeviceUid();
            CCLog("uid=%s",uid.c_str());
            if (values[1]!=uid) {
                CCLog("InviteAcceptCommand value=%s",values[0].c_str());
                InviteAcceptCommand* cmd = new InviteAcceptCommand(values[0].c_str());
                cmd->sendAndRelease();
                CCLog("cmd->sendAndRelease()");
            }
        }
    }
}

std::string BranchController::getBranchValue(std::string cmd){
    std::string value = CCUserDefault::sharedUserDefault()->getStringForKey(cmd.c_str(), "");
    return value;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

void BranchController::doSendToBranchAndroid(std::string cmd, std::string value){
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/hcg/IF/Jni", "doSetBranch", "(Ljava/lang/String;Ljava/lang/String;)V") ) {
        CCLOG("JNI: AppLibHelper method doSetBranch not found!");
        return;
    }
    jstring jcmd = minfo.env->NewStringUTF(cmd.c_str());
    jstring jvalue = minfo.env->NewStringUTF(value.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jcmd, jvalue);
    minfo.env->DeleteLocalRef(jcmd);
    minfo.env->DeleteLocalRef(jvalue);
    minfo.env->DeleteLocalRef(minfo.classID);
}

void BranchController::redeemCreditAndriod(int creditNum){
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/hcg/IF/Jni", "redeemCredit", "(I)V") ) {
        CCLOG("JNI: AppLibHelper method redeemCredit not found!");
        return;
    }
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, creditNum);
    minfo.env->DeleteLocalRef(minfo.classID);
}

int BranchController::getCreditAndroid(){
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/hcg/IF/Jni", "getCredit", "()I") ) {
        CCLOGFUNC("jni: no method getCredit");
        return 0;
    }
    
    int retFromJava = (int)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    return retFromJava;
}

#ifdef __cplusplus
extern "C" {
#endif
    void Java_org_cocos2dx_ext_Native_nativeSetBranchValue(JNIEnv* env, jobject thiz, jstring key, jstring value) {
        string branchKey = JniHelper::jstring2string(key);
        string branchValue = JniHelper::jstring2string(value);
        CCLOG("branchKey: %s",branchKey.c_str());
        CCLOG("branchValue: %s",branchValue.c_str());
        BranchController::getInstance()->addToBranchData(branchKey, branchValue);
    }
#ifdef __cplusplus
}
#endif

#else
void BranchController::doSendToBranchIOS(std::string cmd, std::string value){
    AppLibHelper::sendToBranch(cmd, value);
}
void BranchController::redeemCreditIOS(int creditNum){
    
}

int BranchController::getCreditIOS(){
    
}

#endif//CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
