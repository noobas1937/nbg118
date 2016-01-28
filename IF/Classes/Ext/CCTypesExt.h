//
//  CCBoolean.h
//  aoe
//
//  Created by 谢文杰 on 13-2-20.
//
//

#ifndef aoe_CCBoolean_h
#define aoe_CCBoolean_h

#include "cocos2d.h"
#include "cocosextV3.h"

#include "GAMacros.h"
NS_CC_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */

// Created for SFSObject.

class CC_DLL CCByte : public CCObject
{
public:
    CCByte(unsigned char v)
    : m_nValue(v) {}
    unsigned char getValue() const {return m_nValue;}
    
    static CCByte* create(bool v)
    {
        CCByte* pRet = new CCByte(v);
        pRet->autorelease();
        return pRet;
    }
private:
    unsigned char m_nValue;
};

class CC_DLL CCBoolean : public CCObject
{
public:
    CCBoolean(bool v)
    : m_nValue(v) {}
    bool getValue() const {return m_nValue;}
    
    static CCBoolean* create(bool v)
    {
        CCBoolean* pRet = new CCBoolean(v);
        pRet->autorelease();
        return pRet;
    }
private:
    bool m_nValue;
};

// 暂时还没人用到，用到的时候要到AOENetController里也打开转换函数
class CC_DLL CCLong : public CCObject
{
public:
    CCLong(long v)
    : m_nValue(v) {}
    long getValue() const {return m_nValue;}
    
    static CCLong* create(long v)
    {
        CCLong* pRet = new CCLong(v);
        pRet->autorelease();
        return pRet;
    }
private:
    long m_nValue;
};

//为了支付传递时间戳字段,longlong为64位，long和int为32位
class CC_DLL CCLongLong : public CCObject
{
public:
    CCLongLong(long long v)
    : m_nValue(v) {}
    long long getValue() const {return m_nValue;}
    
    static CCLongLong* create(long long v)
    {
        CCLongLong* pRet = new CCLongLong(v);
        pRet->autorelease();
        return pRet;
    }
private:
    long long m_nValue;
};
//
//class CC_DLL CCDouble : public CCObject
//{
//public:
//    CCDouble(double v)
//    : m_nValue(v) {}
//    double getValue() const {return m_nValue;}
//    
//    static CCDouble* create(double v)
//    {
//        CCDouble* pRet = new CCDouble(v);
//        pRet->autorelease();
//        return pRet;
//    }
//private:
//    double m_nValue;
//};

// end of data_structure group
/// @}

#define __SHORT_FORM_OF_FILE__ \
(strrchr(__FILE__,'/') \
? strrchr(__FILE__,'/')+1 \
: __FILE__ \
)

#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
#   define CCLOGTIME(...)   do {} while (0)
#   define CCLOGTIMEF(...)  do {} while (0)
#   define CCLOGFUNC(...)   do {} while (0)
#   define CCLOGFUNCF(...)  do {} while (0)
#else // COCOS2D_DEBUG > 0
#   define CCLOGTIME(s)                                                 \
        { timeval tv; gettimeofday(&tv,NULL);                           \
        cocos2d::CCLog("%s(%s:%d) TIME %u.%.6u " s, __FUNCTION__, __SHORT_FORM_OF_FILE__, __LINE__, tv.tv_sec, tv.tv_usec); }
#   define CCLOGTIMEF(format, ...)                                      \
        { timeval tv; gettimeofday(&tv,NULL);                           \
        std::string sFmt="%s(%s:%d) TIME %u.%.6u "; sFmt+=format;        \
        cocos2d::CCLog(sFmt.c_str(), __FUNCTION__, __SHORT_FORM_OF_FILE__, __LINE__, tv.tv_sec, tv.tv_usec, ##__VA_ARGS__); }
#   define CCLOGFUNC(s)                                                 \
        { cocos2d::CCLog("%s(%s:%d) " s, __FUNCTION__, __SHORT_FORM_OF_FILE__, __LINE__); }
#   define CCLOGFUNCF(format, ...)                                      \
        { std::string sFmt="%s(%s:%d) "; sFmt+=format;                   \
        cocos2d::CCLog(sFmt.c_str(), __FUNCTION__, __SHORT_FORM_OF_FILE__, __LINE__, ##__VA_ARGS__); }
#endif // COCOS2D_DEBUG

/// 数字转字符串
inline const char* CC_ITOA(int v){return CCString::createWithFormat("%d", v)->getCString();}
inline const char* CC_ITOA(long v){return CCString::createWithFormat("%ld", v)->getCString();}
inline const char* CC_ITOA(double v){return CCString::createWithFormat("%.f", v)->getCString();}
inline const char* CC_ITOA_1(double v){return CCString::createWithFormat("%.1f", v)->getCString();}
inline const char* CC_ITOA_Percent(double v){return CCString::createWithFormat("%.1f%%", v*100)->getCString();}

inline std::string CC_CMDITOA(int v)
{
    int n = v;
    if(v<0) {
        n = -v;
    }
    std::string ret = CC_ITOA(n);
    int pos = ret.length()-3;
    while (pos>0) {
        ret.insert(pos, ",");
        pos -= 3;
    }
    if (v<0) {
        ret = "-"+ret;
    }
    return ret;
}

inline std::string CC_CMDITOAL(long v)
{
    long n = v;
    if(v<0) {
        n = -v;
    }
    std::string ret = CC_ITOA(n);
    int pos = ret.length()-3;
    while (pos>0) {
        ret.insert(pos, ",");
        pos -= 3;
    }
    if (v<0) {
        ret = "-"+ret;
    }
    return ret;
}

inline std::string CC_CMDITOAD(double v)
{
    double n = v;
    if(v<0) {
        n = -v;
    }
    std::string ret = CC_ITOA(n);
    int pos = ret.length()-3;
    while (pos>0) {
        ret.insert(pos, ",");
        pos -= 3;
    }
    if (v<0) {
        ret = "-"+ret;
    }
    return ret;
}

//商店价格用
inline const char* CC_ITOA_MONEY(int v){return CCString::createWithFormat("%d", v)->getCString();}
inline const char* CC_ITOA_MONEY(double v){return CCString::createWithFormat("%.2f", v)->getCString();}
static char q[7] = {' ', 'K', 'M', 'G', 'T', 'P', 'E'};
inline const char* CC_ITOA_K(long v){double t=v;if(t>=1000){int i=0;while(t/1000>=1){t=t/1000;i++;}return CCString::createWithFormat("%.1f%c",t,q[i])->getCString();}else{return CC_ITOA((int)v);}}
inline const char* CC_ITOA_K(double v){if(v>=1000){int i=0;while(v/1000>=1){v=v/1000;i++;}return CCString::createWithFormat("%.1f%c",v,q[i])->getCString();}else{return CC_ITOA((int)v);}}
/// 把秒数转成00:00:00格式
inline const char* CC_SECTOA(int secs){
    if(secs > 24*3600) {
        return CCString::createWithFormat("%dd %02d:%02d:%02d",secs/(24*3600), secs/3600%24,secs/60%60,secs%60)->getCString();
    }
    return CCString::createWithFormat("%02d:%02d:%02d",secs/3600,secs/60%60,secs%60)->getCString();
}

NS_CC_END
 
NS_CC_EXT_BEGIN
/**
 * CCObject pointer wrapper.
 * This will automatically set pointer NULL on construct, release on
 * desctruct, retain on assigning.
 *
 * usage:
 * class Test {
 * public:
 *     void use() {str=CCString::create("test");}
 * private:
 *     CCSafeObject<CCString> str;
 * };
 */
template <class T>
class CCSafeObject {
    
public:
    
	//Construct using a C pointer
	CCSafeObject(T* ptr = nullptr)
    : _ptr(ptr)
	{
        if(ptr != nullptr) {ptr->retain();}
	}
    
	//Copy constructor
	CCSafeObject(const CCSafeObject &ptr)
    : _ptr(ptr._ptr)
	{
		if(_ptr != NULL) {_ptr->retain();}
	}
    
    //Move constructor
	CCSafeObject(CCSafeObject &&ptr)
    : _ptr(ptr._ptr)
	{
        ptr._ptr = nullptr;
	}
    
	~CCSafeObject()
	{
        if(_ptr != nullptr) {CC_SAFE_RELEASE(_ptr);}
	}
    
	//Assign a pointer
	CCSafeObject &operator=(T* ptr)
	{
        if(ptr != nullptr) {ptr->retain();}
        if(_ptr != nullptr) {CC_SAFE_RELEASE(_ptr);}
        _ptr = ptr;
        return (*this);
	}
    
	//Assign another CCSafeObject
	CCSafeObject &operator=(const CCSafeObject &ptr)
	{
        return (*this) = ptr._ptr;
	}
    
    T* getObject() const {
        return _ptr;
    }
    
    void setObject(T* ptr) {
        (*this) = ptr;
    }
    
    T* operator->() const {return _ptr;}		//x->member
    T &operator*() const {return *_ptr;}		//*x, (*x).member
    operator T*() const {return _ptr;}		//T* y = x;
    operator bool() const {return _ptr != nullptr;}	//if(x) {/*x is not NULL*/}
    
    // CAUTION :
    // USE OPERATOR == TO COMPARE TWO CCSafeObject IS UNDEFINED !!!
    
private:
    T *_ptr;	//Actual pointer
};

//template <class CCObjectTypePointer>
//class CCSafeObject {
//public:
//    CCSafeObject()
//    : m_obj(NULL) {}
//    ~CCSafeObject()
//    {CC_SAFE_RELEASE_NULL(m_obj);}
//    CCSafeObject(const CCSafeObject& obj)
//    : m_obj(obj.m_obj)
//    {CC_SAFE_RETAIN(m_obj);}
//    CCSafeObject(const CCObjectTypePointer& obj)
//    : m_obj(obj)
//    {CC_SAFE_RETAIN(m_obj);}
//    
//    /// retain when assigning.
//    CCSafeObject& operator=(const CCSafeObject& obj)
//    {setObject(obj.getObject());return *this;}
//    CCObjectTypePointer& operator=(const CCObjectTypePointer& obj)
//    {setObject(obj);return m_obj;}
//    operator CCObjectTypePointer&()
//    {return m_obj;}
//    CCObjectTypePointer operator->()
//    {return m_obj;}
//    bool operator==(const CCObjectTypePointer& obj)
//    {return m_obj==obj;}
//    bool operator!=(const CCObjectTypePointer& obj)
//    {return m_obj!=obj;}
//    
//    CC_SYNTHESIZE_RETAIN(CCObjectTypePointer, m_obj, Object);
//};

inline bool isTouchInside(CCNode* pNode, CCTouch* touch)
{
    if (!pNode || !pNode->getParent())
        return false;
    CCPoint touchLocation=pNode->getParent()->convertToNodeSpace(touch->getLocation());
    CCRect bBox=pNode->boundingBox();
//    touchLocation.x -= bBox.size.width * pNode->getAnchorPoint().x;
//    touchLocation.y -= bBox.size.height * pNode->getAnchorPoint().y;
    return bBox.containsPoint(touchLocation);
}

inline bool isPointInsideNode(CCNode* pNode, CCPoint worldPos)
{
    if (!pNode || !pNode->getParent())
        return false;
    CCPoint touchLocation=pNode->getParent()->convertToNodeSpace(worldPos);
    CCRect bBox=pNode->boundingBox();
    return bBox.containsPoint(touchLocation);
}
inline bool hasVisibleParents(CCNode* pNode)
{
    auto parent = pNode->getParent();
    for( auto c = parent; c != nullptr; c = c->getParent() )
    {
        if( !c->isVisible() )
        {
            return false;
        }
    }
    return true;
}


inline bool isTouchInTexture(CCNode* pNode, CCTouch* touch)
{
    auto sprite = dynamic_cast<CCSprite*>(pNode);
    if ( sprite && sprite->getBatchNode()) {
        return isTouchInside(pNode, touch);
    }
    
//    unsigned long uul = clock();
    unsigned char data[4];
    //获得点击的OpenGL的世界坐标值
    CCPoint touchPoint = pNode->convertTouchToNodeSpace(touch);
    CCPoint location = ccp((touchPoint.x) * CC_CONTENT_SCALE_FACTOR(), (touchPoint.y) * CC_CONTENT_SCALE_FACTOR());
    CCRenderTexture* renderTexture = CCRenderTexture::create(1* CC_CONTENT_SCALE_FACTOR(),1 * CC_CONTENT_SCALE_FACTOR(), kCCTexture2DPixelFormat_RGBA8888);
//    renderTexture->beginWithClear(0,0,0,0);
    //只保存渲染一个像素的数据
    CCPoint oldPos = pNode->getPosition();
    CCPoint oldAnchor = pNode->getAnchorPoint();
    
    pNode->setAnchorPoint(ccp(0,0));
    pNode->setPosition(ccp(-location.x, -location.y));
    
    renderTexture->beginWithClear(0,0,0,0);
    pNode->visit();
    glReadPixels(0,0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    renderTexture->end();
    
    pNode->setAnchorPoint(oldAnchor);
    pNode->setPosition(oldPos);
    
//    CCLog("aaaaaaaaaaaaaaa%ld",(clock()-uul));
    //检测alpha值
    if(data[0] || data[1] || data[2] || data[3])
    {
        //非透明
        return true;
    }
    return false;
}
NS_CC_EXT_END

#define CC_SYNTHESIZE_SAFEOBJECT(varType, varName, funName)\
protected: cocos2d::extension::CCSafeObject<varType > varName;\
public: virtual varType* get##funName(void) const { return varName.getObject(); }\
public: virtual void set##funName(varType* var){ varName = var; }

#define CC_SAFE_CALLFUNC(varTarget,varFunc,funName)\
protected: cocos2d::CCObject* varTarget;\
protected: cocos2d::SEL_CallFunc varFunc;\
public: virtual cocos2d::CCCallFunc* get##funName(void) const {\
if (varTarget && varFunc) {\
    return cocos2d::CCCallFunc::create(varTarget,varFunc);\
}\
return NULL; \
}\
public: virtual void set##funName(cocos2d::CCCallFunc* func){\
if (func && func->getTargetCallback() && func->getCallFunc()) {\
varTarget = func->getTargetCallback();\
varFunc = func->getCallFunc();\
} else {\
varTarget = NULL;\
varFunc = NULL;\
}\
}\

#define PropertyBuilder(type, name, access_permission) \
access_permission: \
type m_##name; \
public: \
inline void set##name(type v) { \
m_##name = v; \
} \
inline type get##name() { \
return m_##name; \
} \

#endif
