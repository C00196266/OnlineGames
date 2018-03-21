#pragma once

class BitPacker {
public:
	BitPacker();

	void writeBit(int value);
	int readBit();
	void writeInt(int value, int numBitsToWrite);
	int readInt(int numBitsToRead);
private:
	int m_currentBit;
	int m_currentByte;
	int m_buffer[5];

	int m_currentReadBit;
	int m_currentReadByte;
};