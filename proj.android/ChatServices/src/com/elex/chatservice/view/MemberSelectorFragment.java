package com.elex.chatservice.view;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map.Entry;
import java.util.Set;

import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ExpandableListView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;

import com.elex.chatservice.R;
import com.elex.chatservice.controller.ChatServiceController;
import com.elex.chatservice.controller.MenuController;
import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.UserInfo;
import com.elex.chatservice.model.UserManager;
import com.elex.chatservice.model.db.DBDefinition;
import com.elex.chatservice.util.CompatibleApiUtil;
import com.elex.chatservice.util.LogUtil;
import com.elex.chatservice.util.ResUtil;
import com.elex.chatservice.util.ScaleUtil;
import com.elex.chatservice.view.actionbar.ActionBarFragment;

public class MemberSelectorFragment extends ActionBarFragment
{
    private RelativeLayout memberListLayout;
	private LinearLayout chatroom_name_layout;
	private TextView chatroom_name;
	private EditText chatroom_input;

    private Button cs__cancelBtn;
    private Button cs__okBtn;
    private Button cs__quitBtn;
    
    private ExpandableListAdapter listAdapter;
    private ExpandableListView expListView;
    private static List<String> listDataHeader;
    private static HashMap<String, ArrayList<UserInfo>> listDataChild;
    
    private Button buttonAlliance;
    private Button buttonSearch;
    private LinearLayout search_name_layout;
    private TextView search_name;
    private EditText search_name_input;
    private Button search_btn;
    private LinearLayout buttonsLinearLayout;
    private RelativeLayout selected_layout;
    private ExpandableListAdapter selectedListViewAdapter;
    private ExpandableListView selectedListView;
    private static List<String> searchedListDataHeader;
    private static HashMap<String, ArrayList<UserInfo>> searchListDataChild;
    
	public ArrayList<String> memberUidAdded=null;
	public ArrayList<String> memberUidRemoved=null;
	
	public ArrayList<String> commonMemberUidAdded=null;
	public ArrayList<String> commonMemberUidRemoved=null;
	
	public static String roomName="";
	
	public static int memberTab=1;

    public MemberSelectorFragment()
    {
    	System.out.println("MemberSelectorFragment");
    	resetMemberData();
    }

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
    }

    private static Bundle instanceState;
    public void saveState()
    {
        instanceState = new Bundle();
    }

    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
    {
        this.activity = ((MemberSelectorActivity)getActivity());

        return inflater.inflate(ResUtil.getId(this, "layout", "cs__member_selector_fragment"), container, false);
    }

    public static int index = 0;
    
    public void setDataSelectedState(boolean isAllianceTab,String uid,Boolean isSelected)
    {
    	if(isAllianceTab)
    	{
    		listAdapter.setSelectState(uid, isSelected);
    	}
    	else
    	{
    		selectedListViewAdapter.setSelectState(uid, isSelected);
    	}
    }
    
    public void notifyDataSetChanged()
	{
    	boolean isAlliance=(selected_layout!=null && selected_layout.getVisibility()==View.GONE && memberListLayout!=null && memberListLayout.getVisibility()==View.VISIBLE);
		try
		{
			if (listAdapter != null && isAlliance)
			{
				System.out.println("notifyDataSetChanged  ttt");
				listAdapter.notifyDataSetChanged();
			}
			else if (selectedListViewAdapter != null && !isAlliance)
			{
				System.out.println("notifyDataSetChanged  ssss");
				selectedListViewAdapter.notifyDataSetChanged();
				for (int j = 0; j < searchedListDataHeader.size(); j++)
				{
					selectedListView.expandGroup(j);
				}
			}
		}
		catch (Exception e)
		{
			LogUtil.printException(e);
		}
	}
    
    private ArrayList<String> sortRank()
    {
    	ArrayList<String> rankArr = new ArrayList<String>();
    	HashMap<String,Integer> rankMap=UserManager.getInstance().getRankMap();
    	if(rankMap == null) return rankArr;
    	
    	Set<String> rankSet=rankMap.keySet();
    	
    	HashMap<String,Integer> tempMap=new HashMap<String, Integer>();
    	String[] tempKeyArr={"","","","",""};
		for(int i=5;i>0;i--)
		{
			String key=UserManager.getInstance().getRankLang(i);
			if(rankSet.contains(key))
				tempKeyArr[i-1]=key;
		}
    	
		for(int i=4;i>=0;i--)
		{
			if(!tempKeyArr[i].equals(""))
				rankArr.add(tempKeyArr[i]);
		}
//		Set<String> tempSet=tempMap.keySet();
//    	for(String key:tempSet)
//    	{
//    		System.out.println("key:"+key);
//    		if(!key.equals("") && tempMap.get(key).intValue()!=0)
//    			rankArr.add(0,key);
//    	}
    	return rankArr;
    }

    /*
     * Preparing the list data
     */
    private void prepareListData() {
		
    	HashMap<String,ArrayList<UserInfo>> chatRoomMemberMap=UserManager.getInstance().getChatRoomMemberMap();
    	if(chatRoomMemberMap == null) return;
    	
        listDataHeader = sortRank();
        listDataChild = chatRoomMemberMap;
    }
    
    private void prepareJoinedListData()
    {
//    	searchedListDataHeader=new ArrayList<String>();
//    	searchedListDataHeader.add("已选择成员");
//    	searchListDataChild=new HashMap<String, ArrayList<UserInfo>>();
//    	ArrayList<UserInfo> userInfoArr=new ArrayList<UserInfo>();
//    	UserInfo user1=new UserInfo("364515623652");
//    	UserInfo user2=new UserInfo("123465484813");
//    	UserInfo user3=new UserInfo("1354867897135");
//    	userInfoArr.add(user1);
//    	userInfoArr.add(user2);
//    	userInfoArr.add(user3);
//    	searchListDataChild.put("已选择成员", userInfoArr);
    	searchedListDataHeader=new ArrayList<String>();
    	searchListDataChild=new HashMap<String, ArrayList<UserInfo>>();
    	ArrayList<String> arr=UserManager.getInstance().getSelctedMemberArr(false);
    	if(arr!=null && arr.size()>0)
    	{
    		String title=LanguageManager.getLangByKey(LanguageKeys.TIP_SELECTED_MEMBER);
    		if(title.trim().equals(""))
    			title="已加入";
    		searchedListDataHeader.add(title);
    		searchListDataChild=UserManager.getInstance().getJoinedMemberMap(title, arr);
    	}
    }
    
    public void refreshSearchListData(ArrayList<UserInfo> searchUserInfoArr)
    {
    	if(searchUserInfoArr==null)
    		return;
    	System.out.println("refreshSearchListData searchUserInfoArr size:"+searchUserInfoArr.size());
    	String resultStr=LanguageManager.getLangByKey(LanguageKeys.TIP_SEARCH_RESULT);
		if(resultStr.trim().equals(""))
			resultStr="搜索结果";
    	if(searchUserInfoArr.size()>0)
    	{
    		if(!searchedListDataHeader.contains(resultStr))
    			searchedListDataHeader.add(resultStr);
    		if(searchListDataChild.containsKey(resultStr))
    		{
    			searchListDataChild.remove(resultStr);
    		}
    		searchListDataChild.put(resultStr, searchUserInfoArr);
    	}
    	else
    	{
    		if(searchedListDataHeader.contains(resultStr))
    			searchedListDataHeader.remove(resultStr);
    		if(searchListDataChild.containsKey(resultStr))
    			searchListDataChild.remove(resultStr);
    	}
    	
    	selectedListViewAdapter = new ExpandableListAdapter(this.activity,this, searchedListDataHeader, searchListDataChild,false);
		selectedListView.setAdapter(selectedListViewAdapter);
		selectedListViewAdapter.fragment=this;
		
		notifyDataSetChanged();
		
		if(commonMemberUidAdded!=null)
			commonMemberUidAdded.clear();
		if(commonMemberUidRemoved!=null)
			commonMemberUidRemoved.clear();
		onSelectionChanged();
    }

    public void onSelectionChanged()
    {
    	ArrayList<String> memberUidArray=UserManager.getInstance().getSelectMemberUidArr();
    	System.out.println("onSelectionChanged");
    	if(memberUidArray==null || memberUidAdded==null || memberUidRemoved==null || commonMemberUidAdded==null || commonMemberUidRemoved==null)
    		return;
    	int size=memberUidArray.size()+memberUidAdded.size()-memberUidRemoved.size()+commonMemberUidAdded.size()-commonMemberUidRemoved.size();
    	System.out.println("onSelectionChanged size:"+size);
    	String titleName="";
    	if(ChatServiceController.getCurrentChannelType()==DBDefinition.CHANNEL_TYPE_CHATROOM && !ChatServiceController.isCreateChatRoom)
    	{
    		titleName=UserManager.getInstance().getCurrentMail().opponentName;
    	}
    	if(size<0)
    		size=0;
    	if(ChatServiceController.isInChatRoom() && titleName.length()>16 || titleName.equals(""))
		{
    		titleName=LanguageManager.getLangByKey(LanguageKeys.TITLE_CHATROOM);
			if(titleName.equals(""))
				titleName="群聊";
		}
    	getTitleLabel().setText(titleName+"("+size+")");
    	getMemberSelectButton().setVisibility(View.GONE);
    	chatroom_name_layout.setVisibility((size>1&&memberTab==1)?View.VISIBLE:View.GONE);
    }
    
    private void refreshSelectedState()
    {
    	HashMap<String, UserInfo> memberInfoMap=UserManager.getInstance().getChatRoomMemberInfoMap();
		Set<String> allianceUidKeySet=null;
		if(memberInfoMap!=null && memberInfoMap.size()>0)
		{
			allianceUidKeySet=memberInfoMap.keySet();
		}
		
		ArrayList<String> memberUidArray=UserManager.getInstance().getSelectMemberUidArr();
		if(memberUidArray == null)
		{
			memberUidArray = new ArrayList<String>();
		}
		
		if(allianceUidKeySet==null)
			return;
		
		boolean isStateChanged=false;
		HashMap<String, Boolean> map=null;
    	if(memberTab==1 && listAdapter!=null)
    	{
    		map=listAdapter.getSelectStateMap();
    	}
    	else if(memberTab==2)
    	{
    		map=selectedListViewAdapter.getSelectStateMap();
    	}
    	
    	
    	
    	if(map!=null)
		{
    		for(int i=0;i<memberUidAdded.size();i++)
			{
				String uid=memberUidAdded.get(i);
				if(!uid.equals("") && map.containsKey(uid) && !map.get(uid))
				{
					map.put(uid, Boolean.TRUE);
					isStateChanged=true;
				}
			}
			
    		for(int i=0;i<memberUidRemoved.size();i++)
			{
				String uid=memberUidRemoved.get(i);
				if(!uid.equals("") && map.containsKey(uid) && map.get(uid))
				{
					map.put(uid, Boolean.FALSE);
					isStateChanged=true;
				}
			}
			
			Set<String> selectedMapKeySet=map.keySet();
			if(selectedMapKeySet!=null && selectedMapKeySet.size()>0)
			{
				System.out.println("refreshSelectedState 2");
				for(String uidKey:selectedMapKeySet)
				{
					if(!uidKey.equals(""))
					{
						if(!memberUidAdded.contains(uidKey) && !commonMemberUidAdded.contains(uidKey) && !memberUidArray.contains(uidKey) && map.get(uidKey))
    					{
							System.out.println("refreshSelectedState 21");
        					map.put(uidKey, Boolean.FALSE);
        					isStateChanged=true;
    					}
						else if(!memberUidRemoved.contains(uidKey) && !commonMemberUidRemoved.contains(uidKey) && memberUidArray.contains(uidKey) && !map.get(uidKey))
						{
							System.out.println("refreshSelectedState 22");
        					map.put(uidKey, Boolean.TRUE);
        					isStateChanged=true;
    					}
    				}
				}
			}
		}
    	
    	if(isStateChanged)
    		notifyDataSetChanged();
    }

    private int getSelectionCount()
    {
    	int result = 0;
    	Iterator<Entry<String, ArrayList<UserInfo>>> iter = listDataChild.entrySet().iterator();
		while (iter.hasNext())
		{
			Entry<String, ArrayList<UserInfo>> entry = iter.next();
			List<UserInfo> rank = entry.getValue();
			for (int i = 0; i < rank.size(); i++)
			{
				if(rank.get(i).isSelected){
					result++;
				}
			}
		}
    	return result;
    }

	@SuppressLint("SetJavaScriptEnabled")
	public void onViewCreated(View view, Bundle savedInstanceState)
    {
		super.onViewCreated(view, savedInstanceState);

		expListView = (ExpandableListView) view.findViewById(ResUtil.getId(this.activity, "id", "cs__myExpandableListView"));
		prepareListData();
		
		listAdapter = new ExpandableListAdapter(this.activity,this, listDataHeader, listDataChild,true);
		expListView.setAdapter(listAdapter);

		for (int i = 0; i < 5; i++)
		{
	        expListView.expandGroup(i);
		}
		
		selectedListView = (ExpandableListView) view.findViewById(R.id.cs__selectedListView);
		prepareJoinedListData();
		selectedListViewAdapter = new ExpandableListAdapter(this.activity,this, searchedListDataHeader, searchListDataChild,false);
		selectedListView.setAdapter(selectedListViewAdapter);
		
		for (int j = 0; j < searchedListDataHeader.size(); j++)
		{
			selectedListView.expandGroup(j);
		}

		memberListLayout= ((RelativeLayout) view.findViewById(ResUtil.getId(this.activity, "id", "memberListLayout")));
		memberListLayout.setVisibility(View.VISIBLE);
		
		expListView.setOnTouchListener(new OnTouchListener() {
			
			@Override
			public boolean onTouch(View v, MotionEvent event) {
//				System.out.println("expListView 11");
				hideSoftKeyBoard();
				return false;
			}
		});
		
		selectedListView.setOnTouchListener(new OnTouchListener() {
			
			@Override
			public boolean onTouch(View v, MotionEvent event) {
//				System.out.println("selectedListView 11");
				hideSoftKeyBoard();
				return false;
			}
		});
		
		selected_layout=(RelativeLayout)view.findViewById(R.id.selected_layout);
		selected_layout.setVisibility(View.GONE);
		
		fragmentLayout.setOnTouchListener(new OnTouchListener() {
			
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				System.out.println("expListView 22");
				hideSoftKeyBoard();
				return false;
			}
		});
        
        cs__cancelBtn= ((Button) view.findViewById(ResUtil.getId(this.activity, "id", "cs__cancelBtn")));
        cs__cancelBtn.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_CANCEL));
        cs__cancelBtn.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view) {
            	hideSoftKeyBoard();
            	if(ChatServiceController.isCreateChatRoom)
            		activity.exitActivity();
            	else
            		activity.exitActivity();;
            }
        });
        
        cs__okBtn= ((Button) view.findViewById(ResUtil.getId(this.activity, "id", "cs__okBtn")));
        cs__okBtn.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_CONFIRM));
        cs__okBtn.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view) {
            	roomName=chatroom_input.getText().toString().trim();
            	hideSoftKeyBoard();
            	if(ChatServiceController.isCreateChatRoom)
            	{
            		if(memberUidAdded.size()>0 || commonMemberUidAdded.size()>0)
                	{
            			System.out.println("memberUidAdded.size():"+memberUidAdded.size()+"  commonMemberUidAdded.size:"+commonMemberUidAdded.size());
            			ArrayList<String> uidAdded=new ArrayList<String>();
            			for(int i=0;i<memberUidAdded.size();i++)
            			{
            				uidAdded.add(memberUidAdded.get(i));
            			}
            			
            			for(int i=0;i<commonMemberUidAdded.size();i++)
            			{
            				uidAdded.add(commonMemberUidAdded.get(i));
            			}
            			
            			if(uidAdded.size()<=0)
            				return;
            			
                		String content="";
                		if(LanguageManager.getLangByKey(LanguageKeys.TIP_CHATROOM_INVITE).equals(""))
                			content="是否邀请"+UserManager.getInstance().createNameStr(uidAdded);
                		else
                			content=LanguageManager.getLangByKey(LanguageKeys.TIP_CHATROOM_INVITE, UserManager.getInstance().createNameStr(uidAdded));
                		MenuController.showInviteChatRoomMemberConfirm(activity,content, uidAdded);
                	}
            	}
            	else if(UserManager.getInstance().getCurrentMail().opponentUid!="")
            	{
            		ArrayList<String> uidAdded=new ArrayList<String>();
            		ArrayList<String> uidRemoved=new ArrayList<String>();
            		String modifyName = "";
            		String content="";
            		if(memberUidAdded.size()>0 || commonMemberUidAdded.size()>0)
                	{
            			System.out.println("memberUidAdded.size():"+memberUidAdded.size()+"  commonMemberUidAdded.size:"+commonMemberUidAdded.size());
            			
            			for(int i=0;i<memberUidAdded.size();i++)
            			{
            				uidAdded.add(memberUidAdded.get(i));
            			}
            			
            			for(int i=0;i<commonMemberUidAdded.size();i++)
            			{
            				uidAdded.add(commonMemberUidAdded.get(i));
            			}
            			
            			System.out.println("uidAdded.size():"+uidAdded.size());
            			if(uidAdded.size() > 0)
            			{
            				if(LanguageManager.getLangByKey(LanguageKeys.TIP_CHATROOM_INVITE).equals(""))
                    			content="是否邀请"+UserManager.getInstance().createNameStr(uidAdded);
                    		else
                    			content=LanguageManager.getLangByKey(LanguageKeys.TIP_CHATROOM_INVITE, UserManager.getInstance().createNameStr(uidAdded));
            			}
                		
                		if(!ChatServiceController.isInChatRoom())
                		{
                			MenuController.showCreateChatRoomConfirm(activity, content, uidAdded);
                			return;
                		}
                	}
                	
            		if(memberUidRemoved.size()>0 || commonMemberUidRemoved.size()>0)
                	{
            			System.out.println("memberUidRemoved.size():"+memberUidRemoved.size()+"  commonMemberUidRemoved.size:"+commonMemberUidRemoved.size());
            			
            			for(int i=0;i<memberUidRemoved.size();i++)
            			{
            				uidRemoved.add(memberUidRemoved.get(i));
            			}
            			
            			for(int i=0;i<commonMemberUidRemoved.size();i++)
            			{
            				uidRemoved.add(commonMemberUidRemoved.get(i));
            			}
            			
            			if(uidRemoved.size()>0)
            			{
            				String temp = "";
            				if(LanguageManager.getLangByKey(LanguageKeys.TIP_CHATROOM_KICK).equals(""))
            					temp="是否将"+UserManager.getInstance().createNameStr(uidRemoved)+"从群组中移除";
                    		else
                    			temp=LanguageManager.getLangByKey(LanguageKeys.TIP_CHATROOM_KICK, UserManager.getInstance().createNameStr(uidRemoved));
            				
            				if(!temp.equals(""))
            				{
            					if(content.equals(""))
            						content = temp;
            					else
            					{	
            						content = content + "\n";
            						content += temp;
            					}
            				}
            			}
                	}
                	
                	if(!roomName.equals("") && UserManager.getInstance().getCurrentMail().opponentName!=null && !UserManager.getInstance().getCurrentMail().opponentName.equals(roomName))
                	{
                		System.out.println("chatroom_input.getText():"+roomName);
                		modifyName = roomName;
                		String temp="";
                		if(LanguageManager.getLangByKey(LanguageKeys.TIP_CHATROOM_MODIFYNAME).equals(""))
                			temp="是否将群组改名为"+roomName;
                		else
                			temp=LanguageManager.getLangByKey(LanguageKeys.TIP_CHATROOM_MODIFYNAME,roomName);
                		
                		if(!temp.equals(""))
        				{
        					if(content.equals(""))
        						content = temp;
        					else
        					{	
        						content = content + "\n";
        						content += temp;
        					}
        				}
                	}
                	
                	MenuController.showChatRoomManagerConfirm(activity, content, uidAdded, uidRemoved, modifyName);
                	
            	}
            	else
            	{
            		System.out.println("error");
            		activity.exitActivity();
            	}
            }
        });
        
        cs__quitBtn= ((Button) view.findViewById(ResUtil.getId(this.activity, "id", "cs__quitBtn")));
        cs__quitBtn.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_QUIT_CHATROOM));
        cs__quitBtn.setVisibility(ChatServiceController.isCreateChatRoom?View.GONE:View.VISIBLE);
        cs__quitBtn.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view) {
            	String content="";
        		if(LanguageManager.getLangByKey(LanguageKeys.TIP_CHATROOM_QUIT).equals(""))
        			content="是否确认退出群组?";
        		else
        			content=LanguageManager.getLangByKey(LanguageKeys.TIP_CHATROOM_QUIT);
            	MenuController.quitChatRoomConfirm(activity,content);
            }
        });
        
        chatroom_name_layout=(LinearLayout)view.findViewById(ResUtil.getId(this.activity, "id", "chatroom_name_layout"));
        chatroom_input=(EditText)view.findViewById(ResUtil.getId(this.activity, "id", "chatroom_input"));
        chatroom_input.setOnEditorActionListener(new OnEditorActionListener() {
			
			@Override
			public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
				return (event.getKeyCode() == KeyEvent.KEYCODE_ENTER);
			}
		});
        
        onSelectionChanged();
        
        chatroom_name = (TextView)view.findViewById(R.id.chatroom_name);
        chatroom_name.setText(LanguageManager.getLangByKey(LanguageKeys.TIP_MODIFY_CHATROOM_NAME));
        
        search_name_layout=(LinearLayout)view.findViewById(R.id.search_name_layout);
        search_name_layout.setVisibility(View.GONE);
        
        buttonAlliance=(Button)view.findViewById(R.id.buttonAlliance);
        buttonAlliance.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_ALLIANCE));
        buttonSearch=(Button)view.findViewById(R.id.buttonSearch);
        buttonSearch.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_SEARCH));
        CompatibleApiUtil.getInstance().setButtonAlpha(buttonAlliance, true);
    	CompatibleApiUtil.getInstance().setButtonAlpha(buttonSearch, false);
        
        buttonAlliance.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view) {
            	hideSoftKeyBoard();
            	search_name_layout.setVisibility(View.GONE);
            	selected_layout.setVisibility(View.GONE);
            	memberListLayout.setVisibility(View.VISIBLE);
            	CompatibleApiUtil.getInstance().setButtonAlpha(buttonAlliance, true);
            	CompatibleApiUtil.getInstance().setButtonAlpha(buttonSearch, false);
            	memberTab=1;
            	onSelectionChanged();
            	refreshSelectedState();
            }
        });
        
        buttonSearch.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view) {
            	hideSoftKeyBoard();
            	search_name_layout.setVisibility(View.VISIBLE);
            	chatroom_name_layout.setVisibility(View.GONE);
            	selected_layout.setVisibility(View.VISIBLE);
            	memberListLayout.setVisibility(View.GONE);
            	CompatibleApiUtil.getInstance().setButtonAlpha(buttonAlliance, false);
            	CompatibleApiUtil.getInstance().setButtonAlpha(buttonSearch, true);
            	memberTab=2;
            	refreshSelectedState();
            }
        });
        
        search_name=(TextView)view.findViewById(R.id.search_name);
        search_name.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_SEARCH));
        
        search_name_input=(EditText)view.findViewById(R.id.search_name_input);
        
        search_name_input.setOnEditorActionListener(new OnEditorActionListener() {
			
			@Override
			public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
				return (event.getKeyCode() == KeyEvent.KEYCODE_ENTER);
			}
		});
        
        search_btn=(Button)view.findViewById(R.id.search_btn);
        search_btn.setText(LanguageManager.getLangByKey(LanguageKeys.BTN_SEARCH));
        search_btn.setOnClickListener(new View.OnClickListener(){
            public void onClick(View view) {
            	hideSoftKeyBoard();
            	String name=search_name_input.getText().toString();
            	if(!name.trim().equals(""))
            		ChatServiceController.getInstance().host.searchPlayer(name.trim());
            }
            
        });
        
        buttonsLinearLayout=(LinearLayout)view.findViewById(R.id.buttonsLinearLayout);
        
        memberListLayout.getChildAt(0).getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
	        public void onGlobalLayout() {
				adjustHeight();
	        }
	    });
        
        memberTab=1;
        
        if (ConfigManager.getInstance().scaleFontandUI)
		{
			calcScale(activity);
		}
        
        ((MemberSelectorActivity) getActivity()).fragment = this;
    }
	
	public void onStop()
	{
		super.onStop();
		resetMemberData();
	}
	
	public void resetMemberData()
	{
		System.out.println("resetMemberData");
		memberUidAdded=new ArrayList<String>();
		if(!ChatServiceController.isCreateChatRoom&&!ChatServiceController.isInChatRoom() && !UserManager.getInstance().getCurrentMail().opponentUid.equals(""))
			memberUidAdded.add(UserManager.getInstance().getCurrentMail().opponentUid);
		memberUidRemoved=new ArrayList<String>();
		
		commonMemberUidAdded=new ArrayList<String>();
		commonMemberUidRemoved=new ArrayList<String>();
	}
	
	public void onBackButtonClick()
	{
    	hideSoftKeyBoard();
	}
	
    public boolean handleBackPressed()
    {
    	return false;
    }
    
    public static void calcScale(Context context)
	{
		WindowManager windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
		double screenWidth = windowManager.getDefaultDisplay().getWidth();
		double screenHeight = windowManager.getDefaultDisplay().getHeight();
		double scaleX = screenWidth / designWidth;
		double scaleY = screenHeight / designHeight;
		double minScale = Math.min(scaleX, scaleY);
		scaleRatio = minScale;
		scaleRatioButton = minScale;
		// System.out.println("scaleX:"+scaleX+" scaleY:"+scaleY+" minScale:"+minScale);
		// 在大屏上字体可能会偏大，可能需要用dp计算才行
		// 先加个修正因子
		scaleRatio = scaleRatio > (1 / 0.84390234277028) ? scaleRatio * 0.84390234277028 : scaleRatio;
		if (scaleRatio > 1)
		{
			// 小米pad是1.873170518056575
			scaleRatio = 1 + (scaleRatio - 1) * 0.5;
		}
		else
		{
			// 小屏幕不要缩放，否则太小（华为 U8800Pro 800x480）
			scaleRatio = 1 - (1 - scaleRatio) * 0.5;
		}
		System.out.println("scaleRatio = " + scaleRatio);
	}

    private boolean adjustSizeCompleted = false;
    public static double scaleRatio = 0;
	public static double scaleRatioButton = 0;
	private final static double designWidth = 640;
	private final static double designHeight = 852;
	
	public void adjustHeight() {

    	if(!ConfigManager.getInstance().scaleFontandUI){
			adjustSizeCompleted = true;
    		return;
    	}

		if (buttonSearch.getWidth() != 0 && !adjustSizeCompleted && scaleRatio!=0 && scaleRatioButton!=0) {
			buttonsLinearLayout.setLayoutParams(new LinearLayout.LayoutParams(buttonsLinearLayout.getWidth(), (int) (88 * scaleRatioButton)));
			buttonSearch.setLayoutParams(new LinearLayout.LayoutParams(buttonSearch.getWidth(), (int) (79 * scaleRatioButton), 1));
			buttonAlliance.setLayoutParams(new LinearLayout.LayoutParams(buttonAlliance.getWidth(), (int) (79 * scaleRatioButton), 1));
			adjustSizeCompleted = true;
			ScaleUtil.adjustTextSize(buttonSearch, scaleRatio);
			ScaleUtil.adjustTextSize(buttonAlliance, scaleRatio);
			ScaleUtil.adjustTextSize(search_btn, scaleRatio);
		}
	}
}