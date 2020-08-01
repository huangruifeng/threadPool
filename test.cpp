#include<iostream>
#include"thread_pool.h"

#define SLEEP 0

void test1(int num)
{
    utils::thread_pool pool(num);
    pool.run();
    std::thread t1([&pool] {
        for (int i = 0; i < 50; ++i)
        {
            pool.push([i] {
                std::cout << std::this_thread::get_id() << " thread1  " << i << std::endl;
#if SLEEP
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
#endif // SLEEP
            });
        }
    });

    std::thread t2([&pool] {

        for (int i = 50; i < 100; ++i)
        {
           // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            pool.push([b=i] {
                std::cout << std::this_thread::get_id() << " thread2 " << b << std::endl;
#if SLEEP
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
#endif // SLEEP
            });
            
        }
    });

    std::thread t3([&pool] {

        for (int i = 100; i < 150; ++i)
        {
            // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            pool.push([b = i] {
                std::cout << std::this_thread::get_id() << " thread3 " << b << std::endl;
#if SLEEP
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif // SLEEP
            });

        }
    });

    t1.join();
    t2.join();
    t3.join();

    std::this_thread::sleep_for(std::chrono::seconds(10));
}


int main()
{
    //test1(1);
    test1(4);
    system("pause");
    return 0;
}

