/* List interface */

#ifndef LIST_H
#define LIST_H

template< typename VALUE >
class Node {
 public:
   Node() {}
   Node( const VALUE & val ) : value( val ) {}
   bool operator==( const Node & rhs ) { return this->value == rhs.value; }
   bool operator<( const Node & rhs ) { return this->value < rhs.value; }
   bool operator>( const Node & rhs ) { return this->value > rhs.value; }
   VALUE value;
};

template< typename VALUE >
class List {
 public:
   virtual void clear() = 0;
   virtual bool empty() = 0;
   virtual VALUE find( VALUE value ) = 0;
   virtual void insert( VALUE value ) = 0;
   virtual void del( VALUE value ) = 0;
};

#endif
