package com.elex.chatservice;

import android.net.Uri;
import android.os.Build;
import android.os.Build.VERSION;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;

import com.elex.chatservice.exceptions.InstallException;
import com.elex.chatservice.util.HSTimeUtil;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.SocketTimeoutException;
import java.net.URL;
import java.net.URLConnection;
import java.net.UnknownHostException;
import java.security.GeneralSecurityException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;
import java.util.zip.GZIPInputStream;
import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import javax.net.ssl.SSLPeerUnverifiedException;
import org.apache.http.Header;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.StatusLine;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.methods.HttpRequestBase;
import org.apache.http.conn.ConnectTimeoutException;
import org.apache.http.conn.HttpHostConnectException;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public final class HSApiClient
{
  public static final String apiVersion = "1";
  public static final String libraryVersion = "3.4.1";
  public static String acceptLangHead;
  private static int timeStampErrorReplies = 0;
  private HSStorage storage;
  final String apiBase = "/api/lib/";

  final String scheme = "https://";
  final String SOL_REJECTED = "Did not accept the solution";
  final String SOL_ACCEPT = "Accepted the solution";
  final String SOL_REVIEW = "Accepted review request";
  final String SC_SENT = "Screenshot sent";
  final String appId;
  final String domain;
  final String apiKey;

  protected HSApiClient(String domain, String appId, String apiKey, HSStorage storage)
  {
    this.domain = domain;
    this.appId = appId;
    this.apiKey = apiKey;
    this.storage = storage;
    acceptLangHead = String.format("%s;q=1.0", new Object[] { Locale.getDefault().getLanguage() });
  }

  private String bytesToHex(byte[] bytes) {
    char[] hexArray = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    char[] hexChars = new char[bytes.length * 2];

    for (int j = 0; j < bytes.length; j++) {
      int v = bytes[j] & 0xFF;
      hexChars[(j * 2)] = hexArray[(v >>> 4)];
      hexChars[(j * 2 + 1)] = hexArray[(v & 0xF)];
    }

    return new String(hexChars);
  }

  private String getSignature(String sigString) throws GeneralSecurityException, InstallException
  {
    String data = sigString;
    String key = this.apiKey;
    byte[] hmacData = null;

    if (TextUtils.isEmpty(key)) {
      throw new InstallException("apiKey Missing");
    }
    try
    {
      SecretKeySpec secretKey = new SecretKeySpec(key.getBytes("UTF-8"), "HmacSHA256");
      Mac mac = Mac.getInstance("HmacSHA256");
      mac.init(secretKey);
      hmacData = mac.doFinal(data.getBytes("UTF-8"));
      return bytesToHex(hmacData);
    } catch (UnsupportedEncodingException e) {
      throw new GeneralSecurityException(e);
    }
  }

  private String getApiUri(String route) {
    return new String("/api/lib/1" + route);
  }

  private String getApiUrl(String route) throws InstallException {
    if (TextUtils.isEmpty(this.domain)) {
      throw new InstallException("domain Missing");
    }
    return new String("https://" + this.domain + getApiUri(route));
  }

  private String constructGetParams(HashMap<String, String> data) {
    List dataList = new ArrayList();
    List mapKeys = new ArrayList(data.keySet());
    for (Object key : mapKeys) {
      dataList.add(new String(key + "=" + Uri.encode((String)data.get(key))));
    }
    return TextUtils.join("&", dataList);
  }

  private List<NameValuePair> constructPostParams(HashMap<String, String> data) {
    List dataList = new ArrayList();
    List mapKeys = new ArrayList(data.keySet());
    List params = new ArrayList(mapKeys.size());
    for (Object key : mapKeys) {
      params.add(new BasicNameValuePair((String)key, (String)data.get(key)));
    }
    return params;
  }

  private HashMap addAuth(HashMap data, String route, String method) throws InstallException {
    String uriStr = getApiUri(route);
    if (TextUtils.isEmpty(this.appId)) {
      throw new InstallException("appId Missing");
    }
    data.put("platform-id", this.appId);
    data.put("method", method);
    data.put("uri", uriStr);
    String ts = HSTimeUtil.getAdjustedTimestamp(this.storage.getServerTimeDelta());
    data.put("timestamp", ts);

    List mapKeys = new ArrayList(data.keySet());
    List dataList = new ArrayList();

    Collections.sort(mapKeys);
    for (Object key : mapKeys) {
      if ((key != "screenshot") && (key != "meta"))
        dataList.add(new String(key + "=" + data.get(key)));
    }
    try
    {
      data.put("signature", getSignature(TextUtils.join("&", dataList)));

      data.remove("method");
      data.remove("uri");
    } catch (GeneralSecurityException e) {
      Log.d("HelpShiftDebug", "Could not generate signature: " + e.getLocalizedMessage(), e);
      return data;
    }
    return data;
  }

  private void sendFailMessage(Handler failure, int status) {
    Message result = failure.obtainMessage();
    HashMap messageResponse = new HashMap();
    messageResponse.put("status", Integer.valueOf(status));
    result.obj = messageResponse;
    failure.sendMessage(result);
  }

  private void makeRequest(final String method, final String route, final HashMap data, final Handler success, final Handler failure)
  {
    new Thread(new Runnable()
    {
      public void run()
      {
        try {
          HashMap dataCopy = new HashMap(data);
          HttpParams httpParameters = new BasicHttpParams();
          HttpConnectionParams.setConnectionTimeout(httpParameters, 5000);
          HttpConnectionParams.setSoTimeout(httpParameters, 10000);

          HttpClient httpclient = new DefaultHttpClient(httpParameters);
          String userAgent = "Helpshift-Android/3.4.1/" + Build.VERSION.RELEASE;
          httpclient.getParams().setParameter("http.useragent", userAgent);

          String url = getApiUrl(route);
          HttpRequestBase httpreq = new HttpGet();

          if (method == "GET") {
        	String requestStr = constructGetParams(addAuth(data, route, method));
            httpreq = new HttpGet(url + "?" + requestStr);
            String etag = storage.getEtag(route);
            if (!TextUtils.isEmpty(etag))
              httpreq.addHeader("If-None-Match", etag);
          }
          else if (method == "POST") {
            List params = constructPostParams(addAuth(data, route, method));
            HttpPost httpPost = new HttpPost(url);
            try {
              httpPost.setEntity(new UrlEncodedFormEntity(params, "UTF-8"));
            } catch (UnsupportedEncodingException e) {
              Log.d("HelpShiftDebug", "makerequest", e);
            }
            httpPost.addHeader("Content-type", "application/x-www-form-urlencoded");
            httpreq = httpPost;
          }

          httpreq.addHeader("Accept-Language", HSApiClient.acceptLangHead);
          httpreq.addHeader("Accept-Encoding", "gzip");
          httpreq.addHeader("X-HS-V", "Helpshift-Android/3.4.1");
          try
          {
            HttpResponse response = httpclient.execute(httpreq);

            Header[] headers = response.getAllHeaders();
            for (int i = 0; i < headers.length; i++) {
              if (headers[i].getName().equals("ETag")) {
                storage.setEtag(route, headers[i].getValue());
                break;
              }
            }
            StatusLine sline = response.getStatusLine();

            String line = "";
            StringBuilder responseStr = new StringBuilder();
            if (response.getEntity() != null) {
              InputStream responseStream = response.getEntity().getContent();
              Header contentEncoding = response.getFirstHeader("Content-Encoding");
              if ((contentEncoding != null) && (contentEncoding.getValue().equalsIgnoreCase("gzip")))
              {
                responseStream = new GZIPInputStream(responseStream);
              }
              InputStreamReader inputStream = new InputStreamReader(responseStream);
              BufferedReader rd = new BufferedReader(inputStream);
              try
              {
                while ((line = rd.readLine()) != null)
                  responseStr.append(line);
              }
              catch (IOException ex) {
                Log.d("HelpShiftDebug", "IO Exception ex", ex);
              }
            }

            int status = sline.getStatusCode();
            HashMap messageResponse = new HashMap();
            messageResponse.put("status", Integer.valueOf(status));
            if ((status >= 200) && (status < 300)) {
              HSApiClient.timeStampErrorReplies = 0;
              try {
                messageResponse.put("response", new JSONArray(responseStr.toString()));
              }
              catch (JSONException earr) {
                try {
                  messageResponse.put("response", new JSONObject(responseStr.toString()));
                }
                catch (JSONException eobj) {
                  throw eobj;
                }
              }
              Message result = success.obtainMessage();
              result.obj = messageResponse;
              success.sendMessage(result);
            } else if (status == 304) {
              HSApiClient.timeStampErrorReplies++;
              Message result = success.obtainMessage();
              result.obj = null;
              success.sendMessage(result);
            } else if (status == 422) {
              HSApiClient.timeStampErrorReplies++;
              if (HSApiClient.timeStampErrorReplies <= 3) {
                for (int i = 0; i < headers.length; i++)
                  if (headers[i].getName().equals("HS-UEpoch")) {
                    storage.setServerTimeDelta(HSTimeUtil.calculateTimeAdjustment(headers[i].getValue()));
                    makeRequest(method, route, dataCopy, success, failure);
                  }
              }
              else {
            	HSApiClient.timeStampErrorReplies = 0;
                Message result = failure.obtainMessage();
                result.obj = messageResponse;
                failure.sendMessage(result);
              }
            } else {
              HSApiClient.timeStampErrorReplies = 0;
              Message result = failure.obtainMessage();
              result.obj = messageResponse;
              failure.sendMessage(result);
            }
          } catch (JSONException e) {
            sendFailMessage(failure, 1);
            Log.d("HelpShiftDebug", "Exception JSON", e);
          } catch (UnknownHostException e) {
            sendFailMessage(failure, 0);
            Log.d("HelpShiftDebug", "Exception Unknown Host", e);
          } catch (HttpHostConnectException e) {
            sendFailMessage(failure, 0);
            Log.d("HelpShiftDebug", "Exception cannot connect Host", e);
          } catch (SocketTimeoutException e) {
            Log.d("HelpShiftDebug", "Exception Socket timeout", e);
          } catch (ConnectTimeoutException e) {
            sendFailMessage(failure, 0);
            Log.d("HelpShiftDebug", "Exception Socket timeout", e);
          } catch (ClientProtocolException e) {
            sendFailMessage(failure, 1);

            Log.d("HelpShiftDebug", "Exception Client Protocol", e);
          } catch (SSLPeerUnverifiedException e) {
            Log.d("HelpShiftDebug", "Exception SSL Peer Unverified", e);
          } catch (IOException e) {
            sendFailMessage(failure, 1);

            Log.d("HelpShiftDebug", "Exception IO", e);
          }
        } catch (InstallException e) {
          Log.e("HelpShiftDebug", "install() not called", e);
          sendFailMessage(failure, 1);
        }
      }
    }).start();
  }

  private String getMimeType(String url)
  {
    String type = null;
    try {
      InputStream is = new FileInputStream(url);
      type = URLConnection.guessContentTypeFromStream(is);
      if (type == null) {
        type = URLConnection.guessContentTypeFromName(url);
      }
      is.close();
    } catch (IOException e) {
      e.printStackTrace();
    }

    return type;
  }

  private void uploadImage(final String method, final String route, final HashMap plainData, final Handler success, final Handler failure)
  {
    new Thread(new Runnable() {
      public void run() {
        try {
          HashMap data = addAuth(plainData, route, method);
          File screenshotFile = new File((String)data.get("screenshot"));
          String screenshotMimeType = getMimeType(screenshotFile.getPath());
          String[] includeExts = { "image/jpeg", "image/png", "image/gif", "image/x-png", "image/x-citrix-pjpeg", "image/x-citrix-gif", "image/pjpeg" };

          Set allowedMimeTypes = new HashSet(Arrays.asList(includeExts));
          if (!allowedMimeTypes.contains(screenshotMimeType)) {
            HashMap messageResponse = new HashMap();
            messageResponse.put("status", Integer.valueOf(-1));
            Message result = success.obtainMessage();
            result.obj = messageResponse;
            failure.sendMessage(result);
            return;
          }

          URL postUrl = null;
          try {
            postUrl = new URL(getApiUrl(route));
          } catch (MalformedURLException e) {
            Log.d("HelpShiftDebug", e.getMessage(), e);
            sendFailMessage(failure, 2);
          }
          String lineEnd = "\r\n";
          String twoHyphens = "--";
          String boundary = "*****";

          HttpURLConnection conn = null;
          try
          {
            conn = (HttpURLConnection)postUrl.openConnection();

            conn.setDoInput(true);

            conn.setDoOutput(true);

            conn.setUseCaches(false);

            conn.setRequestMethod("POST");
            conn.setConnectTimeout(30000);
            conn.setReadTimeout(30000);

            conn.setRequestProperty("Connection", "Keep-Alive");
            conn.setRequestProperty("Content-Type", "multipart/form-data;boundary=" + boundary);

            DataOutputStream dos = new DataOutputStream(conn.getOutputStream());
            dos.writeBytes(twoHyphens + boundary + lineEnd);
            List mapKeys = new ArrayList(data.keySet());
            for (Object key : mapKeys) {
              if (!key.equals("screenshot")) {
                String value = (String)data.get(key);
                dos.writeBytes("Content-Disposition: form-data; name=\"" + key + "\"; " + lineEnd);

                dos.writeBytes("Content-Type: text/plain;charset=UTF-8" + lineEnd);
                dos.writeBytes("Content-Length: " + value.length() + lineEnd);
                dos.writeBytes(lineEnd);
                dos.writeBytes(value + lineEnd);
                dos.writeBytes(twoHyphens + boundary + lineEnd);
              }
            }

            FileInputStream fileInputStream = new FileInputStream(screenshotFile);
            dos.writeBytes(twoHyphens + boundary + lineEnd);
            dos.writeBytes("Content-Disposition: form-data; name=\"screenshot\"; filename=\"" + screenshotFile.getName() + "\"" + lineEnd);

            dos.writeBytes("Content-Type: " + screenshotMimeType + lineEnd);
            dos.writeBytes("Content-Length: " + screenshotFile.length() + lineEnd);
            dos.writeBytes(lineEnd);

            int maxBufferSize = 1048576;

            int bytesAvailable = fileInputStream.available();

            int bufferSize = Math.min(bytesAvailable, maxBufferSize);
            byte[] buffer = new byte[bufferSize];

            int bytesRead = fileInputStream.read(buffer, 0, bufferSize);

            while (bytesRead > 0) {
              dos.write(buffer, 0, bufferSize);
              bytesAvailable = fileInputStream.available();
              bufferSize = Math.min(bytesAvailable, maxBufferSize);
              bytesRead = fileInputStream.read(buffer, 0, bufferSize);
            }

            dos.writeBytes(lineEnd);
            dos.writeBytes(twoHyphens + boundary + twoHyphens + lineEnd);
            fileInputStream.close();

            int status = conn.getResponseCode();
            String serverResponseMessage = conn.getResponseMessage();

            String line = "";
            StringBuilder responseStr = new StringBuilder();
            InputStream responseStream = conn.getInputStream();
            InputStreamReader inputStream = new InputStreamReader(responseStream);
            BufferedReader rd = new BufferedReader(inputStream);
            try
            {
              while ((line = rd.readLine()) != null)
                responseStr.append(line);
            }
            catch (IOException ex) {
              Log.d("HelpShiftDebug", "IO Exception ex", ex);
              sendFailMessage(failure, 2);
            }
            String response = responseStr.toString();

            HashMap messageResponse = new HashMap();
            messageResponse.put("status", Integer.valueOf(status));

            if ((status >= 200) && (status < 300)) {
              try {
                messageResponse.put("response", new JSONArray(response));
              }
              catch (JSONException earr) {
                try {
                  messageResponse.put("response", new JSONObject(response));
                }
                catch (JSONException eobj) {
                  throw eobj;
                }
              }
              Message result = success.obtainMessage();
              result.obj = messageResponse;
              success.sendMessage(result);
            } else {
              Message result = failure.obtainMessage();
              result.obj = messageResponse;
              failure.sendMessage(result);
            }
            conn.disconnect();
            dos.flush();
            dos.close();
          } catch (Exception e) {
            Log.d("HelpShiftDebug", e.getMessage(), e);
            sendFailMessage(failure, 2);
          }
        } catch (InstallException e) {
          Log.e("HelpShiftDebug", "Error : ", e);
          sendFailMessage(failure, 1);
        }
      }
    }).start();
  }

  protected void fetchFaqs(Handler success, Handler failure)
  {
    makeRequest("GET", "/faqs/", new HashMap(), success, failure);
  }

  protected void fetchFaq(Handler success, Handler failure, String faqId) {
    String route = new String("/faqs/" + faqId + "/");
    makeRequest("GET", route, new HashMap(), success, failure);
  }

  protected void search(Handler success, Handler failure, String query) {
    HashMap data = new HashMap();
    data.put("query", query);
    makeRequest("GET", "/search/", data, success, failure);
  }

  protected void registerProfile(Handler success, Handler failure, String username, String email, String identifier, String crittercismId)
  {
    HashMap data = new HashMap();
    data.put("displayname", username);
    data.put("email", email);
    data.put("identifier", identifier);
    if (crittercismId != null) {
      data.put("crittercism-id", crittercismId);
    }
    makeRequest("POST", "/profiles/", data, success, failure);
  }

  protected void updateUAToken(Handler success, Handler failure, String deviceToken, String profileId)
  {
    HashMap data = new HashMap();
    data.put("token", deviceToken);
    data.put("profile-id", profileId);

    makeRequest("POST", "/update-ua-token/", data, success, failure);
  }

  protected void fetchMyIssues(Handler success, Handler failure, String profileId, String since, String mc, String chatLaunchSource)
  {
    HashMap data = new HashMap();
    data.put("profile-id", profileId);
    data.put("since", since);
    data.put("mc", mc);
    if (chatLaunchSource != null) {
      data.put("chat-launch-source", chatLaunchSource);
    }

    makeRequest("POST", "/my-issues/", data, success, failure);
  }

  protected void createIssue(Handler success, Handler failure, String profileId, String messageText, String meta)
  {
    HashMap data = new HashMap();
    data.put("profile-id", profileId);
    data.put("message-text", messageText);
    data.put("meta", meta);
    makeRequest("POST", "/issues/", data, success, failure);
  }

  protected void addMessage(Handler success, Handler failure, String profileId, String issueId, String messageText, String type, String refers)
  {
    HashMap data = new HashMap();

    if (type == "ca")
      messageText = "Accepted the solution";
    else if (type == "ncr")
      messageText = "Did not accept the solution";
    else if (type == "ar") {
      messageText = "Accepted review request";
    }

    data.put("profile-id", profileId);
    data.put("message-text", messageText);
    data.put("type", type);
    data.put("refers", refers);
    makeRequest("POST", new String("/issues/" + issueId + "/messages/"), data, success, failure);
  }

  protected void addScMessage(Handler success, Handler failure, String profileId, String issueId, String messageText, String type, String refers, String imageUri)
  {
    HashMap data = new HashMap();
    if (type == "sc") {
      messageText = "Screenshot sent";
    }
    data.put("profile-id", profileId);
    data.put("message-text", messageText);
    data.put("type", type);
    data.put("refers", refers);
    data.put("screenshot", imageUri);
    uploadImage("POST", new String("/issues/" + issueId + "/messages/"), data, success, failure);
  }

  protected void fetchMessages(Handler success, Handler failure, String profileId, String issueId, String since, String chatLaunchSource)
  {
    HashMap data = new HashMap();
    data.put("profile-id", profileId);
    data.put("since", since);
    if (chatLaunchSource != null) {
      data.put("chat-launch-source", chatLaunchSource);
    }
    makeRequest("GET", new String("/issues/" + issueId + "/messages/"), data, success, failure);
  }

  protected void markHelpful(Handler success, Handler failure, String faqId)
  {
    HashMap data = new HashMap();
    makeRequest("POST", new String("/faqs/" + faqId + "/helpful/"), data, success, failure);
  }

  protected void markUnhelpful(Handler success, Handler failure, String faqId)
  {
    HashMap data = new HashMap();
    makeRequest("POST", new String("/faqs/" + faqId + "/unhelpful/"), data, success, failure);
  }

  protected void getConfig(Handler success, Handler failure)
  {
    HashMap data = new HashMap();
    makeRequest("GET", "/config/", data, success, failure);
  }

  protected void reportActionEvents(Handler success, Handler failure, JSONArray actions, String identifier, String uid, String profileId, String libraryVersion, String sdkType, String deviceModel, String os, String appVersion, String rom)
  {
    HashMap data = new HashMap();
    data.put("id", identifier);
    data.put("e", actions.toString());
    if (!profileId.equals("")) {
      data.put("profile-id", profileId);
    }
    if (!uid.equals("")) {
      data.put("uid", uid);
    }
    data.put("v", libraryVersion);
    data.put("s", sdkType);
    data.put("dm", deviceModel);
    data.put("os", os);
    data.put("av", appVersion);
    data.put("rs", rom);

    makeRequest("POST", "/events/", data, success, failure);
  }

  protected void getQuestion(String publishId, Handler success, Handler failure) {
    HashMap data = new HashMap();
    makeRequest("GET", "/faqs/" + publishId + "/", data, success, failure);
  }

  protected void updateMessageSeenState(JSONArray messageIds, String source, String readAt, Handler success, Handler failure)
  {
    HashMap data = new HashMap();
    data.put("message-ids", messageIds.toString());
    data.put("source", source);
    data.put("read-at", readAt);
    makeRequest("POST", "/events/messages/seen/", data, success, failure);
  }
}