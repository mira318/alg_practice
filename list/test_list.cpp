#include <cassert>
#include <iostream>

#include "list.h"
#include <string>
#include <list.h>

struct TestStruct
{
  std::string key;
  std::string value1;
  std::string value2;
};

typedef lab618::CSingleLinkedList<TestStruct> TestSingleList;
typedef lab618::CDualLinkedList<TestStruct> TestList;

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
  s += "tmp";
  return s;
}

static void generate(TestStruct *pts)
{
  pts->key = makeRandomString();
  pts->value1 = makeRandomString();
  pts->value2 = makeRandomString();
}

void TestSingleListFunction(const int ELEMENTS_COUNT) {
  // тест односвязного списка

  TestSingleList single_list;
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    TestStruct ts;
    generate(&ts);
    single_list.pushBack(ts);
  }
  assert(single_list.getSize() == ELEMENTS_COUNT);

  for (TestSingleList::CIterator it = single_list.begin(); it.isValid(); ++it) {
    it.getLeaf();
    single_list.erase(it);
  }
  assert(single_list.getSize() == 0);

  for (int i = 0; i < ELEMENTS_COUNT; ++i){
    TestStruct ts;
    generate(&ts);
    single_list.pushFront(ts);
  }
  assert(single_list.getSize() == ELEMENTS_COUNT);
  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    single_list.popFront();
  }
  assert(single_list.getSize() == 0);

  for(int i = 0; i < ELEMENTS_COUNT; ++i) {
    TestStruct ts;
    generate(&ts);
    if(i % 2 == 0) {
      single_list.pushFront(ts);
    } else {
      single_list.pushBack(ts);
    }
    assert(single_list.getSize() == i + 1);
  }

  int t = 0;
  TestSingleList::CIterator iter = single_list.begin();
  while(t < single_list.getSize()) {
    if(t % 3 == 0){
      for(int i = 0; i < t; ++i){
        ++iter;
      }
      single_list.erase(iter);
      iter = single_list.begin();
      assert(single_list.getSize() == ELEMENTS_COUNT - t / 3 - 1);
    }
    t++;
  }
  size_t last_size = single_list.getSize();

  TestSingleList::CIterator start = single_list.begin();
  while(last_size > 0) {
    for(int i = 0; i < last_size - 1; ++i) {
      ++start;
    }
    single_list.erase(start);
    last_size--;
    start = single_list.begin();
    assert(single_list.getSize() == last_size);
  }
  assert(single_list.getSize() == 0);
}



void TestDualListFunction(const int ELEMENTS_COUNT) {
  /// тест двусвязного списка
  TestList list;
  for (int i = 0; i < ELEMENTS_COUNT; ++i)
  {
    TestStruct ts;
    generate(&ts);
    list.pushBack(ts);
  }
  assert(list.getSize() == ELEMENTS_COUNT);

  for (TestList::CIterator it = list.begin(); it.isValid(); ++it)
  {
    it.getLeaf();
    TestStruct ts = *it;
    list.erase(it);
  }
  assert(list.getSize() == 0);

  for (int i = 0; i < ELEMENTS_COUNT; ++i){
    TestStruct ts;
    generate(&ts);
    list.pushFront(ts);
  }

  assert(list.getSize() == ELEMENTS_COUNT);

  for (int i = 0; i < ELEMENTS_COUNT; ++i) {
    list.popFront();
  }
  assert(list.getSize() == 0);

  for(int i = 0; i < ELEMENTS_COUNT; ++i) {
    TestStruct ts;
    generate(&ts);
    if(i % 2 == 0) {
      list.pushFront(ts);
    } else {
      list.pushBack(ts);
    }
    assert(list.getSize() == i + 1);
  }

  int t = 0;
  TestList::CIterator iter = list.begin();
  while(t < list.getSize()) {
    if(t % 3 == 0){
      for(int i = 0; i < t; ++i){
        ++iter;
      }
      list.erase(iter);
      iter = list.begin();
      assert(list.getSize() == ELEMENTS_COUNT - t / 3 - 1);
    }
    t++;
  }
  size_t last_size = list.getSize();

  TestList::CIterator start = list.begin();
  while(last_size > 0) {
    for(int i = 0; i < last_size - 1; ++i) {
      ++start;
    }
    list.erase(start);
    last_size--;
    start = list.begin();
    assert(list.getSize() == last_size);
  }
  assert(list.getSize() == 0);

  for(int i = 0; i < ELEMENTS_COUNT; ++i) {
    TestStruct ts;
    generate(&ts);
    if(i % 2 == 0) {
      list.pushFront(ts);
    } else {
      list.pushBack(ts);
    }
    assert(list.getSize() == i + 1);
  }
  for(int i = 0; i < ELEMENTS_COUNT; ++i) {
    list.popBack();
    assert(list.getSize() == ELEMENTS_COUNT - i - 1);
  }

  for(int i = 0; i < ELEMENTS_COUNT; ++i) {
    TestStruct ts;
    generate(&ts);
    if(i % 2 == 0) {
      list.pushBack(ts);
    } else {
      list.pushFront(ts);
    }
    assert(list.getSize() == i + 1);
  }
  start = list.begin();
  t = ELEMENTS_COUNT;
  while(t > 0){
    list.eraseAndNext(start);
    assert(list.getSize() == t - 1);
    t--;
  }

  for (int i = 0; i < ELEMENTS_COUNT; ++i){
    TestStruct ts;
    generate(&ts);
    list.pushBack(ts);
  }
  for (TestList::CIterator it = list.end(); it.isValid(); --it)
  {
    TestStruct ts = *it;
    list.eraseAndNext(it);
  }
  assert(list.getSize() == 0);
}

int main()
{
  TestSingleListFunction(1000);
  TestDualListFunction(1000);

  TestSingleListFunction(10001);
  TestDualListFunction(10001);
  return 0;
}