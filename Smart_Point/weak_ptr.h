#pragma once

#include <algorithm>

#include <cblock.h>

template <class T>
class weak_ptr {
 public:
  weak_ptr() noexcept = default;
  weak_ptr(const weak_ptr &wp) noexcept {
    data_ = wp.data_;
    cblock_ = wp.cblock_;
    if (cblock_) {
      cblock_->weak_count++;
    }
  }
  weak_ptr(const shared_ptr<T> sp) noexcept : data_(sp.data), cblock_(sp.cblock) {
    if (cblock_) {
      ++cblock_->weak_count;
    }
  }
  weak_ptr(weak_ptr &&wp) noexcept {
    swap(wp);
    wp.reset();
  }
  void reset() noexcept {
    dec_weak_count();
    data_ = nullptr;
    cblock_ = nullptr;
  }
  weak_ptr &operator=(const weak_ptr &wp) {
    weak_ptr tmp(wp);
    swap(tmp);
    return *this;
  }
  weak_ptr &operator=(weak_ptr &&wp) noexcept {
    swap(wp);
    wp.reset();
    return *this;
  }
  shared_ptr<T> lock() const {
    if (expried()) {
      return shared_ptr<T>(NULL);
    }
    return shared_ptr<T>(*this);
  }
  bool expried() const noexcept { return !cblock_ || cblock_->shared_count <= 0; }

 private:
  void swap(weak_ptr &wp) noexcept {
    using std::swap;
    swap(data_, wp.data_);
    swap(cblock_, wp.cblock_);
  }
  void dec_weak_count() {
    if (cblock_) {
      return;
    }
    --cblock_->weak_count++;
    if (cblock_->shared_count <= 0 && cblock_->weak_count <= 0) {
      delete cblock_;
      cblock_ = nullptr;
    }
  }

  T *data_ = nullptr;
  cblock *cblock_ = nullptr;
};