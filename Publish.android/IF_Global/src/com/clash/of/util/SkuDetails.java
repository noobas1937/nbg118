/* Copyright (c) 2012 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.clash.of.util;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Represents an in-app product's listing details.
 */
public class SkuDetails {
    String mItemType;
    String mSku;
    String mType;
    String mPrice;
    String mTitle;
    String mDescription;
    String mJson;
    String mCurrencyCode;

    public SkuDetails(String jsonSkuDetails) throws JSONException {
        this(IabHelper.ITEM_TYPE_INAPP, jsonSkuDetails);
    }

    public SkuDetails(String itemType, String jsonSkuDetails) throws JSONException {
    	try{
	        mItemType = itemType;
	        mJson = jsonSkuDetails;
	        JSONObject o = new JSONObject(mJson);
	        if(o.has("price_currency_code")){
	        	mCurrencyCode = o.optString("price_currency_code");
	        }
	
	        mSku = o.optString("productId");
	        mType = o.optString("type");
	        mPrice = o.optString("price");
	        mTitle = o.optString("title");
	        mDescription = o.optString("description");
    	}catch(Exception ex)
    	{
    		
    	}
    }

    public String getSku() { return mSku; }
    public String getType() { return mType; }
    public String getPrice() { return mPrice; }
    public String getTitle() { return mTitle; }
    public String getDescription() { return mDescription; }
    public String getPriceCurrencyCode() { return mCurrencyCode; }

    @Override
    public String toString() {
        return "SkuDetails:" + mJson;
    }
}
