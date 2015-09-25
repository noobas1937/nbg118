package com.elex.chatservice;

import android.content.Intent;
import android.database.SQLException;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.ListFragment;
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
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.elex.chatservice.app.ActionBarHelper;
import com.elex.chatservice.util.HSActivityUtil;
import com.elex.chatservice.util.HSErrors;
import com.elex.chatservice.util.HSIcons;
import com.elex.chatservice.util.HSRes;
import com.elex.chatservice.view.HSViewPager;
import com.elex.chatservice.view.SimpleMenuItemCompat;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import org.json.JSONException;
import org.json.JSONObject;

public final class HSFaqsFragment extends ListFragment
  implements HSFaqSyncStatusEvents, SimpleMenuItemCompat.MenuItemActions, SimpleMenuItemCompat.QueryTextActions
{
  private final String TAG = "HelpShiftDebug";
  private HSActivity activity;
  private Bundle extras;
  private String searchQuery;
  private HSApiData data;
  private ArrayAdapter adapter;
  private ArrayAdapter searchAdapter;
  private List<Faq> faqItems = new ArrayList();
  private List<Faq> searchItems = new ArrayList();
  private ListView listView;
  private View listFooter;
  private HSViewPager mViewPager;
  private MenuItem mSearchItem;
  private String prevSearchQuery = "";
  private String searchCache = "";
  private boolean searchStarted = false;
  private boolean decomp = false;
  private boolean showReportIssue = false;
  private String currentLang;
  private ActionBarHelper actionBarHelper;
  private TextView faqSyncStatus;
  private ImageView faqsUpdatedIcon;
  private ProgressBar faqsUpdatingIcon;
  private Handler sectionsDbHandler = new Handler() {
    public void handleMessage(Message msg) {
      super.handleMessage(msg);
      ArrayList sections = new ArrayList();
      sections = (ArrayList)msg.obj;
      sections = data.getPopulatedSections(sections);
      faqItems.clear();

      if (sections.size() == 1) {
        ArrayList faqs = data.getFaqsForSection(((Section)sections.get(0)).getPublishId());
        for (int i = 0; i < faqs.size(); i++) {
          Faq faqItem = (Faq)faqs.get(i);
          faqItems.add(new Faq(faqItem.getTitle(), faqItem.getPublishId(), "question"));
        }
      }
      else
      {
        for (int i = 0; i < sections.size(); i++) {
          Section sectionItem = (Section)sections.get(i);
          if (!data.isSectionEmpty(sectionItem)) {
            faqItems.add(new Faq(sectionItem.getTitle(), sectionItem.getPublishId(), "section"));
          }

        }

      }

      if (faqItems.size() == 0) {
        faqItems.add(new Faq(HSRes.getString(activity, "hs__faqs_search_footer"), "0", "empty_status"));
      }

      if (isResumed()) {
        setListShown(true);
      }
      adapter.notifyDataSetChanged();
    }
  };

  private Handler failureHandler = new Handler() {
    public void handleMessage(Message msg) {
      super.handleMessage(msg);
      HashMap result = (HashMap)msg.obj;
      Integer status = (Integer)result.get("status");
      faqsNotUpdated();
      HSErrors.showFailToast(status.intValue(), null, activity);
    }
  };

  public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
  {
    this.activity = ((HSActivity)getActivity());
    this.extras = this.activity.getIntent().getExtras();
    if (this.extras != null) {
      this.decomp = this.extras.getBoolean("decomp");
      this.showReportIssue = ContactUsFilter.showContactUs(ContactUsFilter.LOCATION.SEARCH_FOOTER);
    }
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
    setHasOptionsMenu(true);

    this.currentLang = Locale.getDefault().getLanguage();
    return super.onCreateView(inflater, container, savedInstanceState);
  }

  public void onActivityCreated(Bundle savedInstanceState)
  {
    super.onActivityCreated(savedInstanceState);

    this.mViewPager = ((HSViewPager)this.activity.findViewById(HSRes.getId(this, "id", "pager")));
    this.faqSyncStatus = ((TextView)this.activity.findViewById(HSRes.getId(this.activity, "id", "hs__faqs_fragment_sync_status_text")));

    this.faqsUpdatedIcon = ((ImageView)this.activity.findViewById(HSRes.getId(this.activity, "id", "hs__faqs_fragment_sync_status_icon")));

    this.faqsUpdatingIcon = ((ProgressBar)this.activity.findViewById(HSRes.getId(this.activity, "id", "hs__faqs_fragment_sync_status_spinner")));

    setTextViewAlpha(this.faqSyncStatus, 0.5F);
    faqsUpdating();
  }

  public void onCreateOptionsMenu(Menu menu, MenuInflater inflater)
  {
    this.mSearchItem = menu.findItem(HSRes.getId(this, "id", "hs__action_search"));
    HSIcons.applyMenuItemSelectedFilter(this.activity, this.mSearchItem.getIcon());

    this.actionBarHelper.setQueryHint(this.mSearchItem, HSRes.getString(this.activity, "hs__search_hint"));
    this.actionBarHelper.setOnQueryTextListener(this.mSearchItem, this);
    this.actionBarHelper.setOnActionExpandListener(this.mSearchItem, this);

    this.data.loadIndex();
  }

  public void onViewCreated(View view, Bundle savedInstanceState)
  {
    super.onViewCreated(view, savedInstanceState);

    this.listView = getListView();
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

      HSIcons.applyTextColorPrimaryFilter(this.activity, reportIssueBtn.getCompoundDrawables()[0]);
      reportIssueBtn.setOnClickListener(new View.OnClickListener() {
        public void onClick(View v) {
          performedSearch();
          Intent i = new Intent(activity, HSConversation.class);
          i.putExtra("message", searchQuery);
          actionBarHelper.collapseActionView(mSearchItem);
          i.putExtra("showInFullScreen", HSActivityUtil.isFullScreen(activity));
          i.putExtra("showConvOnReportIssue", extras.getBoolean("showConvOnReportIssue"));
          i.putExtra("chatLaunchSource", "support");
          i.putExtra("decomp", decomp);
          getActivity().startActivityForResult(i, 1);
        }
      });
    }
    try {
      setListShown(false);
      HSApiData.addFaqSyncStatusObserver(this);
      this.data.getSections(this.sectionsDbHandler, this.failureHandler);
    } catch (SQLException e) {
      Log.d("HelpShiftDebug", e.toString(), e);
    }
  }

  public void onListItemClick(ListView l, View v, int position, long id)
  {
    Faq clickedItem;
    if (this.searchStarted) {
      performedSearch();
      clickedItem = (Faq)this.searchItems.get(position);
    } else {
      clickedItem = (Faq)this.faqItems.get(position);
    }

    if (!clickedItem.getType().equals("empty_status"))
    {
      Intent i;
      if (clickedItem.getType().equals("section")) {
        i = new Intent(this.activity, HSQuestionsList.class);
        i.putExtra("sectionPublishId", clickedItem.getPublishId());
      } else {
        i = new Intent(this.activity, HSQuestion.class);
        i.putExtra("questionPublishId", clickedItem.getPublishId());
        i.putExtra("decomp", this.decomp);
      }
      i.putExtra("showInFullScreen", HSActivityUtil.isFullScreen(this.activity));
      i.putExtras(this.extras);
      i.removeExtra("isRoot");
      getActivity().startActivityForResult(i, 1);
    }
  }

  private void setTextViewAlpha(TextView tv, float alpha)
  {
    int color = tv.getCurrentTextColor();
    int newColor = Color.argb((int)Math.floor(alpha * 255.0F), Color.red(color), Color.green(color), Color.blue(color));

    tv.setTextColor(newColor);
  }

  public void faqsUpdating()
  {
    if (HSApiData.faqsSyncing == true) {
      this.faqsUpdatedIcon.setVisibility(8);
      this.faqsUpdatingIcon.setVisibility(0);
      this.faqSyncStatus.setText(HSRes.getString(this.activity, "hs__faqs_updating"));
    }
  }

  public void faqsUpdated()
  {
    this.faqsUpdatedIcon.setVisibility(0);
    this.faqsUpdatingIcon.setVisibility(8);
    this.faqSyncStatus.setText(HSRes.getString(this.activity, "hs__faqs_uptodate"));
  }

  public void faqsNotUpdated()
  {
    this.faqsUpdatedIcon.setVisibility(8);
    this.faqsUpdatingIcon.setVisibility(8);
    this.faqSyncStatus.setText(HSRes.getString(this.activity, "hs__faqs_updation_failure"));
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

  protected void initSearchList(String searchQuery)
  {
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
    initSearchList(this.data.getAllFaqs());
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
      this.searchItems.add(searchItem);
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
    if (this.mViewPager != null) {
      this.mViewPager.setPagingEnabled(false);
    }
    if (!this.decomp) {
      this.actionBarHelper.setNavigationMode(0);
    }
    searchStarted();
    this.activity.showConversationMenu(false);
    return true;
  }

  public boolean menuItemCollapsed()
  {
    performedSearch();
    if (this.mViewPager != null) {
      this.mViewPager.setPagingEnabled(true);
    }
    searchCompleted();
    if (ContactUsFilter.showContactUs(ContactUsFilter.LOCATION.ACTION_BAR)) {
      this.activity.showConversationMenu(true);
    }
    return true;
  }

  public void onDestroy()
  {
    super.onDestroy();
    HSApiData.removeFaqSyncStatusObserver(this);
  }
}