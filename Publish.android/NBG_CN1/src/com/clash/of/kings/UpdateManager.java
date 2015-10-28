package com.clash.of.kings;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.StringWriter;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.util.HashMap;

import com.helpshift.Log;
import com.nostra13.socialsharing.twitter.extpack.winterwell.json.JSONArray;
import com.nostra13.socialsharing.twitter.extpack.winterwell.json.JSONObject;

import org.cocos2dx.ext.Device;

import android.R.string;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.DialogInterface.OnClickListener;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ProgressBar;
import android.widget.Toast;

/**
 *@author coolszy
 *@date 2012-4-26
 *@blog http://blog.92coding.com
 */

public class UpdateManager
{
    /* 下载中 */
    private static final int DOWNLOAD = 1;
    /* 下载结束 */
    private static final int DOWNLOAD_FINISH = 2;
    /* 保存解析的XML信息 */
    HashMap<String, String> mHashMap;
    /* 下载保存路径 */
    private String mSavePath;
    /* 记录进度条数量 */
    private int progress;
    /* 是否取消更新 */
    private boolean cancelUpdate = false;

    private Context mContext;
    /* 更新进度条 */
    private ProgressBar mProgress;
    private Dialog mDownloadDialog;

    private String localName;
    private String downLoadURL;
    
    public String bufferStr;
    private Handler mHandler = new Handler()
    {
        public void handleMessage(Message msg)
        {
            switch (msg.what)
            {
            // 正在下载
            case DOWNLOAD:
                // 设置进度条位置
//                mProgress.setProgress(progress);
            	break;
            case DOWNLOAD_FINISH:
                // 安装文件
                installApk();
                break;
            default:
                break;
            }
        };
    };

    public UpdateManager(Context context)
    {
        this.mContext = context;
    }

    /**
     * 检测软件更新
     */
    public void checkUpdate()
    {
    	// execPHP("http://p1nbg.elexapp.com/gameservice/api/get_cn_apk_ver.php");
        execPHP("http://p1nbg.elexapp.com/gameservice/api/get_cn_apk_ver.php");

//        if (isUpdate())
//        {
//            // 显示提示对话框
//            showNoticeDialog();
//        }
    }
    
    public void execPHP(final String scriptName) {  
//		try {
			
			new Thread(new Runnable() {
				String line;

				public void run() {
					try {
						URLConnection connection = null;
						Log.d("tag10", scriptName);
						connection = new URL(scriptName).openConnection();
						Log.d("tag11", scriptName);
						if(connection == null){
							Log.d("tag12", scriptName);
						}
						connection.connect();
						Log.d("tag13", scriptName);
						InputStream fin = connection.getInputStream();
						final BufferedReader br = new BufferedReader(new InputStreamReader(fin));			

						System.out.println("listener started");

						StringBuffer buffer = new StringBuffer();
						while ((line = br.readLine()) != null) {
							System.out.println(line);
							buffer.append(line);
						
						}
						bufferStr = buffer.toString();
						if(isUpdate()){
							showNoticeDialog();
						}
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			}).start();

//		} catch (IOException e) {
//			e.printStackTrace();
//			bufferStr = null;
//		}
    }
    
    /**
     * 检查软件是否有更新版本
     * 
     * @return
     */
    private boolean isUpdate()
    {
    	//当前版本号
    	String localVersion = Device.getVersionCode();
    	//游戏最新版本号
    	
    	if(bufferStr == null || bufferStr == ""){
    		return false;
    	}

    	JSONObject json = new JSONObject(bufferStr);
		String  updateStr = json.getString("update");
		JSONObject updateJson = new JSONObject(updateStr);
		String version = updateJson.getString("version");
		localName = updateJson.getString("name");
		downLoadURL = updateJson.getString("url") + "?" + version;
		
    	Log.d("tag2", bufferStr.toString());
    	Log.d("tag3", updateStr.toString());
    	Log.d("tag4", localVersion.toString());
    	Log.d("tag5", version.toString());
    	Log.d("tag6", localName.toString());
    	Log.d("tag7", downLoadURL.toString());

		if(version.equals(localVersion)){
			return false;
		}
		
    	return true;
    }

    /**
     * 显示软件更新对话框
     */
    private void showNoticeDialog()
    {
    	showDownloadDialog();
    }

    /**
     * 显示软件下载对话框
     */
    private void showDownloadDialog()
    {
        // 现在文件
        downloadApk();
    }

    /**
     * 下载apk文件
     */
    private void downloadApk()
    {
        // 启动新线程下载软件
        new downloadApkThread().start();
    }

    /**
     * 下载文件线程
     * 
     * @author coolszy
     *@date 2012-4-26
     *@blog http://blog.92coding.com
     */
    private class downloadApkThread extends Thread
    {
        @Override
        public void run()
        {
            try
            {
                // 判断SD卡是否存在，并且是否具有读写权限
                if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED))
                {
                    // 获得存储卡的路径
                    String sdpath = Environment.getExternalStorageDirectory() + "/";
                    mSavePath = sdpath + "download";
                    URL url = new URL(downLoadURL);
//                    URL url = new URL("http://10.1.6.36:8081/hudson/job/IF_cn1/ws/package/20150604/NBG_CN1.apk");
                    Log.d("tag1", url.getPath());
                    // 创建连接
                    HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                    conn.connect();
                    // 获取文件大小
                    int length = conn.getContentLength();
                    // 创建输入流
                    InputStream is = conn.getInputStream();

                    File file = new File(mSavePath);
                    // 判断文件目录是否存在
                    if (!file.exists())
                    {
                        file.mkdir();
                    }
//                    File apkFile = new File(mSavePath, mHashMap.get("name"));
                    File apkFile = new File(mSavePath, localName);
                    FileOutputStream fos = new FileOutputStream(apkFile);
                    int count = 0;
                    // 缓存
                    byte buf[] = new byte[1024];
                    // 写入到文件中
                    do
                    {
                        int numread = is.read(buf);
                        count += numread;
                        // 计算进度条位置
                        progress = (int) (((float) count / length) * 100);
                        // 更新进度
                        mHandler.sendEmptyMessage(DOWNLOAD);
                        if (numread <= 0)
                        {
                            // 下载完成
                            mHandler.sendEmptyMessage(DOWNLOAD_FINISH);
                            break;
                        }
                        // 写入文件
                        fos.write(buf, 0, numread);
                    } while (!cancelUpdate);// 点击取消就停止下载.
                    fos.close();
                    is.close();
                }
            } catch (MalformedURLException e)
            {
                e.printStackTrace();
            } catch (IOException e)
            {
                e.printStackTrace();
            }
            // 取消下载对话框显示
//            mDownloadDialog.dismiss();
        }
    };
    
    /**
     * 安装APK文件
     */
    private void installApk()
    {
//        File apkfile = new File(mSavePath, mHashMap.get("name"));
        File apkfile = new File(mSavePath, localName);

        if (!apkfile.exists())
        {
            return;
        }
        // 通过Intent安装APK文件
        Intent i = new Intent(Intent.ACTION_VIEW);
        i.setDataAndType(Uri.parse("file://" + apkfile.toString()), "application/vnd.android.package-archive");
        mContext.startActivity(i);
    }
}