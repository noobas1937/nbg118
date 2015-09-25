package com.elex.chatservice.res.drawable;

import android.graphics.Rect;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

final class NinePatchChunk
{
  public static final int NO_COLOR = 1;
  public static final int TRANSPARENT_COLOR = 0;
  public Rect mPaddings = new Rect();
  public int[] mDivX;
  public int[] mDivY;
  public int[] mColor;

  private static void readIntArray(int[] data, ByteBuffer buffer)
  {
    int i = 0; for (int n = data.length; i < n; i++)
      data[i] = buffer.getInt();
  }

  private static void checkDivCount(int length)
  {
    if ((length == 0) || ((length & 0x1) != 0))
      throw new RuntimeException("invalid nine-patch: " + length);
  }

  public static NinePatchChunk deserialize(byte[] data)
  {
    ByteBuffer byteBuffer = ByteBuffer.wrap(data).order(ByteOrder.nativeOrder());

    byte wasSerialized = byteBuffer.get();
    if (wasSerialized == 0) return null;

    NinePatchChunk chunk = new NinePatchChunk();
    chunk.mDivX = new int[byteBuffer.get()];
    chunk.mDivY = new int[byteBuffer.get()];
    chunk.mColor = new int[byteBuffer.get()];

    checkDivCount(chunk.mDivX.length);
    checkDivCount(chunk.mDivY.length);

    byteBuffer.getInt();
    byteBuffer.getInt();

    chunk.mPaddings.left = byteBuffer.getInt();
    chunk.mPaddings.right = byteBuffer.getInt();
    chunk.mPaddings.top = byteBuffer.getInt();
    chunk.mPaddings.bottom = byteBuffer.getInt();

    byteBuffer.getInt();

    readIntArray(chunk.mDivX, byteBuffer);
    readIntArray(chunk.mDivY, byteBuffer);
    readIntArray(chunk.mColor, byteBuffer);

    return chunk;
  }
}