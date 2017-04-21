/* List interface */

template< typename VALUE >
class List {
 public:
   virtual void clear() = 0;
   virtual bool empty() = 0;
   virtual size_t size() = 0;
   virtual VALUE find( VALUE value ) = 0;
   virtual void insert( VALUE value ) = 0;
   virtual void del( VALUE value ) = 0;
};
