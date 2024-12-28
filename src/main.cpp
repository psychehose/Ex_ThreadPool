#include <chrono>
#include <iostream>
#include <thread>

#include "ThreadPool.h"

void work(int t, int id) {
  printf("%d start \n", id);
  std::this_thread::sleep_for(std::chrono::seconds(t));
  printf("%d end after %ds\n", id, t);
}

int main() {
  ThreadPool pool(3);

  for (int i = 0; i < 10; ++i) {
    pool.EnqueueJob([i]() { work(i % 3 + 1, i); });
  }
  return 0;
}