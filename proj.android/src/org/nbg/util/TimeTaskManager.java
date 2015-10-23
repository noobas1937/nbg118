package org.nbg.util;


import java.util.HashMap;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;
/**
 * 
 * @author anychen
 *
 */
public class TimeTaskManager {
	private static Map<TimerTask, Timer> taskMap = new HashMap<TimerTask, Timer>();

	/*
	 * 启动一个定时任务
	 */
	public static boolean startTask(TimerTask task, long delay, long period) {
		if (!taskMap.containsKey(task)) {
			Timer timer = new Timer(true);//守护线程,如果游戏退出主线程over了，此线程自动结束
			timer.scheduleAtFixedRate(task, delay, period);
			taskMap.put(task, timer);
			return true;
		}
		return false;
	}
	public static boolean startTask(TimerTask task, long period) {
		return startTask(task,period,period);
	}
	/**
	 * 停止某个任务
	 * 
	 * @param task
	 * @return
	 */
	public static boolean stopTask(TimerTask task) {
		if (taskMap.containsKey(task)) {
			Timer timer = taskMap.remove(task);
			timer.cancel();
			task.cancel();
			return true;
		}
		return false;
	}

	public static void stopAllTask() {
		if (!taskMap.isEmpty()) {
			for (TimerTask task : taskMap.keySet()) {
				Timer timer = taskMap.get(task);
				timer.cancel();
				task.cancel();
			}
			taskMap.clear();
		}
	}
}
