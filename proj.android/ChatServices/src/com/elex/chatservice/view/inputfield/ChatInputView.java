package com.elex.chatservice.view.inputfield;

import java.util.TimerTask;

import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
import android.graphics.Rect;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.SystemClock;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.util.TypedValue;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.util.CompatibleApiUtil;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.ResUtil;

public class ChatInputView extends RelativeLayout {

//	  private final BroadcastReceiver failedMessageRequestChecker;
//	  private final BroadcastReceiver connChecker;
	  private Activity activity;
	  private Bundle extras;
	  private String issueId;
//	  private MessagesAdapter adapter;
//	  private HSStorage hsStorage;
//	  private HSApiData hsApiData;
	  private String chatLaunchSource;
	  private Thread pollerThread;
	  private Handler pollerThreadHandler;
	  
	  private EditText replyField;
	  private LinearLayout messageBox;
	  private LinearLayout messageLinearLayout;
	  private TextView wordCount;
	  private Button addReply;
	  
	  private boolean persistMessageBox;
	  private boolean newActivity;
	  private Handler fetchMessagesSuccess;
	  private Handler fetchMessagesFailure;
	  private Handler replyHandler;
	  private Handler replyFailHandler;


	public ChatInputView(Activity context) {
		super(context);
		this.activity = context;
		
		System.out.println("ChatInputView()");
		
		View.inflate(context, ResUtil.getId(context, "layout", "cs__chat_input_fragment"), this);

//	    this.failedMessageRequestChecker = new BroadcastReceiver()
//	    {
//	      public void onReceive(Context context, Intent intent) {
//	        refreshMessages();
//	      }
//	    };
//	    this.connChecker = new BroadcastReceiver()
//	    {
//	      public void onReceive(Context context, Intent intent) {
////	        startPoller();
//	      }
//	    };

	    this.persistMessageBox = false;
//	    this.newActivity = true;

//	    this.replyHandler = new Handler() {
//	      public void handleMessage(Message msg) {
//	        renderReplyMsg(msg);
//	      }
//	    };
//	    this.replyFailHandler = new Handler() {
//	      public void handleMessage(Message msg) {
//	        super.handleMessage(msg);
//	        if (pollerThreadHandler != null) {
//	          pollerThreadHandler.getLooper().quit();
//	        }
//	        refreshMessages();
//	      }
//	    };
	    
//	    onCreateView(null, null, null);
	    onViewCreated();
	    
	    //在小米pad上，点击输入框、弹出软键盘后，输入框消失了，输入三个回车之后才能显示
//		replyField.setOnTouchListener(new View.OnTouchListener() {
//			public boolean onTouch(View view, MotionEvent motionEvent) {
//	        	System.out.println("ChatInputView onTouch()");
//				activity.runOnUiThread(new Runnable() {
//					public void run() {
////						boolean textChanged = false;
////						if(getInputText()==""){
////							replyField.setText("\n\n\n", TextView.BufferType.EDITABLE);//replyField.setText("\n\n\n");
////							textChanged = true;
////						}
//						
//						//在键盘打开的情况下点击时，不能隐藏
////						replyField.setVisibility(View.INVISIBLE);
//						activity.getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_VISIBLE);
////						if(textChanged){
////							replyField.setText("");
////						}
////						replyField.setVisibility(View.VISIBLE);
//						
////						simulateTouchOnEditText();
//						
//						//无效
////						setVisibility(View.VISIBLE);
//						
//						//会导致点击无法使输入框显形（回车可以）
////						postInvalidate();
//						
//						//无效
////						invalidate();
//						
////						printControls();
//					}
//				});
//				
//				return false;
//			}
//		});
		
		//按back键，有软键盘时会触发此函数一次，关闭后会触发两次
		replyField.setOnKeyListener(new View.OnKeyListener() {
			@Override
			public boolean onKey(View v, int keyCode, KeyEvent event) {
				  System.out.println("onKey()");
			      if (keyCode == KeyEvent.KEYCODE_BACK) {
			    	  System.out.println("KeyEvent.KEYCODE_BACK");
					  System.out.println("isKeyboardOpen = " + isKeyboardOpen);
//			    	  System.out.println("getVisibility() = " + getVisibility());
			    	  if(!isKeyboardOpen && getVisibility() == View.VISIBLE){
			    		  System.out.println("call ChatServiceController.onBackPressed");
			    		  ChatServiceController.getInstance().host.onBackPressed();
			    	  }
			      }
			      
				return false;
			}
		});

        getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
	        @TargetApi(Build.VERSION_CODES.HONEYCOMB)
	        public void onGlobalLayout() {
				if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
					activity.runOnUiThread(new Runnable() {
						public void run() {
							try {
								adjustHeight();

								Rect r = new Rect();
							    getWindowVisibleDisplayFrame(r);
							    int screenHeight = getRootView().getHeight();
							    // r.bottom is the position above soft keypad or device button.
							    // if keypad is shown, the r.bottom is smaller than that before.
							    int keypadHeight = screenHeight - r.bottom;
							    if (keypadHeight > screenHeight * 0.15) {
							    	isKeyboardOpen = true;
								    System.out.println("isKeyboardOpen = " + isKeyboardOpen);
								    printEnabeld();
							    }
							    else {
							    	//用back键关闭软键盘，触发此函数后，马上会接着触发onKey，所以延迟改变isKeyboardOpen，避免触发退出界面逻辑
							    	new java.util.Timer().schedule(new TimerTask(){
							    	       public void run() {
							    	    	   isKeyboardOpen = false;
											   System.out.println("isKeyboardOpen = " + isKeyboardOpen);
											   printEnabeld();
							    	    	   this.cancel();
							    	       }}, 50);
							    }
							} catch (Exception e) {
							}
						}
					});
				}
	        }
	    });
	  }
	
	private void printEnabeld()
	{
		System.out.println("this.getVisibility() = " + this.getVisibility());
		System.out.println("replyField.isEnabled() = " + this.replyField.isEnabled());
	}
	
	private boolean isKeyboardOpen = false;

	//焦点在replyField，view自身收不到下面的事件
	//To receive a keyboard event, a View must have focus.
//	setFocusableInTouchMode(true);
//	requestFocus();
//	  @Override
//	  public boolean onKeyPreIme(int keyCode, KeyEvent event) {
//		  System.out.println("onKeyPreIme()");
//	      if (keyCode == KeyEvent.KEYCODE_BACK) {
//	    	  System.out.println("KeyEvent.KEYCODE_BACK");
//	      }
//	      super.onKeyPreIme(keyCode, event);
//	      return false;
//	  }
//
//	  @Override
//	  public boolean onKeyDown(int keyCode, KeyEvent event) {
//		  System.out.println("onKeyDown()");
//	      if (keyCode == KeyEvent.KEYCODE_BACK) {
//	    	  System.out.println("KeyEvent.KEYCODE_BACK");
//	      }
//	      super.onKeyDown(keyCode, event);
//	      return false;
//	  }

	@TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
	public void simulateTouchOnEditText()
	{
		//无效
		long downTime = SystemClock.uptimeMillis();  
		long eventTime = SystemClock.uptimeMillis() + 100;  
		// List of meta states found here: developer.android.com/reference/android/view/KeyEvent.html#getMetaState()  
		int metaState = 0;  
		MotionEvent evt = MotionEvent.obtain(downTime, eventTime, MotionEvent.ACTION_DOWN, 10.0f, 10.0f, metaState);
		replyField.onTouchEvent(evt);
		
		//无效
		// Show the IME, except when selecting in read-only text.
        final InputMethodManager imm = (InputMethodManager)this.activity.getSystemService(Context.INPUT_METHOD_SERVICE);
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH) {
			imm.viewClicked(replyField);
		}
        imm.showSoftInput(replyField, 0);
        // The above condition ensures that the mEditor is not null
//        mEditor.onTouchUpEvent(event);
	}
	
	private void printControls()
	{
		try {
			printChildren(this, "");
		} catch (Exception e) {
			LogUtil.printException(e);
		}
		
	}
	
	private void printChildren(ViewGroup parent, String tab)
	{
//		System.out.println("printChildren " + parent.getId());
		printChild(parent, tab);
		for (int i = 0; i < parent.getChildCount(); i++) {
        	if(parent.getChildAt(i) instanceof ViewGroup){
        		printChildren((ViewGroup)parent.getChildAt(i), tab + "	");
        	}else if(parent.getChildAt(i) instanceof View){
        		printChild(parent.getChildAt(i), tab);
        	}else{
        		System.out.println("type error: " + parent.getChildAt(i).getClass().toString());
        	}
		}
	}
	
	@TargetApi(Build.VERSION_CODES.HONEYCOMB)
	private void printChild(View child, String tab)
	{
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
			System.out.println(tab+"Id = " + child.getId());
			System.out.println(tab+"	visibility = " + child.getVisibility());
			System.out.println(tab+"	alpha = " + child.getAlpha());
			System.out.println(tab+"	rect = [" + child.getX() + ", " + child.getY() + ", " + child.getWidth() + ", " + child.getHeight()+"]");
			System.out.println(tab+"	");
		}
	}
    
//	@TargetApi(Build.VERSION_CODES.HONEYCOMB)
//	@Override
//	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
//		System.out.println("onMeasure()");
//		final int proposedheight = MeasureSpec.getSize(heightMeasureSpec);
//		final int actualHeight = getHeight();
//
//		if (actualHeight > proposedheight) {
//			// Keyboard is shown
//			System.out.println("Keyboard is shown");
//		} else {
//			// Keyboard is hidden
//			System.out.println("Keyboard is hidden");
//		}
//        int usableHeightNow = computeUsableHeight();
//        System.out.println("usableHeightNow:" + usableHeightNow);
//        System.out.println("getRootView().getHeight():" + getRootView().getHeight());
//		System.out.println("actualHeight = "+actualHeight);
//		System.out.println("proposedheight = "+proposedheight);
//		System.out.println("this.Y = "+this.getY());
//		System.out.println("replyField.Y = "+replyField.getY());
//
//		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
//	}

	  public void onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
	  {
		System.out.println("ChatInputView onCreateView");
//	    this.activity = ((Activity)getActivity());

//	    this.activity.setSupportProgressBarIndeterminateVisibility(false);
//	    this.extras = getArguments();
//	    this.extras.remove("message");

//	    this.hsApiData = new HSApiData(this.activity);
//	    this.hsStorage = this.hsApiData.storage;

//	    this.newIssue = Boolean.valueOf(this.extras.getBoolean("newIssue", false));
//	    this.decomp = Boolean.valueOf(this.extras.getBoolean("decomp", false));
//	    this.chatLaunchSource = this.extras.getString("chatLaunchSource");
//	    this.showConvOnReportIssue = Boolean.valueOf(this.extras.getBoolean("showConvOnReportIssue"));

//	    setHasOptionsMenu(true);

//	    return inflater.inflate(HSRes.getId(this, "layout", "cs__chat_input_fragment"), container, false);
	  }
	  
	  public String getInputText()
	  {
//		  System.out.println("ChatInputView getInputText");
		  return replyField.getText().toString().trim();
	  }
	  
	  private void adjustTextSize(TextView textView, double textRatio)
	  {
	    System.out.println("old getTextSize() = "+textView.getTextSize());//28
		System.out.println("textRatio = "+textRatio);//1.839
		float newTextSize = (int) (textView.getTextSize()*textRatio);
		System.out.println("newTextSize = "+newTextSize);
		textView.setTextSize(TypedValue.COMPLEX_UNIT_PX, newTextSize);
	    System.out.println("new getTextSize() = "+textView.getTextSize());
	  }

	  public void adjustHeight()
	  {
		//S3手机上的尺寸
		int originalW = 173;
		int originalH = 84;
		//资源尺寸
//		BitmapDrawable bmap = (BitmapDrawable) this.getResources().getDrawable(R.drawable.btn_yellow);
//		int originalW = bmap.getBitmap().getWidth();
//		int originalH = bmap.getBitmap().getHeight();

		double ratio =  (double)originalH /  (double)originalW;
		
		if(addReply.getWidth() != 0 && ratio !=0 && addReply.getHeight() != (int) (addReply.getWidth()*ratio))
		{
			System.out.println("do adjunstHeight()");
		    System.out.println("actually change height");
			System.out.println("addReply.width = "+addReply.getWidth());
			System.out.println("addReply.height = "+addReply.getHeight());
			System.out.println("originalH = "+originalH);
			System.out.println("originalW = "+originalW);
			System.out.println("ratio = "+ratio);
			addReply.setLayoutParams(new LinearLayout.LayoutParams(addReply.getWidth(), (int) (addReply.getWidth()*ratio)));
		    
			double textRatio =  (double)addReply.getWidth() /  (double)originalW;
			//在大屏上字体可能会偏大，可能需要用dp计算才行
			//先加个修正因子
			textRatio = textRatio > ((double)1/0.84390234277028) ? textRatio * 0.84390234277028 : textRatio;
		    System.out.println("addReply adjustTextSize");
			adjustTextSize(addReply,textRatio);
		    System.out.println("replyField adjustTextSize");
			adjustTextSize(replyField,textRatio);
			adjustTextSize(wordCount,textRatio);
//			replyField.setText("\n\n\n");
//			replyField.setText("\n");
		}
	  }
	  
	//对原生程序有效，对2dx程序无效，width为0
//	public void onWindowFocusChanged(boolean hasFocus) {
//	    super.onWindowFocusChanged(hasFocus);
//	    System.out.println("onWindowFocusChanged()");
//		System.out.println("addReply.width = "+addReply.getWidth());
//		System.out.println("addReply.height = "+addReply.getHeight());
//		
//		adjunstHeight();
//	 }
	  
	  public void onViewCreated()
	  {
		System.out.println("ChatInputView onViewCreated");
//	    super.onViewCreated(view, savedInstanceState);

	    this.messageBox = ((LinearLayout)this.findViewById(ResUtil.getId(this.activity, "id", "cs__chat_input_relativeLayout")));
//		System.out.println("1");
	    this.replyField = ((EditText)this.findViewById(ResUtil.getId(this.activity, "id", "cs__messageText")));
//		System.out.println("2");
	    this.wordCount = ((TextView)this.findViewById(ResUtil.getId(this.activity, "id", "wordCountTextView")));
//		System.out.println("3");
	    addReply = (Button)this.findViewById(ResUtil.getId(this.activity, "id", "hs__sendMessageBtn"));
//		System.out.println("4");

//	    this.messageLinearLayout = ((LinearLayout)this.findViewById(HSRes.getId(this.activity, "id", "hs__sendMessageLinearLayout")));
		
	    refreshStatus();

	    refreshButton();

	    addReply.setOnClickListener(new View.OnClickListener()
	    {
	      public void onClick(View view) {
	        String replyText = getInputText();
//	        String identity = hsStorage.getIdentity();

	        if (!TextUtils.isEmpty(replyText)) {
	          replyField.setText("");
	          try
	          {
	        	  sendMessage(replyText);
	          } catch (Exception e) {
	    		  System.out.println(e.getMessage());
	          }
	          
//	          addMessage(replyHandler, replyFailHandler, issueId, replyText, "txt", "");
//	          try
//	          {
//	            JSONObject eventData = new JSONObject();
//	            eventData.put("type", "txt");
//	            eventData.put("body", replyText);
//	            eventData.put("id", issueId);
//	            HSFunnel.pushEvent("m", eventData);
//	          } catch (JSONException e) {
//	            Log.d("HelpShiftDebug", "JSONException", e);
//	          }
	        }
	      }
	    });
	    this.replyField.setOnEditorActionListener(new TextView.OnEditorActionListener() {
	      public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
	        if (actionId == 4) {
	          addReply.performClick();
	        }
	        return false;
	      }
	    });
	    this.replyField.addTextChangedListener(new TextWatcher()
	    {
	      public void afterTextChanged(Editable s) {
	          replyField.post(new Runnable() {
	              @Override
	              public void run() {
	              	refreshWordCount();
	              }
	          });
	      }

	      public void beforeTextChanged(CharSequence s, int start, int count, int after) {
	      }

	      public void onTextChanged(CharSequence s, int start, int before, int count) {
	        persistMessageBox = true;
	        refreshButton();
	        activity.runOnUiThread(new Runnable() {
				public void run() {
					try {
						ChatServiceController.getInstance().host.onTextChanged(getInputText());
					} catch (Exception e) {
						// reportException(Main.getInstance(), e);
					}
				}
			});
	      }
	    });
	  }
	  
	  private void refreshButton()
	  {
	        if (getInputText().length() == 0) {
	          addReply.setEnabled(false);
	          CompatibleApiUtil.getInstance().setButtonAlpha(addReply, false);
	        } else {
	          addReply.setEnabled(true);
	          CompatibleApiUtil.getInstance().setButtonAlpha(addReply, true);
	        }
	  }
	  
	  private void sendMessage(String messageText)
	  {
		  System.out.println("ChatInputView sendMessage");
		  System.out.println("messageText = " + messageText);
		  ChatServiceController.getInstance().host.sendMessage(messageText);
//		  ChatServiceController.disableChatInputView();
//		  this.hideKeyboard();
//		  ChatServiceController.enableChatInputView();
//		  this.showKeyboard(this);
	  }
	    
	    public LayoutParams getContainerViewLayoutParams(){
	    	return (LayoutParams)this.getLayoutParams();
	    }
	    
	    public void setContainerViewLayoutParams(LayoutParams containerViewLayoutParams){
	    	this.setLayoutParams(containerViewLayoutParams);
	    }
	  
//	  private void renderReplyMsg(Message msg)
//	  {
//	    HashMap result = (HashMap)msg.obj;
//	    JSONObject message = (JSONObject)result.get("response");
//	    JSONArray messages = new JSONArray();
//	    messages.put(message);
//	    try
//	    {
//	      this.hsStorage.storeMessages(this.issueId, messages);
//	    } catch (JSONException e) {
//	      Log.d("HelpShiftDebug", "JSONException", e);
//	    }
//
//	    refreshMessages();
//	  }

	  private void refreshStatus()
	  {
		  showMessageBox();
	  }

	  private void showMessageBox()
	  {
			System.out.println("ChatInputView showMessageBox");
//	    this.adapter.enableButtons(true);
	    this.messageBox.setVisibility(View.VISIBLE);
	    refreshWordCount();
	  }

	  private void showKeyboard(View v)
	  {
		System.out.println("ChatInputView showKeyboard");
	    v.requestFocus();
	    InputMethodManager imm = (InputMethodManager)this.activity.getSystemService(Context.INPUT_METHOD_SERVICE);
	    imm.showSoftInput(v, 0);
	  }
	  
	  public void setSendButtonText(String text) {
		  this.addReply.setText(text);
	  }
	  
	  public void setEditTextHintText(String hint) {
		  this.replyField.setHint(hint);
	  }
	  
	  public void hideKeyboard() {
		System.out.println("ChatInputView hideKeyboard");
	    InputMethodManager inputmanger = (InputMethodManager)this.activity.getSystemService(Context.INPUT_METHOD_SERVICE);
//	    inputmanger.hideSoftInputFromWindow(this.replyField.getWindowToken(), 0);
        inputmanger.hideSoftInputFromWindow(this.activity.getCurrentFocus().getWindowToken(),  0); 
	    
//        inputmanger.hideSoftInputFromWindow(this.getWindowToken(), 0);
        
//        inputmanger.hideSoftInputFromWindow(this.activity.getCurrentFocus().getWindowToken(),  InputMethodManager.HIDE_NOT_ALWAYS);
        
//        replyField.postDelayed(new Runnable() {
//        	  @Override
//        	  public void run() {
//        		InputMethodManager inputmanger = (InputMethodManager) this.activity.getSystemService(Context.INPUT_METHOD_SERVICE);
//                inputmanger.hideSoftInputFromWindow(this.activity.getCurrentFocus().getWindowToken(),  0); 
//        	  }
//        	},50);
	  }
	  
	    public void setEnabled(boolean enabled) {
	    	this.replyField.setEnabled(enabled);
	    	this.addReply.setEnabled(enabled);
	    	if(!enabled){
	    		CompatibleApiUtil.getInstance().setButtonAlpha(addReply, enabled);
	    	}else{
	    		refreshButton();
	    	}
	    	super.setEnabled(enabled);
	    	
//	    	runOnUiThread(new Runnable() {
//	    	     @Override
//	    	     public void run() {
//
//	    		    	this.replyField.setEnabled(enabled);
//	    		    	this.addReply.setEnabled(enabled);
//	    		    	super.setEnabled(enabled);
//
//	    	    }
//	    	});
//	    	
//	    	Thread thread = new Thread(){
//	            @Override
//	            public void run() {
//	                try {
//	                    synchronized (this) {
//
//	                        runOnUiThread(new Runnable() {
//	                            @Override
//	                            public void run() {
//	                				if(chatInputView != null && chatInputView.getVisibility() == View.VISIBLE){
//	                					chatInputView.setEnabled(true);
//	                				}
//	                            }
//	                        });
//
//	                    }
//	                } catch (InterruptedException e) {
//	                    e.printStackTrace();
//	                }
//	            };
//	        };  
//	        thread.start();
	    }

//	  private void refreshMessages() {
//	  }
//
//	  private void refreshMessages(JSONArray messages) {
//	  }

	  
	  private void refreshWordCount()
	  {
	      if(replyField.getLineCount()>2)
	      {
	      	wordCount.setVisibility(View.VISIBLE);
	      }else{
	      	wordCount.setVisibility(View.GONE);
	      }
	      wordCount.setText(getInputText().length() + "/500");
	  }
	  
//	  private void addMessage(Handler success, Handler failure, String issueId, String messageText, String type, String refers)
//	  {
//	    this.hsStorage.setScreenShotDraft(Boolean.valueOf(false));
//	    this.hsApiData.addMessage(success, failure, issueId, messageText, type, refers);
//	  }
//
//	  private void addMessage(Handler success, Handler failure, String issueId, String messageText, String type, String refers, int failedState)
//	  {
//	    this.hsStorage.setScreenShotDraft(Boolean.valueOf(false));
//	    this.hsApiData.addMessage(success, failure, issueId, messageText, type, refers, failedState);
//	  }
//
//	  private void gotoApp(String marketUrl)
//	  {
//	  }

}
