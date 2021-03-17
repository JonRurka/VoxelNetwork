#pragma once
#include "../stdafx.h"

#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "../Logger.h"
#include "../IUser.h"
#include "Data.h"
#include "BufferUtils.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;


class tcp_connection : public boost::enable_shared_from_this<tcp_connection> {
private:
	tcp::socket socket_;
	char* read_bytes;
	std::map<int, std::vector<char>> send_buff;
	int sent;
	char length_buff[2];
public:
	typedef boost::shared_ptr<tcp_connection> pointer;

	static pointer create(boost::asio::io_context& io_context)
	{
		return pointer(new tcp_connection(io_context));
	}

	tcp::socket& socket()
	{
		return socket_;
	}

	char* read_bytes;

	void Send(char* sending, size_t len)
	{
		boost::shared_ptr<std::vector<char>> message(
			new std::vector(sending, sending + len));


		boost::asio::async_write(socket_, boost::asio::buffer(*message, len),
			boost::bind(&tcp_connection::handle_write, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

	void Send(std::vector<char> sending)
	{
		boost::shared_ptr<std::vector<char>> message(new std::vector(sending));

		boost::asio::async_write(socket_, boost::asio::buffer(*message),
			boost::bind(&tcp_connection::handle_write, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

	void Start_Read() {
		boost::asio::async_read(socket_, boost::asio::buffer(length_buff, 2),
			boost::bind(&tcp_connection::handle_read, shared_from_this(), 
						boost::asio::placeholders::error, 
						boost::asio::placeholders::bytes_transferred));
	}

private:
	tcp_connection(boost::asio::io_context& io_context)
		: socket_(io_context)
	{
		read_bytes = new char[1024];
		sent = 0;
	}

	void handle_write(const boost::system::error_code&, size_t transfered)
	{
		//send_buff.erase(sent_id);
	}

	void handle_read(const boost::system::error_code&, size_t transfered)
	{
		short size = (short)&length_buff;

		char* message = new char[size];

		Logger::Log("Received bytes: " + std::to_string(size));
		//boost::asio::read(socket_, boost::asio::buffer(message, size));

		Start_Read();
	}
};

class tcp_server {
private:
	boost::asio::io_context& io_context_;
	tcp::acceptor acceptor_;

public:
	tcp_server(boost::asio::io_context& io_context)
		: io_context_(io_context),
		acceptor_(io_context, tcp::endpoint(tcp::v4(), 13))
	{
		start_accept();
	}

	void start_accept()
	{
		tcp_connection::pointer new_connection =
			tcp_connection::create(io_context_);

		acceptor_.async_accept(new_connection->socket(),
			boost::bind(&tcp_server::handle_accept, this, new_connection,
				boost::asio::placeholders::error));
	}

	void handle_accept(tcp_connection::pointer new_connection,
		const boost::system::error_code& error)
	{
		if (!error)
		{
			new_connection->Write("test", 4);
		}

		start_accept();
	}


};

class udp_server {
public:
	udp_server(boost::asio::io_context& io_context)
		: socket_(io_context, udp::endpoint(udp::v4(), 13))
	{
		start_receive();
	}

private:
	void start_receive()
	{
		socket_.async_receive_from(
			boost::asio::buffer(recv_buffer_), remote_endpoint_,
			boost::bind(&udp_server::handle_receive, this,
				boost::asio::placeholders::error));
	}

	void handle_receive(const boost::system::error_code& error)
	{
		if (error)
		{
			
		}

		start_receive();
	}

	void Send(char* sending, size_t len)
	{
		boost::shared_ptr<std::vector<char>> message(
			new std::vector(sending, sending + len));

		socket_.async_send_to(boost::asio::buffer(*message, len), remote_endpoint_,
			boost::bind(&udp_server::handle_send, this));

	}

	void Send(std::vector<char> sending)
	{
		boost::shared_ptr<std::vector<char>> message(new std::vector(sending));

		socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
			boost::bind(&udp_server::handle_send, this));
	}

	void handle_send()
	{
	}

	void SetEndPoint(udp::endpoint theEndpoint) {

	}


	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	boost::array<char, 1> recv_buffer_;
};

class AsyncServer {
public:
	class SocketUser {
	public:
		IUser User;
		std::string SessionToken;
		int Permission;
		bool Connected;
		bool IsAuthenticated;
		bool Receiving;
		bool UdpEnabled;
		int UdpID;
		bool CloseMessage;


	}
	

	void Test();
};