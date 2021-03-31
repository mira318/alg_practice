#include <iostream>

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

unsigned int HashFunc(const TestStruct* instance) {
  const int prime_1 = 257;
  unsigned int key_hash = 0;
  int key_len = (instance->key).length();

  for (int i = 0; i < key_len; ++i) {
    key_hash = key_hash * prime_1 + int(instance->key[i]);
  }
  return key_hash;
}

int main() {
  int num_iter = 10000000;
  unsigned int hashes[num_iter];
  TestStruct** checking_instance = new TestStruct*[num_iter];
  for(int i = 0; i < num_iter; ++i) {
    checking_instance[i] = new TestStruct;
    generate(checking_instance[i]);
    hashes[i] = HashFunc(checking_instance[i]);
  }
  std::cout << "hashes" << std::endl;
  for(int i = 0; i < num_iter; ++i) {
    std::cout << hashes[i] << " ";
  }
  std::cout << std::endl;
  for(int i = 0; i < num_iter; ++i) {
    for(int j = 0; j < num_iter; ++j) {
      if(i != j && i < j && hashes[i] == hashes[j]) {
        std::cout << "i = " << i << ", j = " << j << std::endl;
        std::cout << "TestStruct i: " << std::endl;
        checking_instance[i]->toString();
        std::cout << "TestStruct j: " << std::endl;
        checking_instance[j]->toString();
      }
    }
  }
  for(int i = 0; i < num_iter; ++i) {
    delete checking_instance[i];
    checking_instance[i] = nullptr;
  }
  delete[] checking_instance;
  checking_instance = nullptr;
  return 0;
}

