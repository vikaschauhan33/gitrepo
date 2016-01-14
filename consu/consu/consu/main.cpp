//
//  main.cpp
//  consu
//
//  Created by vikas chauhan on 1/14/16.
//  Copyright © 2016 vikas chauhan. All rights reserved.
//

//
//  main.cpp
//  publisher
//
//  Created by vikas chauhan on 1/14/16.
//  Copyright © 2016 vikas chauhan. All rights reserved.
//
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstddef>
#include <cstdlib>

class Base {
    public :
    Base():name("Bil"),age(23),sal(1000000){std::cout<< " Constructor " << std::endl;}
    ~Base(){std::cout<< " destructor " << std::endl;}
    void base() { std::cout << " Calling base" ; }
    friend std::ostream& operator<<(std::ostream& os , Base const & op)
    {
        return os << "Item {" << op.name << " " << op.age << " " << op.sal << "}";
    }
    friend std::ostream& operator<<(std::ostream& os , std::shared_ptr<Base> op)
    {
        return os << "Item {" << op->name << " " << op->age << " " << op->sal << "}";
    }
    friend std::ostream& operator<<(std::ostream& os , std::unique_ptr<Base>& op)
    {
        return os << "Item {" << op->name << " " << op->age << " " << op->sal << "}";
    }
private:
    std::string name;
    int         age;
    int         sal;
    
};
int main(int argc, char *argv[])
{
    using namespace boost::interprocess;
    try{
        //A special shared memory where we can
        //construct objects associated with a name.
        //Connect to the already created shared memory segment
        //and initialize needed resources
        
        using std::vector;
        managed_shared_memory segment
        (open_only
         ,"MySharedMemory");  //segment name
        
        //Alias an STL compatible allocator of ints that allocates ints from the managed
        //shared memory segment.  This allocator will allow to place containers
        //in managed shared memory segments
        //typedef allocator<int, managed_shared_memory::segment_manager> ShmemAllocator;
        typedef allocator<Base, managed_shared_memory::segment_manager> ShmemAllocator;
        
        //Alias a vector that uses the previous STL-like allocator
        //typedef vector<int, ShmemAllocator> MyVector;
        typedef vector<Base, ShmemAllocator> MyVector;
        
        //Find the vector using the c-string name
        MyVector *myvector = segment.find<MyVector>("MyVector").first;
        
        //Use vector in reverse order
        //std::sort(myvector->rbegin(), myvector->rend());
        // . . .
        std::cout<< myvector->size() <<std::endl;;

        for(MyVector::iterator it = myvector->begin() ; it != myvector->end() ; ++it){
            std::cout<< *it <<std::endl;;
        }
        //When done, destroy the vector from the segment
       // segment.destroy<MyVector>("MyVector");
    }
    catch(...){
        shared_memory_object::remove("MySharedMemory");
        throw;
    }
   // shared_memory_object::remove("MySharedMemory");
    return 0;
}
/*{
    using namespace boost::interprocess;
    
    shared_memory_object shm (open_only, "MySharedMemory", read_only);
    
    //Map the whole shared memory in this process
    mapped_region region(shm, read_only);
    
    //Check that memory was initialized to 1
    char *mem = static_cast<char*>(region.get_address());
    for(std::size_t i = 0; i < region.get_size(); ++i)
        if(*mem++ == 2){ std::cout << i <<" : " << *mem << std::endl;}
            //return 1;   //Error checking memory
   
}*/
    //Define file names