package com.elex.chatservice.res.drawable;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.NinePatchDrawable;
import android.graphics.drawable.StateListDrawable;
import android.util.Base64;

public final class HSDraw
{
  public static BitmapDrawable getBitmapDrawable(String imageString)
  {
    byte[] decodedByte = Base64.decode(imageString, 0);
    Bitmap bitmap = BitmapFactory.decodeByteArray(decodedByte, 0, decodedByte.length);
    BitmapDrawable bitmapDrawable = new BitmapDrawable(bitmap);
    return bitmapDrawable;
  }

  public static BitmapDrawable getBitmapDrawable(Context c, String imageString)
  {
    byte[] decodedByte = Base64.decode(imageString, 0);
    Bitmap bitmap = BitmapFactory.decodeByteArray(decodedByte, 0, decodedByte.length);
    BitmapDrawable bitmapDrawable = new BitmapDrawable(c.getResources(), bitmap);
    return bitmapDrawable;
  }

  public static NinePatchDrawable getNinePatchDrawable(Context c, String imageString) {
    byte[] decodedByte = Base64.decode(imageString, 0);
    Bitmap bitmap = BitmapFactory.decodeByteArray(decodedByte, 0, decodedByte.length);
    byte[] chunk = bitmap.getNinePatchChunk();
    NinePatchChunk npc = NinePatchChunk.deserialize(chunk);
    NinePatchDrawable npDrawable = new NinePatchDrawable(c.getResources(), bitmap, chunk, npc.mPaddings, null);

    return npDrawable;
  }

  public static NinePatchDrawable getNinePatchDrawable(Context c, int resId) {
    Bitmap bitmap = BitmapFactory.decodeResource(c.getResources(), resId);
    byte[] chunk = bitmap.getNinePatchChunk();
    NinePatchChunk npc = NinePatchChunk.deserialize(chunk);
    NinePatchDrawable npDrawable = new NinePatchDrawable(c.getResources(), bitmap, chunk, npc.mPaddings, null);

    return npDrawable;
  }

  public static StateListDrawable getStateDrawable(Context c, String normal, String pressed) {
    StateListDrawable states = new StateListDrawable();
    states.addState(new int[] { 16842919 }, getNinePatchDrawable(c, pressed));

    states.addState(new int[0], getNinePatchDrawable(c, normal));
    return states;
  }
}