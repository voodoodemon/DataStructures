/* Basic Skip List */

#include <cassert>
#include <iostream>
#include <vector>
#include "List.h"

// VALUE must have a default ctor
template< typename VALUE >
class SkipList : public List< VALUE > {

 protected:

   static uint16_t randomLevel() {
      uint16_t lvl = 1;
      while ( rand() / static_cast< double >( RAND_MAX ) < p && lvl < maxLevel ) {
         ++lvl;
      }
      return lvl;
   }

   // TODO: these numbers are pulled from pugh's paper, can be tweaked
   static const float p = 0.5;
   static const uint16_t maxLevel = 16;

   class _Node : public Node< VALUE > {
    public:
      _Node() : Node< VALUE >() {}
      _Node( const VALUE & val ) : Node< VALUE >( val ) {
         uint16_t newLevel = randomLevel();
         next.resize( newLevel, NULL );
      }
      std::vector< _Node * > next;
   };

   uint16_t level_;
   _Node * head;

 public:
   SkipList() : level_( 0 ) {
      head = new _Node();
      head->next.resize( maxLevel, NULL );
   }

   // TODO: initiliazer list constructor

   virtual ~SkipList() {
      clear();
      delete head->next[ 0 ];
      delete head;
   }

   virtual void clear() {
      _Node * nxt, * cur;
      cur = head->next[ 0 ];
      while ( cur ) {
         nxt = cur->next[ 0 ];
         delete cur;
         cur = nxt;
      }
      head->next[ 0 ] = cur;
   }

   virtual bool empty() { return head->next[ 0 ]; }

   virtual VALUE find( VALUE value ) {
      uint16_t currLevel = level_;
      _Node * currNode = head;
      do {
         _Node * next = currNode->next[ currLevel - 1 ];
         if ( next ) {
            if ( next->value < value ) {
               // keep traversing the list at this level
               currNode = next;
            } else if ( next->value == value ) {
               // found it!
               return next->value;
            } else {
               --currLevel;
            }
         } else {
            --currLevel;
         }
         // hit the end of the list, either drop a level and try again or exit
      } while ( currLevel > 0 );
      throw "key not in list";
   }

   virtual void insert( VALUE value ) {
      _Node * newNode = new _Node( value );
      uint16_t newLevel = newNode->next.size();
      if ( newLevel > level_ ) {
         level_ = newLevel ;
      }
      uint16_t currLevel = level_;
      _Node * currNode = head;
      do {
         _Node * nextNode = currNode->next[ currLevel - 1 ];
         if ( nextNode && nextNode->value < value ) {
            // keep traversing the list at this level
            currNode = nextNode;
         } else {
            // hit the end of the list, set pointers then drop a level and try again
            if ( currLevel <= newLevel ) {
               newNode->next[ currLevel - 1 ] = nextNode;
               currNode->next[ currLevel - 1 ] = newNode;
            }
            --currLevel;
         }
      } while ( currLevel > 0 );
   }

   virtual void del( VALUE value ) {
      uint16_t currLevel = level_;
      _Node * currNode = head;
      _Node * nextNode;
      bool keepTraversing = false;
      do {
         keepTraversing = false;
         _Node * nextNode = currNode->next[ currLevel - 1 ];
         if ( nextNode ) {
            if ( nextNode->value < value ) {
               // keep traversing the list at this level
               currNode = nextNode;
               continue;
            }
            if ( nextNode->value == value ) {
               // found it, fix pointer at this level
               currNode->next[ currLevel - 1 ] = nextNode->next[ currLevel - 1 ];
               if ( currLevel == 1 ) {
                  // found last reference, delete the node
                  delete nextNode;
                  return;
               }
            }
         }
         --currLevel;
      } while ( currLevel > 0 );
   }
};

// breadth test
// int main() {
//    SkipList<uint32_t> list;
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
//    list.clear();
// 
//    return 0;
// }
