//
//  PlotInfo.h
//  IF
//
//  Created by ganxiaohua on 13-11-6.
//
//

#ifndef __IF__PlotInfo__
#define __IF__PlotInfo__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCTypesExt.h"

class PlotInfo : public cocos2d::CCObject {
public:
    CREATE_FUNC(PlotInfo);
    ~PlotInfo(){
        m_figure.clear();
        m_seat.clear();
        m_content.clear();
        m_names.clear();
    };
    PlotInfo():m_id(std::string()),m_order(0),m_cover(0){};
    virtual bool init(){return true;};
    void parse(cocos2d::CCDictionary* dic);
    
    std::string m_id;
    std::string m_questId;
    int m_order;
    int m_cover;
    std::vector<std::string> m_figure;
    std::vector<std::string> m_seat;
    std::vector<std::string> m_content;
    std::vector<std::string> m_names;
};
#endif /* defined(__IF__PlotInfo__) */
