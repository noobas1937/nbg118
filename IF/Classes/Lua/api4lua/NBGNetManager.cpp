//
//  NBGNetManager.cpp
//  nbg
//
//  Created by gj on 9/1/15.
//
//

#include "NBGNetManager.h"
#include "CCLuaEngine.h"

#pragma mark -

NBGNetManager* NBGNetManager::m_pInstance = nullptr;

NBGNetManager* NBGNetManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new NBGNetManager();
    }
    return m_pInstance;
}

NBGNetManager::NBGNetManager()
: m_isDebugMode(false)
, luaHandler_onConnection(0)
, luaHandler_onResponse(0)
{
    
}

void NBGNetManager::handle_lua_onConnection(bool success)
{
    if (luaHandler_onConnection > 0)
    {
        auto engine = LuaEngine::getInstance();
        auto stack = engine->getLuaStack();
        stack->pushBoolean(success);
        stack->executeFunctionByHandler(luaHandler_onConnection, 1);
        stack->clean();
    }
}

void NBGNetManager::onResponse(void* pLuaValueDict)
{
    if (luaHandler_onResponse > 0)
    {
        auto engine = LuaEngine::getInstance();
        auto stack = engine->getLuaStack();
        auto responseData = (LuaValueDict*)pLuaValueDict;
        stack->pushLuaValueDict(*responseData);
        stack->executeFunctionByHandler(luaHandler_onResponse, 1);
        stack->clean();
    }
}

// -------------------------------------------------------------------
#pragma mark - Android
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include <jni.h>
#include "jni/JniHelper.h"

#define JAVA_NBG_PKG "com/elex/tech/nbg/NBGNetManager"

#pragma mark * cocos2d value <--> sfs2x

static jobject getSFSObject(JNIEnv* env)
{
    JniMethodInfo t;
    if (!JniHelper::getStaticMethodInfo(t, JAVA_NBG_PKG, "getSFSObject", "()Lcom/smartfoxserver/v2/entities/data/ISFSObject;"))
    {
        return NULL;
    }
    jobject jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
    jobject ret = env->NewLocalRef(jobj);
    t.env->DeleteLocalRef(jobj);
    return ret;
}

static jobject getSFSArray(JNIEnv* env)
{
    JniMethodInfo t;
    if (!JniHelper::getStaticMethodInfo(t, JAVA_NBG_PKG, "getSFSArray", "()Lcom/smartfoxserver/v2/entities/data/ISFSArray;"))
    {
        return NULL;
    }
    jobject jobj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
    t.env->DeleteLocalRef(t.classID);
    jobject ret = env->NewLocalRef(jobj);
    t.env->DeleteLocalRef(jobj);
    return ret;
}

static jobject ValueMap2SFSObject(JNIEnv* env, const ValueMap& valueMap, jobject javaObj);
static jobject ValueVector2SFSArray(JNIEnv* env, const ValueVector& valueVector, jobject javaObj);

// static jobject ValueMapIntKey2JavaArray(JNIEnv* env, const ValueMapIntKey& valueMap, jobject javaArray)
// {
//     jobject ret = getIntArray(env);
//     if (!array)
//         return ret;
//     CCObject* pElementInt = NULL;
//     CCARRAY_FOREACH(array, pElementInt) {
//         CCInteger * intValue=dynamic_cast<CCInteger*>(pElementInt);
//         if (!intValue)
//             continue;
        
//         JniMethodInfo minfo;
//         if (! JniHelper::getStaticMethodInfo(minfo, "org/hcg/IF/Net", "addToIntArray", "(Ljava/util/Collection;I)V") )
//             continue;
//         jobject jobj=minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID, ret, intValue->getValue());
//         minfo.env->DeleteLocalRef(minfo.classID);
//     }
//     return ret;
// }

static jobject ValueMap2SFSObject(JNIEnv* env, const ValueMap& valueMap, jobject javaObj)
{
    jclass cls = env->GetObjectClass(javaObj);
    if (!cls) return javaObj;

    for (auto it = valueMap.begin(); it != valueMap.end(); ++it)
    {
        jobject javaKey = env->NewStringUTF(it->first.c_str());
        if (it->second.getType() == Value::Type::BYTE)
        {
            jmethodID mid = env->GetMethodID(cls, "putByte", "(Ljava/lang/String;B)V");
            if (mid) env->CallVoidMethod(javaObj, mid, javaKey, it->second.asByte());
        }
        else if (it->second.getType() == Value::Type::INTEGER)
        {
            jmethodID mid = env->GetMethodID(cls, "putInt", "(Ljava/lang/String;I)V");
            if (mid) env->CallVoidMethod(javaObj, mid, javaKey, it->second.asInt());
        }
        else if (it->second.getType() == Value::Type::FLOAT)
        {
            jmethodID mid = env->GetMethodID(cls, "putFloat", "(Ljava/lang/String;F)V");
            if (mid) env->CallVoidMethod(javaObj, mid, javaKey, it->second.asFloat());
        }
        else if (it->second.getType() == Value::Type::DOUBLE)
        {
            jmethodID mid = env->GetMethodID(cls, "putFloat", "(Ljava/lang/String;F)V");
            if (mid) env->CallVoidMethod(javaObj, mid, javaKey, it->second.asDouble());
        }
        else if (it->second.getType() == Value::Type::BOOLEAN)
        {
            jmethodID mid = env->GetMethodID(cls, "putBool", "(Ljava/lang/String;Z)V");
            if (mid) env->CallVoidMethod(javaObj, mid, javaKey, it->second.asBool());
        }
        else if (it->second.getType() == Value::Type::STRING)
        {
            jmethodID mid = env->GetMethodID(cls, "putUtfString", "(Ljava/lang/String;Ljava/lang/String;)V");
            if (mid)
            {
                jobject javaValue = env->NewStringUTF(it->second.asString().c_str());
                env->CallVoidMethod(javaObj, mid, javaKey, javaValue);
                env->DeleteLocalRef(javaValue);
            }
        }
        else if (it->second.getType() == Value::Type::VECTOR)
        {
            jmethodID mid = env->GetMethodID(cls, "putSFSArray", "(Ljava/lang/String;Lcom/smartfoxserver/v2/entities/data/ISFSArray;)V");
            if (mid)
            {
                jobject javaSFSArray = getSFSArray(env);
                jobject javaValue = ValueVector2SFSArray(env, it->second.asValueVector(), javaSFSArray);
                env->CallVoidMethod(javaObj, mid, javaKey, javaValue);
                env->DeleteLocalRef(javaValue);
            }
        }
        else if (it->second.getType() == Value::Type::MAP)
        {
            jmethodID mid = env->GetMethodID(cls, "putSFSObject", "(Ljava/lang/String;Lcom/smartfoxserver/v2/entities/data/ISFSObject;)V");
            if (mid)
            {
                jobject sub_obj = getSFSObject(env);
                jobject javaValue = ValueMap2SFSObject(env, it->second.asValueMap(), sub_obj);
                env->CallVoidMethod(javaObj, mid, javaKey, javaValue);
                env->DeleteLocalRef(javaValue);
            }
        }
        else if (it->second.getType() == Value::Type::INT_KEY_MAP)
        {

        }
        env->DeleteLocalRef(javaKey);
    }
    env->DeleteLocalRef(cls);

    return javaObj;
}

static jobject ValueVector2SFSArray(JNIEnv* env, const ValueVector& valueVector, jobject javaObj)
{
    jclass cls = env->GetObjectClass(javaObj);
    if (!cls) return javaObj;

    for (auto it = valueVector.begin(); it != valueVector.end(); ++it)
    {
        if (it->getType() == Value::Type::BYTE)
        {
            jmethodID mid = env->GetMethodID(cls, "addByte", "(B)V");
            if (mid) env->CallVoidMethod(javaObj, mid, it->asByte());
        }
        else if (it->getType() == Value::Type::INTEGER)
        {
            jmethodID mid = env->GetMethodID(cls, "addInt", "(I)V");
            if (mid) env->CallVoidMethod(javaObj, mid, it->asInt());
        }
        else if (it->getType() == Value::Type::FLOAT)
        {
            jmethodID mid = env->GetMethodID(cls, "addFloat", "(F)V");
            if (mid) env->CallVoidMethod(javaObj, mid, it->asFloat());
        }
        else if (it->getType() == Value::Type::DOUBLE)
        {
            jmethodID mid = env->GetMethodID(cls, "addFloat", "(F)V");
            if (mid) env->CallVoidMethod(javaObj, mid, it->asDouble());
        }
        else if (it->getType() == Value::Type::BOOLEAN)
        {
            jmethodID mid = env->GetMethodID(cls, "addBool", "(Z)V");
            if (mid) env->CallVoidMethod(javaObj, mid, it->asBool());
        }
        else if (it->getType() == Value::Type::STRING)
        {
            jmethodID mid = env->GetMethodID(cls, "addUtfString", "(Ljava/lang/String;)V");
            if (mid)
            {
                jobject javaValue = env->NewStringUTF(it->asString().c_str());
                env->CallVoidMethod(javaObj, mid, javaValue);
                env->DeleteLocalRef(javaValue);
            }
        }
        else if (it->getType() == Value::Type::VECTOR)
        {
            jmethodID mid = env->GetMethodID(cls, "addSFSArray", "(Lcom/smartfoxserver/v2/entities/data/ISFSArray;)V");
            if (mid)
            {
                jobject javaSFSArray = getSFSArray(env);
                jobject javaValue = ValueVector2SFSArray(env, it->asValueVector(), javaSFSArray);
                env->CallVoidMethod(javaObj, mid, javaValue);
                env->DeleteLocalRef(javaValue);
            }
        }
        else if (it->getType() == Value::Type::MAP)
        {
            jmethodID mid = env->GetMethodID(cls, "addSFSObject", "(Lcom/smartfoxserver/v2/entities/data/ISFSObject;)V");
            if (mid)
            {
                jobject sub_obj = getSFSObject(env);
                jobject javaValue = ValueMap2SFSObject(env, it->asValueMap(), sub_obj);
                env->CallVoidMethod(javaObj, mid, javaValue);
                env->DeleteLocalRef(javaValue);
            }
        }
        else if (it->getType() == Value::Type::INT_KEY_MAP)
        {

        }
    }
    env->DeleteLocalRef(cls);

    return javaObj;
}

#pragma mark * native com.elex.tech.nbg.NBGNetManager

#ifdef __cplusplus
extern "C" {
#endif
    
    bool Java_com_elex_tech_nbg_NBGNetManager_nativeIsDebugMode(JNIEnv *env, jobject thisz)
    {
        return NBGNetManager::getInstance()->getIsDebugMode();
    }

    void Java_com_elex_tech_nbg_NBGNetManager_nativeLuaHandlerOnConnection(JNIEnv *env, jobject thisz, jint success)
    {
        NBGNetManager::getInstance()->handle_lua_onConnection(success);
    }

    jlong Java_com_elex_tech_nbg_NBGNetManager_nativeNewLuaValueDict()
    {
        auto dict = new LuaValueDict();
        return (long)dict;
    }
    
    jlong Java_com_elex_tech_nbg_NBGNetManager_nativeNewLuaValueArray()
    {
        auto arr = new LuaValueArray();
        return (long)arr;
    }

    // --
    
    void Java_com_elex_tech_nbg_NBGNetManager_nativeAddString2LuaValueDict(JNIEnv *env, jobject thisz, jlong pLuaValueDict, jstring key, jstring value)
    {
        auto pDict = (LuaValueDict*)pLuaValueDict;
        const char* k = env->GetStringUTFChars(key, 0);
        const char* v = env->GetStringUTFChars(value, 0);
        (*pDict)[k] = LuaValue::stringValue(v);
        env->ReleaseStringUTFChars(key, k);
        env->ReleaseStringUTFChars(value, v);
    }
        
    void Java_com_elex_tech_nbg_NBGNetManager_nativeAddLuaValueDict2LuaValueDict(JNIEnv *env, jobject thisz, jlong pLuaValueDict, jstring key, jlong value)
    {
        auto pDict = (LuaValueDict*)pLuaValueDict;
        const char* k = env->GetStringUTFChars(key, 0);
        auto sub_luaValueDict = (LuaValueDict*)value;
        (*pDict)[k] = LuaValue::dictValue(*sub_luaValueDict);
        env->ReleaseStringUTFChars(key, k);
        delete sub_luaValueDict;
    }
    
    void Java_com_elex_tech_nbg_NBGNetManager_nativeAddLuaValueArray2LuaValueDict(JNIEnv *env, jobject thisz, jlong pLuaValueDict, jstring key, jlong value)
    {
        auto pDict = (LuaValueDict*)pLuaValueDict;
        const char* k = env->GetStringUTFChars(key, 0);
        auto sub_luaValueArr = (LuaValueArray*)value;
        (*pDict)[k] = LuaValue::arrayValue(*sub_luaValueArr);
        env->ReleaseStringUTFChars(key, k);
        delete sub_luaValueArr;
    }
    
    // --
    
    void Java_com_elex_tech_nbg_NBGNetManager_nativeAddString2LuaValueArray(JNIEnv *env, jobject thisz, jlong pLuaValueArray, jstring value)
    {
        auto pArr = (LuaValueArray*)pLuaValueArray;
        const char* v = env->GetStringUTFChars(value, 0);
        pArr->push_back(LuaValue::stringValue(v));
        env->ReleaseStringUTFChars(value, v);
    }
    
    void Java_com_elex_tech_nbg_NBGNetManager_nativeAddLuaValueDict2LuaValueArray(JNIEnv *env, jobject thisz, jlong pLuaValueArr, jlong value)
    {
        auto pArr = (LuaValueArray*)pLuaValueArr;
        auto sub_luaValue = (LuaValueDict*)value;
        pArr->push_back(LuaValue::dictValue(*sub_luaValue));
        delete sub_luaValue;
    }
    
    void Java_com_elex_tech_nbg_NBGNetManager_nativeAddLuaValueArray2LuaValueArray(JNIEnv *env, jobject thisz, jlong pLuaValueArr, jlong value)
    {
        auto pArr = (LuaValueArray*)pLuaValueArr;
        auto sub_luaValue = (LuaValueArray*)value;
        pArr->push_back(LuaValue::arrayValue(*sub_luaValue));
        delete sub_luaValue;
    }

    // --
        
    void Java_com_elex_tech_nbg_NBGNetManager_nativeOnResponse(JNIEnv *env, jobject thisz, jlong pLuaValueDict)
    {
        auto pDict = (LuaValueDict*)pLuaValueDict;
        NBGNetManager::getInstance()->onResponse(pDict);
        pDict->clear();
        delete pDict;
    }
    
#ifdef __cplusplus
}
#endif

#pragma mark *

void NBGNetManager::connect(const std::string& ip_addr, int port, CXLUAFUNC luaHandlerOnConnection)
{
    luaHandler_onConnection = luaHandlerOnConnection;
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, JAVA_NBG_PKG, "connect", "(Ljava/lang/String;I)V"))
    {
        jstring java_ip_addr = t.env->NewStringUTF(ip_addr.c_str());
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID, java_ip_addr, port);
        
        t.env->DeleteLocalRef(java_ip_addr);
        t.env->DeleteLocalRef(t.classID);
    }
}

void NBGNetManager::login(const ValueMap& requestData, CXLUAFUNC luaHandlerOnLogin)
{
    luaHandler_onResponse = luaHandlerOnLogin;
    JniMethodInfo minfo;
    if (JniHelper::getStaticMethodInfo(minfo, JAVA_NBG_PKG, "login", "(Lcom/smartfoxserver/v2/entities/data/ISFSObject;)V"))
    {
        jobject jdata = getSFSObject(minfo.env);
        jdata = ValueMap2SFSObject(minfo.env, requestData, jdata);
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jdata);
        minfo.env->DeleteLocalRef(jdata);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
}

void NBGNetManager::send(const std::string& cmd, const ValueMap& requestData, CXLUAFUNC luaHandlerOnResponse)
{
    luaHandler_onResponse = luaHandlerOnResponse;
//    [NET_CLIENT sendWithCmd:[NSString stringWithUTF8String:cmd.c_str()] data:requestData];
}

#endif

// -------------------------------------------------------------------
// tao.yu add windows api
#pragma mark - Windows
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

void NBGNetManager::connect(const std::string& ip_addr, int port)
{

}

void NBGNetManager::login(const ValueMap& requestData, CXLUAFUNC luaHandlerOnLogin)
{
    
}

#endif
