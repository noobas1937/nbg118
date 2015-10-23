package org.nbg.stac.empire.pay.platform;

public class PayPlatformConst {
	//--begin------支付渠道定义--------
	public final static int PAY_GOOGLE_MARKET=0;//google电子市场支付渠道
	public final static int PAY_SELF=1;//公司内部支付渠道
	public final static int PAY_AMAZON=2;//AMAZON支付渠道
	public final static int PAY_SAMSUNG=3;//SAMSUNG支付渠道
	
	public final static float PAY_FEE_GOOGLE=0.3F;//google电子市场支付 分成比例
	//--end--------支付渠道定义--------
	
	//日本MARKET支付货币
	public static final String CURRENCY_JP_MARKET="JPY";
	/**
	 * PAYPAL  MARKET支付货币
	 */
	public static final String CURRENCY_PAYPAL = "USD";
	public static final String CURRENCY_MARKET = "USD";
	public static final String CURRENCY_CHINA="CNY";
	public static final String CURRENCY_BRAZIL="BRL";
//	public static final String CURRENCY_POLAND="PLN";
	
//	public final static String GOOGLE_PUBLISH_KEY_AOW="MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA0UUI1Vz+n/HljLKZKZumdonJWJ4PpCcnU2dZdfo/j4yzDG4Z5BHc8OPsht5OYsOPQhD5x4iRbXkdNQyEOzsXbWKo28xRAvl9U1X/xC6dKJ6cKt8VbIKAY3RxKan5UbpOhuSyFpCz4RwdXUKeCDZYzcRFV/8bphfXiNyHj7PqKKrTtl64UEC386/NHSyg1xdgtA0mS48tDTUZPogrI94GMJKmmQzduQlt5d24LGJsyjXG21vfxwI+YO6lj+TfpZ0AZcm8PQOZmGMyRxGGVDKBI/rtfRHk7YWBfdQofztaQ9w92vM8lnjqdB7KntjjXz/DMBA+sFOLfBsxI2BgdwENnQIDAQAB";
//    /**
//     * Google开发者账号上：GOOGLE支付 key
//     */
//    public final static String GOOGLE_PUBLISH_KEY_ELEX =     "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAgR0lRLJ7zHm246yxjlrlnXhE1L7ScrD6fUZxf/JAC53HkYxwgziAf4yh18J/KH+rFB81CPVyKWl4GRG9Uh6g66yOXZ3U0H8Amqe3ajuUanqRMaeCuEFN1Zz5HfOtAmI5zgpGzPDNgxIIl2nTFSx3CoJbXmPlqSvVNZzu0nweNYkdHSsNS2gSKxX9qKDKat6ZEzqr83htfKPZnwxcqAQ5BgBSHjPGIaPm7t2fb8fIITkVf80iTy8Eu1Sd6VcGoAjasbi56HRyW2dhEcm97dDJHQ63Pq7Glge0CXnKdd2Vzsp2xUXzFB3sKYpWKDV4etESHmJtSdD1EK4OvxH8o8ZvawIDAQAB";
//    public final static String GOOGLE_PUBLISH_KEY_ELEX_2DX = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAsKkkTvB2d4QYpjLFjEeAzi+hVrEnp/Wzj4Um1I4yM7ocRveJdAEbIOd8Lc4M/PyziH4AEEjm1AVYUQy0qXwMlsDp83MCy8eGnQxfcdV6kVFjiBStzSxDuow9ItdEKQOPz8OdWpmie3vXVr+1PPC5CzygQdeqzqVKYoIummepPdCt45D4uZ9oHWSasuIPsQ/0ufP8KDWYWf1OrxGxdVAXwqvVmQQfvqOP74MjIfpo2qDH5JfU36hTpoymYrRntEa9NkVBhPOIZtVcu6zRkIh9VdgddwlzlkcxmL+uQtqvDylYJNZaYjlRCyYfQG1ECjgN9YssgZhDmIsydzdk+neemwIDAQAB";
//	
//    /**
//     * Google开发者账号上：GOOGLE支付 key
//     */
//    public final static String GOOGLE_PUBLISH_KEY_JP = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAhgvbVkB1GHq2xbX+8gBj5NfLlKVdvsKtb1v1lGISswCWFlfDm8GiMN+Ld0R/T4cZ2CqWHFCgLYSjV7pV5RgN4ZQR2AkTplB+NEa2a8+7fiqOGbUQ5kZXXQaKuN9lNnwu4MiaAMILF/dDEkYgLSsX2bLQrZqOVd4GH9scR9q76BsEYIP7xvuSRCPgKsYlGARlTYc78Cq2c9DaYbf8SgWqWzVPn4Cba+q05BsUJwh7rRHaAGtUBnVkNrxeGX3eXFXSW98R1M5ubvNjlpyBAB7JExOwgQh/6dg4RU19S6WVgYPmjCTOYTWNVgfQNPsxBi+f2jaEYH/73ecuBjfjnftjGwIDAQAB";
	
	public final static int GOOGLE_PUBLISH_KEY_ELEX=0;
	public final static int GOOGLE_PUBLISH_KEY_JP=1;
	public final static int GOOGLE_PUBLISH_KEY_AOW=2;
	
	
	/**
	  * checkout支付渠道----对应337平台
	  */
	public final static String PAY_CHANNEL_MARKET = "googlecheckout";
	/**
	 * 公司支付渠道
	 */
	public final static String PAY_CHANNEL_SELF = "pay_self";
	/**
	 * Amazon支付渠道
	 */
	public final static String PAY_CHANNEL_AMAZON = "amazon";
	public final static String PAY_CHANNEL_SAMSUNG = "samsung";
}
