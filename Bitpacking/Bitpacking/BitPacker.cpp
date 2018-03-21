#include "Bitpacker.h"

BitPacker::BitPacker() {
	m_currentBit = 0;
	m_currentByte = 0;
	m_currentReadBit = 0;
	m_currentReadByte = 0;

	for (int i = 0; i < 5; i++) {
		m_buffer[i] = 0;
	}
}

void BitPacker::writeBit(int value) {
	if (m_currentBit == 8) {
		m_currentBit = 0;
		m_currentByte++;
		m_buffer[m_currentByte] = 0;
	}

	m_buffer[m_currentByte] |= (value << m_currentBit);
	m_currentBit++;
}

int BitPacker::readBit() {
	if (m_currentReadBit == 8) {
		m_currentReadBit = 0;
		m_currentReadByte++;
	}

	unsigned int checkBit = m_buffer[m_currentReadByte] >> m_currentReadBit;
	m_currentReadBit++;

	return (checkBit & 1);
}

void BitPacker::writeInt(int value, int numBitsToWrite) {
	for (int i = 0; i < numBitsToWrite; i++) {
		int bitToWrite = (value >> i) & 1;

		writeBit(bitToWrite);
	}
}

int BitPacker::readInt(int numBitsToRead) {
	int returnValue = 0;

	for (int i = 0; i < numBitsToRead; i++) {
		int bitToRead = readBit();

		returnValue = returnValue | (bitToRead << i);
	}

	return returnValue;
}