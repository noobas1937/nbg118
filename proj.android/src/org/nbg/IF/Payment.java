package org.nbg.IF;

import android.app.Activity;
import android.util.Log;

import java.lang.ref.WeakReference;
import java.util.HashMap;
import java.util.Map;

/**
 * Created with IntelliJ IDEA.
 * User: long
 * Date: 13-12-3
 * Time: 下午5:15
 * To change this template use File | Settings | File Templates.
 */
public class Payment {

    // game activity
    static public WeakReference<Activity> m_activity = null;

    public void init(Activity activity) {
        m_activity = new WeakReference<Activity>(activity);
    }

    public void doInit() {
        // todo: override
    }

    public void buyGold(String goldType,String itemId,String toUID) {
        // todo: override
    }

    public void consumeCallback(String orderId,int consumeState) {
        // todo: override
    }
    
    public void queryPurchaseOrder(){
    	// override
    }

    public static void callBuyGold(String goldType,String itemId,String dUID) {
        if (m_activity.get() != null) {
            ((IF)m_activity.get()).m_payment.buyGold(goldType,itemId,dUID);
        }
    }

    public static void onConsumeCallback(String orderId,int consumeState) {
        if (m_activity.get() != null) {
            ((IF)m_activity.get()).m_payment.consumeCallback(orderId, consumeState);
        }
    }

    public static void callPayInit() {
        if (m_activity.get() != null) {
            ((IF)m_activity.get()).m_payment.doInit();
        }
    }
    
    public static void callPayFailed(final int failedType,final String failedMsg) {
        IF.getInstance().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                nativeCallPayFailed(failedType, failedMsg);
            }
        });
    }

    public static void callPaySuccess(final String orderId,final String purchaseTime,final String productId,final String signData,final String signature) {
        IF.getInstance().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                nativeCallPaySuccess(orderId, purchaseTime, productId,signData,signature);
            }
        });
    }

    public static native void nativeCallPaySuccess(String orderId,String purchaseTime,String productId,String signData,String signature);

    public static native void nativeCallPayFailed(int failedType,String failedMsg);
}
