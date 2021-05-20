#include "hash.h"
#include <iostream>
#include <cassert>

struct TestStruct
{
  std::string key;
  std::string value1;
  std::string value2;
  void toString() const{
    std::cout << key << " " << value1 << " " << value2 << std::endl;
  }
};

struct TestStruct2
{
  std::string key;
  std::string data;
  std::string value1;
  std::string value2;
  void toString() const{
    std::cout << key << " " << data << " " << value1 << " " << value2 << std::endl;
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

static std::string makeLessRandomString(int minL = 2, int maxL = 4)
{
  int length = rand() % maxL + minL;
  std::string s;
  s.reserve(length);
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

static void generate2(TestStruct2 *pts)
{
  pts->key = makeRandomString();
  pts->data = makeLessRandomString();
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

unsigned int HashFunc2(const TestStruct2* instance) {
  const int prime_key = 701;
  const int prime_data = 509;
  unsigned int key_hash = 0;
  int key_len = (instance->key).length();
  for (int i = 0; i < key_len; ++i) {
    key_hash = key_hash * prime_key + int(instance->key[i]);
  }
  unsigned int data_hash = 0;
  int data_len = (instance->data).length();
  for (int i = 0; i < data_len; ++i) {
    data_hash = data_hash * prime_data + int(instance->data[i]);
  }
  return key_hash + data_hash;
}

int Compare2(const TestStruct2* pElement, const TestStruct2* pElement2) {
  // Старая функция сравнения возвращала 0, если элементы равны, и моя реализация несколько подлажена под этот факт,
  // поэтому новая функция сравнения тоже будет возвращать 0, если элементы равны.
  if(pElement->key == pElement2->key) {
    return pElement->data.compare(pElement2->data);
  }
  return -1;
}

int Compare(const TestStruct* pElement, const TestStruct* pElement2) {
  return pElement->key.compare(pElement2->key);
}

typedef lab618::CHash<TestStruct, &HashFunc, &Compare> TestHashTable;

typedef lab618::CHash<TestStruct2, &HashFunc2, &Compare2> TestHashTable2;

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










void ExamTestHash(int hashTableSize, int default_block_size, int num_el) {
  TestHashTable2 MyExamTable = TestHashTable2(hashTableSize, default_block_size);
  TestStruct2 *pointers[num_el];


  for (int i = 0; i < num_el; ++i) {
    TestStruct2 *to_cur_el = new TestStruct2;
    generate2(to_cur_el);
    pointers[i] = to_cur_el;
    assert(MyExamTable.add(to_cur_el) == true);
  }

  for (int i = 0; i < num_el; ++i) {
    TestStruct2 *founded = MyExamTable.find(*pointers[i]);
    assert(founded->value1 == pointers[i]->value1);
    assert(founded->value2 == pointers[i]->value2);
    assert(founded->key == pointers[i]->key);
    assert(founded->data == pointers[i]->data);
  }

  for (int i = 0; i < num_el; ++i) {
    assert(MyExamTable.remove(*pointers[i]) == true);
  }

  for (int i = 0; i < num_el; ++i) {
    assert(MyExamTable.find(*pointers[i]) == 0);
  }

  MyExamTable.clear();

  for (int i = 0; i < num_el; ++i) {
    delete pointers[i];
  }

  for (int i = 0; i < num_el; ++i) {
    TestStruct2 *to_cur_el = new TestStruct2;
    generate2(to_cur_el);
    pointers[i] = to_cur_el;
    assert(MyExamTable.update(to_cur_el) == false);
  }


  for (int i = 0; i < num_el; ++i) {
    if (i % 7 == 0) {
      pointers[i]->value1 = "updated" + std::to_string(i);
      pointers[i]->value2 = "twice updated" + std::to_string(i);
      assert(MyExamTable.update(pointers[i]) == true);
    }
  }

  for (int i = num_el - 1; i >= 0; --i) {
    if (i % 7 == 0) {
      TestStruct2 ts;
      ts.key = pointers[i]->key;
      ts.data = pointers[i]->data;
      TestStruct2 *new_ts = MyExamTable.find(ts);
      assert(new_ts->value1 == "updated" + std::to_string(i));
      assert(new_ts->value2 == "twice updated" + std::to_string(i));
    } else {
      assert(MyExamTable.update(pointers[i]) == true);
    }
  }
  MyExamTable.clear();
  for (int i = 0; i < num_el; ++i) {
    delete pointers[i];
  }
}


int main(){
  /*testHashTableAddDelete(1, 1, 10);
  testHashTableAddDelete(10, 10, 100);
  testHashTableAddDelete(10, 10, 1000);
  testHashTableAddDelete(52, 7, 394);
  testHashTableAddDelete(666, 37, 10001);
  testHashTableAddDelete(1, 1, 9999);


  testHashTableUpdateFind(1, 1, 10);
  testHashTableUpdateFind(10, 10, 1000);
  testHashTableUpdateFind(999, 17, 701);
  testHashTableUpdateFind(1, 1, 9999);*/

  ExamTestHash(15, 5, 17);
  ExamTestHash(100, 5, 200);
  ExamTestHash(10, 1, 1000);
  ExamTestHash(2000, 60, 394);
  return 0;
}

