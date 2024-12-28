#include <cstdio>
#include <functional>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  ThreadPool(size_t num_threads);
  ~ThreadPool();

  void EnqueueJob(std::function<void()> job);

 private:
  size_t num_threads_;

  std::vector<std::thread> worker_threads_;

  std::queue<std::function<void()>> jobs_;

  std::condition_variable cv_job_q_;
  std::mutex m_job_q_;

  bool stop_all;

  void WorkerThread();
};