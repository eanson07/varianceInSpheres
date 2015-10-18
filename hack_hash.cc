#include <functional>
#include <thread>
#include <iostream>
#include <string>
#include <sstream>

namespace std
{

  template <> 
  size_t hash<thread::id>::operator()(const thread::id val) const
  {
    stringstream ss;
    auto hasher = hash<string>();
    ss << val;
    cerr << val << endl;
    return hasher(ss.str());
    //    return (size_t) stol(ss.str());
  }



}

