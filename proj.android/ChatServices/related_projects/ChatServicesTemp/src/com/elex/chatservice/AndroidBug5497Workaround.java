package com.elex.chatservice;

import android.app.Activity;
import android.graphics.Rect;
import android.view.View;
import android.view.ViewTreeObserver;
import android.widget.FrameLayout;

public class AndroidBug5497Workaround {

    // For more information, see https://code.google.com/p/android/issues/detail?id=5497
    // To use this class, simply invoke assistActivity() on an Activity that already has its content view set.

    public static void assistActivity (Activity activity) {
        new AndroidBug5497Workaround(activity);
    }
    
    public static void assistActivity (View view) {
        new AndroidBug5497Workaround(view);
    }

    private View mChildOfContent;
    private int usableHeightPrevious;
    private FrameLayout.LayoutParams frameLayoutParams;
	private int tmpHeight=-100;

    private AndroidBug5497Workaround(Activity activity) {
        FrameLayout content = (FrameLayout) activity.findViewById(android.R.id.content);
        mChildOfContent = content.getChildAt(0);
        mChildOfContent.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
            public void onGlobalLayout() {
            	//只调这个函数也可以layout正确
//                mChildOfContent.requestLayout();
                
                possiblyResizeChildOfContent();
            }
        });
        frameLayoutParams = (FrameLayout.LayoutParams) mChildOfContent.getLayoutParams();
    }
    
    private AndroidBug5497Workaround(View view) {
        mChildOfContent = view;
        mChildOfContent.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
            public void onGlobalLayout() {
                possiblyResizeChildOfContent();
            }
        });
        frameLayoutParams = (FrameLayout.LayoutParams) mChildOfContent.getLayoutParams();
    }
    
    private void possiblyResizeChildOfContent() {
//    	System.out.println("possiblyResizeChildOfContent");
    	
        int usableHeightNow = computeUsableHeight();
        System.out.println("usableHeightPrevious:" + usableHeightPrevious);
        System.out.println("usableHeightNow:" + usableHeightNow);
        if (usableHeightNow != usableHeightPrevious) {
            int usableHeightSansKeyboard = mChildOfContent.getRootView().getHeight();
            System.out.println("usableHeightSansKeyboard:" + usableHeightSansKeyboard);
            int heightDifference = usableHeightSansKeyboard - usableHeightNow;
            System.out.println("heightDifference:" + heightDifference);
            if (heightDifference > (usableHeightSansKeyboard/4)) {
                // keyboard probably just became visible
                frameLayoutParams.height = usableHeightSansKeyboard - heightDifference;
//                frameLayoutParams.height = tmpHeight;
//                tmpHeight += 100;
            	System.out.println("1frameLayoutParams.height = " + frameLayoutParams.height);
            } else {
                // keyboard probably just became hidden
                frameLayoutParams.height = usableHeightSansKeyboard;
            	System.out.println("2frameLayoutParams.height = " + frameLayoutParams.height);
            }
            mChildOfContent.requestLayout();
            usableHeightPrevious = usableHeightNow;
            ChatServiceBridge.set2dxViewHeight(frameLayoutParams.height, usableHeightSansKeyboard);
        }
    }

    private int computeUsableHeight() {
        Rect r = new Rect();
        //In effect, this tells you the available area where content can be placed and remain visible to users.
        mChildOfContent.getWindowVisibleDisplayFrame(r);
        return (r.bottom - r.top);
    }

}