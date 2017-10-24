/*
 * json_parser.hpp
 *
 *  Created on: Aug 2, 2017
 *      Author: kris
 */

#ifndef JSON_PARSER_HPP_
#define JSON_PARSER_HPP_

#include "matrix.hpp"

    void ParseJson(std::stringstream & ss, matrix & m)
    {
		try{
			//id: 188 last: 0.885 lowestAsk: 0.904401 highestBid: 0.885 percentChange: -0.00628879 baseVolume: 451.637 quoteVolume: 490.593 isFrozen: 0 high24hr: 0.977467 low24hr: 0.88
			boost::property_tree::ptree pt;
			boost::property_tree::read_json(ss, pt);
	        BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt)
	        {
	        	Book * b = m.AddPair(v.first,'_');
	        	auto p = v.second.begin();
	        	b->sum.id = p->second.get_value<int>();
		        p++;
		        b->sum.lastTrade = p->second.get_value<double>();
		        p++;
		        b->sum.bestPx[static_cast<int>(Side::Bid)] = p->second.get_value<double>();
		        p++;
		        b->sum.bestPx[static_cast<int>(Side::Offer)] = p->second.get_value<double>();
		        p++;
		        b->sum.percentChange = p->second.get_value<double>();
		        p++;
		        b->sum.bestVol[static_cast<int>(Side::Bid)] = p->second.get_value<double>();
		        p++;
		        b->sum.bestVol[static_cast<int>(Side::Offer)] = p->second.get_value<double>();
		        p++;
		        b->sum.isFrozen = p->second.get_value<bool>();
		        p++;
		        b->sum.dayHigh = p->second.get_value<double>();
		        p++;
		        b->sum.dayLow = p->second.get_value<double>();
		        p++;
	    //        std::cout << std::endl;
	            // etc
	        }
			cout<< std::endl;
			m.GenerateArbitrationChains();
		//	m.showMatrix();
		}
		catch (const std::exception& e)
		{
			std::cerr <<"JSON parser exception: "<< e.what() << std::endl;
		}
    }



#endif /* JSON_PARSER_HPP_ */
