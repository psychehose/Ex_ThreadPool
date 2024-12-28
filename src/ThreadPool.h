#include <condition_variable>
#include <cstdio>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  ThreadPool(size_t num_threads);
  ~ThreadPool();

  template <class F, class... Args>
  std::future<typename std::invoke_result<F, Args...>::type> EnqueueJob(
      F f, Args... args) {
    if (stop_all) {
      throw std::runtime_error("ThreadPool is stopped");
    }

    using return_type = typename std::invoke_result<F, Args...>::type;
    auto job = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(f, args...));
    std::future<return_type> job_result_future = job->get_future();
    jobs_.push([job]() { (*job)(); });
    cv_job_q_.notify_one();

    return job_result_future;
  }

 private:
  size_t num_threads_;

  std::vector<std::thread> worker_threads_;

  std::queue<std::function<void()>> jobs_;

  std::condition_variable cv_job_q_;
  std::mutex m_job_q_;

  bool stop_all;

  void WorkerThread();
};