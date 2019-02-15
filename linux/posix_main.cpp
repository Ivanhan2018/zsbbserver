#if !defined(_WIN32)
//g++ posix_main.cpp -o posix_main -lboost_system -lboost_filesystem
/*
[hxh@localhost linux]$ ./posix_main
Usage: tut1 path
[hxh@localhost linux]$ ./posix_main posix_main.cpp
posix_main.cpp 545
*/
#include <iostream>  
#include <boost/filesystem.hpp> 
using namespace boost::filesystem; 
 
int main(int argc, char *argv[])  
{  
  if (argc < 2) {  
    std::cout << "Usage: tut1 path\n";  
    return 1;  
  }  
  std::cout << argv[1] << " " << file_size(argv[1]) << std::endl;  
  return 0; 
}

#endif // !defined(_WIN32)
