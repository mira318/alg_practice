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
          int q = *(reinterpret_cast<int*>(pdata + i));
          std::cout << q << " ";
        }
        std::cout << std::endl;
        bool* is_t = new bool[size];
        int not_t = firstFreeIndex;
        memset(is_t, true, size);
        while(not_t != -1) {
          is_t[not_t] = false;
          not_t = *(reinterpret_cast<int*>(pdata + not_t));
        }
        std::cout << std::endl << "is_t:" << std::endl;
        for(int i = 0; i < size; ++i) {
          std::cout << is_t[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "as T:" << std::endl;
        for(int i = 0; i < size; ++i) {
          if(is_t[i]) {
            T q = *(reinterpret_cast<T*>(pdata + i));
            q.toString();
          } else {
            std::cout << "no T" << std::endl;
          }
        }
        std::cout << std::endl;
        delete[] is_t;
        is_t = 0;
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
      m_blkSize(_default_block_size), m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct), m_pBlocks(0),
      m_pCurrentBlk(0){
      if(_default_block_size <= 0) {
        throw("Can't use less than 1 element in block");
      }
    }

    virtual ~CMemoryManager()
    {
      clear();
    }

    // Получить адрес нового элемента из менеджера
    T* newObject()
    {
      if(m_pBlocks == 0) { // менаджер ещё ничего не выделял, создаём новый блок
        m_pCurrentBlk = m_pBlocks = newBlock(); // он совсем пустой и в указателе на следующий будет 0
      }
      // в менаджере что-то есть, значит есть и текущий блок
      if(m_pCurrentBlk->firstFreeIndex == -1) {
        // в текущем блоке кончилось место, ищем первый пустой и делаем его текущим
        bool find = false;
        block* cur = m_pBlocks;
        block* last = 0; // здесь должен оказаться последний выделенный блок, если не найдём свободного места
        while(!find && cur != 0) { //m_pBlocks != 0 знчит хоть раз зайдём и не можем получить last == 0
          if(cur->firstFreeIndex != -1) {
            find = true;
            m_pCurrentBlk = cur;
          } else {
            last = cur;
            cur = cur->pnext;
          }
        }
        if (!find) { // Нет места во всех выделенных блоках, выделяем новый
          m_pCurrentBlk = newBlock();
          last->pnext = m_pCurrentBlk;
        }
      }
      // После всего сделанного m_pCurrent точно содержит указатель на блок, где есть свободное место
      int free_index = m_pCurrentBlk->firstFreeIndex; // точно >= 0
      int next_free = *(reinterpret_cast<int*>(m_pCurrentBlk->pdata + free_index));
      m_pCurrentBlk->usedCount++;
      m_pCurrentBlk->firstFreeIndex = next_free;
      memset(reinterpret_cast<void*>(m_pCurrentBlk->pdata + free_index),0, sizeof(T));
      ::new(reinterpret_cast<void*>(m_pCurrentBlk->pdata + free_index)) T;
      return reinterpret_cast<T*>(m_pCurrentBlk->pdata + free_index);
    }

    // Освободить элемент в менеджере

    bool deleteObject(T* p)
    {
      // перебираем блок, ищем блок с нашим элементом
      block* block_with_it = m_pBlocks;
      int it_index;
      bool find = false;
      while(!find && block_with_it != 0){
        if(p >= block_with_it->pdata && p <= (block_with_it->pdata + m_blkSize - 1)) {
          it_index = p - block_with_it->pdata;
          find = true;
        }
        if(!find){
          block_with_it = block_with_it->pnext;
        }
      }
      if(block_with_it == 0) { // перебрали все блоки и не нашли, значит такого элемента нет
        return false;
      }
      // Теперь в block_with_it лежит указатель на блок с элементом, но он может быть уже удалён - тогда менеджер упадёт
      // удаляем сам элемент
      p->~T();
      memset(reinterpret_cast<void*>(block_with_it->pdata + it_index),0, sizeof(T));
      block_with_it->usedCount--;

      // Переставляем индексы: индекс новой свободной ячейки становится в начало последовательности
      int was_free_index = block_with_it->firstFreeIndex;
      block_with_it->firstFreeIndex = it_index;
      int* place_for_old_index = reinterpret_cast<int*>(block_with_it->pdata + it_index);
      *place_for_old_index = was_free_index;
      return true; // вернули, что удалили успешно
    }

    // Очистка данных, зависит от m_isDeleteElementOnDestruct
    void clear()
    {
      block* cur = m_pBlocks;
      bool* is_t = new bool[m_blkSize];
      while(cur != 0) {
        block* next = cur->pnext;
        deleteBlock(cur, is_t); //вот эта функция зависит от m_isDeleteElementOnDestruct
        cur = next;
      }
      m_pBlocks = 0;
      m_pCurrentBlk = 0;
      delete[] is_t;
      is_t = 0;
    }

    void ToString() {
      std::cout << "m_isDeleteElementsOnDestruct = " << m_isDeleteElementsOnDestruct << std::endl;
      std::cout << "m_blkSize = " << m_blkSize << std::endl;
      std::cout << "m_pBlocks = " << m_pBlocks << std::endl;
      std::cout << "m_pCurrentBlk = " << m_pCurrentBlk << std::endl;
      std::cout << "=======blocks=======" << std::endl;

      block* cur_blk = m_pBlocks;
      while(cur_blk != 0) {
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
      for(int i = 0; i < m_blkSize; ++i){
        int* place_int = reinterpret_cast<int*>(new_block->pdata + i);
        *place_int = i + 1;
      }
      int* last_place = reinterpret_cast<int*>(new_block->pdata + m_blkSize - 1);
      *last_place = -1;
      new_block->firstFreeIndex = 0;
      new_block->usedCount = 0;
      new_block->pnext = 0;
      return new_block;
    }

    // Освободить память блока данных. Применяется в clear
    // Завасит от m_isDeleteElementOnDestruct
    void deleteBlock(block *p, bool* is_t)
    {
      if(!m_isDeleteElementsOnDestruct && p->usedCount != 0) {
        throw("You have to delete all elements!");
      }
      memset(is_t, true, m_blkSize);
      int not_t = p->firstFreeIndex;
      while(not_t != -1) {
        is_t[not_t] = false;
        not_t = *(reinterpret_cast<int*>(p->pdata + not_t));
      }
      for(int i = 0; i < m_blkSize; ++i) {
        if(is_t[i]){
          reinterpret_cast<T*>(p->pdata + i)->~T();
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