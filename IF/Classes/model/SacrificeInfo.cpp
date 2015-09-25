//
//  SacrificeInfo.cpp
//  IF
//
//  Created by lifangkai on 14-11-19.
//
//

#include "SacrificeInfo.h"
void SacrificeInfo::parse(CCDictionary *dict){

//    int num;
//    int hint;
//    int resource;
    freshTime = dict->valueForKey("ct")->doubleValue() / 1000;
    if(dict->objectForKey("prayCount")){
        mPrayCount = dict->valueForKey("prayCount")->intValue();
    }
    if(dict->objectForKey("freeFromItem")){
        setPrayCountFromItem(dict->valueForKey("freeFromItem")->intValue());
    }
    if(dict->objectForKey("prayCountFree")){
        mPrayCountFree = dict->valueForKey("prayCountFree")->intValue();
    }
    if(dict->objectForKey("prayCountMax")){
        mPrayCountMax = dict->valueForKey("prayCountMax")->intValue();
    }
    if(dict->objectForKey("wood")){
        wood = dict->valueForKey("wood")->intValue();
    }
    if(dict->objectForKey("food")){
        food = dict->valueForKey("food")->intValue();
    }
    if(dict->objectForKey("iron")){
        iron = dict->valueForKey("iron")->intValue();
    }
    if(dict->objectForKey("silver")){
        silver = dict->valueForKey("silver")->intValue();
    }
    if(dict->objectForKey("steel")){
        steel = dict->valueForKey("steel")->intValue();
    }
    if(dict->valueForKey("goldfood")){
        goldfood = dict->valueForKey("goldfood")->intValue();
    }
    if(dict->valueForKey("goldwood")){
        goldwood = dict->valueForKey("goldwood")->intValue();
    }
    if(dict->valueForKey("goldsilver")){
        goldsilver = dict->valueForKey("goldsilver")->intValue();
    }
    if(dict->valueForKey("goldiron")){
        goldiron = dict->valueForKey("goldiron")->intValue();
    }
    if(dict->valueForKey("goldsteel")){
        goldsteel = dict->valueForKey("goldsteel")->intValue();
    }
    if(dict->valueForKey("isFirstFree")){
        isFirstFree = dict->valueForKey("isFirstFree")->intValue();
    }
    if(dict->valueForKey("isSteelOpen")){
        isSteelOpen = dict->valueForKey("isSteelOpen")->intValue() == 0? true:false;
    }
    isDataReturn = true;
}
void SacrificeInfo::setPrayCountFromItem(const int count){
    mPrayCountFromItem = MAX(count,0);
}
bool SacrificeInfo::canPray(){
    return mPrayCount < mPrayCountMax?true:false;
}
bool SacrificeInfo::canFreePray(){
    if(mPrayCountFromItem>0)
        return true;
    return (mPrayCount < mPrayCountFree)? true:false;
}
int SacrificeInfo::getLeftFreeCount(){
    int freeCount = mPrayCountFree - mPrayCount;
    if(freeCount == 0){
        if(mPrayCountFromItem == 0){
            return 0;
        }else{
            return mPrayCountFromItem>0?mPrayCountFromItem:-1;
        }
    }else if(freeCount>0){
        return freeCount + mPrayCountFromItem;
    }else if(mPrayCountFromItem>0)
        return mPrayCountFromItem;
    return -1;
}
