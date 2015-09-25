//
//  ccMoreTypes.h
//  IF
//
//  Created by Émilie.Jiang on 15/3/9.
//
//

#ifndef IF_ccMoreTypes_h
#define IF_ccMoreTypes_h

#include "cocos2d.h"

USING_NS_CC;

NS_CC_BEGIN

// rich font definition, support shadow color
struct ccRichFontDefinition : public ccFontDefinition {
    // font shadow color
    int m_shadowColor;
};
static inline ccColor4B ccc4FromInt(int c) {
    return ccc4((c >> 16) & 0xff,
                (c >> 8) & 0xff,
                c & 0xff,
                (c >> 24) & 0xff);
}

NS_CC_END


#define CC_SYNTHESIZE_BOOL(varName, funName) \
protected: bool varName; \
public: virtual bool is##funName(void) const { return varName; } \
public: virtual void set##funName(bool var) { varName = var; } \
public: virtual void mark##funName() { varName = true; } \
public: virtual void clear##funName() { varName = false; }

// readonly for bool type
#define CC_SYNTHESIZE_READONLY_BOOL(varName, funName)\
protected: bool varName;\
public: virtual bool is##funName(void) const { return varName; }

// special macro for bool type, custom setter
#define CC_SYNTHESIZE_BOOL_SETTER(varName, funName) \
protected: bool varName; \
public: virtual bool is##funName(void) const { return varName; } \
public: virtual void set##funName(bool var);

// macro for custom setter
#define CC_SYNTHESIZE_SETTER(varType, varName, funName) \
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; } \
public: virtual void set##funName(varType var);

// macro for custom getter
#define CC_SYNTHESIZE_GETTER(varType, varName, funName) \
protected: varType varName;\
public: virtual varType get##funName(void) const; \
public: virtual void set##funName(varType var) { varName = var; }

#endif
