package com.elex.chatservice;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.ListFragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;

import com.elex.chatservice.app.ActionBarActivity;
import com.elex.chatservice.app.ActionBarHelper;
import com.elex.chatservice.util.HSActivityUtil;
import com.elex.chatservice.util.HSErrors;
import com.elex.chatservice.util.HSIcons;
import com.elex.chatservice.util.HSRes;
import com.elex.chatservice.view.SimpleMenuItemCompat;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Locale;
import org.json.JSONException;
import org.json.JSONObject;

public final class HSSectionFragment extends ListFragment
  implements HSFaqSyncStatusEvents, SimpleMenuItemCompat.MenuItemActions, SimpleMenuItemCompat.QueryTextActions
{
  public static final String TAG = "HelpShiftDebug";
  private ArrayList<Faq> faqItems = new ArrayList();
  private ArrayList<Faq> searchItems = new ArrayList();
  private HSActivity activity;
  private HSApiData data;
  private ArrayAdapter adapter;
  private ArrayAdapter searchAdapter;
  private String sectionId;
  private String sectionPubId;
  private String publishId;
  private ListView listView;
  private View listFooter;
  private MenuItem mSearchItem;
  private String searchQuery;
  private String prevSearchQuery;
  private String searchCache = "";
  private Boolean isDecomp = Boolean.valueOf(false);
  private boolean searchStarted = false;
  private boolean showReportIssue;
  private boolean isVisible;
  private boolean eventSent = false;
  private String currentLang;
  private ActionBarHelper actionBarHelper;
  private Handler sectionSuccessHandler = new Handler() {
    public void handleMessage(Message msg) {
      super.handleMessage(msg);
      Section section = (Section)msg.obj;
      updateSectionData(section);
    }
  };

  private Handler sectionFailHandler = new Handler() {
    public void handleMessage(Message msg) {
      super.handleMessage(msg);
      HashMap result = (HashMap)msg.obj;
      Integer status = (Integer)result.get("status");
      if (status.intValue() == 404) {
        Log.d("HelpShiftDebug", "Specified question does not exist");
      }
      ((HSSection)activity).faqsNotUpdated();
      HSErrors.showFailToast(status.intValue(), null, activity);
    }
  };

  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    Bundle extras = getArguments();
    if (extras != null) {
      this.publishId = extras.getString("sectionPublishId");
      if (this.publishId == null) {
        this.publishId = "";
      }
      this.showReportIssue = ContactUsFilter.showContactUs(ContactUsFilter.LOCATION.SEARCH_FOOTER);
      if (extras.getBoolean("decomp") == true) {
        HSAnalytics.decomp = true;
        this.isDecomp = Boolean.valueOf(true);
      }
    }
  }

  public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
  {
    this.activity = ((HSActivity)getActivity());
    this.actionBarHelper = this.activity.getActionBarHelper();
    this.data = new HSApiData(this.activity);

    if (this.showReportIssue == true) {
      this.listFooter = inflater.inflate(HSRes.getId(this.activity, "layout", "hs__search_list_footer"), null, false);
    }
    else {
      this.listFooter = inflater.inflate(HSRes.getId(this.activity, "layout", "hs__no_faqs"), null, false);
    }

    int rowResId = HSRes.getId(this.activity, "layout", "hs__simple_list_item_1");

    this.adapter = new ArrayAdapter(this.activity, rowResId, this.faqItems);

    this.searchAdapter = new ArrayAdapter(this.activity, rowResId, this.searchItems);

    setListAdapter(this.adapter);
    this.currentLang = Locale.getDefault().getLanguage();
    return super.onCreateView(inflater, container, savedInstanceState);
  }

  public void onViewCreated(View view, Bundle savedInstanceState)
  {
    this.listView = getListView();

    if (this.isDecomp.booleanValue() == true) {
      this.listView.setOnTouchListener(new View.OnTouchListener()
      {
        public boolean onTouch(View view, MotionEvent ev) {
          if (mSearchItem != null) {
            actionBarHelper.clearFocus(mSearchItem);
          }
          return false;
        }
      });
      if (this.showReportIssue == true) {
        Button reportIssueBtn = (Button)this.listFooter.findViewById(HSRes.getId(this.activity, "id", "report_issue"));

        reportIssueBtn.setOnClickListener(new View.OnClickListener() {
          public void onClick(View v) {
            performedSearch();
            Intent i = new Intent(activity, HSConversation.class);
            i.putExtra("message", searchQuery);
            actionBarHelper.collapseActionView(mSearchItem);
            i.putExtra("showInFullScreen", HSActivityUtil.isFullScreen(activity));
            i.putExtra("showConvOnReportIssue", activity.getIntent().getExtras().getBoolean("showConvOnReportIssue"));
            i.putExtra("chatLaunchSource", "support");
            getActivity().startActivityForResult(i, 1);
          }
        });
      }
      setHasOptionsMenu(true);
    }
    HSApiData.addFaqSyncStatusObserver(this);

    if (this.isDecomp.booleanValue() == true)
      this.data.getSection(this.publishId, this.sectionSuccessHandler, this.sectionFailHandler);
    else
      this.data.getSectionSync(this.publishId, this.sectionSuccessHandler, this.sectionFailHandler);
  }

  public void setUserVisibleHint(boolean isVisibleToUser)
  {
    super.setUserVisibleHint(isVisibleToUser);
    this.isVisible = isVisibleToUser;

    if ((isVisibleToUser) && (this.sectionId != null))
      try {
        JSONObject eventData = new JSONObject();
        eventData.put("id", this.sectionId);
        HSFunnel.pushEvent("b", eventData);
        this.eventSent = true;
      } catch (JSONException e) {
        Log.d("HelpShiftDebug", e.toString(), e);
      }
  }

  public void onResume()
  {
    super.onResume();

    if (((this.isDecomp.booleanValue()) || (this.isVisible)) && (this.sectionId != null))
      try
      {
        JSONObject eventData = new JSONObject();
        eventData.put("id", this.sectionId);
        HSFunnel.pushEvent("b", eventData);
        this.eventSent = true;
      } catch (JSONException e) {
        Log.d("HelpShiftDebug", "event data", e);
      }
  }

  public void onDestroy()
  {
    super.onDestroy();
    HSApiData.removeFaqSyncStatusObserver(this);
  }

  public void onCreateOptionsMenu(Menu menu, MenuInflater inflater)
  {
    if (this.isDecomp.booleanValue()) {
      this.mSearchItem = menu.findItem(HSRes.getId(this, "id", "hs__action_search"));
      HSIcons.applyMenuItemSelectedFilter(this.activity, this.mSearchItem.getIcon());

      this.actionBarHelper.setQueryHint(this.mSearchItem, HSRes.getString(this.activity, "hs__search_hint"));
      this.actionBarHelper.setOnQueryTextListener(this.mSearchItem, this);
      this.actionBarHelper.setOnActionExpandListener(this.mSearchItem, this);

      this.data.loadIndex();
    }
  }

  public void faqsUpdating()
  {
  }

  public void faqsUpdated()
  {
    updateSectionData(this.data.getSection(this.publishId));
  }

  public void searchIndexesUpdated()
  {
    Handler mainThreadHandler = new Handler(Looper.getMainLooper());
    mainThreadHandler.post(new Runnable() {
      public void run() {
        initSearchList(searchCache);
      }
    });
  }

  private void updateSectionData(Section section)
  {
    ArrayList sectionFAQArray = this.data.getFaqsForSection(this.publishId);
    if ((this.isDecomp.booleanValue()) && (section != null)) {
      ActionBarHelper actionBarHelper = this.activity.getActionBarHelper();
      actionBarHelper.setTitle(section.getTitle());
    }

    if (section == null) {
      Log.d("HelpShiftDebug", "Specified section does not exist");
      HSErrors.showFailToast(404, null, this.activity);
    } else {
      this.faqItems.clear();
      this.sectionId = section.getSectionId();
      this.sectionPubId = section.getPublishId();

      if ((getUserVisibleHint()) && (this.sectionId != null) && (!this.eventSent)) {
        try {
          JSONObject eventData = new JSONObject();
          eventData.put("id", this.sectionId);
          HSFunnel.pushEvent("b", eventData);
          this.eventSent = true;
        } catch (JSONException e) {
          Log.d("HelpShiftDebug", e.toString(), e);
        }
      }
      for (int i = 0; i < sectionFAQArray.size(); i++) {
        Faq faqItem = (Faq)sectionFAQArray.get(i);
        this.faqItems.add(faqItem);
      }

      if (this.faqItems.size() == 0) {
        this.faqItems.add(new Faq(HSRes.getString(this.activity, "hs__faqs_search_footer"), "0", "empty_status"));
      }

      this.adapter.notifyDataSetChanged();
    }
  }

  public void onListItemClick(ListView list, View view, int questionPosition, long arg3)
  {
    Faq clickedItem;
    if (this.searchStarted) {
      performedSearch();
      clickedItem = (Faq)this.searchItems.get(questionPosition);
    } else {
      clickedItem = (Faq)this.faqItems.get(questionPosition);
    }
    if (!clickedItem.getType().equals("empty_status")) {
      Intent i = new Intent(this.activity, HSQuestion.class);
      i.putExtra("questionPublishId", clickedItem.getPublishId());
      i.putExtra("decomp", this.isDecomp);
      i.putExtra("showInFullScreen", HSActivityUtil.isFullScreen(this.activity));
      i.putExtras(getActivity().getIntent().getExtras());
      i.removeExtra("isRoot");
      getActivity().startActivityForResult(i, 1);
    }
  }

  protected void initSearchList(String searchQuery) {
    boolean searchWithAny = false;
    this.searchQuery = searchQuery;
    if ((this.currentLang.equals("zh")) || (this.currentLang.equals("ja")) || (this.currentLang.equals("ko"))) {
      searchWithAny = true;
    }
    if ((searchQuery.length() == 0) || ((searchQuery.length() < 3) && (!searchWithAny)))
      initSearchList();
    else
      initSearchList(this.data.localFaqSearch(searchQuery));
  }

  protected void initSearchList()
  {
    initSearchList(this.data.getFaqsForSection(this.sectionPubId));
  }

  private void initSearchList(ArrayList<Faq> searchArray) {
    if ((searchArray.size() == 0) || (this.showReportIssue == true))
      this.listFooter.setVisibility(0);
    else {
      this.listFooter.setVisibility(8);
    }

    this.searchItems.clear();
    for (int i = 0; i < searchArray.size(); i++) {
      Faq searchItem = (Faq)searchArray.get(i);
      if (searchItem.getSectionPublishId().equals(this.sectionPubId)) {
        this.searchItems.add(searchItem);
      }
    }
    this.searchAdapter.notifyDataSetChanged();
  }

  protected void searchCompleted() {
    if (this.listView.getFooterViewsCount() != 0) {
      this.listView.removeFooterView(this.listFooter);
    }
    setListAdapter(this.adapter);
    this.adapter.notifyDataSetChanged();
    this.searchStarted = false;
  }

  protected void searchStarted() {
    if ((this.listView.getFooterViewsCount() == 0) && (this.showReportIssue == true)) {
      this.listView.addFooterView(this.listFooter);
    } else {
      this.listView.addFooterView(this.listFooter, null, false);
      this.listView.setFooterDividersEnabled(false);
    }
    initSearchList();
    setListAdapter(this.searchAdapter);
    this.searchAdapter.notifyDataSetChanged();
    this.searchStarted = true;
  }

  private void performedSearch() {
    String searchString = this.actionBarHelper.getQuery(this.mSearchItem).toString().trim();
    performedSearch(searchString);
  }

  private void performedSearch(String searchString) {
    if ((searchString.length() > 2) && (!searchString.equals(this.prevSearchQuery)))
    {
      JSONObject eventObj = new JSONObject();
      try {
        eventObj.put("s", searchString);
      } catch (JSONException e) {
        Log.d("HelpShiftDebug", "JSONException", e);
      }
      HSFunnel.pushEvent("s", eventObj);
      this.prevSearchQuery = searchString;
    }
  }

  public boolean queryTextSubmitted(String query)
  {
    return false;
  }

  public boolean queryTextChanged(String newText)
  {
    if (newText.length() == 0)
      performedSearch(this.searchCache);
    else {
      this.searchCache = newText;
    }
    initSearchList(newText);
    return false;
  }

  public boolean menuItemExpanded()
  {
    this.prevSearchQuery = "";
    this.searchCache = "";
    searchStarted();
    this.activity.showConversationMenu(false);
    return true;
  }

  public boolean menuItemCollapsed()
  {
    performedSearch();
    searchCompleted();
    if (ContactUsFilter.showContactUs(ContactUsFilter.LOCATION.ACTION_BAR)) {
      this.activity.showConversationMenu(true);
    }
    return true;
  }
}