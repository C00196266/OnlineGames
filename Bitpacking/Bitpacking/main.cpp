#include "Bitpacker.h"
#include <iostream>

int main() {
	BitPacker bitPacker;

	int health = 10;
	int ammo = 3;

	bitPacker.writeInt(health, 10);
	bitPacker.writeInt(ammo, 5);
	
	int receivedHealth = bitPacker.readInt(10);
	int receivedAmmo = bitPacker.readInt(5);

	std::cout << "Received health: " << receivedHealth << std::endl;
	std::cout << "Received ammo: " << receivedAmmo << std::endl;

	system("PAUSE");

	return 0;
}