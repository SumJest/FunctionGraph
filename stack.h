#ifndef stack_H
#define stack_H
#include <iostream>
#include <stdexcept>
#define NMAX 100

template<typename T> class stack
{
	private:
			T a[NMAX];
			int sz;
	public: 	
		stack()
		{
			sz=0;
		}
		
		stack(T *array)
		{
			a=array;
		}
		
		void f_push(T value)
		{
			 if(sz==NMAX){std::cout<<"Error in stack.h(f_push): Stack is overflow";throw std::exception();}
			 a[sz]=value;
			 sz++;
		}
		T f_pop()
		{
			if(sz==0){std::cout<<"Error in stack.h(f_pop): Stack is empty";throw std::exception();}
			sz--;
			return a[sz];
		}
		T f_top()
		{
			if(sz==0){std::cout<<"Error in stack.h(f_top): Stack is empty";throw std::exception();}
			return a[sz-1];
		}
		int getSize()
		{
			return sz;
		}
};

#endif
