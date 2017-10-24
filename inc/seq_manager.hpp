/*
 * seq_manager.hpp
 *
 *  Created on: Jul 9, 2017
 *      Author: kris
 */

#ifndef SEQ_MANAGER_HPP_
#define SEQ_MANAGER_HPP_
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/set.hpp>
#include "Book.hpp"

using namespace boost::intrusive;

enum class Action {New, Change, Delete};

class seqItem {

public:
	set_member_hook<> s_member_hook_;
	list_member_hook<> l_member_hook_;
	double qty;
	double price;
	int sequence;
	Side s;
	Action a;
	friend bool operator< (const seqItem &a, const seqItem &b)
	{
		return a.sequence < b.sequence;
	}
};
template<typename T,
template< template <Side> class K, Side s> class BOOK,
template< Side> class LEV>
class Updater
{
private:
	int lastSeq;
	mem_alloc<T> * alloc;
	typedef boost::intrusive::set< T
	            , member_hook< T, set_member_hook<>, &T::s_member_hook_>
	            > SavedItemSetType;
	SavedItemSetType savedSeqSet;
	std::shared_ptr< BOOK<LEV,Side::Offer> >  bookAsk;
	std::shared_ptr<BOOK<LEV,Side::Bid>> bookBid;
	void Upd(const Action a,const Side s, const double px, const double qty )
	{
		switch(a){
		case Action::New:
		case Action::Change:
			Modify(s,px,qty);
		break;
		case Action::Delete:
			Remove(s,px);
		break;
		}
	}
	void Modify(const Side s, const double px, const double qty )
	{
		if(s == Side::Bid )
		{
			LEV<Side::Bid> * l = bookBid->FindInsertLevel(px);
			l->qty = qty;
		}
		else {
			LEV<Side::Offer> * l = bookAsk->FindInsertLevel(px);
			l->qty = qty;
		}
	}
	void Remove(const Side s, const double price)
	{
		cout<<"Remove "<<price<<endl;
		if(s == Side::Bid )
		{
			bookBid->DeletePx(price);
		}
		else {
			bookAsk->DeletePx(price);
		}
	}

public:
	void PrintBooks(const Side s)
	{
		if(s == Side::Bid )
		{
			cout<<" Bids: ";
			bookBid->ShowBook();
		}
		else {
			cout<<" Offers: ";
			bookAsk->ShowBook();
		}
	}
	Updater(size_t maxE,
			int seq,
			std::shared_ptr< BOOK<LEV,Side::Offer> > & a,
			std::shared_ptr<BOOK<LEV,Side::Bid> > & b) : lastSeq{seq}, bookAsk{a}, bookBid{b}
	{
		alloc = new mem_alloc<T>(maxE);
	}
	void Process(const Action a,
			const int seq,
			const Side s,
			const double px,
			const double qty)
	{
		if(lastSeq >= seq){
			printf("Sequence too old to process: %d, current %d \n",seq,lastSeq);
			return;
		}
		if(lastSeq + 1 == seq)
		{
			lastSeq++;
			Upd(a,s,px,qty);
		}
		else {
			auto nEl = alloc->allocate();
			nEl->qty = qty;
			nEl->price = px;
			nEl->sequence = seq;
			nEl->s = s;
			nEl->a = a;
			savedSeqSet.insert(*nEl);
		}
		bool it = false;
		do {
			it = false;
			if(!savedSeqSet.empty())
			{
				auto nEl = savedSeqSet.begin();
				if(lastSeq + 1 == nEl->sequence)
				{
					Upd(nEl->a,nEl->s,nEl->price,nEl->qty);
					alloc->deallocate(&(*nEl));
					savedSeqSet.erase(nEl);
					it = true;
					lastSeq++;
				}
			}
		} while(it);

	}

};

#endif /* SEQ_MANAGER_HPP_ */
