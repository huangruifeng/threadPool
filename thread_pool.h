#ifndef _THREAD_POOL_
#define _THREAD_POOL_ 1

#include<thread>
#include<atomic>
#include<mutex>
#include<deque>
#include<vector>
#define MAX_DEQUE_SIZE 2048
namespace utils
{

class work
{
public:
    virtual void operator()() = 0;
    virtual ~work() = default;
};

template<class T>
class async_work : public work
{
public:
async_work(const T call) :_call(call)
{}
void operator()() override
{
    _call();
}
    T _call;
};

class thread_pool
{  
public:
    thread_pool(unsigned int thread_count):_running_thread_num(0),_thread_count(thread_count),_stop(true){}
    ~thread_pool()
    {
        if(!_stop)
        {
            stop_all();
        }
    };
    bool run();
    void stop_all();

template<typename T>
int push(const T &task)
{
    return push_async(std::shared_ptr<work>(new async_work<T>(task)));
}
void clear();
private:
int push_async(const std::shared_ptr<work> task);


private:
    class thread_item;
    void thread_func(thread_item* item);

    class thread_item
    {
    public:
        thread_item(thread_pool* pool):_pool(pool),_running(false)
        {
            _thread = std::thread(&thread_pool::thread_func,pool,this);
        }
        ~thread_item()
        {
            if(_thread.joinable())
            {
                _thread.join();
            }
        }
        std::thread _thread;
        bool _running;
        thread_pool* _pool;
    };

    std::atomic<int> _running_thread_num;
    int _thread_count;
    bool _stop;

    std::vector<thread_item*> _thread_vector;
    std::mutex _mutex;
    std::condition_variable _cond;

    std::deque<std::shared_ptr<work>> _works;
    int64_t _maxSize = MAX_DEQUE_SIZE;
};
}
#endif
