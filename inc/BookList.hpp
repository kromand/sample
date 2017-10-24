/*
 * BookList.hpp
 *
 *  Created on: Jul 22, 2017
 *      Author: kris
 */

#ifndef BOOKLIST_HPP_
#define BOOKLIST_HPP_

#include <memory>
#include "Book.hpp"
#include "SummaryUpdate.hpp"

constexpr int maxPriceLevels = 100;

class Book {

private:
	typedef std::shared_ptr<BookSide<level, Side::Bid>> BookBidType;
	typedef std::shared_ptr<BookSide<level, Side::Offer>> BookAskType;

	BookBidType  bookBid;
	BookAskType  bookAsk;
	int frontId;
	int backId;
	string pairName;
public:

	MKT_UPDATE sum;
	std::string GetName() const {return pairName;}
	Book(string name) : pairName{name} {
		bookBid =  std::make_shared<BookSide<level, Side::Bid>> (name,maxPriceLevels);
		bookAsk =  std::make_shared<BookSide<level, Side::Offer>> (name,maxPriceLevels);
	}

	Book() {
		bookBid =  std::make_shared<BookSide<level, Side::Bid>> ("NA",maxPriceLevels);
		bookAsk =  std::make_shared<BookSide<level, Side::Offer>> ("NA",maxPriceLevels);
	}
	void SetName(const string & name)
	{
		pairName = name;
	}
	double GetTop(const int legId, const Side s)
	{
		double retQty = 0;
		if(legId == frontId)
		{
			retQty = sum.bestPx[static_cast<int>(s)] * sum.bestVol[static_cast<int>(s)];
		}
		else if(legId == backId)
		{
				retQty = sum.bestVol[static_cast<int>(RetOpposite(s))];
		}
		else {
			//error code
		}
		return retQty;
	}
	double Trade(int & leg, double & qtyInvested)
	{
		double retVal = 0;
		if(leg == frontId)
		{
			qtyInvested = sum.bestPx[static_cast<int>(Side::Offer)] * sum.bestVol[static_cast<int>(Side::Offer)];
			retVal = sum.bestVol[static_cast<int>(Side::Offer)];
			leg = backId;
		}
		else if(leg == backId)
		{
			qtyInvested = sum.bestVol[static_cast<int>(Side::Bid)];
			retVal = sum.bestPx[static_cast<int>(Side::Bid)] * sum.bestVol[static_cast<int>(Side::Bid)];
			leg = frontId;
		}
		else {
			retVal = -1;
		}
		return retVal;
	}
	int GetFront() const
	{
		return frontId;
	}
	int GetBack() const
	{
		return backId;
	}
	void SetLegs(const int f, const int b)
	{
		frontId = f;
		backId = b;
	}

};
class BookList{
private:
	int bookCount = 0;

	std::unordered_map<string,Book *> bookMap;
	vector<Book> bookList;

public:
	BookList()
	{
		bookList = vector<Book>(100,Book(std::string("NA")) );
	}
	Book * AddBook(const string & newBookInstr)
	{
		Book * b = &bookList[bookCount];
		b->SetName(newBookInstr);
		bookMap[newBookInstr] = b;
		bookCount++;
		return b;
	}
};



#endif /* BOOKLIST_HPP_ */
