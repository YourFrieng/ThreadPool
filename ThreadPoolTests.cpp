#include "ThreadPool.h"

void Task1( )
{
        
    std::this_thread::sleep_for(1000ms);
    std::cout << "Hello, World!" << std::endl;
}

void Task2( )
{
    std::this_thread::sleep_for(100ms);
    std::cout << 25 - 15 << std::endl;
}

void Task3( )
{
    std::this_thread::sleep_for(500ms);
    std::cout << 25 * 10 << std::endl;
}


void Task4( )
{
    std::this_thread::sleep_for(50ms);
    std::cout << "Task4" << std::endl;
}


int main( )
{
    ThreadPool th1;

    th1.StartPool( );
    for (size_t i = 0; i < 50; i++)
    {
        if( i % 2 == 0)
            th1.AddTask( Task1 );
        else
            th1.AddTask( Task2 );
    }

    for (size_t i = 0; i < 50; i++)
    {
        if( i % 2 == 0)
            th1.AddTask( Task2 );
        else
            th1.AddTask( Task3 );
    }

    for (size_t i = 0; i < 50; i++)
    {
        if( i % 2 == 0)
            th1.AddTask( Task3 );
        else
            th1.AddTask( Task1 );
    }
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < 20; i++)
    {
        th1.AddTask( Task4 );

    }
    char k;
    std::cin >> k;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end-start;
    std::cout << "Waited " << elapsed.count() << " ms\n";

    th1.EndPool( );
    
    return 0;
}
