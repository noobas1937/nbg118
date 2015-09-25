//
//  LoginRewardInfo.h
//  IF
//
//  Created by 邹 程 on 13-12-2.
//
//

#ifndef __IF__LoginRewardInfo__
#define __IF__LoginRewardInfo__

#define LOTTERY_SLOT   4
#define LOTTERY_ITEM   5

class LoginRewardInfo {
public:
    bool canDoLottery;
    int lotteryResult[LOTTERY_SLOT];
    int lotteryValue[LOTTERY_SLOT][LOTTERY_ITEM];
    LoginRewardInfo():
    canDoLottery(false)
    {
        for (int i=0; i<LOTTERY_SLOT; i++) {
            lotteryResult[i] = -1;
            for (int j=0; j<LOTTERY_ITEM; j++) {
                lotteryValue[i][j] = -1;
            }
        }
    }
};

#endif /* defined(__IF__LoginRewardInfo__) */
