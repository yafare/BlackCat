
#ifndef SINGLETON_H
#define SINGLETON_H

#include <stdlib.h> // atexit

#include "Once.h"

template<typename T>
class Singleton
{
public:
	static T& instance()
	{
		pthread_once(&ponce_, &Singleton::init);
		return *value_;
	}

private:
	  Singleton();
	  ~Singleton();

	  static void init()
	  {
		  value_ = new T();
		  ::atexit(destroy);
	  }

	  static void destroy()
	  {
		  delete value_;
	  }

private:
	  static pthread_once_t ponce_;
	  static T*             value_;
};

template<typename T>
pthread_once_t Singleton<T>::ponce_
#ifndef _MSC_VER
= PTHREAD_ONCE_INIT
#endif  // _MSC_VER
;

template<typename T>
T* Singleton<T>::value_ = NULL;

#endif

