/* Basic LinkedList */

#include <cassert>
#include <random>
#include <iostream>
// #include "List.cpp"

template< typename VALUE >
class LinkedList : public List< VALUE > {

 protected:

   class Node {

    public:

      Node( const VALUE & val ) : value( val ) {}

      Node() : next( NULL ) {}

      bool operator==( const Node & rhs ) {
         return !( this->value - rhs.value );
      }
      bool operator<( const Node & rhs ) {
         return ( this->value - rhs.value ) < 0;
      }
      bool operator>( const Node & rhs ) {
         return ( this->value - rhs ) > 0;
      }
      VALUE value;
      Node * next;

    private:

      // - if left less than right, 0 if equal, + if less greater than right
      static int compare( const Node & l, const Node & r ) {
         return l.value - r.value;
      }
   };

   size_t size_;
   Node * head;

 public:
   LinkedList() : size_( 0 ) {
      head = new Node();
      head->next = NULL;
   }

   // TODO: initiliazer list constructor

   virtual ~LinkedList() {
      clear();
      delete head;
   }

   virtual void clear() {
      Node * nxt, * cur;
      cur = head->next;
      while ( cur ) {
         nxt = cur->next;
         delete cur;
         cur = nxt;
      }
      head->next = NULL;
   }

   virtual bool empty() { return head->next == NULL; }

   virtual size_t size() { return size_; }

   virtual VALUE find( VALUE value ) {
      Node * currNode = head->next;
      while ( currNode ) {
         if ( currNode->value == value ) {
            return currNode->value;
         }
         currNode = currNode->next;
      }
      throw "key not in list";
   }

   virtual void insert( VALUE value ) {
      Node * newNode = new Node( value );
      ++size_;
      Node * currNode = head;
      while ( currNode->next && currNode->next->value < value ) {
         currNode = currNode->next;
      }
      newNode->next = currNode->next;
      currNode->next = newNode;
   }

   virtual void del( VALUE value ) {
      Node * currNode = head;
      Node * nextNode = currNode->next;
      while ( nextNode ) {
         if ( nextNode->value < value ) {
            currNode = nextNode;
            nextNode = currNode->next;
         } else if ( nextNode->value == value ) {
            currNode->next = nextNode->next;
            delete nextNode;
            --size_;
            return;
         }
      }
   }
};

// breadth test
// int main() {
//    LinkedList<uint32_t> list;
// 
//    // ensure list is created correctly
//    assert( list.empty() );
// 
//    // add some elements and ensure they're all found
//    list.insert( 1 );
//    list.insert( 5 );
//    list.insert( 10 );
//    list.insert( 50 );
//    list.insert( 100 );
//    std::cout << "found: " << list.find( 1 ) << std::endl;
//    std::cout << "found: " << list.find( 5 ) << std::endl;
//    std::cout << "found: " << list.find( 10 ) << std::endl;
//    std::cout << "found: " << list.find( 50 ) << std::endl;
//    std::cout << "found: " << list.find( 100 ) << std::endl;
// 
//    // sanity check to make sure no elements are found which aren't in the list
//    try {
//       uint32_t _ = list.find( 0 );
//       assert( false );
//    } catch ( ... ) {
//       std::cout << "did not find 0" << std::endl;
//    }
//    try {
//       uint32_t _ = list.find( 51 );
//       assert( false );
//    } catch ( ... ) {
//       std::cout << "did not find 51" << std::endl;
//    }
//    try {
//       uint32_t _ = list.find( 101 );
//       assert( false );
//    } catch ( ... ) {
//       std::cout << "did not find 101" << std::endl;
//    }
// 
//    // delete a middle element, ensure it's gone and the others remain
//    list.del( 10 );
//    std::cout << "found: " << list.find( 1 ) << " after del" << std::endl;
//    std::cout << "found: " << list.find( 5 ) << " after del" << std::endl;
//    std::cout << "found: " << list.find( 50 ) << " after del" << std::endl;
//    std::cout << "found: " << list.find( 100 ) << " after del" << std::endl;
//    try {
//       uint32_t _ = list.find( 10 );
//       assert( false );
//    } catch ( ... ) {
//       std::cout << "did not find 10 after del" << std::endl;
//    }
// 
//    // delete the first element, ensure it's gone and the others remain
//    list.del( 1 );
//    std::cout << "found: " << list.find( 5 ) << " after del" << std::endl;
//    std::cout << "found: " << list.find( 50 ) << " after del" << std::endl;
//    std::cout << "found: " << list.find( 100 ) << " after del" << std::endl;
//    try {
//       uint32_t _ = list.find( 1 );
//       assert( false );
//    } catch ( ... ) {
//       std::cout << "did not find 1 after del" << std::endl;
//    }
// 
//    // delete the last element, ensure it's gone and the others remain
//    list.del( 100 );
//    std::cout << "found: " << list.find( 5 ) << " after del" << std::endl;
//    std::cout << "found: " << list.find( 50 ) << " after del" << std::endl;
//    try {
//       uint32_t _ = list.find( 100 );
//       assert( false );
//    } catch ( ... ) {
//       std::cout << "did not find 100 after del" << std::endl;
//    }
// 
//    // clear the list ensure no elemenst remain
//    assert( list.size() == 2 );
//    list.clear();
// 
//    return 0;
// }
