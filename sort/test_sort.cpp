#include <iostream>
#include "sort.h"
#include "sort.cpp"
//#include <alg_practice_sort/sort>
struct TestStruct
{
  std::string key;
  std::string value1;
  std::string value2;
};

bool comparer(TestStruct* A, TestStruct* B) {
  if(A->key < B->key) {
    return 1;
  }
  if(A->key > B->key) {
    return -1;
  }
  return 0;
}

const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghigklmnopqrstuvwxyzАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя";
const int alphabet_length = sizeof(alphabet);

static std::string makeRandomString(int minL = 7, int maxL = 14)
{
  int length = rand() % maxL + minL;
  std::string s;
  s.reserve(length);
  for(int j = 0; j < length; ++j) {
    s += alphabet[rand() % alphabet_length];
  }
  return s;
}

static void generate(TestStruct *pts)
{
  pts->key = makeRandomString();
  pts->value1 = makeRandomString();
  pts->value2 = makeRandomString();
}

int main() {
  int number = 42;
  TestStruct** p_whole = new TestStruct*[number];
  for(int i = 0; i < number; ++i) {
    p_whole[i] = new TestStruct();
    if(p_whole[i] == nullptr) {
      throw std::exception();
    }
    generate(p_whole[i]);
  }
  std::cout << "unsorted:" << std::endl;
  for(int i = 0; i < number; ++i) {
    std::cout << p_whole[i]->key << " " << p_whole[i]->value1 << " " << p_whole[i]->value2 << std::endl;
  }
  templates::heapSort(reinterpret_cast<void **>(p_whole), number,
                      reinterpret_cast<int (*)(const void *, const void *)>(comparer));
  std::cout << "sorted:" << std::endl;
  for(int i = 0; i < number; ++i) {
    std::cout << p_whole[i]->key << " " << p_whole[i]->value1 << " " << p_whole[i]->value2 << std::endl;
  }
  for(int i = 0; i < number; ++i) {
    delete p_whole[i];
    p_whole[i] = nullptr;
  }
  delete[] p_whole;
  return 0;
}
