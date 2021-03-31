#ifndef MEMORY_MANAGER_HEAD_H_2021_02_18
#define MEMORY_MANAGER_HEAD_H_2021_02_18

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
    };
  public:
    class CException
    {
    public:
      CException()
      {
      }
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
        //неть надо проверить, нет ли память спереди --> сделать 2 отдельных ifa
        m_pCurrentBlk = newBlock();
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
      int next_free = *(reinterpret_cast<int*>(m_pCurrentBlk + free_index * sizeof(T)));
      m_pCurrentBlk->usedCount++;
      m_pCurrentBlk->firstFreeIndex = next_free;
      T* t_place = reinterpret_cast<T*>(m_pCurrentBlk + free_index + sizeof (T)); //mistake should use new
      return t_place;
    }

    // Освободить элемент в менеджере

    bool deleteObject(T* p)
    {
      block* block_with_it = m_pBlocks;
      int it_index;
      bool find = false;
      while(!find && block_with_it != nullptr){
        for(int i = 0; (i < m_blkSize && !find); ++i) {
          T* cur_t = block_with_it;
          if(block_with_it->pdata[i] == p){
            it_index = i;
            find = true;
          }
        }
      }
      if(block_with_it == nullptr) {
        return false;
      }
      int last_free_index = block_with_it->firstFreeIndex;
      block_with_it->firstFreeIndex = it_index;
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
  private:

    // Создать новый блок данных, применяется в newObject
    block* newBlock()
    {
      block* new_block = new block;
      new_block->pdata = new char[sizeof(T) * m_blkSize];
      new_block->pnext = nullptr;
      for(int i = 0; i < m_blkSize; ++i){
        int* place = reinterpret_cast<int*>(new_block->pdata + i * sizeof(T));
        *place = i + 1;
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
      delete[] p->pdata;
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
  };
}; // namespace lab618

#endif // #define MEMORY_MANAGER_HEAD_H_2021_02_18