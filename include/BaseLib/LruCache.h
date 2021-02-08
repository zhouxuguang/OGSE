//
//  LURCache.h LRU缓存类，用于在内存中缓存经常出现的数据
//  baselib
//
//  Created by zhouxuguang on 16/7/19.
//  Copyright © 2016年 zhouxuguang. All rights reserved.
//

#ifndef OGSE_BASELIB_COMMON_LRU_CACHE_INCLUDE_H
#define OGSE_BASELIB_COMMON_LRU_CACHE_INCLUDE_H

#include "PreCompile.h"
#include "JenkinsHash.h"

namespace baselib
{
    
    /**
     * GenerationCache callback used when an item is removed
     */
    template<typename EntryKey, typename EntryValue>
    class OnEntryRemoved 
	{
    public:
        virtual ~OnEntryRemoved() { };
        virtual void operator()(EntryKey& key, EntryValue& value) = 0;
    }; // class OnEntryRemoved
    
    template <typename TKey, typename TValue>
    class LruCache 
	{
    public:
        explicit LruCache(uint32_t maxCapacity);
        
        virtual ~LruCache();
        
        enum Capacity 
		{
            kUnlimitedCapacity,
        };
        
        //设置移除监听器
        void SetOnEntryRemovedListener(OnEntryRemoved<TKey, TValue>* listener);
        
        //当前缓存项的个数
        size_t GetSize() const;
        
        //设置最大容量
        void SetMaxCapacity(uint32_t maxCapacity);
        
        //根据键取得值
        const TValue& Get(const TKey& key);
        
        //将键值对放到缓存中
        bool Put(const TKey& key, const TValue& value);
        
        //查询key的值是否存在
        bool Contains(const TKey &key);
        
        //移除键为key的项
        bool Remove(const TKey& key);
        bool RemoveOldest();
        void Clear();
        const TValue& PeekOldestValue();
        
    private:
        LruCache(const LruCache& that);  // disallow copy constructor
        
        struct Entry 
		{
            TKey key;
            TValue value;
            Entry* parent;
            Entry* child;
            
            Entry(TKey key_, TValue value_) : key(key_), value(value_), parent(NULL), child(NULL) 
			{
            }
            const TKey& getKey() const { return key; }
        };
        
        struct HashForEntry : public std::unary_function<Entry*, hash_t> 
		{
            size_t operator() (const Entry* entry) const 
			{
                return hash_type(entry->key);
            };
        };
        
        struct EqualityForHashedEntries : public std::unary_function<Entry*, hash_t> 
		{
            bool operator() (const Entry* lhs, const Entry* rhs) const 
			{
                return lhs->key == rhs->key;
            };
        };
        
        typedef std::unordered_set<Entry*, HashForEntry, EqualityForHashedEntries> LruCacheSet;
        
        void attachToCache(Entry& entry);
        void detachFromCache(Entry& entry);
        
        typename LruCacheSet::iterator findByKey(const TKey& key) 
		{
            Entry entryForSearch(key, mNullValue);
            typename LruCacheSet::iterator result = mSet->find(&entryForSearch);
            return result;
        }
        
        LruCacheSet* mSet;
        OnEntryRemoved<TKey, TValue>* mListener;
        Entry* mOldest;
        Entry* mYoungest;
        uint32_t mMaxCapacity;
        TValue mNullValue;
        
    public:
        // To be used like:
        // while (it.next())
        // {
        //   it.value(); it.key();
        // }
        class Iterator 
		{
        public:
            Iterator(const LruCache<TKey, TValue>& cache):
            mCache(cache), mIterator(mCache.mSet->begin()), mBeginReturned(false) 
			{
            }
            
            bool next() 
			{
                if (mIterator == mCache.mSet->end()) 
				{
                    return false;
                }
                if (!mBeginReturned) 
				{
                    // mIterator has been initialized to the beginning and
                    // hasn't been returned. Do not advance:
                    mBeginReturned = true;
                } 
				else 
				{
                    std::advance(mIterator, 1);
                }
                return mIterator != mCache.mSet->end();
            }
            
            const TValue& value() const 
			{
                return (*mIterator)->value;
            }
            
            const TKey& key() const 
			{
                return (*mIterator)->key;
            }
        private:
            const LruCache<TKey, TValue>& mCache;
            typename LruCacheSet::iterator mIterator;
            bool mBeginReturned;
        };
    };
    
    // Implementation is here, because it's fully templated
    template <typename TKey, typename TValue>
    LruCache<TKey, TValue>::LruCache(uint32_t maxCapacity)
    : mSet(new LruCacheSet())
    , mListener(NULL)
    , mOldest(NULL)
    , mYoungest(NULL)
    , mMaxCapacity(maxCapacity)
    , mNullValue(NULL) 
	{
        mSet->max_load_factor(1.0);
    };
    
    template <typename TKey, typename TValue>
    LruCache<TKey, TValue>::~LruCache() 
	{
        // Need to delete created entries.
        Clear();
    };
    
    template<typename K, typename V>
    void LruCache<K, V>::SetOnEntryRemovedListener(OnEntryRemoved<K, V>* listener) 
	{
        mListener = listener;
    }
    
    template <typename TKey, typename TValue>
    size_t LruCache<TKey, TValue>::GetSize() const 
	{
        return mSet->size();
    }
    
    template <typename TKey, typename TValue>
    void LruCache<TKey,TValue>::SetMaxCapacity(uint32_t maxCapacity)
    {
        this->mMaxCapacity = maxCapacity;
    }
    
    template <typename TKey, typename TValue>
    bool LruCache<TKey,TValue>::Contains(const TKey &key)
    {
        typename LruCacheSet::const_iterator find_result = findByKey(key);
        if (find_result != mSet->end()) 
		{
            return true;
        }
        return false;
    }
    
    template <typename TKey, typename TValue>
    const TValue& LruCache<TKey, TValue>::Get(const TKey& key) 
	{
        typename LruCacheSet::const_iterator find_result = findByKey(key);
        if (find_result == mSet->end()) 
		{
            return mNullValue;
        }
        Entry *entry = *find_result;
        detachFromCache(*entry);
        attachToCache(*entry);
        return entry->value;
    }
    
    template <typename TKey, typename TValue>
    bool LruCache<TKey, TValue>::Put(const TKey& key, const TValue& value) 
	{
        if (mMaxCapacity != kUnlimitedCapacity && GetSize() >= mMaxCapacity) 
		{
            RemoveOldest();
        }
        
        if (findByKey(key) != mSet->end()) 
		{
            return false;
        }
        
        Entry* newEntry = new Entry(key, value);
        mSet->insert(newEntry);
        attachToCache(*newEntry);
        return true;
    }
    
    template <typename TKey, typename TValue>
    bool LruCache<TKey, TValue>::Remove(const TKey& key) 
	{
        typename LruCacheSet::const_iterator find_result = findByKey(key);
        if (find_result == mSet->end()) 
		{
            return false;
        }
        Entry* entry = *find_result;
        mSet->erase(entry);
        if (mListener) 
		{
            (*mListener)(entry->key, entry->value);
        }
        detachFromCache(*entry);
        delete entry;
        return true;
    }
    
    template <typename TKey, typename TValue>
    bool LruCache<TKey, TValue>::RemoveOldest() 
	{
        if (mOldest != NULL) 
		{
            return Remove(mOldest->key);
            // TODO: should probably abort if false
        }
        return false;
    }
    
    template <typename TKey, typename TValue>
    const TValue& LruCache<TKey, TValue>::PeekOldestValue() 
	{
        if (mOldest) 
		{
            return mOldest->value;
        }
        return mNullValue;
    }
    
    template <typename TKey, typename TValue>
    void LruCache<TKey, TValue>::Clear() 
	{
        if (mListener) 
		{
            for (Entry* p = mOldest; p != NULL; p = p->child) 
			{
                (*mListener)(p->key, p->value);
            }
        }
        mYoungest = NULL;
        mOldest = NULL;
        
        LruCacheSet::iterator iter = mSet->begin();
        for (; iter != mSet->end(); ++ iter)
        {
            Entry* entry = *iter;
            delete entry;
        }
        
        mSet->clear();
		delete mSet;
		mSet = NULL;
    }
    
    template <typename TKey, typename TValue>
    void LruCache<TKey, TValue>::attachToCache(Entry& entry) 
	{
        if (mYoungest == NULL) 
		{
            mYoungest = mOldest = &entry;
        } 
		else 
		{
            entry.parent = mYoungest;
            mYoungest->child = &entry;
            mYoungest = &entry;
        }
    }
    
    template <typename TKey, typename TValue>
    void LruCache<TKey, TValue>::detachFromCache(Entry& entry) 
	{
        if (entry.parent != NULL) 
		{
            entry.parent->child = entry.child;
        } 
		else 
		{
            mOldest = entry.child;
        }

        if (entry.child != NULL) 
		{
            entry.child->parent = entry.parent;
        } 
		else 
		{
            mYoungest = entry.parent;
        }
        
        entry.parent = NULL;
        entry.child = NULL;
    }
    
}


#endif /* OGSE_BASELIB_COMMON_LRU_CACHE_INCLUDE_H */
