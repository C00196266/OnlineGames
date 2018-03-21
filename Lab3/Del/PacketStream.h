#pragma once

#include <sstream>
#include <iostream>
#include <string>

class PacketStream { 
public:
	void writeInt(int val);
	void readInt(int &val);
	void toCharArray(char* array);
	void fromCharArray(char* array);

private:       
	std::ostringstream outputStream;
	std::istringstream inputStream; 
};
