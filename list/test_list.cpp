#include <iostream>
#include <assert.h>

//#include "list.h"
#include <list.h>
#include <string>

const int ELEMENTS_COUNT = 4;

struct TestStruct
{
  std::string key;
  std::string value1;
  std::string value2;
  void toString(){
    std::cout << key << " " << value1 << " " << value2;
  }
};

typedef lab618::CSingleLinkedList<TestStruct> TestSingleList;
typedef lab618::CDualLinkedList<TestStruct> TestList;

const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghigklmnopqrstuvwxyzАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя";
const int alphabet_length = sizeof(alphabet);


static std::string makeRandomString(int minL = 1, int maxL = 100)
{
  int length = rand() % maxL + minL;
  std::string s;
  s.reserve(length);
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

void TestListFunction()
{
  // тест односвязного списка

  TestSingleList single_list;
  for (int i = 0; i < ELEMENTS_COUNT; ++i)
  {
    TestStruct ts;
    generate(&ts);
    single_list.pushBack(ts);
  }

  assert(single_list.getSize() == ELEMENTS_COUNT);

  for (TestSingleList::CIterator it = single_list.begin(); it.isValid(); ++it)
  {
    it.getLeaf();
    TestStruct ts = *it;
    single_list.erase(it);
  }

  assert(single_list.getSize() == 0);



  for (int i = 0; i < ELEMENTS_COUNT; ++i)
  {
    TestStruct ts;
    generate(&ts);
    single_list.pushFront(ts);
  }
  assert(single_list.getSize() == ELEMENTS_COUNT);
  for (int i = 0; i < ELEMENTS_COUNT; ++i)
  {
    single_list.popFront();
  }
  assert(single_list.getSize() == 0);




  /// тест двусвязного списка

  TestList list;
  for (int i = 0; i < ELEMENTS_COUNT; ++i)
  {
    TestStruct ts;
    generate(&ts);
    list.pushBack(ts);
  }

  assert(list.getSize() == ELEMENTS_COUNT);

  int t = 0;
  for (TestList::CIterator it = list.begin(); it.isValid(); ++it)
  {
    t++;
    it.getLeaf();
    TestStruct ts = *it;
    list.erase(it);
    assert(list.getSize() == ELEMENTS_COUNT - t);
  }

  assert(list.getSize() == 0);

  for (int i = 0; i < ELEMENTS_COUNT; ++i)
  {
    TestStruct ts;
    generate(&ts);
    list.pushFront(ts);
  }
  assert(list.getSize() == ELEMENTS_COUNT);
  for (int i = 0; i < ELEMENTS_COUNT; ++i)
  {
    list.popFront();
  }
  assert(list.getSize() == 0);


  for (int i = 0; i < ELEMENTS_COUNT; ++i)
  {
    TestStruct ts;
    generate(&ts);
    list.pushFront(ts);
  }
  assert(list.getSize() == ELEMENTS_COUNT);
  for (int i = 0; i < ELEMENTS_COUNT; ++i)
  {
    list.popBack();
    assert(list.getSize() == ELEMENTS_COUNT - i - 1);
  }
  assert(list.getSize() == 0);

  for (int i = 0; i < ELEMENTS_COUNT; ++i)
  {
    TestStruct ts;
    generate(&ts);
    list.pushBack(ts);
  }
  assert(list.getSize() == ELEMENTS_COUNT);

  for (TestList::CIterator i = list.begin(); i.isValid(); ) // сам переставит итератор
  {
    list.eraseAndNext(i);
  }
  assert(list.getSize() == 0);

  for(int i = 0; i < ELEMENTS_COUNT; ++i)
  {
    TestStruct ts;
    generate(&ts);
    list.pushBack(ts);
    assert(list.getSize() == i + 1);
  }

  t = 0;
  TestList::CIterator it = list.begin();
  while(it.isValid())
  {
    t++;
    if(t % 3 == 0) {
      it.getLeaf();
      TestStruct ts = *it;
      list.erase(it);
      assert(list.getSize() == ELEMENTS_COUNT - (t / 3));
    }
    ++it;
  }
}

int main()
{
  TestListFunction();
  return 0;
}
