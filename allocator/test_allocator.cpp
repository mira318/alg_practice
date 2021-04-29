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

void TestAllocatorNewFunction(int block_size, int num_el)
{
  TestManager manager = TestManager (block_size, true);
  for(int i = 0; i < num_el; ++i) {
    TestStruct ts;
    generate(&ts);
    TestStruct* iterator = manager.newObject();
    *iterator = ts;
  }
  manager.clear();
}

void TestAllocatorDeleteFunction(int block_size, int num_el) {
  TestManager good_manager = TestManager(block_size, true);
  TestManager bad_manager = TestManager(block_size, false);
  TestStruct* pointers[num_el];
  for(int i = 0; i < num_el; ++i) {
    TestStruct ts;
    generate(&ts);
    TestStruct* iterator = good_manager.newObject();
    *iterator = ts;
    pointers[i] = iterator;
  }

  for(int i = 0; i < num_el; ++i) {
    assert(good_manager.deleteObject(pointers[i]) == true);
  }
  good_manager.clear();

  for(int i = 0; i < num_el; ++i) {
    TestStruct ts;
    generate(&ts);
    TestStruct* iterator = bad_manager.newObject();
    *iterator = ts;
    pointers[i] = iterator;
  }

  for(int i = 0; i < num_el; ++i) {
    bad_manager.deleteObject(pointers[i]);
  }
  bad_manager.clear();

  for(int i = 0; i < num_el; ++i) {
    TestStruct ts;
    generate(&ts);
    TestStruct* iterator = good_manager.newObject();
    *iterator = ts;
    pointers[i] = iterator;
  }

  for(int i = 0; i < num_el; ++i) {
    if(i % 3 == 0){
      good_manager.deleteObject(pointers[i]);
    }
  }
  good_manager.clear();

  for(int i = 0; i < num_el; ++i) {
    TestStruct ts;
    generate(&ts);
    TestStruct* iterator = bad_manager.newObject();
    *iterator = ts;
    pointers[i] = iterator;
  }
  for(int i = 0; i < num_el; ++i) {
    if(i % 5 == 0){
      bad_manager.deleteObject(pointers[i]);
    }
  }
  for(int i = 0; i < num_el; ++i) {
    if(i % 5 == 0) {
      assert(bad_manager.deleteObject(pointers[i]) == false);
    } else {
      assert(bad_manager.deleteObject(pointers[i]) == true);
    }
  }
  bad_manager.clear();


  for(int i = 0; i < num_el; ++i) {
    TestStruct ts;
    generate(&ts);
    TestStruct* iterator = good_manager.newObject();
    *iterator = ts;
    pointers[i] = iterator;
  }

  for(int i = num_el - 1; i >= 0; --i) {
    assert(good_manager.deleteObject(pointers[i]) == true);
  }
  good_manager.clear();

  for(int i = 0; i < num_el; ++i) {
    TestStruct ts;
    generate(&ts);
    TestStruct* iterator = bad_manager.newObject();
    *iterator = ts;
    pointers[i] = iterator;
  }

  for(int i = num_el - 1; i >= 0; --i) {
    assert(bad_manager.deleteObject(pointers[i]) == true);
  }
  bad_manager.clear();

}

void TestAllocatorRandomDeleteFunction(int block_size, int num_el, int num_deletions){
  TestManager good_manager = TestManager(block_size, true);
  TestManager bad_manager = TestManager(block_size, false);
  TestStruct* pointers[num_el];
  for(int i = 0; i < num_el; ++i) {
    TestStruct ts;
    generate(&ts);
    TestStruct* iterator = good_manager.newObject();
    *iterator = ts;
    pointers[i] = iterator;
  }
  for(int i = 0; i < num_deletions; ++i) {
    good_manager.deleteObject(pointers[rand() % num_el]);
  }
  good_manager.clear();

  for(int i = 0; i < num_el; ++i) {
    TestStruct ts;
    generate(&ts);
    TestStruct* iterator = bad_manager.newObject();
    *iterator = ts;
    pointers[i] = iterator;
  }
  for(int i = 0; i < num_deletions; ++i) {
    bad_manager.deleteObject(pointers[rand() % num_el]);
  }
  for(int i = 0; i < num_el; ++i) {
    bad_manager.deleteObject(pointers[i]);
  }
  bad_manager.clear();
}

int main() {
  TestAllocatorNewFunction(1, 10);
  TestAllocatorNewFunction(2, 55);
  TestAllocatorNewFunction(10, 99);
  TestAllocatorNewFunction(100, 1000);
  TestAllocatorNewFunction(10, 0);

  TestAllocatorDeleteFunction(1, 10);
  TestAllocatorDeleteFunction(2, 55);
  TestAllocatorDeleteFunction(10, 99);
  TestAllocatorDeleteFunction(100, 1000);
  TestAllocatorDeleteFunction(10, 0);

  TestAllocatorRandomDeleteFunction(10, 1000, 200);
  TestAllocatorRandomDeleteFunction(13, 1000, 1000);
  return 0;
}