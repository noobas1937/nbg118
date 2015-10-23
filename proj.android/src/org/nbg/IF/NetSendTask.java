package org.nbg.IF;

import org.cocos2dx.ext.Native;

import android.os.AsyncTask;
import com.smartfoxserver.v2.entities.data.ISFSObject;
import sfs2x.client.requests.ExtensionRequest;

/**
 * Created by long on 14-2-21.
 */
public class NetSendTask extends AsyncTask<Net,Void,Void> {
    final private String cmd;
    final private ISFSObject obj;
    @Override
    protected Void doInBackground(Net... params) {
        if(params[0].sfsClient != null)
        {
//            params[0].sfsClient.send(new ExtensionRequest(cmd, obj));
			try{
				params[0].sfsClient.send(new ExtensionRequest(cmd, obj));
			}catch(java.lang.NullPointerException e){
				//网络中断等内部原因，导致
				// 连接丢失
				params[0].getInstance().releaseSfsClient();
				Native.postNotification("NConnectionLost");
				Runtime.getRuntime().gc();
//				java.lang.NullPointerException
//				at sfs2x.client.core.sockets.TCPSocketLayer.writeSocket(TCPSocketLayer.java:152)
//				at sfs2x.client.core.sockets.TCPSocketLayer.write(TCPSocketLayer.java:254)
//				at sfs2x.client.core.SFSIOHandler.writeTCP(SFSIOHandler.java:401)
//				at sfs2x.client.core.SFSIOHandler.onDataWrite(SFSIOHandler.java:395)
//				at sfs2x.client.core.SFSProtocolCodec.onPacketWrite(SFSProtocolCodec.java:79)
//				at sfs2x.client.bitswarm.BitSwarmClient.send(BitSwarmClient.java:223)
//				at sfs2x.client.SmartFox.send(SmartFox.java:893)
//				at com.elextech.aoe.Net.send(Net.java:373)
			}
        }
        return null;
    }
    public NetSendTask (String c,ISFSObject o) {
        this.cmd = c;
        this.obj = o;
    }
}
