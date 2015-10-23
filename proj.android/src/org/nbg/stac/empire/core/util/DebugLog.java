/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.nbg.stac.empire.core.util;

import android.util.Log;

public final class DebugLog {

	private static int logLevel = Log.ERROR;
	public final static String GAME_TAG = "COK";

	private DebugLog() {
	}

	public static void setLogLevel(int level) {
		logLevel = level;
	}

	public static int d(String msg) {
		return d(GAME_TAG, msg);
	}

	public static int d(String tag, String msg) {
		int result = 0;
		if (logLevel <= Log.DEBUG) {
			result = Log.d(tag, msg);
		}
		return result;
	}

	public static int d(String msg, Throwable tr) {
		return d(GAME_TAG, msg, tr);
	}

	public static int d(String tag, String msg, Throwable tr) {
		int result = 0;
		if (logLevel <= Log.DEBUG) {
			result = Log.d(tag, msg, tr);
		}
		return result;
	}

	public static int i(String msg) {
		return i(GAME_TAG, msg);
	}

	public static int i(String tag, String msg) {
		int result = 0;
		if (logLevel <= Log.INFO) {
			result = Log.i(tag, msg);
		}
		return result;
	}

	public static int i(String msg, Throwable tr) {
		int result = 0;
		if (logLevel <= Log.INFO) {
			result = Log.i(GAME_TAG, msg, tr);
		}
		return result;
	}

	public static int i(String tag, String msg, Throwable tr) {
		int result = 0;
		if (logLevel <= Log.INFO) {
			result = Log.i(tag, msg, tr);
		}
		return result;
	}

	public static int w(String msg) {
		return w(GAME_TAG, msg);
	}

	public static int w(String tag, String msg) {
		int result = 0;
		if (logLevel <= Log.WARN) {
			result = Log.w(tag, msg);
		}
		return result;
	}

	public static int w(String msg, Throwable tr) {
		return w(GAME_TAG, msg, tr);
	}

	public static int w(String tag, String msg, Throwable tr) {
		int result = 0;
		if (logLevel <= Log.WARN) {
			result = Log.w(tag, msg, tr);
		}
		return result;
	}

	public static int e(String msg) {
		return e(GAME_TAG, msg);
	}

	public static int e(String tag, String msg) {
		int result = 0;
		if (logLevel <= Log.ERROR) {
			result = Log.e(tag, msg);
		}
		return result;
	}

	public static int e(String msg, Throwable tr) {
		return e(GAME_TAG, msg, tr);
	}

	public static int e(String tag, String msg, Throwable tr) {
		int result = 0;
		if (logLevel <= Log.ERROR) {
			result = Log.e(tag, msg, tr);
		}
		return result;
	}
}
