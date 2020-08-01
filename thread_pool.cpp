#include"thread_pool.h"

namespace utils
{
    int thread_pool::push_async(const std::shared_ptr<work> w)
    {
        unsigned int size = 0;
        //todo 超过线程池缓存的数据
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _works.emplace_back(w);
            size  = _works.size();
            _cond.notify_one();
            return size;
        }
    }

    void thread_pool::clear()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _works.clear();
    }

    bool thread_pool::run()
    {
        _stop = false;
        _thread_vector.resize(_thread_count);

        try{
            for(int i = 0;i<_thread_count;i++)
            {
                //Tip windows resize 之后 使用 push_back vector size ++ 
                //_thread_vector.push_back(new thread_item(this));
                _thread_vector[i] = new thread_item(this);
            }
        }catch(...)
        {
            return false;
        }

        //Waiting for threads to be ready
        for(int i = 0;i<_thread_vector.size();)
        {
            if(_thread_vector[i]->_running == false)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }else
            {
                i++;
            }
        }
        return true;
    }

    void thread_pool::thread_func(thread_item* item)
    {
        
        for (;;)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            while ((_works.empty()) && _stop == false)
            {
                if (item->_running == false)
                    item->_running = true;
                _cond.wait(lock);
            }

            //_cond.wait(lock,[this,item]{
            //    if(item->_running == false)
            //        item->_running = true;
            //    return !(_works.empty() && _stop == false);
            //});

            if (_stop)
            {
                --_running_thread_num;
                break;
            }
            ++_running_thread_num;
            auto w = _works.front();
            _works.pop_front();

            lock.unlock();

            if (w != nullptr)
            {
                (*w)();
            }
            --_running_thread_num;
        }

    }

    void thread_pool::stop_all()
    {
        if(_stop)
        {
            return;
        }
        _stop = true;

        _cond.notify_all();

        for(auto& th : _thread_vector)
        {
            delete th;
        }
        _thread_vector.clear();
        
        return;
    }
}