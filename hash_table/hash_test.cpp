#include "hash.h"
#include <iostream>
#include <assert.h>

struct TestStruct
{
  std::string key;
  std::string value1;
  std::string value2;
  void toString() const{
    std::cout << key << " " << value1 << " " << value2 << std::endl;
  }
};

const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghigklmnopqrstuvwxyzАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя";
const int alphabet_length = sizeof(alphabet);

static std::string makeRandomString(int minL = 7, int maxL = 20)
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

unsigned int HashFunc(const TestStruct* instance) {
  const int prime_1 = 257;
  unsigned int key_hash = 0;
  int key_len = (instance->key).length();

  for (int i = 0; i < key_len; ++i) {
    key_hash = key_hash * prime_1 + int(instance->key[i]);
  }
  return key_hash;
}

int Compare(const TestStruct* pElement, const TestStruct* pElement2) {
  if(pElement->value1 == pElement2->value1) {
    if(pElement->value2 == pElement2->value2) {
      return pElement->key.compare(pElement2->key);
    } else {
      return -1;
    }
  } else {
    return -1;
  }
}

typedef lab618::CHash<TestStruct, &HashFunc, &Compare> TestHashTable;

void testHashTableAddDelete(int hashTableSize, int default_block_size, int num_el) {
  TestHashTable MyTable = TestHashTable(hashTableSize, default_block_size);
  TestStruct* pointers[num_el];
  for(int i = 0; i < num_el; ++i) {
    TestStruct* to_cur_el = new TestStruct;
    generate(to_cur_el);
    pointers[i] = to_cur_el;
    assert(MyTable.add(pointers[i]) == true);
  }
  MyTable.clear();
  for(int i = 0; i < num_el; ++i) {
    delete pointers[i];
  }

  for(int i = 0; i < num_el; ++i) {
    TestStruct* to_cur_el = new TestStruct;
    generate(to_cur_el);
    pointers[i] = to_cur_el;
    assert(MyTable.add(to_cur_el) == true);
  }

  for(int i = 0; i < num_el; ++i) {
    assert(MyTable.add(pointers[i]) == false);
  }

  for(int i = 0; i < num_el; ++i) {
    assert(MyTable.remove(*pointers[i]) == true);
  }

  for(int i = 0; i < num_el; ++i) {
    assert(MyTable.remove(*pointers[i]) == false);
  }

  MyTable.clear();

  for(int i = 0; i < num_el; ++i) {
    delete pointers[i];
  }

  for(int i = 0; i < num_el; ++i) {
    TestStruct* to_cur_el = new TestStruct;
    generate(to_cur_el);
    pointers[i] = to_cur_el;
    assert(MyTable.add(to_cur_el) == true);
  }

  for(int i = num_el - 1; i >= 0; --i) {
    assert(MyTable.add(pointers[i]) == false);
  }


  for(int i = 0; i < num_el; ++i) {
    if(i % 5 == 0) {
      assert(MyTable.remove(*pointers[i]) == true);
    }
  }

  for(int i = num_el - 1; i >= 0; --i) {
    if(i % 5 == 0) {
      assert(MyTable.remove(*pointers[i]) == false);
    } else {
      assert(MyTable.remove(*pointers[i]) == true);
    }
  }

  for(int i = 0; i < num_el; ++i) {
    delete pointers[i];
  }
}

int main(){
  testHashTableAddDelete(1, 1, 10);
  testHashTableAddDelete(10, 10, 100);
  testHashTableAddDelete(10, 10, 1000);
  testHashTableAddDelete(52, 7, 394);
  testHashTableAddDelete(666, 37, 10001);
  return 0;
}

