#include <iostream>
#include <assert.h>

#include "list.h"
#include <string>

const int ELEMENTS_COUNT = 1000;

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


static std::string makeRandomString(int minL = 7, int maxL = 14)
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

  return; //пока только односвязный список
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
}

int main()
{
  TestListFunction();
}
