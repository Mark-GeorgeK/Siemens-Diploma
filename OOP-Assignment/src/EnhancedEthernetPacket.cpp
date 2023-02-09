#include "Ethernet.h"

void EnhancedEthernetPacket::getDataBlock(vector<uint8_t>& bytes) {
	// change..
	vector<uint8_t> data(bytes.begin() + 22, bytes.end() - 4);
	data_ = data;
}

void EnhancedEthernetPacket::printPacket(ofstream& file) {
	file << "CRC: " << setw(8) << (int)fcs_ << endl;
	file << "Concatenation Indicator: 0" << endl;
	file << "Destination Address: " << setw(4);
	for (int j = 0; j < 6; j += 2)
		file << (int)((destination_[j] << 8) | destination_[j + 1]);
	file << endl;
	file << "Message Type: " << setw(2) << ((ethertype_ & 0xFF00) >> 8) << endl;
	file << "Payload Size: " << setw(4) << data_.size() << endl;
	file << "Protocol Version: 1" << endl;
	file << "RTC ID: " << setw(4) << (int)fcs_ << endl;
	file << "Sequence ID: " << setw(4) << (int)fcs_ << endl;
	file << "Source Address: " << setw(4);
	for (int j = 0; j < 6; j += 2)
		file << (int)((source_[j] << 8) | source_[j + 1]);
	file << endl;
	file << "Type: " << setw(4) << (int)ethertype_ << endl;
	file << endl;
	for (int i = 0; i < 230; i++)
		file << "*";
	file << endl << endl;
}
