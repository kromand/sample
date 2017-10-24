/*
 * mem_allocator.h
 *
 *  Created on: Jul 1, 2017
 *      Author: kris
 */

#ifndef MEM_ALLOCATOR_HPP_
#define MEM_ALLOCATOR_HPP_

#include <boost/intrusive/list.hpp>
#include <boost/intrusive/set.hpp>
#include <vector>
#include <cassert>
#include <memory>
//#include "m_types.hpp"

using namespace std;
using namespace boost::intrusive;


template<typename T>
class mem_alloc {
private:
	typedef boost::intrusive::list< T
	            , member_hook< T, list_member_hook<>, &T::l_member_hook_>
	            > intrusiveList_t;
	intrusiveList_t elements;
	std::unique_ptr<vector<T>> memBlock;
	void link()
	{
		for(auto t = memBlock->begin(); t != memBlock->end(); t++)
		{
			elements.push_back(*t);
		}
	}
public:

	int alloc_count;
	typedef size_t size_type;
	typedef off_t offset_type;
	typedef T* pointer;
	typedef unique_ptr<T> u_pntr_t;
	typedef unique_ptr<T, void (*)(T*)> u_pntr_tt;
	typedef const T* const_pointer;

	mem_alloc(size_t size = 100) :alloc_count{0}
	{
		memBlock = unique_ptr<vector<T>>(new vector<T>(size));
		assert(memBlock->size() == size);
		link();
	}
	~mem_alloc()
	{
		elements.clear();
	}
	void dealloc()
	{

	}
	u_pntr_t allocate(char t)
	{
		auto end = &elements.back();
		elements.pop_back();
		return u_pntr_t(end,dealloc);
	}
	void deallocate(u_pntr_t el)
	{
		elements.push_back(*el);

	}
	pointer allocate(size_type n = 1)
	{
		auto end = &elements.back();
		elements.pop_back();
		return end;
	}
	void deallocate(pointer el)
	{
		elements.push_back(*el);

	}
};

#endif /* MEM_ALLOCATOR_HPP_ */
