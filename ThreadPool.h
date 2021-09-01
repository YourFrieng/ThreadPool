#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <functional>
#include <chrono>

using std::chrono::operator""ms;

using TaskQueue = std::queue< std::function< void() > >;

class ThreadPool
{
private:
    bool work = true;
    std::mutex MutexQue;
    TaskQueue tasks;
    void DoTasks( );
public:
    void AddTask( std::function< void() > );
    void StartPool( );
    void EndPool( );
    ThreadPool( );
    ~ThreadPool( );
};

void ThreadPool::AddTask( std::function< void() > function  )
{
    std::lock_guard< std::mutex > lock( MutexQue );
    tasks.push( function );
}

void ThreadPool::EndPool( )
{
    work = false;
}

void ThreadPool::DoTasks( )
{
    while ( true )
    {
        if( !work ) break;

        MutexQue.lock( );
        if( tasks.empty( ) )
        {
            MutexQue.unlock( );
            std::this_thread::sleep_for( 500ms );
            continue;
        }
        auto fun = tasks.front( );
        tasks.pop( );
        MutexQue.unlock( );
        fun( );
    }
    
}

void ThreadPool::StartPool( )
{
    for (size_t i = 0; i < std::thread::hardware_concurrency( ); i++)
    {
        std::thread th( &ThreadPool::DoTasks, this );
        th.detach( );
    }   
}


ThreadPool::ThreadPool( ) = default;
ThreadPool::~ThreadPool() = default;


