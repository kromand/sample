/*
 * b_intrusive.hpp
 *
 *  Created on: Jun 29, 2017
 *      Author: kris
 */

#ifndef B_INTRUSIVE_LEVELS_HPP_
#define B_INTRUSIVE_LEVELS_HPP_


#include <boost/intrusive/list.hpp>
#include <boost/intrusive/set.hpp>
#include <vector>
#include <iostream>
#include "MemAllocFixed.hpp"
#include "m_types.hpp"

using namespace boost::intrusive;
using namespace std;

template< template <Side> class K, Side s>
class BookSide
{
private:
	typedef K<s> T;
	typedef boost::intrusive::set< T
	            , member_hook< T, set_member_hook<>, &T::s_member_hook_>
	            > setLev;
	typedef boost::intrusive::list< T
		            , member_hook< T, list_member_hook<>, &T::l_member_hook_>
		            > listLev;
	setLev priceLevels;
	std::string instrumentId;
	mem_alloc<T> * alloc;
	struct delete_disposer
	{
	   void operator()(T *delete_this)
	   {  alloc->deallocate(delete_this);  }
	};

public:
	const std::string DefaultName = "NA";
	BookSide(std::string && instId, size_t maxPxLevels) :instrumentId{std::move(instId)}
	{
		alloc = new mem_alloc<T>(maxPxLevels);
	}
	BookSide(std::string & instId, size_t maxPxLevels) :instrumentId{instId}
	{
		alloc = new mem_alloc<T>(maxPxLevels);
	}
	~BookSide()
	{
		priceLevels.clear();
	}

	bool SetName(std::string nm)
	{
		if(DefaultName != instrumentId) {
			instrumentId = nm;
			return true;
		}
		else {
			return false;
		}

	}
	T * FindPx(const double px)
	{
		auto t = std::lower_bound(priceLevels.begin(),priceLevels.end() , px);
		cout<<"Px: "<<t->price<<endl;
		if(t == priceLevels.end() || t->price != px)
		{

			return nullptr;
		}
		else {
			return &(*t);
		}
	}
	bool DeletePx(const double px)
	{
		auto t = std::lower_bound(priceLevels.begin(),priceLevels.end() , px);
		if(t != priceLevels.end() && t->price == px)
		{
			priceLevels.erase(t);
			alloc->deallocate(&(*t));
			return false;
		}
		else {
			cout<<"Level not found!"<<endl;
			//return &(*t);
			return true;
		}
	}
	T * FindInsertLevel(const double px)
	{
		auto t = std::lower_bound(priceLevels.begin(),priceLevels.end() , px);
		if(t == priceLevels.end() || t->price != px)
		{
			auto nEl = alloc->allocate();
			nEl->price = px;
			priceLevels.insert(*nEl);
			return nEl;
		}

		return &(*t);
	}
	bool InsertNew(double px)
	{
		auto newEl = alloc->allocate();
		auto ret = priceLevels.insert(*newEl);
		if(ret.priceLevels == false)
		{
			alloc->deallocate(newEl);
		}

		return true;
	}
	void ShowBook()
	{
		for(auto & t : priceLevels)
		{
			cout<<t.price<<" ";
		}
		cout<<endl;
	}
};

#endif /* B_INTRUSIVE_LEVELS_HPP_ */
