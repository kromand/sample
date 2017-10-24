/*
 * matrix.cpp
 *
 *  Created on: Aug 2, 2017
 *      Author: kris
 */
#include "matrix.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

matrix::matrix(BookList * b) :maktData{b}
{
	idToName.reserve(START_SIZE);
	nameToId.reserve(START_SIZE);
	matr.reserve(START_SIZE);
	instrCount = 0;
}
int matrix::GetInstrId(string && name)
{
	int id = 0;
	if(nameToId.find(name) == nameToId.end())
	{
		matr.push_back(vector<BOOKID>());
		nameToId[name] = instrCount;
		idToName[instrCount] = name;
		id = instrCount;
		instrCount++;
	}
	else {

		id = nameToId[name];
	}
	return id;
}
Book * matrix::AddPair(const string & curPair, const char separator )
{
	int sep = curPair.find(separator);
	const int frontId = GetInstrId(curPair.substr(0,sep));
	const int backId = GetInstrId(curPair.substr(sep + 1,curPair.length()));
	Book * b = maktData->AddBook(curPair);
	b->SetLegs(frontId,backId);
	matr[frontId].push_back({backId, b});
	matr[backId].push_back({frontId, b});
	return b;

}
void matrix::Print()
{
	int count = 0;
	cout<<matr.size()<<endl;
	for(vector<BOOKID> & t : matr)
	{
		cout<<idToName[count++]<<" : connections: "<<t.size()<<endl;
	}
}
void matrix::DisplayPath(const vector<BOOKID> & t, const int orgNode)
{
	cout<<t.size()<<": "<<idToName[orgNode]<<"->";
	for(const BOOKID & j: t)
	{
		cout<<idToName[j.id]<<"("<<j.id<<")->";
	}
	cout<<endl<<"		";
	for(const BOOKID & j: t)
	{
		//cout<<j.bk->GetName()<<" ";
	}
	//cout<<endl;
}
void matrix::showMatrix()
{
	for(unsigned int t = 0 ; t < matr.size(); t++)
	{
		for(unsigned int x = 0 ; x < matr[t].size(); x++)
		{
			cout<<idToName[t]<<" "<<idToName[matr[t][x].id]<<": "<<matr[t][x].bk->GetName()<<endl;;
		}
	}
}
void matrix::CurNode(int prevNode, int orgNode, vector<BOOKID> & path, int & count)
{
	if(matr[prevNode].size() == 1) return;
	else {
		for(unsigned int t = 0 ; t < matr[prevNode].size(); t++)
		{
			BOOKID & newNode = matr[prevNode][t];
			if(std::find(path.begin(), path.end(),newNode) != path.end())
			{
				continue;
			}
			if(newNode.id == orgNode && path.size() > 1 && path.size() < 4)
			{
				//count++;
				path.push_back(newNode);
				DisplayPath(path,orgNode);
				ExecuteChain(path,orgNode);
				cout<<endl;
				path.pop_back();
			}
			else {
				path.push_back(newNode);
				CurNode(newNode.id, orgNode, path,count);
				path.pop_back();

			}
		}

	}
}
void matrix::ExecuteChain(const vector<BOOKID> & path, const int orgNode)
{
	std:array<double,2> qties, originalQty;
	originalQty[0] = qties[0] = path[0].bk->GetTop(orgNode, Side::Bid);
	originalQty[1] = qties[1] = path[0].bk->GetTop(orgNode, Side::Offer);
	int sd = 1;
	int leg = orgNode;
	auto limNode = path.end();
	for(auto it = path.begin(); it != path.end(); it++)
	{
		cout<<" WQ:"<<originalQty[sd];
		const double qty = originalQty[sd];
		double newQty = it->bk->Trade(leg, originalQty[sd]);
		if(originalQty[sd] < qty)
		{
			cout<<" GQ:("<<originalQty[sd]<<") ";
			limNode = it;
		}
		originalQty[sd] = newQty;
		cout<<" NQ->"<<newQty;
	}
	if(limNode != path.end())
	{
		cout<<" LIMITED"<<endl;
	}
}
void matrix::GenerateArbitrationChains()
{
	vector<BOOKID> path;
	path.reserve(10);

	for(unsigned int t = 0; t < matr.size(); t++)
	{
		int count = 0;
		CurNode(t,t, path,count);
		if(count != 0) cout<<idToName[t]<<": "<<count<<" #cons "<< matr[t].size()<<endl;
		path.clear();
	}
}
