#ifndef TEMPLATES_LIST_2021_02_11
#define TEMPLATES_LIST_2021_02_11

namespace lab618
{
  template<class T>
  class CSingleLinkedList
  {
  private:
    struct leaf
    {
      T data;
      leaf* pnext;
      leaf(T& _data, leaf* _pnext):data(_data), pnext(_pnext){}
    };
  public:
    class CIterator
    {
    public:
      CIterator(): m_pCurrent(0), m_pBegin(0){}

      explicit CIterator(leaf *p):m_pBegin(0), m_pCurrent(p){}

      CIterator(const CIterator &src):m_pBegin(src.m_pBegin), m_pCurrent(src.m_pCurrent){}

      ~CIterator(){}

      //здесь должен быть консруктор копирования
      CIterator& operator = (const CIterator&  src)
      {
        m_pBegin = src.m_pBegin;
        m_pCurrent = src.m_pCurrent;
        return *this;
      }

      bool operator != (const CIterator&  it) const
      {
        return this->m_pCurrent != it.m_pCurrent || this->m_pBegin != it.m_pBegin ;
      }

      void operator++()
      {
        if(m_pCurrent == 0) {
          // в m_pBegin может быть nullptr. Тогда как раз встанем в начало списка, а в m_pCurrent положим nullptr.
          // Мы никак не можем узнать, что там должно быть. Если в листе ничего нет, то это правильное решение.
          // Если в листе что-то есть, то итератор невалидный.
          m_pCurrent = m_pBegin;
          m_pBegin = 0;
          return;
        }
        m_pCurrent = m_pCurrent->pnext;
      }

      T& getData()
      {
        if(m_pCurrent == 0) {
          throwError();
        }
        return m_pCurrent->data;
      }

      T& operator* ()
      {
        return getData();
      }

      leaf* getLeaf()
      {
        if(m_pCurrent == 0){
          throwError();
        }
        return m_pCurrent;
      }

      void setLeaf(leaf* p)
      {
        if(m_pBegin != 0){
          m_pBegin = 0;
        }
        m_pCurrent = p;
      }

      void setLeafPreBegin(leaf* p)
      {
        m_pCurrent = 0;
        m_pBegin = p;
      }

      bool isValid() {
        return m_pCurrent != 0;
      }

    private:
      //храним голову списка, если мы находимся перед началом
      leaf* m_pBegin;
      // храним текущее положение
      leaf* m_pCurrent;
    };

  public:

    CSingleLinkedList():m_pBegin(0), m_pEnd(0){}

    virtual ~CSingleLinkedList()
    {
      clear();
    }

    void pushBack(T& data)
    {
      leaf* new_leaf = new leaf(data, 0);
      if(m_pEnd == 0) {
        m_pEnd = new_leaf;
        m_pBegin = m_pEnd;
        return;
      }
      m_pEnd->pnext = new_leaf;
      m_pEnd = new_leaf;
    }

    void pushFront(T& data)
    {
      leaf* new_leaf = new leaf(data, 0);
      new_leaf->pnext = m_pBegin;
      m_pBegin = new_leaf;

      if(m_pEnd == 0) {
        m_pEnd = m_pBegin;
      }
    }

    T popFront()
    {
      if(m_pBegin == 0) {
        throw std::runtime_error("List is empty");
      }
      T tmp;
      tmp = m_pBegin->data;
      leaf* new_begin = m_pBegin->pnext;
      delete m_pBegin;
      m_pBegin = new_begin;
      if (m_pBegin == 0) {
        m_pEnd = 0;
      }
      return tmp;
    }

    // изменяет состояние итератора. выставляет предыдущую позицию.
    void erase(CIterator& it)
    {
      if(!it.isValid()){
        return;
      }
      leaf* it_leaf = it.getLeaf();
      if(m_pBegin == it_leaf)
      {
        if(m_pBegin == m_pEnd) {
          m_pEnd = it_leaf->pnext;
        }
        m_pBegin = it_leaf->pnext;
        it.setLeafPreBegin(m_pBegin);
        delete it_leaf;
        return;
      }

      leaf* prev = 0;
      prev = m_pBegin;
      while(prev != 0 && prev->pnext != it_leaf) {
        prev = prev->pnext;
      }
      it.setLeaf(prev);
      if(prev != 0) {
        prev->pnext = it_leaf->pnext;
      }
      if(!prev->pnext) {//удаляем конец
        m_pEnd = prev;
      }
      it_leaf->pnext = 0;
      delete it_leaf;
    }

    int getSize()
    {
      if(m_pBegin == 0) {
        return 0;
      }
      size_t size = 1;
      leaf* current = m_pBegin;
      while(current != m_pEnd) {
        current = current->pnext;
        size++;
      }
      return size;
    }

    void clear()
    {
      leaf* current = m_pBegin;
      m_pBegin = 0;
      m_pEnd = 0;
      leaf* next_to;
      while(current != 0) {
        next_to = current->pnext;
        delete current;
        current = next_to;
      }
    }

    CIterator begin()
    {
      return CIterator(m_pBegin);
    }

  private:
    leaf* m_pBegin, *m_pEnd;
    static void throwError() {
      throw std::runtime_error("Iterator is not valid!");
    }
  };









  template<class T>
  class CDualLinkedList
  {
  private:
    struct leaf
    {
      T data;
      leaf * pnext, *pprev;
      leaf(T& _data, leaf * _pprev, leaf * _pnext) : pnext(_pnext), pprev(_pprev), data(_data){}
    };
  public:
    class CIterator
    {
    public:
      CIterator() : m_pBegin(0), m_pCurrent(0), m_pEnd(0) {}

      explicit CIterator(leaf *p) : m_pBegin(0), m_pCurrent(p), m_pEnd(0){}

      CIterator(const CIterator &src) : m_pBegin(src.m_pBegin), m_pCurrent(src.m_pCurrent), m_pEnd(src.m_pEnd){}

      ~CIterator() {}

      CIterator& operator = (const CIterator&  src)
      {
        m_pBegin = src.m_pBegin;
        m_pCurrent = src.m_pCurrent;
        m_pEnd = src.m_pEnd;
        return *this;
      }

      bool operator != (const CIterator&  it) const
      {
        return (m_pCurrent != it.m_pCurrent) && (m_pEnd != it.m_pEnd) && (m_pBegin != it.m_pBegin);
      }

      void operator++()
      {
        if(m_pCurrent == 0) {
          // в m_pBegin может быть nullptr. Тогда как раз встанем в начало списка, а в m_pCurrent положим nullptr.
          // Мы никак не можем узнать, что там должно быть. Если в листе ничего нет, то это правильное решение.
          // Если в листе что-то есть, то итератор невалидный.
          m_pCurrent = m_pBegin;
          m_pBegin = 0;
          return;
        }
        m_pCurrent = m_pCurrent->pnext;
      }

      void operator--()
      {
        if (m_pEnd != 0) {
          m_pCurrent = m_pEnd;
          m_pEnd = 0;
        } else {
          if (m_pCurrent != 0) {
            m_pCurrent = m_pCurrent->pprev;
          }
        }
      }

      T& getData()
      {
        if(m_pCurrent == 0) {
          throw std::runtime_error("Iterator is not valid");
        }
        return m_pCurrent->data;
      }

      T& operator* ()
      {
        return getData();
      }

      leaf* getLeaf()
      {
        if(m_pCurrent == 0) {
          throw std::runtime_error("Iterator is not valid");
        }
        return m_pCurrent;
      }

      // применяется в erase и eraseAndNext
      void setLeaf(leaf* p)
      {
        m_pBegin = 0;
        m_pEnd = 0;
        m_pCurrent = p;
      }

      // применяется в erase и eraseAndNext
      void setLeafPreBegin(leaf* p)
      {
        m_pCurrent = 0;
        m_pBegin = p;
        m_pEnd = 0;
      }

      // применяется в erase и eraseAndNext
      void setLeafPostEnd(leaf* p)
      {
        m_pBegin = 0;
        m_pCurrent = 0;
        m_pEnd = p;
      }

      bool isValid() {
        return m_pCurrent != 0;
      }

    private:
      //храним голову списка, если мы находимся перед началом
      leaf* m_pBegin;
      // храним текущее положение
      leaf* m_pCurrent;
      //храним конец списка, если мы находимся после конца
      leaf* m_pEnd;
    };

  public:

    CDualLinkedList() : m_pBegin(0), m_pEnd(0){};

    virtual ~CDualLinkedList()
    {
      clear();
    };

    void pushBack(T& data)
    {
      leaf* new_leaf = new leaf(data, 0, 0);
      if(m_pEnd == 0) {
        m_pEnd = new_leaf;
        m_pBegin = m_pEnd;
        return;
      }
      m_pEnd->pnext = new_leaf;
      new_leaf->pprev = m_pEnd;
      m_pEnd = new_leaf;
    }

    T popBack()
    {
      if (m_pEnd == 0) {
        throw std::runtime_error("List is empty");
      }

      T tmp = tmp->data;
      leaf* newEnd = m_pEnd->pprev;
      delete m_pEnd;
      m_pEnd = newEnd;
      return tmp;
    }

    void pushFront(T& data)
    {
      leaf* new_leaf = new leaf(data, 0, 0);
      if(m_pBegin == 0) {
        m_pBegin = new_leaf;
        m_pEnd = m_pBegin;
        return;
      }
      m_pBegin->pprev = new_leaf;
      new_leaf->pnext = m_pBegin;
      m_pBegin = new_leaf;
    }

    T popFront()
    {
      if (m_pBegin == 0) {
        throw std::runtime_error("List is empty");
      }

      T tmp = m_pBegin->data;
      leaf* newBegin = m_pBegin->pnext;
      if (newBegin != 0) {
        newBegin->pprev = 0;
      }

      delete m_pBegin;
      m_pBegin = newBegin;
      return tmp;
    }

    // изменяет состояние итератора. выставляет предыдущую позицию.
    void erase(CIterator& it)
    {
      if(m_pBegin == 0) {
        throw std::runtime_error("List is empty");
      }

      leaf* it_leaf = it.getLeaf();
      if(it_leaf == 0) {
        throw std::runtime_error("Iterator is not valid");
      }

      if(it_leaf == m_pBegin) {
        leaf* newBegin = m_pBegin->pnext;
        it.setLeafPreBegin(newBegin);
        m_pBegin = newBegin;
        delete it_leaf;
        return;
      }

      if(it_leaf == m_pEnd) {
        leaf* newEnd = m_pEnd->pprev;
        m_pEnd = newEnd;
      }
      it_leaf--;
      delete it_leaf;
      it_leaf = 0;
    }

    // изменяет состояние итератора. выставляет следующую позицию.
    void eraseAndNext(CIterator& it)
    {
      if(m_pBegin == 0) {
        throw std::runtime_error("List is empty");
      }
      leaf* it_leaf = it.getLeaf();
      if(it_leaf == 0) {
        throw std::runtime_error("Iterator is not valid");
      }
      if(it_leaf == m_pEnd) {
        leaf* newEnd = m_pEnd->pprev;
        it.setLeafPostEnd(it_leaf);
        m_pEnd = newEnd;
        delete it_leaf;
        return;
      }
      if(it_leaf == m_pBegin) {
        leaf* newBegin = m_pBegin->pnext;
        m_pBegin = newBegin;
      }
      it_leaf++;
      delete it_leaf;
      it_leaf = 0;
    }

    int getSize()
    {
      size_t size = 0;
      for(leaf* cur = m_pBegin; cur != 0; cur = cur->pnext)
      {
        size++;
      }
      return size;
    }

    void clear()
    {
      leaf* cur = m_pBegin;
      m_pBegin = 0;
      m_pEnd = 0;
      leaf* next;
      while(cur != 0) {
        next = cur->pnext;
        delete cur;
        cur = next;
      }
    }

    CIterator begin()
    {
      return CIterator(m_pBegin);
    }

    CIterator end()
    {
      return CIterator(m_pEnd);
    }

  private:
    leaf *m_pBegin, *m_pEnd;
  };
};
#endif //#ifndef TEMPLATES_LIST_2021_02_11