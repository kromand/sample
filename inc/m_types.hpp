/*
 * m_types.hpp
 *
 *  Created on: Jul 8, 2017
 *      Author: kris
 */

#ifndef M_TYPES_HPP_
#define M_TYPES_HPP_

#include <boost/intrusive/list.hpp>
#include <boost/intrusive/set.hpp>

using namespace boost::intrusive;

enum class Side {Bid = 0, Offer = 1};
constexpr Side RetOpposite(const Side s)
{
	return (s == Side::Bid ? Side::Offer : Side::Bid);
}

class item {
private:
	int qty;
public:
	set_member_hook<> s_member_hook_;
	bool operator<(const item & it)
	{
		return it.qty < qty;
	}
};


template<Side s>
class level {
private:
	//typedef boost::intrusive::list<item
	//            , member_hook< item, list_member_hook<>, &item::l_member_hook_>
	//            > itList;
	//itList items;
public:

	set_member_hook<> s_member_hook_;
	list_member_hook<> l_member_hook_;

	double price;
	double qty;
	void Init(const double px)
	{
		price = px;
	}

	   friend bool operator< (const level &a, const level &b)
	      {  if(s == Side::Bid) return a.price > b.price;
	      else return a.price < b.price;}
	   friend bool operator== (const level &a, const level &b)
	      {  return a.price == b.price;  }

	   friend bool operator== (const level &a, const double &b)
	      {  return a.price == b;  }
	   friend bool operator< ( const level &a, const double &b)
	      {  if(s == Side::Bid) return a.price > b;
	      else return a.price < b;}

};

#endif /* M_TYPES_HPP_ */
