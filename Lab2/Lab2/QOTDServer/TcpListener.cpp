#include "TcpListener.h"

CTcpListener::CTcpListener(std::string ipAddress, int port, MessageRecievedHandler handler)
	: m_ipAddress(ipAddress), m_port(port), MessageReceived(handler) {
	
}

CTcpListener::~CTcpListener() {
	Cleanup();
}

// Send a message to the specified client
void CTcpListener::Send(int clientSocket, std::string msg) {
	send(clientSocket, msg.c_str(), msg.length() + 1, 0);
}

// Initialize winsock
bool CTcpListener::Init() {
	WSAData data;
	WORD ver = MAKEWORD(2, 2);

	// initialise winsock
	int wsInit = WSAStartup(ver, &data);
	if (wsInit != 0)
	{
		std::cerr << "Can't Initialize winsock! Quitting" << std::endl;
		return wsInit == 1;
	}

	// create socket
	CreateSocket();

	m_isRunning = true;
	
	return wsInit == 0;
}

// The main processing loop
void CTcpListener::Run() {
	while (m_isRunning == true)
	{
		m_copy = m_master;

		m_socketCount = select(0, &m_copy, NULL, NULL, NULL);

		for (int i = 0; i < m_socketCount; i++) {
			SOCKET sock = m_copy.fd_array[i];

			// checks if socket is a connecting client
			if (sock == m_listening) {
				SOCKET client = accept(m_listening, NULL, NULL);

				FD_SET(client, &m_master);

				std::string msg = "Welcome to the server, friend!\r\n";
				send(client, msg.c_str(), msg.length(), 0);
			}

			else {
				char buffer[4096];
				ZeroMemory(buffer, 4096);

				int bytesIn = recv(sock, buffer, 4096, 0);

				// if no bytes were received
				if (bytesIn <= 0) {
					closesocket(sock);
					FD_CLR(sock, &m_master);
				}

				else {
					// checks if command is quit
					if (buffer[0] == '\\') {
					
						std::string cmd = std::string(buffer, bytesIn);
						if (cmd == "\\quit") {
							m_isRunning = false;
							break;
						}
						else {
							continue;
						}
					}

					// send messages
					for (int i = 0; i < m_master.fd_count; i++) {
						SOCKET outSock = m_master.fd_array[i];

						if (m_master.fd_array[i] != m_listening && m_master.fd_array[i] != sock) {
							std::ostringstream ss;
							ss << "SOCKET #" << sock << ": " << buffer << "\r\n";
							//std::string strOut = ss.str();

							Send(outSock, ss.str());
							//send(outSock, strOut.c_str(), strOut.size() + 1, 0);
						}
					}
				}
			}
		}
	}
}

void CTcpListener::Cleanup() {
	FD_CLR(m_listening, &m_master);
	closesocket(m_listening);

	// Message to let users know what's happening.
	std::string msg = "Server is shutting down. Goodbye\r\n";

	while (m_master.fd_count > 0)
	{
		// Get the socket number
		SOCKET sock = m_master.fd_array[0];

		// Send the goodbye message
		send(sock, msg.c_str(), msg.size() + 1, 0);

		// Remove it from the master file list and close the socket
		FD_CLR(sock, &m_master);
		closesocket(sock);
	}

	// Cleanup winsock
	WSACleanup();
}

// Create a socket
void CTcpListener::CreateSocket() {
	m_listening = socket(AF_INET, SOCK_STREAM, 0);

	if (m_listening == INVALID_SOCKET)
	{
		std::cerr << "Can't create a socket! Quitting" << std::endl;
	}

	// bind the ip and port to socket
	m_addr.sin_family = AF_INET; // IPv4 socket
	m_addr.sin_addr.s_addr = inet_addr(m_ipAddress.c_str()); // ip address
	m_addr.sin_port = htons(m_port); // port

	bind(m_listening, (sockaddr*)&m_addr, sizeof(m_addr));
	listen(m_listening, SOMAXCONN);

	FD_ZERO(&m_master);

	FD_SET(m_listening, &m_master);
}

// Wait for a connection
SOCKET CTcpListener::WaitForConnection(SOCKET listening) {
	SOCKET client = accept(listening, NULL, NULL);
	return client;
}