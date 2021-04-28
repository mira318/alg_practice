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
      leaf(T& _data, leaf* _pnext): data(_data), pnext(_pnext){}
    };
  public:
    class CIterator
    {
    public:
      CIterator(): m_pCurrent(0), m_pBegin(0){}

      explicit CIterator(leaf *p): m_pCurrent(p), m_pBegin(0){}

      CIterator(const CIterator &src): m_pCurrent(src.m_pCurrent), m_pBegin(src.m_pBegin){}

      ~CIterator()= default;

      CIterator& operator = (const CIterator&  src)
      {
        m_pCurrent = src.m_pCurrent;
        m_pBegin = src.m_pBegin;
        return *this;
      }

      bool operator != (const CIterator&  it) const
      {
        return (it.m_pCurrent != m_pCurrent) || (it.m_pBegin != m_pBegin);
      }

      void operator++()
      {
        if(m_pCurrent == 0) {
          m_pCurrent = m_pBegin;
          m_pBegin = 0;
          return;
        }
        m_pCurrent = m_pCurrent->pnext;
      }

      T& getData()
      {
        if(m_pCurrent == 0){
          throw("Invalid iterator");
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
          throw("Invalid iterator");
        }
        return m_pCurrent;
      }

      void setLeaf(leaf* p)
      {
        m_pCurrent = p;
        m_pBegin = 0;
      }

      void setLeafPreBegin(leaf* p)
      {
        m_pCurrent = 0;
        m_pBegin = p;
      }

      bool isValid() {
        return (m_pCurrent != 0);
      }

    private:
      //храним голову списка, если мы находимся перед началом
      leaf* m_pBegin;
      // храним текущее положение
      leaf* m_pCurrent;
    };

  public:

    CSingleLinkedList(): m_pEnd(0), m_pBegin(0){}

    virtual ~CSingleLinkedList()
    {
      clear();
    }

    void pushBack(T& data)
    {
      leaf* new_leaf = new leaf(data, 0);
      if(m_pEnd == 0) { // Пустой лист
        m_pEnd = m_pBegin = new_leaf;
        return;
      }
      m_pEnd->pnext = new_leaf;
      m_pEnd = new_leaf;
    }

    void pushFront(T& data)
    {
      leaf* new_leaf = new leaf(data, 0);
      if(m_pBegin == 0) {
        m_pEnd = m_pBegin = new_leaf;
        return;
      }
      new_leaf->pnext = m_pBegin;
      m_pBegin = new_leaf;
    }

    T popFront()
    {
      if(m_pBegin == 0) {
        throw("List is empty");
      }
      T tmp = m_pBegin->data;
      leaf* next = m_pBegin->pnext;
      if(m_pBegin == m_pEnd) { // последний элемент списка, next = 0
        m_pEnd = 0;
      }
      delete m_pBegin;
      m_pBegin = next;
      return tmp;
    }

    // изменяет состояние итератора. выставляет предыдущую позицию.
    void erase(CIterator& it)
    {
      leaf* it_leaf = it.getLeaf(); // если итератор не валидный, выбросим исключение из getLeaf()
      if(m_pBegin == 0) {
        throw("Empty list, nothing to erase");
      }
      if(it_leaf == m_pBegin) {
        if(m_pBegin == m_pEnd) {
          it.setLeafPreBegin(0); // выставим невалидный итератор -- в листе ничего не осталось
          m_pBegin = m_pEnd = 0;
        } else {
          // у списка другой конец, значит там хотя бы 2 элемента
          leaf* next = m_pBegin->pnext;
          m_pBegin = next;
          it.setLeafPreBegin(next);
        }
        delete it_leaf;
        it_leaf = 0;
        return;
      }
      leaf* cur_prev = m_pBegin;
      // ищем элемент в списке, хотим сохранить предыдущий, поэтому случай с m_pBegin == it_leaf разобран отдельно
      while(cur_prev != 0 && cur_prev->pnext != it_leaf) { // используем особенность &&
        cur_prev = cur_prev->pnext;
      }
      // не нашли элемент, значит нам дали плохой итератор
      if(cur_prev == 0) {
        throw("Invalid iterator");
      }
      // нашли элемент, значит он лежит в cur_prev->pnext и не нулевой т к итератор валидный
      leaf* next = cur_prev->pnext->pnext;
      cur_prev->pnext = next;
      it.setLeaf(cur_prev); // можем и 0 передать
      delete it_leaf;
      it_leaf = 0;
    }

    int getSize()
    {
      int size = 0;
      leaf* cur = m_pBegin;
      while(cur != 0) {
        cur = cur->pnext;
        size++;
      }
      return size;
    }

    void clear()
    {
      leaf* cur = m_pBegin;
      leaf* next;
      while (cur != 0){
        next = cur->pnext;
        delete cur;
        cur = next;
      }
      m_pBegin = 0;
      m_pEnd = 0;
    }
    CIterator begin()
    {
      return CIterator(m_pBegin);
    }

  private:
    leaf* m_pBegin, *m_pEnd;
  };








  template<class T>
  class CDualLinkedList
  {
  private:
    struct leaf
    {
      T data;
      leaf * pnext, *pprev;
      leaf(T& _data, leaf * _pprev, leaf * _pnext): data(_data), pprev(_pprev), pnext(_pnext){}
    };
  public:
    class CIterator
    {
    public:
      CIterator(): m_pEnd(0), m_pBegin(0), m_pCurrent(0){}

      explicit CIterator(leaf *p):m_pBegin(0), m_pCurrent(p), m_pEnd(0){}

      CIterator(const CIterator &src): m_pBegin(src.m_pBegin), m_pCurrent(src.m_pCurrent), m_pEnd(src.m_pEnd){}

      ~CIterator()= default;

      CIterator& operator = (const CIterator&  src)
      {
        m_pBegin = src.m_pBegin;
        m_pCurrent = src.m_pCurrent;
        m_pEnd = src.m_pEnd;
        return *this;
      }

      bool operator != (const CIterator&  it) const
      {
        return (m_pCurrent != it.m_pCurrent) || (m_pBegin != it.m_pBegin) || (m_pEnd != it.m_pEnd);
      }

      void operator++()
      {
        if(m_pCurrent == 0) { // если это не был случай с пре-головой списка, итератор останется невалидным
          m_pCurrent = m_pBegin;
          m_pBegin = 0;
          return;
        }
        m_pCurrent = m_pCurrent->pnext;
      }

      void operator--()
      {
        if(m_pCurrent == 0) { // если это не был случай с после-концом списка, итератор останется невалидным
          m_pCurrent = m_pEnd;
          m_pEnd = 0;
          return;
        }
        m_pCurrent = m_pCurrent->pprev;
      }

      T& getData()
      {
        if(m_pCurrent == 0) {
          throw("Invalid iterator");
        }
        return m_pCurrent->data;
      }

      T& operator*()
      {
        return getData();
      }

      leaf* getLeaf()
      {
        if(m_pCurrent == 0) {
          throw("Invalid iterator");
        }
        return m_pCurrent;
      }

      // применяется в erase и eraseAndNext
      void setLeaf(leaf* p)
      {
        m_pCurrent = p;
        m_pBegin = 0;
        m_pEnd = 0;
      }

      // применяется в erase и eraseAndNext
      void setLeafPreBegin(leaf* p)
      {
        m_pBegin = p;
        m_pCurrent = 0;
        m_pEnd = 0;
      }

      // применяется в erase и eraseAndNext
      void setLeafPostEnd(leaf* p)
      {
        m_pEnd = p;
        m_pCurrent = 0;
        m_pBegin = 0;
      }

      bool isValid() {
        return (m_pCurrent != 0);
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

    CDualLinkedList(): m_pBegin(0), m_pEnd(0){};

    virtual ~CDualLinkedList()
    {
      clear();
    };

    void pushBack(T& data)
    {
      leaf* new_leaf = new leaf(data, 0, 0);
      if(m_pBegin == 0) { // Лист пустой
        m_pBegin = m_pEnd = new_leaf;
        return;
      }
      m_pEnd->pnext = new_leaf;
      new_leaf->pprev = m_pEnd;
      m_pEnd = new_leaf;
    }

    T popBack()
    {
      T res;
      if(m_pEnd == 0) { // Возможно, только если в листе ничего нет
        throw("Empty list");
      }
      if(m_pEnd == m_pBegin) { // в листе всего один элемент
        res = m_pBegin->data;
        delete m_pBegin;
        m_pBegin = m_pEnd = 0;
        return res;
      }
      res = m_pEnd->data;
      leaf* new_end = m_pEnd->pprev; // начало в другом элементе, значит m_pEnd->prev != 0
      new_end->pnext = 0;
      delete m_pEnd;
      m_pEnd = new_end;
      return res;
    }

    void pushFront(T& data)
    {
      leaf* new_leaf = new leaf(data, 0, 0);
      if(m_pBegin == 0) { // Пустой лист
        m_pEnd = m_pBegin = new_leaf;
        return;
      }
      new_leaf->pnext = m_pBegin;
      m_pBegin->pprev = new_leaf;
      m_pBegin = new_leaf;
    }

    T popFront()
    {
      T res;
      if(m_pBegin == 0) {
        throw("Empty list");
      }
      if(m_pBegin == m_pEnd) { // в листе всего один элемент
        res = m_pBegin->data;
        delete m_pEnd;
        m_pEnd = m_pBegin = 0;
        return res;
      }
      res = m_pBegin->data;
      leaf* new_begin = m_pBegin->pnext; // конец в другом элементе, значит m_pBegin->next != 0
      new_begin->pprev = 0;
      delete m_pBegin;
      m_pBegin = new_begin;
      return res;
    }

    // изменяет состояние итератора. выставляет предыдущую позицию.
    void erase(CIterator& it)
    {
      leaf* it_leaf = it.getLeaf(); // вот тут проверяем валидность
      if(m_pBegin == 0) {
        throw("Empty list");
      }
      if(m_pBegin == it_leaf) {
        if(m_pBegin == m_pEnd) { // в листе может быть один элемент, но он не может быть пустым
          it.setLeafPreBegin(0); // у пустого листа уже не будет головы, чтобы на неё указывать
          delete m_pBegin;
          m_pBegin = m_pEnd = 0;
          return;
        }
        // Нужно удалить голову, точно есть следующий элемент
        leaf* new_begin = m_pBegin->pnext;
        new_begin->pprev = 0;
        it.setLeafPreBegin(new_begin);
        delete m_pBegin;
        m_pBegin = new_begin;
        return;
      }
      if(m_pEnd == it_leaf) { // можем удалять конец, но случай m_pBegin == m_pEnd уже разобран: m_pEnd->pprev != 0
        leaf* new_end = m_pEnd->pprev;
        new_end->pnext = 0;
        it.setLeaf(new_end);
        delete m_pEnd;
        m_pEnd = new_end;
        return;
      }
      // Перед нами самый обычный лист, у него есть и предыдущий и следующий
      leaf* prev = it_leaf->pprev;
      leaf* next = it_leaf->pnext;
      prev->pnext = next;
      next->pprev = prev;
      it.setLeaf(prev);
      delete it_leaf;
      it_leaf = 0;
    }

    // изменяет состояние итератора. выставляет следующую позицию.
    void eraseAndNext(CIterator& it)
    {
     leaf* it_leaf = it.getLeaf(); // вот тут проверили валидность
     if(m_pBegin == 0) {
       throw("Empty list");
     }
     if(it_leaf == m_pEnd) {
       if(m_pEnd == m_pBegin) {
         it.setLeafPostEnd(0); // у нового пустого списка не будет ни конца, ни начала
         delete m_pBegin;
         m_pBegin = m_pEnd = 0;
         return;
       }
       // нужно удалить хвост, точно есть предыдущий элемент
       leaf* new_end = m_pEnd->pprev;
       new_end->pnext = 0;
       it.setLeafPostEnd(new_end);
       delete m_pEnd;
       m_pEnd = new_end;
       return;
     }
     if(it_leaf == m_pBegin) { // можем удалять начало, но случай m_pBegin == m_pEnd уже разобран
       leaf* new_begin = m_pBegin->pnext;
       new_begin->pprev = 0;
       it.setLeaf(new_begin);
       delete m_pBegin;
       m_pBegin = new_begin;
       return;
     }
     // удаляем самый обычный лист, у него есть и следующий и предыдущий
     leaf* prev = it_leaf->pprev;
     leaf* next = it_leaf->pnext;
     prev->pnext = next;
     next->pprev = prev;
     it.setLeaf(next);
     delete it_leaf;
     it_leaf = 0;
    }

    int getSize()
    {
      int sz = 0;
      for(leaf* i = m_pBegin; i != 0; i = i->pnext) {
        sz++;
      }
      return sz;
    }

    void clear()
    {
      for(leaf* i = m_pBegin; i != 0; i = i->pnext) {
        delete i;
      }
      m_pBegin = m_pEnd = 0;
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
    leaf* m_pBegin, *m_pEnd;
  };
};
#endif //#ifndef TEMPLATES_LIST_2021_02_11