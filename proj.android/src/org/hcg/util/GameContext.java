package org.hcg.util;

import org.hcg.IF.IF;
import org.hcg.stac.empire.publish.IPublishChannel;

import android.app.Activity;

/** 简单和aoe旧代码兼容 */
public class GameContext {
    private static IPublishChannel gamePublisher;
	/**
	 * 获得程序主Activity
	 */
	public static Activity getActivityInstance() {
		return IF.getInstance();
	}
	
	public static IF getGameInstance() {
        return (IF) getActivityInstance();
    }
	
	public static IPublishChannel getGamePublisher() {
        if (gamePublisher == null) {
            gamePublisher = GameContext.getGameInstance().getPublishImpl();
            if (gamePublisher == null) {
                throw new RuntimeException("init-Publisher-failed..");
            }
        }
        return gamePublisher;
    }
}
