#include "Ethernet.h"

void Printer::writePacketsToFile(const string &filename, vector<EthernetPacket *> &packets_)
{
	string file_name = filename + ".txt";
	ofstream file(file_name);
	if (!file.is_open())
	{
		cout << "Error: Failed to open file " << file_name << endl;
		return;
	}

	for (int i = 0; i < packets_.size(); ++i)
	{
		file << "Packet # " << dec << i << ":" << endl;
		file << uppercase << hex << setfill('0') << setw(2);
		for (uint8_t byte : packets_[i]->preamble_)
			file << (int)byte;
		for (uint8_t byte : packets_[i]->destination_)
			file << (int)byte;
		for (uint8_t byte : packets_[i]->source_)
			file << (int)byte;
		file << setw(4) << (int)packets_[i]->ethertype_;
		for (uint8_t byte : packets_[i]->data_)
			file << (int)byte;
		file << setw(8) << (int)packets_[i]->fcs_ << endl;

		packets_[i]->printPacket(file);
	}
}
