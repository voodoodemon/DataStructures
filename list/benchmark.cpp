/* Benchmark of List implementations */

#include "List.h"
#include "LinkedList.cpp"
#include "SkipList.cpp"
#include <random>
#include <iostream>
#include <chrono>
#include <forward_list>

// from http://stackoverflow.com/questions/2808398/easily-measure-elapsed-time
template<typename TimeT = std::chrono::milliseconds>
struct measure
{
    template<typename F, typename ...Args>
    static typename TimeT::rep execution(F&& func, Args&&... args)
    {
        auto start = std::chrono::steady_clock::now();
        std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
        auto duration = std::chrono::duration_cast< TimeT>
                            (std::chrono::steady_clock::now() - start);
        return duration.count();
    }
};

template< typename LIST, typename VALUE >
void insertValues( LIST & l, std::vector< VALUE > vals ) {
   for ( VALUE val : vals ) {
      l.insert( val );
   }
}

template< typename LIST, typename VALUE >
void deleteValues( LIST & l, std::vector< VALUE > vals ) {
   for ( VALUE val : vals ) {
      l.del( val );
   }
}

template< typename LIST, typename VALUE >
void findValues( LIST & l, std::vector< VALUE > vals ) {
   static VALUE ret;
   for ( VALUE val : vals ) {
      ret = l.find( val );
   }
}

template< typename LIST, typename VALUE >
void testInsert( LIST & l, std::vector< VALUE > vals ) {
   std::cout << measure<>::execution( insertValues< LIST, VALUE >, l, vals )
             << std::endl;
}

template< typename LIST, typename VALUE >
void testDelete( LIST & l, std::vector< VALUE > vals ) {
   std::cout << measure<>::execution( deleteValues< LIST, VALUE >, l, vals )
             << std::endl;
}

template< typename LIST, typename VALUE >
void testFind( LIST & l, std::vector< VALUE > vals ) {
   std::cout << measure<>::execution( findValues< LIST, VALUE >, l, vals )
             << std::endl;
}

template< typename VALUE >
void stdInsertValues( std::forward_list< VALUE > & l, std::vector< VALUE > vals ) {
   for ( VALUE val : vals ) {
      auto itPrev = l.before_begin();
      for ( auto it = l.begin(); it != l.end(); ++it ) {
         if ( *it == val ) {
            break;
         }
         itPrev = it;
      }
      l.insert_after( itPrev, val );
   }
}

template< typename VALUE >
void stdDeleteValues( std::forward_list< VALUE > & l, std::vector< VALUE > vals ) {
   for ( VALUE val : vals ) {
      auto itPrev = l.before_begin();
      auto it = l.begin();
      for ( ; it != l.end(); ++it ) {
         if ( *it == val ) {
            break;
         }
         itPrev = it;
      }
      l.erase_after( itPrev, it );
   }
}

template< typename VALUE >
void stdFindValues( std::forward_list< VALUE > & l, std::vector< VALUE > vals ) {
   static VALUE ret;
   for ( VALUE val : vals ) {
      ret = *std::find( l.begin(), l.end(), val );
   }
}

template< typename LIST, typename VALUE >
void testStdInsert( LIST & l, std::vector< VALUE > vals ) {
   std::cout << measure<>::execution( stdInsertValues< VALUE >, l, vals )
             << std::endl;
}

template< typename LIST, typename VALUE >
void testStdDelete( LIST & l, std::vector< VALUE > vals ) {
   std::cout << measure<>::execution( stdDeleteValues< VALUE >, l, vals )
             << std::endl;
}

template< typename LIST, typename VALUE >
void testStdFind( LIST & l, std::vector< VALUE > vals ) {
   std::cout << measure<>::execution( stdFindValues< VALUE >, l, vals )
             << std::endl;
}

int main() {
   std::forward_list<int> l;
   LinkedList<int> ll;
   SkipList<int> sl;
   std::vector<int> values;
   for ( int i = 0; i < 100000; ++i ) {
      values.push_back( random() );
   }

   // test std::list
   std::cout << "Times for std::list:" << std::endl;
   std::cout << "\tInsert: ";
   testStdInsert( l, values );
   std::cout << "\tFind: ";
   testStdFind( l, values );
   std::cout << "\tDelete: ";
   testStdDelete( l, values );

   // test linked list
   std::cout << "Times for LinkedList:" << std::endl;
   std::cout << "\tInsert: ";
   testInsert( ll, values );
   std::cout << "\tFind: ";
   testFind( ll, values );
   std::cout << "\tDelete: ";
   testDelete( ll, values );

   // test skip list
   std::cout << "Times for SkipList:" << std::endl;
   std::cout << "\tInsert: ";
   testInsert( sl, values );
   std::cout << "\tFind: ";
   testFind( sl, values );
   std::cout << "\tDelete: ";
   testDelete( sl, values );

   return 0;
}
