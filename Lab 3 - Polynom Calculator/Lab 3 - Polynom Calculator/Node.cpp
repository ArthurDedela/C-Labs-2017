#include "Node.h"
#include <new>



Node::Node(Monom * ptr, Node *pNext)
{
  try {
    monom = new Monom;
  }
  catch (std::bad_alloc) {
    exit(-1);
  }
  monom->set_coef(ptr->get_coef());
  monom->set_pow(ptr->get_pow());
  next = pNext;
}


Node::~Node()
{
}
