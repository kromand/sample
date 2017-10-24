/*
 * code_frags.h
 *
 *  Created on: Jun 25, 2017
 *      Author: kris
 */

#ifndef CODE_FRAGS_H_
#define CODE_FRAGS_H_


/*
        boost::asio::io_service io;
        boost::asio::ip::tcp::resolver resolver(io);
        boost::asio::ip::tcp::resolver::query query("api.poloniex.com", "80");
        boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
        boost::asio::ip::tcp::endpoint end_p = iter->endpoint();

        cout<<end_p.address()<<" "<<end_p.port()<<endl;
        */

//using namespace boost::asio;
//using namespace boost::asio::ip;
/*beast websocket library. Handshake upgrade fails
 *
#include <boost/bind.hpp>
//#include <beast/core.hpp>
//#include <beast/websocket.hpp>
#include <iostream>
#include <string>
io_service ioservice;
tcp::socket tcp_socket{ioservice};

void websocket()
{
	std::cout << "Using Boost "
	          << BOOST_VERSION / 100000     << "."  // major version
	          << BOOST_VERSION / 100 % 1000 << "."  // minor version
	          << BOOST_VERSION % 100                // patch level
	          << std::endl;
    // Normal boost::asio setup
    std::string const host = "api.poloniex.com";
    boost::asio::io_service ios;
    boost::asio::ip::tcp::resolver r{ios};
    boost::asio::ip::tcp::socket sock{ios};
    boost::asio::connect(sock,
        r.resolve(boost::asio::ip::tcp::resolver::query{host, "80"}));

    // WebSocket connect and send message using beast
    beast::websocket::stream<boost::asio::ip::tcp::socket&> ws{sock};
    ws.handshake(host, "/");
    cout<<"Sending sub request "<<endl;
    ws.write(boost::asio::buffer(std::string("BTC_BBR")));

    // Receive WebSocket message, print and close using beast
    while(1)
    {
		beast::multi_buffer b;
		beast::websocket::opcode op;
		ws.read(op, b);
		std::cout << beast::buffers(b.data()) << "\n";
    }
    ws.close(beast::websocket::close_code::normal);
}
*/

/*
 * typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef autobahn::wamp_websocketpp_websocket_transport<websocketpp::config::asio_tls_client> websocket_transport;

try {
    //std::cerr << "Connecting to realm: " << parameters->realm() << std::endl;

    boost::asio::io_service io;
    //bool debug = parameters->debug();

    client ws_client;
    ws_client.init_asio(&io);
    ws_client.set_tls_init_handler([&](websocketpp::connection_hdl) {
        return websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12_client);
    });
    auto transport = std::make_shared < autobahn::wamp_websocketpp_websocket_transport<websocketpp::config::asio_tls_client> >(
            ws_client, "wss://api.poloniex.com:443", true);

    // create a WAMP session that talks WAMP-RawSocket over TCP
    auto session = std::make_shared<autobahn::wamp_session>(io, true);

    transport->attach(std::static_pointer_cast<autobahn::wamp_transport_handler>(session));

    // Make sure the continuation futures we use do not run out of scope prematurely.
    // Since we are only using one thread here this can cause the io service to block
    // as a future generated by a continuation will block waiting for its promise to be
    // fulfilled when it goes out of scope. This would prevent the session from receiving
    // responses from the router.
    boost::future<void> connect_future;
    boost::future<void> start_future;
    boost::future<void> join_future;
    boost::future<void> subscribe_future;
    connect_future = transport->connect().then([&](boost::future<void> connected) {
        try {
            connected.get();
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            io.stop();
            return;
        }

        std::cerr << "transport connected" << std::endl;

        start_future = session->start().then([&](boost::future<void> started) {
            try {
                started.get();
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                io.stop();
                return;
            }

            std::cerr << "session started" << std::endl;

            join_future = session->join("realm1").then([&](boost::future<uint64_t> joined) {
                try {
                    std::cerr << "joined realm: " << joined.get() << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << e.what() << std::endl;
                    io.stop();
                    return;
                }

                subscribe_future = session->subscribe("BTC_ETH", &on_topic1).then([&] (boost::future<autobahn::wamp_subscription> subscribed)
                {
                    try {
                        std::cerr << "subscribed to topic: " << subscribed.get().id() << std::endl;
                    }
                    catch (const std::exception& e) {
                        std::cerr << e.what() << std::endl;
                        io.stop();
                        return;
                    }

                });
            });
        });
    });

    std::cerr << "starting io service" << std::endl;
    io.run();
    std::cerr << "stopped io service" << std::endl;
}
catch (std::exception& e) {
    std::cerr << "exception: " << e.what() << std::endl;
    ret_var.successfully_ran = false;
    return ret_var;
}
		stringstream ss;
			//
		//	ss << event.argument<std::string>(i);
		try{
			boost::property_tree::ptree pt;
			boost::property_tree::read_json(ss, pt);
			for(boost::property_tree::ptree::const_iterator it = pt.begin(); it != pt.end(); it++ )
			{
				cout<<it->first<<" "<<it->second.get_value<std::string>();
			}
			cout<< std::endl;
		}
		catch (const std::exception& e)
		{
			std::cerr <<"JSON parser exception: "<< e.what() << std::endl;
		}


*/
#endif /* CODE_FRAGS_H_ */