#include <functional>
#include <thread>
#include <sstream>

namespace std
{

  /*
  template<>
  static  hash<std::thread::id>
  {
   public:
    size_t operator()(const std::thread::id &val) const
    {
      stringstream ss;
      ss << val;
      return (size_t) stol(ss.str());
    }

  };
  */

  template <> 
  size_t hash<thread::id>::operator()(const thread::id val) const
  {
    stringstream ss;
    ss << val;
    return (size_t) stol(ss.str());
  }



}

