package org.nbg.util;

public class EncryptUtil {
	static private final int ENC_KEY = 0xf9b37d;
	private static int s_pkgKey = ENC_KEY, r_pkgKey = ENC_KEY;

	/**
	 * 
	 * @param data
	 * @return
	 */
	public static byte[] encode(byte[] data) {
		if (data == null) {
			return null;
		}
		//
		//s_pkgKey++;
		if (s_pkgKey == Integer.MAX_VALUE) {
			s_pkgKey = ENC_KEY;
		}
		// ////System.out.println("发送, encode 前:" + s_pkgKey);
		// print(data);
		//
		int resume = 0;
		byte s_crc1 = (byte) ((~(s_pkgKey % 0x100)) & 0xff);
		for (int i = 0, len = data.length; i < len; i++) {
			resume += data[i];
			data[i] = encryptByte(data[i], s_crc1);
		}
		byte s_crc2 = (byte) ((resume % 0x100) & 0xff);

		byte[] tmpb = new byte[data.length + 1];
		System.arraycopy(data, 0, tmpb, 1, data.length);
		tmpb[0] = encryptByte(s_crc2, s_crc1);
		// ////System.out.println("发送, encode 后:" + s_pkgKey);
		// print(data);

		return tmpb;
	}

	
	/**
	 * 加密
	 * 
	 * @param b
	 * @param key
	 * @return
	 */
	private static byte encryptByte(byte b, byte key) {
		int i = b & 0xff;
		i = (i + key) % 0x100;
		b = (byte) ((~(i & 0xff)) & 0xff);
		b = (byte) ((b ^ (key << 1)) & 0xff);
		return b;
	}

	//server 端解密
	
	/**
	 * 
	 * @param data
	 * @return
	 */
	public static byte[] decode(byte[] data) {
		if (data == null) {
			return null;
		}
		//
		//r_pkgKey++;
		if (r_pkgKey == Integer.MAX_VALUE) {
			r_pkgKey = ENC_KEY;
		}

		int resume = 0;
		byte s_crc1 = (byte) ((~(r_pkgKey % 0x100)) & 0xff);
		for (int i = 1, len = data.length; i < len; i++) {
			data[i] = decryptByte(data[i], s_crc1);
			resume += data[i];
		}

		data[0] = decryptByte(data[0], s_crc1);
		byte s_crc2 = (byte) ((resume % 0x100) & 0xff);
 
		if (data[0] != s_crc2) {
			return null;
		}
		byte[] tmpb = new byte[data.length - 1];
		System.arraycopy(data, 1, tmpb, 0, data.length - 1);

		return tmpb;
	}

	/**
	 * 解密
	 * 
	 * @param b
	 * @param key
	 * @return
	 */
	private static byte decryptByte(byte b, byte key) {
		b = (byte) ((b ^ (key << 1)) & 0xff);
		b = (byte) ((~b) & 0xff);
		int i = b & 0xff;
		i = (i + 256 - key) % 0x100;
		return (byte) (i & 0xff);
	}

}
