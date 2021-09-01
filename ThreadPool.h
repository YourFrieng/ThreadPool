#include <thread>
#include <queue>
#include <mutex>
#include <functional>
#include <chrono>
#include <vector>
#include <atomic>

using std::chrono::operator""ms;

using TaskQueue = std::queue< std::function< void() > >;
using ThreadVector = std::vector< std::thread >;

class ThreadPool
{
public:
    ThreadPool( );
    ~ThreadPool( ) = default;
    void AddTask( std::function< void() > );
    void StartPool( );
    void EndPool( );

private:
    std::atomic<bool> m_stop = false;
    std::mutex m_mutex_queue;
    TaskQueue m_tasks;
    ThreadVector m_threads;
    void DoTasks( );
};

void ThreadPool::AddTask( std::function< void() > function  )
{
    std::lock_guard< std::mutex > lock( m_mutex_queue );
    m_tasks.push( function );
}

void ThreadPool::EndPool( )
{
    m_stop = true;
    for (auto& th : m_threads)
    {
        if( th.joinable( ) )
        {
            th.join( );
        } 
    }   
}

void ThreadPool::DoTasks( )
{
    while ( !m_stop )
    {
        std::unique_lock locker( m_mutex_queue );
        if( m_tasks.empty( ) )
        {
            locker.unlock( );
            std::this_thread::sleep_for( 500ms );
            continue;
        }
        auto fun = m_tasks.front( );
        m_tasks.pop( );
        locker.unlock( );
        fun( );
    }
    
}

void ThreadPool::StartPool( )
{
    for (auto& th : m_threads)
    {
        th = std::thread( &ThreadPool::DoTasks, this );
    }   
}


ThreadPool::ThreadPool( )
:m_threads( std::thread::hardware_concurrency( ) )
{}



