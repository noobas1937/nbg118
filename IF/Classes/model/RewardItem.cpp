//
//  RewardItem.cpp
//  IF
//
//  Created by ganxiaohua on 13-11-27.
//
//

#include "RewardItem.h"

std::string RewardItem::getItemName(){
    std::string str="";
    switch (m_type) {
        case R_SILVER:
            str = _lang("105002");
            break;
        case R_WOOD:
            str = _lang("105003");
            break;
        case R_IRON:
            str = _lang("105005");
            break;
        case R_EXP:
            str = _lang("105115");
            break;
        case  R_GOODS:
            str = "";
            break;
        case  R_GENERAL:
            str = "";
            break;
        default:
            break;
    }
    
    return str;
}