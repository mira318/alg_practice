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

      ~CIterator()
      {
        m_pBegin = nullptr;
        m_pCurrent = nullptr;
      }

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
        m_pBegin = it_leaf->pnext;
        it.setLeafPreBegin(m_pBegin);
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
      leaf(T& _data, leaf * _pprev, leaf * _pnext)
      {
        data = _data;
        pnext = _pnext;
        pprev = _pprev;
      }
    };
  public:
    class CIterator
    {
    public:
      CIterator()
      {
      }

      CIterator(leaf *p)
      {
      }

      CIterator(const CIterator &src)
      {
      }

      ~CIterator()
      {
      }

      CIterator& operator = (const CIterator&  src)
      {
      }

      bool operator != (const CIterator&  it) const
      {
      }

      void operator++()
      {
      }

      void operator--()
      {
      }

      T& getData()
      {
        T tmp;
        return tmp;
      }

      T& operator* ()
      {
        T tmp;
        return tmp;
      }

      leaf* getLeaf()
      {
        return 0;
      }

      // применяется в erase и eraseAndNext
      void setLeaf(leaf* p)
      {
      }

      // применяется в erase и eraseAndNext
      void setLeafPreBegin(leaf* p)
      {
      }

      // применяется в erase и eraseAndNext
      void setLeafPostEnd(leaf* p)
      {
      }

      bool isValid() {
        return false;
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

    CDualLinkedList()
    {
    };

    virtual ~CDualLinkedList()
    {
    };

    void pushBack(T& data)
    {
    }

    T popBack()
    {
      T tmp;
      return tmp;
    }

    void pushFront(T& data)
    {
    }

    T popFront()
    {
      T tmp;
      return tmp;
    }

    // изменяет состояние итератора. выставляет предыдущую позицию.
    void erase(CIterator& it)
    {
    }

    // изменяет состояние итератора. выставляет следующую позицию.
    void eraseAndNext(CIterator& it)
    {
    }

    int getSize()
    {
      return 0;
    }

    void clear()
    {
    }

    CIterator begin()
    {
      return CIterator();
    }

    CIterator end()
    {
      return CIterator();
    }

  private:
    leaf* m_pBegin, *m_pEnd;
  };
};
#endif //#ifndef TEMPLATES_LIST_2021_02_11