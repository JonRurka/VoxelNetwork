#pragma once
#include "../stdafx.h"

#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/timer.hpp>

#include "../Logger.h"
#include "../IUser.h"
#include "Data.h"
#include "BufferUtils.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;
using boost::asio::ip::address;


class tcp_connection : public boost::enable_shared_from_this<tcp_connection> {
private:
	tcp::socket socket_;
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
		ZeroMemory(length_buff, 2);
		sent = 0;
	}

	void handle_write(const boost::system::error_code&, size_t transfered)
	{
		//send_buff.erase(sent_id);
	}

	void handle_read(const boost::system::error_code&, size_t transfered);
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
			//new_connection->Send("test", 4);
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
		IUser* User;
		std::string SessionToken;
		int Permission;
		bool Connected;
		bool IsAuthenticated;
		bool Receiving;
		bool UdpEnabled;
		int UdpID;
		bool CloseMessage;

		tcp::endpoint TcpEndPoint;
		udp::endpoint UdpEndPoint;

	private:
		AsyncServer* _server;
		boost::timer timeOutWatch;

	public:

		SocketUser(AsyncServer* server, tcp_connection::pointer client, tcp::endpoint endPoint)
		{
			_server = server;
			//SessionToken = HashHelper.RandomKey(8);
			TcpEndPoint = endPoint;
			timeOutWatch.restart();
			UdpID = -1;
			Permission = 0;
			Connected = true;
			IsAuthenticated = false;
			UdpEnabled = false;

			

		}

		void EnableUdp(int port)
		{
			//UdpEndPoint.port() = port;
			//Logger.Log("UDP end point: {0}:{1}", udpEndPoint.Address.ToString(), udpEndPoint.Port);
			UdpEnabled = true;
		}

		void SetUser(IUser* user)
		{
			User = user;
			if (User != NULL)
			{
				//Logger::Log("Set user: " + ((User)User).Name);
				//User.SetSocket(this);
			}
			else
			{
				//Logger::Log("IUser null");
			}
		}

		void Send(char command, std::string message, Protocal type = Protocal_Tcp)
		{
			std::vector<char> msg(message.begin(), message.end());

			Send(command, msg, type);
		}

		void Send(char cmd, std::vector<char> data, Protocal type = Protocal_Tcp)
		{
			Send(BufferUtils::AddFirst(cmd, data), type);
		}

		void Send(std::vector<char> data, Protocal type = Protocal_Tcp)
		{
			if (!Connected)
				return;

			//try
			//{
				if (type == Protocal_Tcp || !UdpEnabled)
				{
					//if (_stream != null && data != null)
					//	_stream.SendMessasge(data);
				}
				else
				{
					//_server.SendUdp(data, UdpEndPoint);
				}
			/*}
			catch (IOException)
			{
				Close(false, "Send IOException");
			}
			catch (SocketException)
			{
				Close(false, "Send SocketException");
			}
			catch (Exception ex)
			{
				Logger.LogError("{0}: {1}\n{2}", ex.GetType(), ex.Message, ex.StackTrace);
			}*/
		}

		std::string GetIP()
		{
			address addr = TcpEndPoint.address();
			return addr.to_string();
		}

		void Close(bool sendClose, std::string reason = "")
		{
			if (Connected)
			{
				Connected = false;

				if (User != NULL)
				{
					//User.Disconnected();
				}

				//if (CloseMessage)
				//	Logger::Log("{0}: closed {1}", SessionToken, reason != "" ? "- " + reason : "");
				//_server.RemoveUdpID(UdpID);
			}
		}

	private:

		void ProcessReceiveBuffer(std::vector<char> buffer, Protocal type)
		{
			timeOutWatch.restart();

			if (buffer.size() > 0)
			{
				char command = buffer[0];
				buffer = BufferUtils::RemoveFront(Remove_CMD, buffer);
				Data data(type, command, buffer);
				//_server.Process(this, data);
			}
			else {
				//Logger::Log(std::to_string(type) + ": Received empty buffer!");
			}
		}
	};
	

	void Test();
};