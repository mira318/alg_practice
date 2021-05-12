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
  return pElement->key.compare(pElement2->key);
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

void testHashTableUpdateFind(int hashTableSize, int default_block_size, int num_el) {
  TestHashTable MyTable = TestHashTable(hashTableSize, default_block_size);
  TestStruct* pointers[num_el];


  for(int i = 0; i < num_el; ++i) {
    TestStruct* to_cur_el = new TestStruct;
    generate(to_cur_el);
    pointers[i] = to_cur_el;
    assert(MyTable.add(to_cur_el) == true);
  }

  for(int i = 0; i < num_el; ++i) {
    TestStruct* founded = MyTable.find(*pointers[i]);
    assert(founded->value1 == pointers[i]->value1);
    assert(founded->value2 == pointers[i]->value2);
  }

  for(int i = 0; i < num_el; ++i) {
    assert(MyTable.remove(*pointers[i]) == true);
  }

  for(int i = 0; i < num_el; ++i) {
    assert(MyTable.find(*pointers[i]) == 0);
  }

  MyTable.clear();

  for(int i = 0; i < num_el; ++i) {
    delete pointers[i];
  }

  for(int i = 0; i < num_el; ++i) {
    TestStruct* to_cur_el = new TestStruct;
    generate(to_cur_el);
    pointers[i] = to_cur_el;
    assert(MyTable.update(to_cur_el) == false);
  }


  for(int i = 0; i < num_el; ++i) {
    if(i % 7 == 0) {
      pointers[i]->value1 = "updated" + std::to_string(i);
      pointers[i]->value2 = "twice updated" + std::to_string(i);
      assert(MyTable.update(pointers[i]) == true);
    }
  }

  for(int i = num_el - 1; i >= 0; --i) {
    if(i % 7 == 0) {
      TestStruct ts;
      ts.key = pointers[i]->key;
      TestStruct* new_ts = MyTable.find(ts);
      assert(new_ts->value1 == "updated" + std::to_string(i));
      assert(new_ts->value2 == "twice updated" + std::to_string(i));
    } else {
      assert(MyTable.update(pointers[i]) == true);
    }
  }
  MyTable.clear();
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

  testHashTableUpdateFind(1, 1, 10);
  testHashTableUpdateFind(10, 10, 1000);
  testHashTableUpdateFind(999, 17, 701);
  testHashTableUpdateFind(3, 1, 9999);
  return 0;
}

