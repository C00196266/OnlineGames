#include "Net.h"

void playerA(Net &theNet) {
	theNet.setupUDP(28000, "127.0.0.1");

	std::cout << "Player A port: " << theNet.portNum << std::endl;
	std::cout << "Player A IP: " << theNet.getReceiverIP() << std::endl;

	theNet.sendData("127.0.0.1", theNet.portNum, "Hey Guys It Worked YAAAAAAAAAAAAY");
}

void playerB(Net &theNet) {
	theNet.setupUDP(28001, "127.0.0.1");

	std::cout << "Player B port: " << theNet.portNum << std::endl;
	std::cout << "Player B IP: " << theNet.getReceiverIP() << std::endl;

	char message[200] = "";
	theNet.receiveData(message);

	std::cout << "Received: " << message << std::endl;
}

int main() {
	Net net;

	net.initialise();

	std::string input;

	std::cin >> input;

	if (input == "a") {
		playerA(net);
	}
	else if (input == "b") {
		playerB(net);
	}

	system("PAUSE");

	return 0;
}