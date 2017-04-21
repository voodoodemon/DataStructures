/* Basic Skip List */

#include <cassert>
#include <random>
#include <iostream>
#include <vector>

// VALUE must have a default ctor
template< typename VALUE >
class SkipList {

 protected:

   class Node {

    public:

      static Node * makeHead() {
         return new Node( head );
      }
      static Node * makeTail() {
         return new Node( tail );
      }

      Node( const VALUE & val ) : value( val ), rank_( _ ) {}

      bool operator==( const Node & rhs ) {
         return !compare( this, rhs );
      }
      bool operator<( const Node & rhs ) {
         return compare( this, rhs ) < 0;
      }
      bool operator>( const Node & rhs ) {
         return compare( this, rhs ) > 0;
      }

      bool isTail() { return rank_ == tail; }

      std::vector< Node * > next;
      VALUE value;

    private:

      enum NodeRank {
         head = 0,
         _ = 1,
         tail = 2
      };

      Node( const NodeRank & rank ) : rank_( rank ) {}

      // - if left less than right, 0 if equal, + if less greater than right
      static int compare( const Node & l, const Node & r ) {
         return ( l.rank_ == r.rank_ ) ? l.value - r.value : l.rank_ - r.rank;
      }

      NodeRank rank_;
   };

   uint16_t randomLevel() {
      uint16_t lvl = 1;
      while ( random() < p && lvl < maxLevel ) {
         ++lvl;
      }
      return lvl;
   }

   // TODO: these numbers are pulled from pugh's paper, can be tweaked
   static const float p = 0.5;
   static const uint16_t maxLevel = 16;

   uint16_t level;
   Node * head;

 public:
   SkipList() : level( 0 ) {
      head = Node::makeHead();
      Node * tail = Node::makeTail();
      head->next.resize( maxLevel, tail );
   }

   // TODO: initiliazer list constructor

   virtual ~SkipList() {
      clear();
      delete head->next[ 0 ];
      delete head;
   }

   void clear() {
      Node * nxt, * cur;
      cur = head;
      do {
         nxt = cur->next[ 0 ];
         delete cur;
         cur = nxt;
      } while ( !cur->isTail() );
   }

   bool empty() { return head->next[ 0 ]->isTail(); }

   VALUE find( VALUE value ) {
      uint16_t currLevel = level;
      Node * currNode = head;
      do {
         Node * next = currNode->next[ currLevel - 1 ];
         if ( !next->isTail() ) {
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

   void insert( VALUE value ) {
      Node * newNode = new Node( value );
      uint16_t currLevel = randomLevel();
      newNode->next.resize( currLevel, NULL );
      if ( currLevel > level ) {
         level = currLevel;
      }
      Node * currNode = head;
      bool keepTraversing;
      do {
         keepTraversing = false;
         Node * nextNode = currNode->next[ currLevel - 1 ];
         if ( !nextNode->isTail() ) {
            if ( nextNode->value < value ) {
               // keep traversing the list at this level
               currNode = nextNode;
               keepTraversing = true;
            }
         }
         if ( !keepTraversing ) {
            // hit the end of the list, set pointers then drop a level and try again
            newNode->next[ currLevel - 1 ] = nextNode;
            currNode->next[ currLevel - 1 ] = newNode;
            --currLevel;
         }
      } while ( currLevel > 0 );
   }

   void del( VALUE value ) {
      uint16_t currLevel = level;
      Node * currNode = head;
      Node * nextNode;
      bool keepTraversing = false;
      do {
         keepTraversing = false;
         uint16_t index = currLevel - 1;
         Node * nextNode = currNode->next[ index ];
         if ( !nextNode->isTail() ) {
            if ( nextNode->value < value ) {
               // keep traversing the list at this level
               currNode = nextNode;
               keepTraversing = true;
            } else if ( nextNode->value == value ) {
               // found it, fix pointer at this level
               currNode->next[ index ] = nextNode->next[ index ];
               if ( currLevel == 1 ) {
                  // found last reference, delete the node
                  delete nextNode;
               }
               --currLevel;
            }
         } else {
            --currLevel;
         }
      } while ( currLevel > 0 );
   }
};

// breadth test
int main() {
   SkipList<uint32_t> s;
   s.insert( 1 );
   s.insert( 10 );
   s.insert( 100 );
   std::cout << "found: " << s.find( 1 ) << std::endl;
   std::cout << "found: " << s.find( 10 ) << std::endl;
   std::cout << "found: " << s.find( 100 ) << std::endl;
   try {
      uint32_t _ = s.find( 0 );
      assert( false );
   } catch ( ... ) {
      std::cout << "did not find 0" << std::endl;
   }
   s.del( 10 );
   std::cout << "found: " << s.find( 1 ) << " after del" << std::endl;
   try {
      uint32_t _ = s.find( 10 );
      assert( false );
   } catch ( ... ) {
      std::cout << "did not find 10 after del" << std::endl;
   }
   std::cout << "found: " << s.find( 100 ) << " after del" << std::endl;
   return 0;
}
