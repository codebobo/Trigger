#ifndef __CACHE_MAP_H__
#define __CACHE_MAP_H__
#include "TrantorTimerLoop.h"

#include <map>
#include <TrantorTimestamp.h>
#include <mutex>
#include <deque>
#include <vector>
#include <set>

class CallbackEntry
{
public:
    CallbackEntry(std::function<void()> cb):cb_(cb){}
    ~CallbackEntry()
    {
        cb_();
    }
private:
    std::function<void()> cb_;
};

typedef std::shared_ptr<CallbackEntry> CallbackEntryPtr;
typedef std::weak_ptr<CallbackEntry> WeakCallbackEntryPtr;

typedef std::set<CallbackEntryPtr> CallbackBucket;
typedef std::deque<CallbackBucket> CallbackBucketQueue;

template <typename T1,typename T2>
class CacheMap
{
public:
    CacheMap(int interval,int limit)
    :timeInterval_(interval),
     timer_(TrantorTimerLoop::Instance())
    {
        bucketCount_=limit/interval+1;
        event_bucket_queue_.resize(bucketCount_);
        timer_.runEvery(interval, [=](){
            CallbackBucket tmp;
            {
                std::lock_guard<std::mutex> lock(bucketMutex_);
                tmp = event_bucket_queue_.front();
                event_bucket_queue_.pop_front();
                event_bucket_queue_.push_back(CallbackBucket());
            }
        });
    };
    typedef struct MapValue
    {
        int timeout=0;
        T2 value;
    }MapValue;
    void insert(const T1& key,const T2& value,int timeout)
    {
        if(timeout>0)
        {
            {
                std::lock_guard<std::mutex> lock(mtx_);
                map_[key].value=value;
                map_[key].timeout=timeout;
            }
            eraseAfter(timeout,key);
        }
        else
        {
            std::lock_guard<std::mutex> lock(mtx_);
            map_[key].value=value;
            map_[key].timeout=timeout;
        }
    }
    T2& operator [](const T1& key){
        int timeout=0;
        std::lock_guard<std::mutex> lock(mtx_);
        if(map_.find(key)!=map_.end())
        {
            timeout=map_[key].timeout;
        }
        if(timeout)
            eraseAfter(timeout,key);


        return map_[key].value;
    }
    bool find(const T1& key)
    {
        int timeout=0;
        bool flag=false;
        std::lock_guard<std::mutex> lock(mtx_);
        if(map_.find(key)!=map_.end())
        {
            timeout=map_[key].timeout;
            flag=true;
        }
        if(timeout)
            eraseAfter(timeout,key);


        return flag;
    }
protected:
    std::map< T1,MapValue > map_;
    CallbackBucketQueue event_bucket_queue_;
    std::map< T1, WeakCallbackEntryPtr > weak_entry_map_;
    std::mutex mtx_;
    std::mutex weakPtrMutex_;
    std::mutex bucketMutex_;
    int bucketCount_;
    int timeInterval_;
    TrantorTimerLoop& timer_;

    void eraseAfter(int delay,const T1& key)
    {
        uint32_t bucketIndexToPush;
        uint32_t bucketNum = uint32_t(delay / timeInterval_) + 1;
        uint32_t queue_size = event_bucket_queue_.size();

        if (bucketNum >= queue_size)
        {
            bucketIndexToPush = queue_size - 1;
        }
        else
        {
            bucketIndexToPush = bucketNum;
        }

        CallbackEntryPtr entryPtr;
        {
            std::lock_guard<std::mutex> lock(weakPtrMutex_);
            if(weak_entry_map_.find(key)!=weak_entry_map_.end())
            {
                entryPtr=weak_entry_map_[key].lock();
            }
        }
        if(entryPtr)
        {
            std::lock_guard<std::mutex> lock(bucketMutex_);
            event_bucket_queue_[bucketIndexToPush].insert(entryPtr);
        }
        else
        {
            std::function<void ()>cb=[=](){
                std::lock_guard<std::mutex> lock(mtx_);
                std::lock_guard<std::mutex> lock1(weakPtrMutex_);
                WeakCallbackEntryPtr tmpWeakPtr;
                if(weak_entry_map_.find(key)!=weak_entry_map_.end())
                {
                    tmpWeakPtr=weak_entry_map_[key];
                    if(!tmpWeakPtr.lock())
                    {
                        weak_entry_map_.erase(key);
                        {

                            map_.erase(key);
                        }
                    }

                }

            };
            entryPtr=std::make_shared<CallbackEntry>(cb);
            {
                std::lock_guard<std::mutex> lock(weakPtrMutex_);
                weak_entry_map_[key] = WeakCallbackEntryPtr(entryPtr);
            }
            {
                std::lock_guard<std::mutex> lock(bucketMutex_);
                event_bucket_queue_[bucketIndexToPush].insert(entryPtr);
            }
        }
    }
};
#endif