#ifndef MEMORY_MANAGER_HEAD_H_2021_02_18
#define MEMORY_MANAGER_HEAD_H_2021_02_18

#include <iostream>
#include <cstring>

namespace lab618
{
  template <class T>
  class CMemoryManager
  {
  private:
    struct block
    {
      // Массив данных блока
      T* pdata;
      // Адрес слудующего блока
      block *pnext;
      // Первая свободная ячейка
      int firstFreeIndex;
      // Число заполненных ячеек
      int usedCount;
      void ToString(int size){
        std::cout << "pnext = " << pnext << std::endl;
        std::cout << "firstFreeIndex = " << firstFreeIndex << std::endl;
        std::cout << "usedCount = " << usedCount << std::endl;
        std::cout << "size = " << size << std::endl;
        std::cout << "as int:" << std::endl;
        for(int i = 0; i < size; ++i) {
          int q = *(reinterpret_cast<int*>(pdata + i * sizeof(T)));
          std::cout << q << " ";
        }
        bool is_t[size];
        memset(is_t, true, size);
        int not_t = firstFreeIndex;
        while(not_t != -1) {
          is_t[not_t] = false;
          not_t = *(reinterpret_cast<int*>(pdata + not_t * sizeof(T)));
        }
        std::cout << std::endl;
        std::cout << "as T:" << std::endl;
        for(int i = 0; i < size; ++i) {
          if(is_t[i]) {
            T q = *(reinterpret_cast<T*>(pdata + i * sizeof(T)));
            q.toString();
          }
        }
        std::cout << std::endl;
      }
    };
  public:
    class CException
    {
    public:
      CException(){}
    };

  public:
    CMemoryManager(int _default_block_size, bool isDeleteElementsOnDestruct = false):
      m_blkSize(_default_block_size), m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct), m_pBlocks(nullptr),
      m_pCurrentBlk(nullptr){}

    virtual ~CMemoryManager()
    {
      clear();
    }

    // Получить адрес нового элемента из менеджера
    T* newObject()
    {
      if(m_pBlocks == nullptr) {
        m_pBlocks = newBlock();
        m_pCurrentBlk = m_pBlocks;
      }
      if(m_pCurrentBlk->firstFreeIndex == -1) {
        bool find = false;
        block* cur = m_pBlocks;
        block* last = nullptr;
        while(!find && cur != nullptr){
          if(cur->firstFreeIndex != -1) {
            find = true;
            m_pCurrentBlk = cur;
          } else {
            last = cur;
            cur = cur->pnext;
          }
        }
        if (!find) {
          m_pCurrentBlk = newBlock();
          if(last != nullptr) {
            last->pnext = m_pCurrentBlk;
          }
        }
      }
      int free_index = m_pCurrentBlk->firstFreeIndex;
      int next_free = *(reinterpret_cast<int*>(m_pCurrentBlk->pdata + free_index * sizeof(T)));
      m_pCurrentBlk->usedCount++;
      m_pCurrentBlk->firstFreeIndex = next_free;
      memset(reinterpret_cast<char*>(m_pCurrentBlk->pdata + free_index * sizeof(T)), 0, sizeof(T));
      ::new(reinterpret_cast<char*>(m_pCurrentBlk->pdata + free_index * sizeof(T))) T;
      return reinterpret_cast<T*>(m_pCurrentBlk->pdata + free_index * sizeof(T));
    }

    // Освободить элемент в менеджере

    bool deleteObject(T* p)
    {
      /* перебрать блок и найти по-умному через сравнения*/
      block* block_with_it = m_pBlocks;
      int it_index;
      bool find = false;
      while(!find && block_with_it != nullptr){
        if(p <= reinterpret_cast<T*>(block_with_it->pdata) &&
            p >= reinterpret_cast<T*>(block_with_it->pdata + (m_blkSize - 1) * sizeof(T))) {
          for(int i = 0; (i < m_blkSize && !find); ++i) {
            T* t_iter = reinterpret_cast<T*>(block_with_it->pdata + i * sizeof(T));
            if(t_iter == p){
              it_index = i;
              find = true;
            }
          }
        }

        block_with_it = block_with_it->pnext;
      }
      if(block_with_it == nullptr) {
        return false;
      }
      int last_free_index = block_with_it->firstFreeIndex;
      block_with_it->firstFreeIndex = it_index;
      p->~T();
      memset(reinterpret_cast<char*>(p), 0, sizeof(T));
      block_with_it->pdata[it_index] = last_free_index;
      block_with_it->usedCount--;
      return true;
    }

    // Очистка данных, зависит от m_isDeleteElementOnDestruct
    void clear()
    {
      block* cur = m_pBlocks;
      while(cur != nullptr) {
        block* next = cur->pnext;
        deleteBlock(cur); //вот эта функция зависит от m_isDeleteElementOnDestruct
        cur = next;
      }
    }

    void ToString() {
      std::cout << "m_isDeleteElementsOnDestruct = " << m_isDeleteElementsOnDestruct << std::endl;
      std::cout << "m_blkSize = " << m_blkSize << std::endl;
      std::cout << "m_pBlocks = " << m_pBlocks << std::endl;
      std::cout << "m_pCurrentBlk = " << m_pCurrentBlk << std::endl;
      std::cout << "=======blocks=======" << std::endl;

      block* cur_blk = m_pBlocks;
      while(cur_blk != nullptr) {
        cur_blk->ToString(m_blkSize);
        cur_blk = cur_blk->pnext;
      }
    }
  private:

    // Создать новый блок данных, применяется в newObject
    block* newBlock()
    {
      block* new_block = new block;
      new_block->pdata = reinterpret_cast<T*>(new char[sizeof(T) * m_blkSize]);
      new_block->pnext = nullptr;
      for(int i = 0; i < m_blkSize; ++i){
        int* place_int = reinterpret_cast<int*>(new_block->pdata + i * sizeof(T));
        *place_int = i + 1;
        new_block->ToString();
      }
      int* last_place = reinterpret_cast<int*>(new_block->pdata + (m_blkSize - 1) * sizeof(T));
      *last_place = -1;
      new_block->firstFreeIndex = 0;
      new_block->usedCount = 0;
      return new_block;
    }

    // Освободить память блока данных. Применяется в clear
    // Завасит от m_isDeleteElementOnDestruct
    void deleteBlock(block *p)
    {
      if(!m_isDeleteElementsOnDestruct && p->usedCount != 0) {
        throw CException();
      }
      bool is_t[m_blkSize];
      memset(is_t, true, m_blkSize);
      int not_t = p->firstFreeIndex;
      while(not_t != -1) {
        is_t[not_t] = false;
        not_t = *(reinterpret_cast<int*>(p->pdata + not_t * sizeof(T)));
      }
      for(int i = 0; i < m_blkSize; ++i) {
        if(is_t[i]){
          reinterpret_cast<T*>(p->pdata + i * sizeof(T))->~T();
        }
      }
      delete[] reinterpret_cast<char*>(p->pdata);
      delete p;
    }

    // Размер блока
    int m_blkSize;
    // Начало списка блока
    block* m_pBlocks;
    // Текущий блок
    block* m_pCurrentBlk;
    // Удалять ли элементы при освобождении
    bool m_isDeleteElementsOnDestruct;
  }; // memory manager
}; // namespace lab618

#endif // #define MEMORY_MANAGER_HEAD_H_2021_02_18