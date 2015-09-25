package org.hcg.IF.jsondeserializer;

import java.util.Iterator;

import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import com.smartfoxserver.v2.entities.data.ISFSArray;
import com.smartfoxserver.v2.entities.data.ISFSObject;
import com.smartfoxserver.v2.entities.data.SFSArray;
import com.smartfoxserver.v2.entities.data.SFSDataType;
import com.smartfoxserver.v2.entities.data.SFSDataWrapper;
import com.smartfoxserver.v2.entities.data.SFSObject;

public class ChatServiceJsonDeSerializer
{
	private static ChatServiceJsonDeSerializer instance = new ChatServiceJsonDeSerializer();
	
	public static ChatServiceJsonDeSerializer getInstance()
	{
		return instance;
	}

	private ChatServiceJsonDeSerializer()
	{
	}
	
	public SFSObject newFromJsonData(String jsonStr)
	{
		return (SFSObject) ChatServiceJsonDeSerializer.getInstance().json2object(jsonStr);
	}
	
	public ISFSObject json2object(String jsonStr)
	{
		if (jsonStr.length() < 2)
		{
			throw new IllegalStateException((new StringBuilder("Can't decode SFSObject. JSON String is too short. Len: ")).append(
					jsonStr.length()).toString());
		}
		else
		{
			JSONObject jso = JSONObject.parseObject(jsonStr);
			return decodeSFSObject(jso);
		}
	}
	
	private ISFSObject decodeSFSObject(JSONObject jso)
	{
		ISFSObject sfsObject = SFSObject.newInstance();
		for (Iterator iterator = jso.keySet().iterator(); iterator.hasNext();)
		{
			Object key = iterator.next();
			Object value = jso.get(key);
			SFSDataWrapper decodedObject = decodeJsonObject(value);
			if (decodedObject != null)
				sfsObject.put((String) key, decodedObject);
			else
				throw new IllegalStateException((new StringBuilder("(json2sfsobj) Could not decode value for key: ")).append(key)
						.toString());
		}

		return sfsObject;
	}
	
	private SFSDataWrapper decodeJsonObject(Object o)
	{
		if (o instanceof Integer) return new SFSDataWrapper(SFSDataType.INT, o);
		if (o instanceof Long) return new SFSDataWrapper(SFSDataType.LONG, o);
		if (o instanceof Double) return new SFSDataWrapper(SFSDataType.DOUBLE, o);
		if (o instanceof Boolean) return new SFSDataWrapper(SFSDataType.BOOL, o);
		if (o instanceof String) return new SFSDataWrapper(SFSDataType.UTF_STRING, o);
		if (o instanceof JSONObject)
		{
			JSONObject jso = (JSONObject) o;
			if (jso==null)
				return new SFSDataWrapper(SFSDataType.NULL, o);
			else
				return new SFSDataWrapper(SFSDataType.SFS_OBJECT, decodeSFSObject(jso));
		}
		if (o instanceof JSONArray)
			return new SFSDataWrapper(SFSDataType.SFS_ARRAY, decodeSFSArray((JSONArray) o));
		else
			throw new IllegalArgumentException(String.format(
					"Unrecognized DataType while converting JSONObject 2 SFSObject. Object: %s, Type: %s",
					new Object[] { o, o != null ? o.getClass() : "null" }));
	}
	
	private ISFSArray decodeSFSArray(JSONArray jsa)
	{
		ISFSArray sfsArray = SFSArray.newInstance();
		for (Iterator iter = jsa.iterator(); iter.hasNext();)
		{
			Object value = iter.next();
			SFSDataWrapper decodedObject = decodeJsonObject(value);
			if (decodedObject != null)
				sfsArray.add(decodedObject);
			else
				throw new IllegalStateException((new StringBuilder("(json2sfarray) Could not decode value for object: ")).append(value)
						.toString());
		}

		return sfsArray;
	}
}
