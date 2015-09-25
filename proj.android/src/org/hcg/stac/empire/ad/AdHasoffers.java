package org.hcg.stac.empire.ad;

import android.app.Activity;
import android.content.Context;

import org.hcg.IF.IF;
import com.mobileapptracker.MobileAppTracker;

public class AdHasoffers {
	private static MobileAppTracker mobileAppTracker;
	private static boolean isSetReffer;

	public static void initAdHasoffer(Context ctx) {
		mobileAppTracker = new MobileAppTracker(ctx, "8200",
				"e8e0af4f8f035510e8435fafad3b6890");
		try {
			mobileAppTracker.setReferrer(((Activity) ctx)
					.getCallingPackage());
			isSetReffer = true;
		} catch (Exception e) {

		}
		// if (true) {
		// mobileAppTracker.setAllowDuplicates(true);
		// mobileAppTracker.setDebugMode(true);
		// }
	}

	public static void onTrackUpdate() {
		if (mobileAppTracker != null) {
			mobileAppTracker.trackInstall();
			// mobileAppTracker.trackUpdate();
		}
	}

	public static void onTrackAction_purchase(float money, String currency) {
		if (!isSetReffer) {
			try {
				mobileAppTracker.setReferrer(IF.getInstance().getCallingPackage());
			} catch (Exception e) {
				
			}
		}
		if (mobileAppTracker != null) {
			mobileAppTracker.trackAction("purchase", money, currency);
		}
	}
}
