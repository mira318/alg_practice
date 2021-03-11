#include "sort.h"
#include <cstdint>
#include <stdexcept>
//#include <alg_practice_sort/sort>

namespace templates {
  const long long MAX_QUEUE_SIZE = PTRDIFF_MAX;
  class BinaryHeap {
  public:
    BinaryHeap(int length, CompareSortType pCompareFunc){
      capacity = length;
      comparer = pCompareFunc;
      data_ = new void*[capacity];
    }

    ~BinaryHeap(){
      while (!isEmpty()) {
        pop();
      }
      delete[] data_;
      data_ = nullptr;
    }

    bool isEmpty() const noexcept{
      return (data_ == nullptr || data_[0] == nullptr);
    }

    void push(void* incoming_object){
      data_[size] = incoming_object;
      increaseKey(size);
      size++;
    }

    void pop() noexcept{
      if (isEmpty()) {
        return;
      }
      std::swap(data_[0], data_[size - 1]);
      data_[size - 1] = nullptr;
      size--;
      if (size > 0) {
        heapify(0);
      }
    }

    void*& top(){
      if (isEmpty()) {
        throw std::out_of_range("queue is empty");
      }
      return data_[0];
    }

  private:
    void** data_ = nullptr;
    size_t size = 0;
    std::ptrdiff_t capacity = 0;
    int (*comparer)(const void*, const void*);
    void heapify(int index){
      void *left_son;
      void *right_son;

      if (size > (index * 2) + 1) {
        left_son = data_[(index * 2) + 1];
      } else {
        left_son = nullptr;
      }

      if (size > (index * 2) + 2) {
        right_son = data_[(index * 2) + 2];
      } else {
        right_son = nullptr;
      }

      int largest;
      if (left_son != nullptr && comparer(left_son, data_[index])) {
        largest = (index * 2) + 1;
      } else {
        largest = index;
      }
      if (right_son != nullptr && comparer(right_son, data_[largest])) {
        largest = (index * 2) + 2;
      }

      if (largest != index) {
        std::swap(data_[largest], data_[index]);
        heapify(largest);
      }
    }

    void increaseKey(int index) {
      while (index > 0 && comparer(data_[index],  data_[(index - 1) / 2])) {
        std::swap(data_[index], data_[(index - 1) / 2]);
        index = (index - 1) / 2;
      }
    }
  };

  void heapSort(void **ppArray, int length, CompareSortType pCompareFunc) {
    BinaryHeap sorting_heap = BinaryHeap(length, pCompareFunc);
    for(int i = 0; i < length; ++i) {
      sorting_heap.push(ppArray[i]);
    }
    for(int i = 0; i < length; ++i) {
      ppArray[i] = sorting_heap.top();
      sorting_heap.pop();
    }
  }
}