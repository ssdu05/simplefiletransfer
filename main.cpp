
#include <string>
#include <asio.hpp>
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {

	std::string filename = argv[1];

	std::ifstream ifs(filename, std::ifstream::in | std::ifstream::binary);
	
	auto rdbuf = ifs.rdbuf();

	// set reading file pointer to end of file and get the pos of that = size of file
	auto size = rdbuf->pubseekoff(0, ifs.end, ifs.in);
	rdbuf->pubseekpos(0, ifs.in); // reset the reading file pointer to start of file
	std::cout << "size: " << size << std::endl;
	// create buffer for file 
	char* fileBuf = new char[size];
	// fill buffer
	rdbuf->sgetn(fileBuf, size);

	ifs.close();
	
	std::string remoteIP = "127.0.0.1";
	unsigned short remotePort = 4000;

	asio::io_context io_context;

	asio::ip::tcp::socket sock(io_context);

	asio::ip::tcp::endpoint epRemote(asio::ip::make_address_v4(remoteIP), remotePort);

	sock.connect(epRemote);

	sock.send(asio::buffer(fileBuf, size));

	sock.close();
	

	return 0;
}


