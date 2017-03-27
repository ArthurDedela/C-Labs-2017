#pragma once
#include "Monom.h"

class Node
{
  Monom* monom;
  Node* next;

public:
  Node(Monom *pMonom, Node* pNext = nullptr);
  
  Monom* get_monom() { return monom; };
  Node* get_next() { return next; };
  
  void set_monom(Monom* mon) { monom = mon; };
  void set_next(Node* nxt) { next = nxt; };

  ~Node();
};

