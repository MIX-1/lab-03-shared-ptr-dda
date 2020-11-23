// Copyright 2020 MIX-1 <danilonil1@yandex.ru>

#ifndef INCLUDE_SHAREDPTR_HPP_
#define INCLUDE_SHAREDPTR_HPP_

#include <string>
#include <atomic>

using std :: cout;
using std :: atomic;

template <typename T>
struct Counter {
 public:
  explicit Counter(T* p) : ptr(p), counter(1) {}

  void AddCounter() {++counter;}

  void Clean() {
    if(!--counter) {
      ptr = nullptr;
      delete this;
    }
  }

  size_t get() {return counter;}

 private:
  T *ptr;

  atomic<size_t> counter;
  };

  template <typename T>
  class SharedPtr {
   public:
    SharedPtr() {
      ptr = nullptr;
      counter = new Counter<T>(nullptr);
    }

    explicit SharedPtr(T* p) : ptr(p), counter(new Counter<T>(p)) {}

  SharedPtr(const SharedPtr& r) : ptr(r.ptr), counter(r.counter) {counter->AddCounter();}

  SharedPtr(SharedPtr&& r)  noexcept : ptr(std::move(r.ptr)), counter(std::move(r.counter)) {}

  ~SharedPtr() {
    counter->Clean();
  }

  auto operator= (const SharedPtr& r) -> SharedPtr& {
    ptr = r.ptr;
    counter = r.counter;
    return *this;
  }

  auto operator=(SharedPtr&& r) -> SharedPtr& {
    ptr = std::move(r.ptr);
    counter = std::move(r.counter);
    return *this;
  }

  // проверяет, указывает ли указатель на объект
  explicit operator bool() const {
    return (ptr != nullptr);
  }

  auto operator*() const -> T& {return *ptr;}

  auto operator->() const -> T* {return ptr;}

  auto get() -> T* {return ptr;}

  void swap(SharedPtr& r) {
    T *temp_p = std::move(r.ptr);
    Counter<T> *temp_c = std::move(r.counter);
    r.ptr = std::move(ptr);
    r.counter = std::move(counter);
    ptr = std::move(temp_p);
    counter = std::move(temp_c);
  }
  // возвращает количество объектов SharedPtr, которые ссылаются на тот же управляемый объект

  [[nodiscard]] auto use_count() const -> size_t {
    if(ptr != nullptr){
      return counter->get();
    } else {
      return 0;
    }
  }

 private:
  T *ptr;

  Counter<T>* counter;
};


#endif // INCLUDE_SHAREDPTR_HPP_
