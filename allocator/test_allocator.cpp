//#include "mm.h"
#include <mm.h>
#include <iostream>
#include <assert.h>

#include <string>

const int ELEMENTS_COUNT = 1000;

struct TestStruct
{
  std::string key;
  std::string value1;
  std::string value2;
  void toString(){
    std::cout << key << " " << value1 << " " << value2 << std::endl;
  }
};

typedef lab618::CMemoryManager<TestStruct> TestManager;

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

void TestAllocatorFunction()
{
  int block_size = 42;
  int num_el = 47;
  TestManager manager(block_size, true);
  for(int i = 0; i < num_el; ++i) {
    TestStruct ts;
    generate(&ts);
    TestStruct* iterator = manager.newObject();
    *iterator = ts;
  }
  manager.ToString();
}

int main() {
  TestAllocatorFunction();
  return 0;
}