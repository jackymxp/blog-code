#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <iostream>

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template<typename T, typename Container = std::queue <T>>
class ThreadQueue {
public:
    using container_type = Container;
    using value_type = typename Container::value_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using size_type = typename Container::size_type;
    using mutex_type = std::mutex;
    using condition_variable_type = std::condition_variable;
private:
    Container queue_;
    mutable mutex_type mutex_;
    condition_variable_type cond_;

public:
    ThreadQueue() = default;

    ThreadQueue(const ThreadQueue &) = delete;

    ThreadQueue &operator=(const ThreadQueue &) = delete;

    value_type get(){
        value_type elem;
        pop(elem);
        return elem;
    }

    void pop(reference elem) {
        std::unique_lock <mutex_type> lock(mutex_);
        cond_.wait(lock, [this]() { return !queue_.empty(); });
        elem = std::move(queue_.front());
        queue_.pop();
    }

    bool try_pop(reference elem) {
        std::unique_lock <mutex_type> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }
        elem = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard <mutex_type> lock(mutex_);
        return queue_.empty();
    }

    size_type size() const {
        std::lock_guard <mutex_type> lock(mutex_);
        return queue_.size();
    }

    void push(const value_type &elem) {
        {
            std::lock_guard <mutex_type> lock(mutex_);
            queue_.push(elem);
        }
        cond_.notify_one();
    }

    void push(value_type &&elem) {
        {
            std::lock_guard <mutex_type> lock(mutex_);
            queue_.push(std::move(elem));
        }
        cond_.notify_one();
    }
};








class ThreadPool {
public:
    ThreadPool(size_t);

    template<class F, class... Args>
    auto enqueue(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

    ~ThreadPool();

private:
    // need to keep track of threads so we can join them
    std::vector <std::thread> workers;
    // the task queue
    std::queue <std::function<void()>> tasks;

    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads) : stop(false) {
    for (size_t i = 0; i < threads; ++i)
        workers.emplace_back(
                [this] {
                    for (;;) {
                        std::function<void()> task;
                        std::unique_lock <std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                        task();
                    }
                }
        );
}

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename
    std::result_of<F(Args...)>::type;

    auto task = std::make_shared < std::packaged_task < return_type() >> (
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    std::future <return_type> res = task->get_future();
    std::unique_lock <std::mutex> lock(queue_mutex);

// don't allow enqueueing after stopping the pool
    if (stop)
        throw std::runtime_error("enqueue on stopped ThreadPool");
    tasks.emplace([task]() {
        (*task)();
    });

    condition.notify_one();
    return res;
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool() {
    {
        std::unique_lock <std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker: workers)
        worker.join();
}

int tast1(int m) {
    const int len = 100000;
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {}
    }
    return m * m;
}

int test2(int m, int n) {
    return m + n;
}

int main(void) {
    // create thread pool with 4 worker threads
    ThreadPool pool(4);

    std::vector<decltype(pool.enqueue(tast1, 0))> v;
    int len = 10;
    for (int i = 0; i < len; i++) {
        v.emplace_back(pool.enqueue(tast1, i));
    }

    for (int i = 0; i < len; i++) {
        std::cout << i << " " << v[i].get() << std::endl;
    }

}


