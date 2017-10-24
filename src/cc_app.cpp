//============================================================================
// Name        : cc_app.cpp
// Author      : kd
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "wss_connect.hpp"
#include "MemAllocFixed.hpp"
#include "BookList.hpp"
#include "restbed"
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/set.hpp>
#include "https_client.hpp"
#include "matrix.hpp"

using namespace restbed;

void CompTest()
{
	std::shared_ptr<BookSide<level, Side::Bid>> bidBook =
			std::make_shared<BookSide<level, Side::Bid>> ("BTC_BBR",1000);

	std::shared_ptr<BookSide<level, Side::Offer>> askBook =
			std::make_shared<BookSide<level, Side::Offer>> ("BTC_BBR",1000);

	std::unique_ptr<Updater<seqItem,BookSide,level>> updater =
			std::make_unique<Updater<seqItem,BookSide,level> >(1000,0,askBook, bidBook);

	updater->Process(Action::Change,1,Side::Bid, .45436, 34.4546);
	updater->PrintBooks(Side::Bid);
	updater->Process(Action::Change,2,Side::Bid, 5, 34.4546);
	updater->PrintBooks(Side::Bid);
	updater->Process(Action::Change,3,Side::Bid, -3, 34.4546);
	updater->Process(Action::Change,4,Side::Bid, -12, 34.4546);
	updater->Process(Action::Delete,5,Side::Bid, .45436, 34.4546);
	updater->PrintBooks(Side::Bid);
	updater->PrintBooks(Side::Offer);
	seqItem t;
	cout<<sizeof(t.s_member_hook_)<<" "<<sizeof(t.l_member_hook_)<<endl;
}

void print( const shared_ptr< Response >& response )
{
    fprintf( stderr, "*** Response ***\n" );
    fprintf( stderr, "Status Code:    %i\n", response->get_status_code( ) );
    fprintf( stderr, "Status Message: %s\n", response->get_status_message( ).data( ) );
    fprintf( stderr, "HTTP Version:   %.1f\n", response->get_version( ) );
    fprintf( stderr, "HTTP Protocol:  %s\n", response->get_protocol( ).data( ) );

    for ( const auto header : response->get_headers( ) )
    {
        fprintf( stderr, "Header '%s' > '%s'\n", header.first.data( ), header.second.data( ) );
    }

    auto length = response->get_header( "Content-Length", 0 );

    Http::fetch( length, response );

    fprintf( stderr, "Body:           %.*s...\n\n", 25, response->get_body( ).data( ) );
}
void poloniex_restbed_get()
{
	//"https://poloniex.com/public?command=returnTicker"
    auto request = make_shared< Request >( Uri("https://poloniex.com/public"));
//    request->set_header( "Accept", "*/*" );
    request->set_header( "Connection", "keep-alive" );

//    request->set_header( "Host", "poloniex.com" );
    request->set_header( "Content-Type", "application/x-www-form-urlencoded" );
 //   request->set_body("command=returnTicker");
    request->set_query_parameter( "command", "returnTicker" );
    request->set_method("GET");
    auto response = Http::sync( request );
    print( response );

    auto future = Http::async( request, [ ]( const shared_ptr< Request >, const shared_ptr< Response > response )
    {
        fprintf( stderr, "Printing async response\n" );
        print( response );
    } );

    future.wait( );

}
void websocket()
{
	std::shared_ptr<BookSide<level, Side::Bid>> bidBook =
			std::make_shared<BookSide<level, Side::Bid>> ("BTC_BBR",1000);

	std::shared_ptr<BookSide<level, Side::Offer>> askBook =
			std::make_shared<BookSide<level, Side::Offer>> ("BTC_BBR",1000);
	std::string p = "-0.00742634";
	cout<<std::stod(p)<<endl;
	cout<<"******** Crypto currency application ********"<< endl;
	wss_connection poloniex{true, bidBook, askBook};
	poloniex.wss_start();
}
int httpsClient()
{
	BookList bk;
	matrix p(&bk);
    try
    {
    	string server = "poloniex.com";
    	string path = "/public?command=returnTicker";
        boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
        ctx.set_default_verify_paths();

        boost::asio::io_service io_service;
        client<false> c(io_service, ctx, server, path, p);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}
int main(int argc, char** argv)
{
	std::cout << "Using Boost "
	          << BOOST_VERSION / 100000     << "."  // major version
	          << BOOST_VERSION / 100 % 1000 << "."  // minor version
	          << BOOST_VERSION % 100                // patch level
	          << std::endl;
	httpsClient();


    return 0;
}

