#ifndef HASH_HEAD_H_2021_03_11
#define HASH_HEAD_H_2021_03_11

#include "mm.h" // необходимо использовать свой memorymanager

namespace lab618
{
  /**
  Шаблонный класс CHash.
  В качестве параметров шаблона применяется:
  тип - T;
  функция вычисления хеша - unsigned int HashFunc(const T* pElement);
  функция сравнения - int Compare(const T *pElement, const T* pElement2).

   Класс реализует алгоритм контейнера Хеш-таблица (ассоциативный массив). Данный алгоритм базируется на идее формирования линейной адресации
  произвольных элементов в зависимости от специально вычисляемого целого значения, уникального для каждого элемента (хеш).
   Формирование линейной адресации позволяет получать и сохранять элементы в контейнер за время не зависящее от числа элементов в контейнере — O(1).
   В идеальном случае каждому хешу соответствует ячейка в линейном массиве данных. На самом деле подобрать функцию вычисления хеша, таким образом,
  чтобы соблюдалась уникальность сложно, или получаются числа очень большой размерности. Для преодоления данной проблемы, в Хеш-таблице допускаются
  коллизии — одинаковые значения, уникальность элементов определяется их полями (функциями сравнения).
   Существует два способа разрешения коллизий — разреженная Хеш-таблица и Хеш-таблица со списками. Разреженные Хеш-таблицы при возникновении коллизии
  начинают искать первую свободную ячейку в самой таблице, поиск аналогично происходит от ячейки с номером равным значению хеша, пока не найдется нужный
  элемент. Хеш-таблица со списками, в каждой ячейке Хеш-таблицы хранит начало списка элементов с одинаковыми значениями хеша. Соответственно,
  вставка производится по алгоритму: вычисляем значение хеша для элемента, проверяем есть ли в списке с данным индексом (значение хеш функции)
  данный элемент с помощью функции сравнения, если нет до добавляем его в список. Поиск производится по алгоритму: вычисляем значение хеша для элемента,
  проверяем есть ли в списке с данным индексом искомый элемент с помощью функции сравнения.
   Размер Хеш-таблицы, как правило, меньше размерности функции вычисления хеша, поэтому вычисленное значение хеша ограничивают по размеру таблицы
  с помощью операции вычисления остатка от деления.
   Функция вычисления хеша и операция сравнения задаются как параметры шаблона, что позволяет построить разные Хеш-таблицы для одного набора элементов.
   Основные недостатки Хеш-таблиц:
    1. Сложность подбора хорошей хеш функции (функции дающей наименьшее число коллизий);
    2. Отсутствие упорядоченности элементов;
    3. Вычислительная сложность хеш функции может добавить большую константу в вычислительную сложность алгоритма.

  Данный класс реализует алгоритм Хеш-таблицы со списками
  Данный класс не хранит данные — хранит, только указатели на них.
  Хранение данных производится вне рамок данного класса!
  */
  template <class T, unsigned int(*HashFunc)(const T* pElement), int(*Compare)(const T *pElement, const T* pElement2) >
  class CHash
  {
  private:
    /**
    Лист списка элементов. Обратите внимание, что хранится указатель на данные.
    Данный класс не хранит данные — хранит, только указатели на них.
    Хранение данных производится вне рамок данного класса!
    */
    struct leaf
    {
      T* pData;
      leaf *pnext;
    };

  public:
    /**
    Исключение, которое применяется при нехватке памяти на работу алгоритма
    */
    class CMemoryException
    {
    public:
      CMemoryException(){}
    };

  public:
    /**
    Конструктор с параметрами: размер Хеш-таблицы, и размер блока памяти для менеджера памяти.
    Размер Хеш таблицы реализуем жестко — изменение размера таблицы в зависимости от числа элементов в контейнере не требуется.
    Все создаваемые листики списков разрешения коллизий храним в менеджере памяти.
    */
    CHash(int hashTableSize, int default_block_size): m_tableSize(hashTableSize),
        m_Memory(CMemoryManager<leaf>(default_block_size, false)) {
      m_pTable = new leaf*[m_tableSize];
      for(int i = 0; i < m_tableSize; ++i) {
        m_pTable[i] = 0;
      }
    }
    /**
    Деструктор. Должен освобождать всю выделенную память
    */
    virtual ~CHash() {
      clear();
      m_Memory.clear();
      delete[] m_pTable;
      m_pTable = 0;
    }

    void toString() {
      std::cout << "table size = " << m_tableSize << std::endl;
      std::cout << "m_pTable = " << m_pTable << std::endl;
      std::cout << "table:" << std::endl;
      for(int i = 0; i < m_tableSize; ++i) {
        std::cout << m_pTable[i] << std::endl;
      }
    }

    /**
    Функция добавления элемента в Хеш-таблицу. Возвращает false, если элемент уже есть и true, если элемент добавлен.
    */
    bool add(T* pElement) {
      unsigned int key_hashed;
      leaf* to_next = 0;

      // Проверим, есть ли элемент, заодно посчитаем key
      leaf* had_leaf = findLeaf(pElement, key_hashed);
      if(had_leaf != 0){
        return false;
      }

      if(m_pTable[key_hashed] != 0) {
        to_next = m_pTable[key_hashed];
      }

      m_pTable[key_hashed] = m_Memory.newObject();
      leaf to_put;
      to_put.pnext = to_next; // возможно это 0
      to_put.pData = pElement;
      *m_pTable[key_hashed] = to_put;
      return true;
    }
    /**
    Функция обновления элемента в Хеш-таблице. Обновляет, если элемент уже есть, добавляет, если элемента еще нет.
    Возвращает false, если был добавлен новый элемент, true если элемент обновлен.
    */
    bool update(T* pElement)
    {
      unsigned int key_hashed;
      leaf* find_it = findLeaf(pElement, key_hashed);
      if(find_it == 0) {
        // Тут происходит добавление, потомучто элемент не найден, и код сильно похож на код функции add
        // Да, это копипаста. Я могла бы вызвать add, но тогда придётся 2 раза вызвать find_leaf, а значит
        // 2 раза пройти по цепочке для key

        leaf* to_next = 0;
        if(m_pTable[key_hashed] != 0) {
          to_next = m_pTable[key_hashed];
        }
        m_pTable[key_hashed] = m_Memory.newObject();
        leaf to_put;
        to_put.pData = pElement;
        to_put.pnext = to_next; // возможно это 0
        *m_pTable[key_hashed] = to_put;
        return false;
      }
      find_it->pData = pElement;
      return true;
    }

    /**
    Функция поиска элемента в Хеш-таблице. Возвращает указатель на данные. Если элемента не нашлось, то null.
    Обратите внимание, что для поиска используется частично заполненный объект, т.е. В нем должны быть заполнены поля на основе которых рассчитывается хеш.*/
    T* find(const T& pElement)
    {
      unsigned int key_hashed;
      leaf* find_it = findLeaf(&pElement, key_hashed);
      if(find_it == 0) {
        return 0;
      } else {
        return find_it->pData;
      }
    }

    /**
    Функция удаления элемента из Хеш-таблицы. Возвращает false, если не нашлось элемента, true если элемент был удален.
    */
    bool remove(const T& element)
    {
      unsigned int key_hashed;
      leaf* where_it = findLeaf(&element, key_hashed);
      if(where_it == 0) {
        return false;
      }

      // Нужен предыдущий элемент. Т. к. список односвязный, придётся дойти.
      leaf* cur_leaf = m_pTable[key_hashed];
      leaf* prev = 0;
      while(cur_leaf != where_it) {
        prev = cur_leaf;
        cur_leaf = cur_leaf->pnext;
      }

      if(prev == 0) {
        // Удаляем голову
        m_pTable[key_hashed] = where_it->pnext;
      } else {
        // prev != 0
        prev->pnext = where_it->pnext;
      }

      m_Memory.deleteObject(where_it);
      return true;
    }

    /**
    Удаление всех элементов. Можно вызвать в деструкторе
    */
    void clear()
    {
      leaf* cur_leaf;
      leaf* next_leaf;
      for(int i = 0; i < m_tableSize; ++i){
        if(m_pTable[i] != 0) {
          cur_leaf = m_pTable[i];
          while (cur_leaf != 0) {
            next_leaf = cur_leaf->pnext;
            m_Memory.deleteObject(cur_leaf);
            cur_leaf = next_leaf;
          }
          m_pTable[i] = 0;
        }
      }
    }
  private:
    /**

    Элементарная функция поиска узла в Хеш-таблицу. Возвращает найденный узел и в переменную idx выставляет актуальный индекс хеш-таблицы.
    Данную функцию следует использовать в функциях add, update, find.
    Алгоритм функции:
     1. вычисляем хеш функцию
     2. вычисляем индекс в Хеш-таблице (приводим вычисленное значение хеш функции к размеру массива)
     3. Перебираем список значений и если нашли, то возвращаем его.
     4. Если ничего не нашли, то возвращаем null
    */
    leaf *findLeaf(const T* pElement, unsigned int & idx) {
      //Индекс выставляется всегда

      unsigned int cur_hash = HashFunc(pElement);
      unsigned int key_hashed = cur_hash % m_tableSize;
      idx = key_hashed;
      if (m_pTable[key_hashed] != 0) {
        leaf *cur_leaf = m_pTable[key_hashed];
        while (cur_leaf != 0) {
          if (Compare(cur_leaf->pData, pElement) == 0) {
            return cur_leaf;
          }
          cur_leaf = cur_leaf->pnext;
        }
      } else {
        return 0;
      }
    }

    /**
    Размер Хеш-таблицы
    */
    int m_tableSize;
    /**
    Хеш-таблица
    */
    leaf** m_pTable;
    /**
    Менеджер памяти, предназначен для хранение листов списков разрешения коллизий
    */
    CMemoryManager<leaf> m_Memory;
  };
}; // namespace templates

#endif // #define HASH_HEAD_H_2021_03_11