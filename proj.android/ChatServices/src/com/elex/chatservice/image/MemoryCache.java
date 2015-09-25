package com.elex.chatservice.image;

//内存缓存
public class MemoryCache<K, V> extends InAbsCache<K, V>
{
	public MemoryCache(int cacheSize)
	{
		super(cacheSize);
		// TODO Auto-generated constructor stub
	}

	@Override
	public void cache(K localUrl, V value)
	{
		// TODO Auto-generated method stub
		cacheToMemory(localUrl, value);
	}

	@Override
	public V get(K key)
	{
		// TODO Auto-generated method stub
		return getFromMemory(key);
	}

	@Override
	public boolean containsKey(K key)
	{
		// TODO Auto-generated method stub
		return memoryCacheContainsKey(key);
	}

	@Override
	public void removeCache(K key)
	{
		// TODO Auto-generated method stub
		removeMemoryCache(key);
	}
}
