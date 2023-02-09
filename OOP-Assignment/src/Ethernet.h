#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <array>
#include <vector>

#define RawEthernetCode 35063
#define EnhancedEthernetCode 44798
#define minPacketSize 60
#define maxPacketSize 1514

using namespace std;

class EthernetPacket {
public:
	array<uint8_t, 8> preamble_;
	array<uint8_t, 6> destination_;
	array<uint8_t, 6> source_;
	uint16_t ethertype_;
	vector<uint8_t> data_;
	uint32_t fcs_;

	static EthernetPacket* getEthernetPacketInstance(uint16_t ethertype);
	void parsePacket(vector<uint8_t>& bytes);
	virtual void getDataBlock(vector<uint8_t>& bytes) = 0;
	virtual void printPacket(ofstream& file) = 0;
};

class RawEthernetPacket : public EthernetPacket {
public:
	void getDataBlock(vector<uint8_t>& bytes) override;
	void printPacket(ofstream& file) override;
};

class EnhancedEthernetPacket : public EthernetPacket {
public:
	void getDataBlock(vector<uint8_t>& bytes) override;
	void printPacket(ofstream& file) override;
};

class Scanner {
public:
	static void readPacketsFromFile(const string& filename, vector<EthernetPacket*>& packets_);
};

class Printer {
public:
	static void writePacketsToFile(const string& filename, vector<EthernetPacket*>& packets_);
};
