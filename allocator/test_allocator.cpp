#include "mm.h"
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

void TestAllocatorFunction()
{}

int main() {
  TestAllocatorFunction();
  return 0;
}