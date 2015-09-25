//
//  BattleConstData.h
//  IF
//
//  Created by ganxiaohua on 13-9-26.
//
//

#ifndef __IF__BattleConstData__
#define __IF__BattleConstData__

//士兵 位置计算数据
#define BATTLE_ARMY_GAP_X 160 //89
#define BATTLE_ARMY_GAP_Y 80 // 45

#define LEFT_OFF_POINT CCPoint(-367,-9) // -45
#define RIGHT_OFF_POINT CCPoint(-140,117)  //120


#define BATTLE_UNIT_GAP_X 175  //123
#define BATTLE_UNIT_GAP_Y 83   //62


#define BATTLE_MOVE_TIME 1.2
#define BATTLE_INIT_MOVE_BATTLE_TIME 2.5
#define MOVE_BATTLE_UNIT_RAND_TIME_FACTOR 2000.0 //(rand()%1000)/6000.0

#define ATTACK_SOLDIER_DELAY_PER_UNIT 0.07
#define SOLDIER_DELAY_PER_UNIT 0.2
#define HURT_SOLDIER_DELAY_PER_UNIT 0.1
#define IDLE_SOLDIER_DELAY_PER_UNIT 0.2
#define MOVE_SOLDIER_DELAY_PER_UNIT 0.07

#define SEQUENCE_PER_TIME 0.8

#define  MSG_BATTLE_SHOW_HP            "battle_show_hp"
#define  MSG_BATTLE_DIE_HERO           "battle_die_hero"
#define  MSG_BATTLE_HIT_HURT_EFFECT           "battle_hit_hurt_effect"
#define  MSG_BATTLE_MOVE_ON_BATTLE_COMPLETE      "battle_move_on_battle_complete"
#define  MSG_BATTLE_SHAKE_BG           "battle_shake_bg"
#define  MSG_BATTLE_ADD_INIT_SOLDIER        "battle_add_init_soldier"


#define  MSG_BATTLE_SEQUENCE_RESULT       "battle_sequence_result"
#define  MSG_BATTLE_QUICK_END       "battle_quick_end"
#define  MSG_BATTLE_CHANGE_ROUND       "battle_change_round"
#define  MSG_BATTLE_SHOW_NAME       "battle_show_name"
#define  MSG_BATTLE_USE_BIG_SKILL       "battle_use_bigSkill"
#define  MSG_BATTLE_MOVE_ON_BATTAT_POSITION      "battle_move_on_battle_position"
#define  MSG_BATTLE_SHAKE_BG       "battle_shake_bg"
#define  MSG_BATTLE_PLAYBACK       "battle_playback"
#define  MSG_BATTLE_SEQUENCE_INDEX       "battle_sequence_index"
#define  MSG_BATTLE_REPLAY_BATTLE       "battle_replay_battle"

#define  MOVE       "move"
#define  ATTACK       "attack"
#define  STAND       "stand"
#define  HURT       "hurt"
#define  DEATH       "die"

#define  BATTLE_DAMAGE1_K1           0.15
#define  BATTLE_DAMAGE1_K2           0.15
#define  BATTLE_DAMAGE1_K3           20
#define  BATTLE_DAMAGE1_K4           0.05
#define  BATTLE_DAMAGE1_K5           0.05

#endif /* defined(__IF__BattleConstData__) */
