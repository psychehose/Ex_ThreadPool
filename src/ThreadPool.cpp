#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t num_threads)
    : num_threads_(num_threads), stop_all(false) {
  worker_threads_.reserve(num_threads_);

  for (size_t i = 0; i < num_threads; ++i) {
    worker_threads_.emplace_back([this]() { this->WorkerThread(); });
  }
}

ThreadPool::~ThreadPool() {
  stop_all = true;
  cv_job_q_.notify_all();

  for (auto& thread : worker_threads_) {
    thread.join();
  }
}

void ThreadPool::EnqueueJob(std::function<void()> job) {
  if (stop_all) {
    throw ::std::runtime_error("ThreadPool is stopped");
  }
  {
    std::lock_guard<std::mutex> lock(m_job_q_);
    jobs_.push(std::move(job));
  }
  cv_job_q_.notify_one();
}
void ThreadPool::WorkerThread() {
  while (true) {
    std::unique_lock<std::mutex> lock(m_job_q_);
    // wait 함수는 주어진 조건이 false일 때 대기, true일 때 진행
    cv_job_q_.wait(lock, [this]() { return !this->jobs_.empty() || stop_all; });

    if (stop_all && this->jobs_.empty()) {
      return;
    }

    std::function<void()> job = std::move(this->jobs_.front());
    this->jobs_.pop();
    lock.unlock();

    job();
  }
}