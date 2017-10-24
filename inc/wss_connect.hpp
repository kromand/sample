/*
 * wss_connect.hpp
 *
 *  Created on: Jun 25, 2017
 *      Author: kris
 */

#ifndef WSS_CONNECT_HPP_
#define WSS_CONNECT_HPP_

#include <autobahn/autobahn.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <autobahn/wamp_websocketpp_websocket_transport.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <boost/version.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>

#include "seq_manager.hpp"
#include "Book.hpp"

using namespace std;
using namespace boost::asio;
namespace pt = boost::property_tree;



enum class ConnectionState {Disconnected, InProgress, Connected};

class wss_connection
{
private:
	static constexpr size_t BOOK_SIZE = 1000;
	typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
	typedef autobahn::wamp_websocketpp_websocket_transport<websocketpp::config::asio_tls_client> websocket_transport;
	bool debugModeOn;
	std::unique_ptr<Updater<seqItem,BookSide,level>> updater;
	void connect();
	static void rx_handler(const autobahn::wamp_event& event);
public:
	ConnectionState state;
	wss_connection(bool debug,
			std::shared_ptr<BookSide<level, Side::Bid>> & bidBook,
			std::shared_ptr<BookSide<level, Side::Offer>> & askBook) : debugModeOn{debug}
	{
		updater = std::make_unique<Updater<seqItem,BookSide,level> >(1000,0,askBook, bidBook);

		state = ConnectionState::Disconnected;
	}
	void wss_start();

};



#endif /* WSS_CONNECT_HPP_ */
