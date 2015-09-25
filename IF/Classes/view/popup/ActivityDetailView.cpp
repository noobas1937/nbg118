//
//  ActivityDetailView.cpp
//  IF
//
//  Created by xxrdsg on 15-5-5.
//
//

#include "ActivityDetailView.h"
#include "ActivityController.h"
#include "LuaController.h"

ActivityDetailView* ActivityDetailView::create(ActivityEventObj* obj)
{
    ActivityDetailView* ret = new ActivityDetailView(obj);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityDetailView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    if (LuaController::getInstance()->checkActLuaValid(m_obj))
    {
        this->setContentSize(CCSize(640, 852));
        CCNode* node = LuaController::getInstance()->createActivityView(m_obj);
        if (node) {
            this->addChild(node);
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

ActivityRwdView* ActivityRwdView::create(ActivityEventObj *obj)
{
    ActivityRwdView* ret = new ActivityRwdView(obj);
    if (ret && ret->init())
    {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

ActivityRwdView* ActivityRwdView::createByActId(std::string actId)
{
    auto& arr = ActivityController::getInstance()->activityArr;
    if (arr->count() == 0) {
        return nullptr;
    }
    CCObject* item = nullptr;
    ActivityEventObj* obj = nullptr;
    bool st = false;
    CCARRAY_FOREACH(arr, item)
    {
        obj = dynamic_cast<ActivityEventObj*>(item);
        if (obj->id == actId) {
            st = true;
            break;
        }
    }
    if (!st) return nullptr;
    return ActivityRwdView::create(obj);
}

bool ActivityRwdView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    if (LuaController::getInstance()->checkActLuaValid(m_obj))
    {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        CCNode* node = LuaController::getInstance()->createActivityRwdView(m_obj);
        if (node && node->getChildrenCount())
        {
            this->addChild(node);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}