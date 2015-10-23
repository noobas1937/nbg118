package org.nbg.util;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import android.net.Uri;

import org.cocos2dx.ext.Device;
import org.cocos2dx.ext.Udid;

public class CommonUtil {

	/**
	 * 根据Id取得名字
	 * 
	 * @param id
	 * @return
	 */
	public static String getStringName(int id) {
		if (GameContext.getActivityInstance() == null)
			return "";
		if (GameContext.getActivityInstance().getResources() == null) {
			return "";
		}
		String name = GameContext.getActivityInstance().getResources()
				.getString(id);
		return name;
	}
	
	public static boolean isEmpty(String value) {
		if (value == null || value.equals("")) {
			return true;
		} else {
			return false;
		}
	}
	
	public final static byte[] getAssetBitmap(String file) {
		try {
			// "image/icon.png"
			InputStream is = GameContext.getActivityInstance().getAssets()
					.open(file);
			int size = is.available();
			byte[] buf = new byte[size];
			is.read(buf, 0, size);
			return buf;
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	private static final String SHARE_IMG_DIR = "share_img";

	public final static Uri getAssetsBitmapForUri(String fileName) {
		try {
			if (!isAvaiableSDcard()) {
				return null;
			}
			File dir = new File(getSDcardPath() + File.separator
					+ SHARE_IMG_DIR);
			if (!dir.exists()) {
				dir.mkdir();
			}
			String share_imgPath = dir.getCanonicalPath() + File.separator
					+ fileName;
			File file = new File(share_imgPath);
			if (!file.exists()) {
				byte[] jpgData = getAssetBitmap(fileName);
				if (jpgData == null)
					return null;
				FileOutputStream fos = new FileOutputStream(file);
				fos.write(jpgData);
				fos.flush();
				fos.close();
				// DebugLog.d("TAG", "file:" + file.getCanonicalPath());
				// FileInputStream fis = new FileInputStream(file);
				// byte[] jpgData1 = new byte[fis.available()];
				// fis.read(jpgData1);
				// DebugLog.d("TAG", "size:" + jpgData1.length);
			}
			return Uri.fromFile(file);
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	public static boolean isAvaiableSDcard() {
		// 可能出异常 NullPointerException
		// Attempt to invoke interface method 'android.os.storage.StorageVolume[] android.os.storage.IMountService.getVolumeList()' on a null object reference
		try{
			String sdCardState = android.os.Environment.getExternalStorageState();
			return android.os.Environment.MEDIA_MOUNTED.equals(sdCardState);
		}
		catch (Exception e)
		{
			return false;
		}
	}

	public static String getSDcardPath() {
		try {
			File SDFile = android.os.Environment.getExternalStorageDirectory();
			return SDFile.getCanonicalPath();
		} catch (IOException e) {
			return null;
		}
	}
	
	public static boolean isVersion_Ad_Mdd() {
		return GameContext.getActivityInstance().getPackageName()
				.equals("com.stac.cok.main.mdd");
	}

	public static boolean isVersion_Global_Free() {
		return GameContext.getActivityInstance().getPackageName()
				.equals("com.stac.cok.main.free");
	}
	
	public static void setNewDeviceForTest() {
		// TODO
	}
	
	public static boolean isNewInstallDevice() {
		if (DeviceUtil.isEmulator()) {// 排除模拟器
			return false;
		}
		String cardUid = Device.getUid();
		if (CommonUtil.isEmpty(cardUid)) {// || cardUid.startsWith("Test-")) {
			Udid.isNewInstallDevice = true;
		}
		return Udid.isNewInstallDevice;
	}
	
	public static final float[] PRICE_USD = new float[] { 4.99f, 9.99f, 19.99f, 49.99f, 99.99f, 24.99f, 999.99f, 0.99f };
	public static final float[] PRICE_CNY = new float[] { 30.0f, 68.0f, 128.0f, 328.0f, 648.0f, 163.0f, 6498.0f, 6.0f };
	public static final float[] PRICE_THB = new float[] { 160.0f, 330.0f, 650.0f, 1650.0f, 3250.0f, 830.0f, 32500.0f, 35.0f };
	
	public static final String[] PRODUCT_COMMON = new String[] { "gold_1", "gold_2", "gold_3", "gold_4", "gold_5", "gold_6", "gold_7", "gold_8" };
	
	private static int getProductIndex(float price){
		int index = -1;
		for(int i=0;i<PRICE_USD.length;i++){
			if(PRICE_USD[i] == price)
				index = i;
		}
		return index;
	}

	public static float getPaidAmount(String currency,float price){
		float amount = 0;
		int index = getProductIndex(price);
		if(index != -1){
			amount = PRICE_USD[index];
			if(currency.equals("CNY"))
				amount = PRICE_CNY[index];
		}
		return amount;
	}
	
	public static int getPaidAmountInCents(String currency,float price){
		float amount = getPaidAmount(currency, price);
		return (int)(Math.ceil(amount*100));
	}
	
	public static String getPaidProduct(String pf,float price){
		String product = "";
		int index = getProductIndex(price);
		if(index != -1){
			product = PRODUCT_COMMON[index];
		}
		return product;
	}
	
}
