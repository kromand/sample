/*
 * matrix.hpp
 *
 *  Created on: Aug 2, 2017
 *      Author: kris
 */

#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <vector>
#include <unordered_map>
#include <string>
#include "BookList.hpp"

using namespace std;
typedef struct _bookid
{
	int id;
	Book * bk;
	friend bool operator==(const _bookid & a,const  _bookid & b)
		{
		return a.id == b.id;
		}
}BOOKID, *P_BOOKID;
class matrix{
private:
	const int START_SIZE = 100;
	vector<vector<BOOKID>> matr;
	std::vector<std::string> names;
	std::unordered_map<int, string> idToName;
	std::unordered_map<string, int> nameToId;
	int instrCount = 0;
	BookList * maktData;
public:
	matrix(BookList * b);
	Book * AddPair(const string & curPair, const char separator);
	void ExecuteChain(const vector<BOOKID> & path, const int orgNode);
	int GetInstrId(string && name);
	void Print();
	void CurNode(int prevNode, int orgNode, vector<BOOKID> & path, int & count);
	void DisplayPath(const vector<BOOKID> & t,const int orgNode);
	void GenerateArbitrationChains();
	void showMatrix();

};



#endif /* MATRIX_HPP_ */
