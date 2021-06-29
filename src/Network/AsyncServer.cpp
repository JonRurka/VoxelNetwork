#include "AsyncServer.h"

void tcp_connection::handle_read(const boost::system::error_code&, size_t transfered)
{
	short size = (short)&length_buff;

	char* message = new char[size];

	Logger::Log("Received bytes: " + std::to_string(size));
	//boost::asio::read(socket_, boost::asio::buffer(message, size));

	Start_Read();
}


void AsyncServer::Test()
{
	//try {
		//boost::asio::io_context io_context;

		

	//}

}
