package com.clash.of;

import android.util.Log;
import android.app.Activity;

import com.clash.of.kings.EmpireActivity;
import com.clash.of.util.*;

import org.nbg.IF.IF;
import org.nbg.IF.Payment;
import org.apache.commons.lang.StringUtils;
import org.cocos2dx.ext.Device;
import org.cocos2dx.ext.Native;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.lang.ref.WeakReference;

/**
 * Created with IntelliJ IDEA.
 * User: long
 * Date: 13-12-2
 * Time: 下午4:25
 * To change this template use File | Settings | File Templates.
 */
public class PayGoogle extends Payment {
    static final String Debug_Tag = "COKPayGoogle";
    static final boolean Debug = false;

    // encoded google key from Google Play developer console
    static final String Encoded_Public_key = Native.nativeGetGoogleEncodedPublickey();

    // add consume package sku here
    static final String SKU_GOLD_1 = "gold_1";
    static final String SKU_GOLD_2 = "gold_2";
    static final String SKU_GOLD_3 = "gold_3";
    static final String SKU_GOLD_4 = "gold_4";
    static final String SKU_GOLD_5 = "gold_5";
    static final String SKU_GOLD_6 = "gold_6";
    static final String SKU_GOLD_8 = "gold_8";
    static final String SKU_PROME_CODE_1 = "gold_101";
    static final String SKU_PROME_CODE_2 = "gold_102";
    static final String SKU_PROME_CODE_3 = "gold_103";
    static final String SKU_PROME_CODE_4 = "gold_106";
    static final String SKU_PROME_CODE_5 = "gold_107";
	static final List<String> productIDs = Arrays.asList(
					SKU_GOLD_1
					,SKU_GOLD_2
					,SKU_GOLD_3
					,SKU_GOLD_4
					,SKU_GOLD_5
					,SKU_GOLD_6
					,SKU_GOLD_8
					,SKU_PROME_CODE_1
					,SKU_PROME_CODE_2
					,SKU_PROME_CODE_3
					,SKU_PROME_CODE_4
					,SKU_PROME_CODE_5
					);

    // request code
    static final int Request_Code = 3578;


    public void debugLog(String msg) {
        if (Debug) {
            Log.d(Debug_Tag,msg);
        }
    }

    // wait to consume product cache
    static Map<String,Purchase> m_toBeConsumed = new HashMap<String,Purchase>();

    // help instance
    public IabHelper m_helper = null;

    // init state
    public boolean m_isInitSuccess = false;

    public boolean queryingPurchaseOrder = false;
    
    public void doInit() {
        if (!m_isInitSuccess) {
            debugLog("Creating IAB helper.");
            m_helper = new IabHelper(m_activity.get(),Encoded_Public_key);
            m_helper.enableDebugLogging(Debug);
            debugLog("Starting setup.");
            m_helper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
                public void onIabSetupFinished(IabResult result) {
                    debugLog("Setup finished.");

                    if (!result.isSuccess()) {
                        // Oh noes, there was a problem.
                        debugLog("Problem setting up in-app billing: " + result);
                        return;
                    }

                    // Have we been disposed of in the meantime? If so, quit.
                    if (m_helper == null) return;

                    // not set up ? quit
                    if (!m_helper.mSetupDone) return;

                    m_isInitSuccess = true;
                    
                    // IAB is fully set up. Now, let's get an inventory of stuff we own.
//                    debugLog("Setup successful. Querying inventory.");
//                    m_helper.queryInventoryAsync(mGotInventoryListener);
//    				try{
//    					queryPurchaseOrder();
//    				}catch(java.lang.OutOfMemoryError e){
//    					Runtime.getRuntime().gc();
//    				}
                }
            });
        }
    }

	public void queryPurchaseOrder() {
		try {
			if(m_helper != null){
				Log.d("COK", "COK queryPurchaseOrder");
				queryingPurchaseOrder = true;
				m_helper.queryInventoryAsync(true,productIDs,mGotInventoryListener);//
			}
		} catch (java.lang.IllegalStateException e) {
			queryingPurchaseOrder = false;
			e.printStackTrace();
		}
	}
	
    private void setWaitScreen(boolean isVisible) {
        // todo: add a wait payment interface
    }

    // Listener that's called when we finish querying the items and subscriptions we own
    IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
        public void onQueryInventoryFinished(IabResult result, Inventory inventory) {
        	queryingPurchaseOrder = false;
            debugLog("Query inventory finished.");

            // Have we been disposed of in the meantime? If so, quit.
            if (m_helper == null) return;

            // Is it a failure?
            if (result.isFailure()) {
                debugLog("Failed to query inventory: " + result);
                return;
            }

//            m_isInitSuccess = true;
            debugLog("Query inventory was successful.");
            try{           
	            String payInfo = "";
	            debugLog(inventory.getAllPurchases().toString());
				for(String sku:productIDs){
					debugLog(sku);
					if(inventory.hasDetails(sku)){
						debugLog(inventory.getSkuDetails(sku).getPrice());
//						if(inventory.getSkuDetails(sku).getPriceCurrencyCode().equals("EUR")
//								|| inventory.getSkuDetails(sku).getPriceCurrencyCode().equals("RUB")
//								|| inventory.getSkuDetails(sku).getPriceCurrencyCode().equals("TWD")
//								|| inventory.getSkuDetails(sku).getPriceCurrencyCode().equals("KRW")
//								|| inventory.getSkuDetails(sku).getPriceCurrencyCode().equals("HKD")
//								|| inventory.getSkuDetails(sku).getPriceCurrencyCode().equals("JPY")
//								)
						payInfo += sku + ":" + inventory.getSkuDetails(sku).getPrice() + ";";
					}
				}
				debugLog(payInfo);
//		        if(payInfo != ""){
		        	PayGoogle.nativeCallPayInfo(payInfo);
//		        }
            }
            catch(Exception ex)
            {
            	
            }
            /*
             * Check for items we own. Notice that for each purchase, we check
             * the developer payload to see if it's correct! See
             * verifyDeveloperPayload().
             */

            // Check for gold delivery -- if we own gold, we should fill up the pocket immediately
            for (Purchase purchase : inventory.getAllPurchases()) {
                if (purchase != null && verifyDeveloperPayload(purchase) && !queryingPurchaseOrder) {
                    sendConsumeGold(purchase);
                }
            }

            debugLog("Initial inventory query finished; enabling main UI.");
        }
    };

    /** Verifies the developer payload of a purchase. */
    private boolean verifyDeveloperPayload(Purchase p) {

        // todo:right now just skip this check!

//        String payload = p.getDeveloperPayload();

        /*
         * TODO: verify that the developer payload of the purchase is correct. It will be
         * the same one that you sent when initiating the purchase.
         *
         * WARNING: Locally generating a random string when starting a purchase and
         * verifying it here might seem like a good approach, but this will fail in the
         * case where the user purchases an item on one device and then uses your app on
         * a different device, because on the other device you will not have access to the
         * random string you originally generated.
         *
         * So a good developer payload has these characteristics:
         *
         * 1. If two different users purchase an item, the payload is different between them,
         *    so that one user's purchase can't be replayed to another user.
         *
         * 2. The payload must be such that you can verify it even when the app wasn't the
         *    one who initiated the purchase flow (so that items purchased by the user on
         *    one device work on other devices owned by the user).
         *
         * Using your own server to store and verify developer payloads across app
         * installations is recommended.
         */
    	
    	if(StringUtils.isBlank(p.getDeveloperPayload()))
    		return true;
    	else
    		return p.getDeveloperPayload().indexOf(Device.getUid()) != -1;
    }

    // Callback for when a purchase is finished
    IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
        public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
            debugLog("Purchase finished: " + result + ", purchase: " + purchase);

            // if we were disposed of in the meantime, quit.
            if (m_helper == null) return;

            if (result.isFailure()) {
                debugLog("Error purchasing: " + result);
                callPayFailed(result.getResponse(),result.getMessage());
                setWaitScreen(false);
                return;
            }
            if (!verifyDeveloperPayload(purchase)) {
                debugLog("Error purchasing. Authenticity verification failed.");
                callPayFailed(-2,"Google payment verify failed!");
                setWaitScreen(false);
                return;
            }

            debugLog("Purchase successful.");

            sendConsumeGold(purchase);
        }
    };

    // Called when consumption is complete
    IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener() {
        public void onConsumeFinished(Purchase purchase, IabResult result) {
            debugLog("Consumption finished. Purchase: " + purchase + ", result: " + result);

            // if we were disposed of in the meantime, quit.
            if (m_helper == null) return;

            m_toBeConsumed.remove(purchase.getOrderId());

            // We know this is the "gas" sku because it's the only one we consume,
            // so we don't check which sku was consumed. If you have more than one
            // sku, you probably should check...
            if (result.isSuccess()) {
                // successfully consumed, so we apply the effects of the item in our
                // game world's logic, which in our case means filling the gas tank a bit
                debugLog("Consumption successful. Provisioning.");

                // todo: add gold to user account
            }
            else {
                debugLog("Error while consuming: " + result);
            }
            setWaitScreen(false);
            debugLog("End consumption flow.");
        }
    };

    public void buyGold(String goldType,String itemId,String toUID) {
        // launch the gas purchase UI flow.
        // We will be notified of completion via mPurchaseFinishedListener

        if (!m_isInitSuccess) {
            callPayFailed(-1,"Google payment not init!");
            return;
        }

        setWaitScreen(true);
        debugLog("Launching purchase flow for gold.");

        // consume package
        String sku = goldType;
//        switch (goldType) {
//            case 2 :{
//                sku = SKU_GOLD_2;
//                break;
//            }
//            case 3 :{
//                sku = SKU_GOLD_3;
//                break;
//            }
//            case 4 :{
//                sku = SKU_GOLD_4;
//                break;
//            }
//            case 5 :{
//                sku = SKU_GOLD_5;
//                break;
//            }
//            case 11 :{
//                sku = SKU_GOLD_11;
//                break;
//            }
//            default:
//                break;
//        }

        /* TODO: for security, generate your payload here for verification. See the comments on
         *        verifyDeveloperPayload() for more info. Since this is a SAMPLE, we just use
         *        an empty string, but on a production app you should carefully generate this. */
        
        try {
        	String perload = Device.getUid() + "|" + itemId;
        	if(toUID.length() > 0){
        		perload = perload + "|" + toUID;
        	}
        	debugLog("google pay params: " + perload);
	        m_helper.launchPurchaseFlow(m_activity.get(), sku, Request_Code,
	                mPurchaseFinishedListener, perload);
        }catch(Exception e){
			e.printStackTrace();
		}
    }

    private void sendConsumeGold(Purchase purchase) {
        String orderId = purchase.getOrderId();
        String packageName = purchase.getPackageName();
        String productId = purchase.getSku();
        String token = purchase.getToken();
        String signature = purchase.getSignature();
        Long tmpTime = purchase.getPurchaseTime();
        String purchaseTime = tmpTime.toString();
        debugLog("Start consume purchase: " +
                "\n orderId: " + orderId +
                "\n packageName: " + packageName +
                "\n productId: " + productId +
                "\n token: " + token +
                "\n signature: " + signature
        );
        m_toBeConsumed.put(orderId,purchase);
        callPaySuccess(orderId,purchaseTime,productId,purchase.getOriginalJson(),signature);
    }

    public void consumeCallback(String orderId,int consumeState) {
        if (m_toBeConsumed.isEmpty() || m_toBeConsumed.get(orderId) == null) {
            debugLog("Consume callback error : cache purchase not match! orderId: "+orderId);
            return;
        }
        // consume state:
        // 1: consume success
        // 2: already consumed
        // 3: verify failed
        // todo: add state decorator
        switch (consumeState) {
            case 1:{
                debugLog("Consume callback : consume state:"+consumeState);
                break;
            }
            case 2:{
                debugLog("Consume callback : consume state:"+consumeState);
                break;
            }
            case 3:{
                debugLog("Consume callback : consume state:"+consumeState);
                break;
            }
            default:{
                debugLog("Consume callback : consume state:"+consumeState);
                break;
            }
        }
        final String tmpOrderId = orderId;
        IF.getInstance().runOnUiThread(new Runnable() {
            @Override
            public void run() {
            	try{
            		m_helper.consumeAsync(m_toBeConsumed.get(tmpOrderId), mConsumeFinishedListener);
            	} catch (java.lang.IllegalStateException e) {
        			e.printStackTrace();
        		}
            }
        });

    }

    public static native void nativeCallPayInfo(String info);
    
//    public static native void nativeCallPaySuccess(String orderId,String purchaseTime,String productId);


//    // Billing response codes
//    public static final int BILLING_RESPONSE_RESULT_OK = 0;
//    public static final int BILLING_RESPONSE_RESULT_USER_CANCELED = 1;
//    public static final int BILLING_RESPONSE_RESULT_BILLING_UNAVAILABLE = 3;
//    public static final int BILLING_RESPONSE_RESULT_ITEM_UNAVAILABLE = 4;
//    public static final int BILLING_RESPONSE_RESULT_DEVELOPER_ERROR = 5;
//    public static final int BILLING_RESPONSE_RESULT_ERROR = 6;
//    public static final int BILLING_RESPONSE_RESULT_ITEM_ALREADY_OWNED = 7;
//    public static final int BILLING_RESPONSE_RESULT_ITEM_NOT_OWNED = 8;


//    public static native void nativeCallPayFailed(int failedType,String failedMsg);
    
}
