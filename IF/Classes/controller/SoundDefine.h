//
//  SoundDefine.h
//  IF
//
//  Created by zhaohf on 13-8-30.
//
//

#ifndef __IF__SoundDefine__
#define __IF__SoundDefine__

//路径配置
#define MUSIC_PATH  "sounds/background/"
#define EFFECTS_PATH  "sounds/effects/"

//音量配置
#define MUSIC_DEFAULT_VOLUME 0.9
#define EFFECT_DEFAULT_VOLUME 0.9

//扩展名配置
#if (CC_TARGET_PLATFORM  == CC_PLATFORM_ANDROID)
#define MUSIC_EXT ".ogg"
#define EFFECT_EXT ".ogg"
#else
#define MUSIC_EXT ".aac"
#define EFFECT_EXT ".aac"
#endif

//场景音乐
#define Music_M_city_1          "m_city"//播放背景主界面声音
#define Music_M_city_3          "m_field"//播放背景地图声音
#define Music_Sfx_logo_loading  Music_M_city_1// NBTODO "sfx_logo_loading"//播放登录LOGO背景音乐
//#define Music_M_battle_1        "m_battle_1"//战斗场景背景音乐(野地，竞技场，藏宝图)
//#define Music_Begin_Music       "Begin_music"//开场动画音乐
#define Music_M_battle_1        "m_city"//战斗场景背景音乐(野地，竞技场，藏宝图)
#define Music_Begin_Music       "m_field"//开场动画音乐

//引导音效
#define Music_Sfx_guide_speech          "ui_guide"//引导框对话点击音效，操作后会继续弹出对话框
#define Music_Sfx_guide_attack_city     "guide_begin"//弓箭、城墙破坏等战斗音效

//世界音效
#define Music_Sfx_scene_change          "world_cloud"//点击后出现云的音效
#define Music_Sfx_world_click_space     "world_space"//点击空地后出现圈菜单的音效
#define Music_Sfx_world_click_attack    "world_atcbutton"//世界中点击他人城堡后点击攻击、宣战、占领音效
#define Music_Sfx_world_click_monster   "world_monster"//世界中点击怪物通用音效
#define Music_Sfx_world_attack          "world_attack"//世界中攻击城市、攻击怪物的统一攻击音效
#define Music_Sfx_world_click_march     "world_march"//世界中点击行军部队、出征音效，有军队行军的声音

//按钮点击音效
#define Music_Sfx_click_button       "ui_confirm"
#define Music_Sfx_button_click_cancel   "ui_cancle"
#define Music_Sfx_button_click_reward   "ui_reward"
#define Music_Sfx_UI_scroll             "ui_scroll"//铁匠铺翻书的音效
#define Music_Sfx_UI_jewel              "ui_jewel"//锻造装备界面点击品质宝石音效
#define Music_Sfx_UI_putonequipment     "ui_putonequipment"//穿装备音效

//界面音效
#define Music_Sfx_UI_levelup            "ui_levelup"//英雄升级后的弹框音效
#define Music_Sfx_UI_collect_item       "ui_collect"//升级界面等道具收取的音效
#define Music_Sfx_UI_secret             "harvest_light"//神秘商人获取道具，迁城的音效
#define Music_Sfx_UI_blacksmith         "ui_blacksmith"//装备开始锻造时的音效
#define Music_Sfx_UI_slide              "ui_slide"//滑动书的音效
#define Music_Sfx_UI_foodreward         "ui_foodreward"//奖励界面收获粮食
#define Music_Sfx_UI_woodreward         "ui_woodreward"//奖励界面收获木头
#define Music_Sfx_UI_buildfinish       "ui_buildfinish"//建筑完成或升级完成时显示提示的音效

//转盘音效
#define Music_Sfx_UI_drafting           "ui_drafting"//选牌音效
#define Music_Sfx_UI_roulette           "ui_roulette"//转轮盘音效
#define Music_Sfx_UI_shuffle            "ui_shuffle"//洗牌音效
#define Music_Sfx_UI_loteryrwd          "ui_loteryrwd"//转盘奖励音效
#define Music_Sfx_UI_putdown            "ui_putdown"//转盘宝箱炸开音效

//建筑音效
#define Music_Sfx_city_space            "city_click"//点击空地的音效，操作后会弹出建筑列表
#define Music_Sfx_city_build            "city_build"//点击后执行造建筑操作，建筑升级同这个声效，有建筑锤子的声音
#define Music_Sfx_city_complete         "city_complete"//建筑完成后会有光效及战力提升
#define Music_Sfx_city_castle           "city_castle"//点击城堡建筑的音效
#define Music_Sfx_city_barrack          "city_barrack"//点击兵营建筑的音效
#define Music_Sfx_city_stable           "city_stable"//点击马厩建筑的音效
#define Music_Sfx_city_range            "city_range"//点击靶场建筑的音效
#define Music_Sfx_city_chariot          "city_chariot"//点击战车工坊建筑的音效
#define Music_Sfx_city_drill            "city_drill"//点击校场建筑的音效
#define Music_Sfx_city_college          "city_college"//点击学院建筑的音效
#define Music_Sfx_city_embassy          "city_embassy"//点击大使馆建筑的音效
#define Music_Sfx_city_depot            "city_depot"//点击仓库建筑的音效
#define Music_Sfx_city_hall             "city_hall"//点击战争大厅建筑的音效
#define Music_Sfx_city_fortress         "city_fortress"//点击战争堡垒建筑的音效
#define Music_Sfx_city_details          "city_details"//点击旗子建筑的进入音效
#define Music_Sfx_city_blacksmith       "city_blacksmith"//点击铁匠铺建筑的音效
#define Music_Sfx_city_watchtower       "city_watchtower"//点击瞭望塔建筑的音效
#define Music_Sfx_city_turret           "city_turret"//点击箭塔建筑的音效
#define Music_Sfx_city_walls            "city_walls"//点击城墙建筑的音效
#define Music_Sfx_city_wishing          "city_wishing"//点击许愿池建筑的音效
#define Music_Sfx_city_military         "city_military"//点击行军帐篷建筑的音效
#define Music_Sfx_city_hospital         "city_hospital"//点击急救帐篷建筑的音效
#define Music_Sfx_city_market           "city_market"//点击市场建筑的音效建筑的音效
#define Music_Sfx_city_building         "city_building"//点击其他后续活动建筑的统一点击音效，如目前的限时活动建筑、宝箱
#define Music_Sfx_city_train            "city_train"//点击训练士兵时通用
#define Music_Sfx_harvest_soldier       "harvest_soldier"//点击士兵收获的气泡音效
#define Music_Sfx_city_harvest_farm     "harvest_farm"//点击粮食收获的气泡音效
#define Music_Sfx_city_harvest_wood     "harvest_wood"//点击木头收获的气泡音效
#define Music_Sfx_city_harvest_iron     "harvest_iron"//点击铁收获的气泡音效
#define Music_Sfx_city_harvest_mithril  "harvest_mithril"//点击秘银收获的气泡音效
#define Music_Sfx_city_harvest_equipment   "harvest_equipment"//点击铁匠铺收获的音效
#define Music_Sfx_city_farm             "city_farm"//点击农田建筑音效
#define Music_Sfx_city_wood             "city_wood"//点击伐木场建筑音效
#define Music_Sfx_city_iron             "city_iron"//点击铁矿建筑音效
#define Music_Sfx_city_mithril            "city_mithril"//点击秘银矿场建筑音效

#define Music_Sfx_city_harvest_bowman         "harvest_bowman"//点击弓兵收获的气泡音效
#define Music_Sfx_city_harvest_cavalryman     "harvest_cavalryman"//点击骑兵收获的气泡音效
#define Music_Sfx_city_harvest_infantry       "harvest_infantry"//点击步兵收获的气泡音效
#define Music_Sfx_city_harvest_chariot        "harvest_chariot"//点击战车收获的气泡音效

#define Music_Daily_Effect               "daily_active_effect"//活跃度音效

//礼炮音效
#define Music_Sfx_firework_in1           "firework_in1"//城内礼炮
#define Music_Sfx_firework_in2           "firework_in2"//城内礼炮
#define Music_Sfx_firework_out           "firework_out"//城外礼炮



//#define Music_Sfx_city_city     "sfx_city_city"//主城点击音效
//#define Music_Sfx_city_barrack  "sfx_barrack"//兵营点击音效
//#define Music_Sfx_city_arena    "sfx_city_arena"//竞技场点击音效
//#define Music_Sfx_city_market   "sfx_city_market"//市场点击音效
//#define Music_Sfx_city_tech     "sfx_city_tech"//科技点击音效
//#define Music_Sfx_city_lottery  "sfx_city_lottery"//主场景点击抽奖
//#define Music_Sfx_city_warehouse     "sfx_warehouse"//主场景点击仓库
//#define Music_Sfx_city_tower    "sfx_city_tower"//塔点击音效
//#define Music_Sfx_city_wall     "sfx_city_wall"//城墙点击音效
//
////音效
//#define Music_Boss_fire         "boss_fire"
//#define Music_M_battle_lose     "m_battle_lose"//战斗失败
//#define Music_M_battle_win      "m_battle_win"//战斗胜利音效
//#define Music_Sfx_battle_hurt   "sfx_battle_hurt"//攻击
//#define Music_Sfx_click_button  "sfx_click_button"//按钮声音
//#define Music_Sfx_healer_attack_1  "healer_attack_1"//治疗声音
//#define Music_Sfx_rce_troops    "sfx_rce_troops"//士兵走路（收兵）
//
//#define Music_FarmHarvest       "_FarmHarvest"//
//#define Music_building_upgrade  "_building_upgrade"//
//#define Music_resource_get      "_resource_get"//
//
//#define Music_resource_gold      "sfx_gold"//
//#define Music_resource_wood      "sfx_wood"//
#endif
