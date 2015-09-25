package org.hcg.util;

import java.util.List;

//import com.conquest.empire.main.R;
//import org.hcg.IF.TypeConst;

import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.content.Context;


public class FormulaUtil {
	/**
	 * 判断程序是否在前台
	 * 
	 * @return
	 */
	public static boolean isAppOnForeground(Context context) {
		ActivityManager activityManager = (ActivityManager) context
				.getSystemService(Context.ACTIVITY_SERVICE);
		String packageName = context.getPackageName();
		// Returns a list of application processes that are running on the
		// device
		List<RunningAppProcessInfo> appProcesses = activityManager
				.getRunningAppProcesses();
		if (appProcesses == null)
			return false;
		for (RunningAppProcessInfo appProcess : appProcesses) {
			// importance:
			// The relative importance level that the system places
			// on this process.
			// May be one of IMPORTANCE_FOREGROUND, IMPORTANCE_VISIBLE,
			// IMPORTANCE_SERVICE, IMPORTANCE_BACKGROUND, or IMPORTANCE_EMPTY.
			// These constants are numbered so that "more important" values are
			// always smaller than "less important" values.
			// processName:
			// The name of the process that this object is associated with.
			if (appProcess.processName.equals(packageName)
					&& appProcess.importance == RunningAppProcessInfo.IMPORTANCE_FOREGROUND) {
				return true;
			}
		}
		return false;
	}

	/*
	public static String getNotificationText(Context context, int type) {
		switch (type) {
		case TypeConst.UPGRADE_MAIN_BUILDING:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.city_hall_text), "");
		case TypeConst.UPGRADE_CROPLAND_1:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.corn_text), "1");
		case TypeConst.UPGRADE_CROPLAND_2:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.corn_text), "2");
		case TypeConst.UPGRADE_CROPLAND_3:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.corn_text), "3");
		case TypeConst.UPGRADE_CROPLAND_4:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.corn_text), "4");
		case TypeConst.UPGRADE_LUMBER_MILL_1:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.wood_field_text), "1");
		case TypeConst.UPGRADE_LUMBER_MILL_2:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.wood_field_text), "2");
		case TypeConst.UPGRADE_LUMBER_MILL_3:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.wood_field_text), "3");
		case TypeConst.UPGRADE_LUMBER_MILL_4:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.wood_field_text), "4");
		case TypeConst.UPGRADE_IRON_MINE_1:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.iron_mines_text), "1");
		case TypeConst.UPGRADE_IRON_MINE_2:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.iron_mines_text), "2");
		case TypeConst.UPGRADE_IRON_MINE_3:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.iron_mines_text), "3");
		case TypeConst.UPGRADE_IRON_MINE_4:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.iron_mines_text), "4");
		case TypeConst.UPGRADE_STONE_MINE_1:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.stone_mines_text), "1");
		case TypeConst.UPGRADE_STONE_MINE_2:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.stone_mines_text), "2");
		case TypeConst.UPGRADE_STONE_MINE_3:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.stone_mines_text), "3");
		case TypeConst.UPGRADE_STONE_MINE_4:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.stone_mines_text), "4");
		case TypeConst.UPGRADE_BARRACKS:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.barracks_text), "");
		case TypeConst.UPGRADE_WAREHOUSE:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.warehouse_text), "");
		case TypeConst.UPGRADE_WOOD_CUTTING:
			return context.getString(R.string.notification_tech_upgrade,
					context.getString(R.string.tech_wood_text));
		case TypeConst.UPGRADE_DIGGING:
			return context.getString(R.string.notification_tech_upgrade,
					context.getString(R.string.tech_stone_text));
		case TypeConst.UPGRADE_SMELTING:
			return context.getString(R.string.notification_tech_upgrade,
					context.getString(R.string.tech_iron_text));
		case TypeConst.UPGRADE_PLANTING:
			return context.getString(R.string.notification_tech_upgrade,
					context.getString(R.string.tech_crop_text));
		case TypeConst.UPGRADE_SHOOTING_SKILL:
		case TypeConst.UPGRADE_SCOUT_SKILL:
		case TypeConst.UPGRADE_TURRET_RESEARCH:
		case TypeConst.UPGRADE_TRAP_RESEARCH:
		case TypeConst.UPGRADE_MARCHING_SKILL:
		case TypeConst.UPGRADE_ATTACK_SKILL:
		case TypeConst.UPGRADE_DEFENSIVE_SKILL:
			return "";
		case TypeConst.UPGRADE_CITYWALL:
			return context.getString(R.string.notification_building_upgrade,
					context.getString(R.string.walls_text), "");
			// 兵种训练相关的都在100-149之间
		case TypeConst.TRAINING_INFANTRY:
			return context.getString(R.string.notification_training,
					context.getString(R.string.soldier_1));
		case TypeConst.TRAINING_CAVALRY:
			return context.getString(R.string.notification_training,
					context.getString(R.string.soldier_2));
		case TypeConst.TRAINING_ARCHER:
			return context.getString(R.string.notification_training,
					context.getString(R.string.soldier_3));
		case TypeConst.TRAINING_CATAPULT:
			return context.getString(R.string.notification_training,
					context.getString(R.string.soldier_4));
			// 行军相关的都在150-199之间
		case TypeConst.MARCH_ATTACK:
		case TypeConst.MARCH_OCCUPY:
		case TypeConst.MARCH_HUNT:
		case TypeConst.MARCH_DISPATCH:
			// 暂时攻击,打猎,占领在行军的时候召回都统一返回这个
		case TypeConst.CALLBACK_ATTACK:
		case TypeConst.CALLBACK_OCCUPY:
		case TypeConst.CALLBACK_HUNT:
		case TypeConst.GIVEUP_OCCUPY:
		case TypeConst.MARCH_ATTACKED:
			// 在野地打猎中,在倒计时中不会出现
		case TypeConst.HUNTING:

		default:
			break;
		}
		return "";
	}
	*/

	/**
	 * 根据玩家离开的天数，获得通知文本
	 * 
	 * @param context
	 * @param days
	 * @return
	 */
	/*
	public static String getAFKText(Context context, int days) {
		switch (days) {
		case 1:
			return context.getString(R.string.notification_user_afk_1);
		case 3:
			return context.getString(R.string.notification_user_afk_3);
		case 7:
			return context.getString(R.string.notification_user_afk_7);
		case 15:
			return context.getString(R.string.notification_user_afk_15);
		case 30:
			return context.getString(R.string.notification_user_afk_30);
		default:
			return "";
		}
	}
	*/

	public static int getNextAFKDay(int days) {
		switch (days) {
		case 1:
			return 3;
		case 3:
			return 7;
		case 7:
			return 15;
		case 15:
			return 30;
		}
		return 0;
	}
}
