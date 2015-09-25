//
//  ActivityDetailView.h
//  IF
//
//  Created by xxrdsg on 15-5-5.
//
//

#ifndef __IF__ActivityDetailView__
#define __IF__ActivityDetailView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "ActivityEventObj.h"

class ActivityDetailView: public PopupBaseView
{
public:
    static ActivityDetailView* create(ActivityEventObj* obj);
    ActivityDetailView(ActivityEventObj* obj): m_obj(obj){};
    virtual bool init();
    ActivityEventObj* m_obj;
};

class ActivityRwdView: public PopupBaseView
{
public:
    static ActivityRwdView* create(ActivityEventObj* obj);
    static ActivityRwdView* createByActId(std::string actId);
    ActivityRwdView(ActivityEventObj* obj): m_obj(obj){};
    ~ActivityRwdView(){};
    virtual bool init();    
    ActivityEventObj* m_obj;
};

#endif /* defined(__IF__ActivityDetailView__) */
