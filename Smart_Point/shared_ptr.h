#pragma once

#include <algorithm>
#include <cassert>

#include "cblock.h"

template <class T>
class weak_ptr;

template <class T>
class shared_ptr {
 public:
  shared_ptr() noexcept = default;
  explicit shared_ptr(T *data) : data_(data) {
    if (data_) {
      cblock_ = new cblock();
    }
  }
  shared_ptr(const shared_ptr &sp) noexcept : data_(sp.data_), cblock_(sp.cblock_) {
    if (cblock_) {
      cblock_->shared_count++;
    }
  }
  template <class D>
  shared_ptr(const shared_ptr<D> &sp) noexcept : data_(sp.data_), cblock_(sp.cblock_) {
    if (cblock_) {
      cblock_->shared_count++;
    }
  }
  explicit shared_ptr(const weak_ptr<T> &wp) noexcept : data_(wp.data_), cblock_(cblock_) {
    if (cblock_) {
      ++cblock_->shared_count;
    }
  }
  shared_ptr(shared_ptr &&sp) noexcept {
    swap(sp);
    sp.reset();
  }
  shared_ptr &operator=(shared_ptr &sp) {
    shared_ptr tmp(sp);
    swap(tmp);
    return *this;
  }
  shared_ptr &operator=(shared_ptr &&sp) noexcept {
    swap(sp);
    sp.reset();
    return *this;
  }
  T *get() const noexcept { return data_; }
  T *operator->() const noexcept { return data_; }
  T &operator*() const noexcept { return *data_; }

  explicit operator bool() const noexcept { return data_ != nullptr; }
  ~shared_ptr() { dec_shared_count(); }
  void reset() {
    dec_shared_count();
    data_ = nullptr;
    cblock_ = nullptr;
  }
  void reset(T *data) {
    dec_shared_count();
    data_ = data;
    cblock_ = new cblock();
  }

 private:
  void swap(shared_ptr &sp) noexcept {
    using std::swap;
    swap(data_, sp.data_);
    swap(cblock_, sp.cblock_);
  }
  void dec_shared_count() {
    if (!cblock_) {
      return;
    }
    cblock_->shared_count--;
    if (cblock_->shared_count <= 0) {
      delete data_;
      data_ = nullptr;
      if (cblock_->weak_count <= 0) {
        delete cblock_;
        cblock_ = nullptr;
      }
    }
  }
  T *data_ = nullptr;
  cblock *cblock_ = nullptr;
  template <class Y>
  friend class shared_ptr;
  template <class Y>
  friend class weak_ptr;
};
