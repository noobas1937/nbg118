package org.hcg.util;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.TimerTask;

import android.content.Context;
import android.util.Log;

import org.hcg.util.GameContext;

/**
 * 行云日志发送任务 进入游戏后：启动发送任务，启动之前先检查本地是否有日志缓存，如有加入发送队列 退出游戏前：停止发送任务，将剩余日志保存到本地
 */
public class CloudAnalysisManager {
	private static ArrayList<String> logList = new ArrayList<String>();
	private static TimerTask logTimeTask;
	private static boolean DEBUG = false;

	public static void addLog(String logUrl) {
		if (logUrl != null) {
			logList.add(logUrl);
		}
	}

	private static void startLogTask() {
		if (logTimeTask == null) {
			logTimeTask = new TimerTask() {

				@Override
				public void run() {
					if (DEBUG) {
						Log.d("LogTask", "begin send task:" + logList.size());
					}
					if (!logList.isEmpty()) {
						int threadSize = logList.size() / 4 + 1;
						final ArrayList<String> tempList = new ArrayList<String>();
						tempList.addAll(logList);
						for (int i = 0; i < threadSize; i++) {
							new Thread(new Runnable() {// 启动多任务发送

										@Override
										public void run() {
											while (!tempList.isEmpty()) {
												String log = null;
												 //读取时同步即可，缩小同步范围
												synchronized (tempList) {
													if (!tempList.isEmpty()) {
														log = tempList
																.remove(0);
													}
												}
												if (log != null) {
													httpRequestForURL(log);
												}
											}
										}
									}).start();
						}
						logList.removeAll(tempList);
					}
					if (DEBUG) {
						Log.d("LogTask", "end send task:" + logList.size());
					}
				}
			};
			if (DEBUG) {
				Log.d("LogTask", "startLogTask:" + logList.size());
			}
			TimeTaskManager.startTask(logTimeTask, 30 * 1000);// 30s执行一次
		}
	}

	private static void httpRequestForURL(final String xaUrl) {
		URL url = null;
		try {
			url = new URL(xaUrl);
		} catch (MalformedURLException e) {
			e.printStackTrace();
		}
		if (DEBUG)
			Log.d("LogTask", "send:" + xaUrl);
		try {
			HttpURLConnection httpConnector = (HttpURLConnection) url
					.openConnection();
			httpConnector.setConnectTimeout(2000);
			httpConnector.connect();
			httpConnector.getResponseCode();// 正真请求后台
		} catch (IOException e) {
			e.printStackTrace();
		}
		if (DEBUG)
			Log.d("LogTask", "----send----over-----");
	}

	private static File getLogDir() {
		return GameContext.getActivityInstance().getDir("log",
				Context.MODE_PRIVATE);
	}

	public static void onFinished() {
		if (DEBUG)
			Log.d("LogTask", "onFinished");
		if (logTimeTask != null) {
			logTimeTask.cancel();
			logTimeTask = null;
		}
		// 立即将剩余日志保存本地文件
		if (!logList.isEmpty()) {
			int size = logList.size();
			if (DEBUG)
				Log.d("LogTask", "save log to local:" + size);
			File logDir = getLogDir();
			File logFile = new File(logDir.getPath() + File.separator
					+ "log.txt");
			try {
				FileOutputStream fos_db = new FileOutputStream(logFile);
				DataOutputStream dos = new DataOutputStream(fos_db);
				dos.writeInt(size);
				for (String log : logList) {
					dos.writeUTF(log);
				}
				dos.flush();
				fos_db.flush();
				fos_db.close();
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	private static void checkLogInLocal() {
		File logDir = getLogDir();
		File logFile = new File(logDir.getPath() + File.separator + "log.txt");
		try {
			if (!logFile.exists()) {
				// TODO 是否创建？
				return;
			}
			FileInputStream fis_db = new FileInputStream(logFile);
			DataInputStream dis = new DataInputStream(fis_db);
			if (dis.available() > 0) {
				int size = dis.readInt();
				if (DEBUG)
					Log.d("LogTask", "read log in local:" + size);
				for (int i = 0; i < size; i++) {
					String log = dis.readUTF();
					if (DEBUG)
						Log.d("LogTask", "log-local:" + log);
					logList.add(log);
				}
			}
			fis_db.close();
			logFile.delete();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void onStart() {
		checkLogInLocal();
		startLogTask();
	}
}

