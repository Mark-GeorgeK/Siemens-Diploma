#include "Ethernet.h"

class EthernetPacket {
public:
	array<uint8_t, 8> preamble_;
	array<uint8_t, 6> destination_;
	array<uint8_t, 6> source_;
	uint16_t ethertype_;
	vector<uint8_t> data_;
	uint32_t fcs_;

	// Circular dependency problem to be resolved..
	//static EthernetPacket* getEthernetPacketInstance(uint16_t ethertype) {
	//	if (ethertype == RawEthernetCode) return new RawEthernetPacket();
	//	else if (ethertype == EnhancedEthernetCode) return new EnhancedEthernetPacket();
	//	// else error
	//}

	void parsePacket(vector<uint8_t>& bytes) {
		// Extract the preamble
		copy(bytes.begin(), bytes.begin() + 8, preamble_.begin());

		// Extract the destination
		copy(bytes.begin() + 8, bytes.begin() + 14, destination_.begin());

		// Extract the source
		copy(bytes.begin() + 14, bytes.begin() + 20, source_.begin());

		// Extract the ethertype
		ethertype_ = (bytes[20] << 8) | bytes[21];

		// Extract the data
		getDataBlock(bytes);

		// Extract the FCS
		fcs_ = (bytes[bytes.size() - 4] << 24) | (bytes[bytes.size() - 3] << 16) | (bytes[bytes.size() - 2] << 8) | bytes[bytes.size() - 1];
	}

	// Cannot instantiate object problem to be resolved..
	virtual void getDataBlock(vector<uint8_t>& bytes) = 0;
	virtual void printPacket(ofstream& file) = 0;
	//void virtual getDataBlock(vector<uint8_t>& bytes) {}
	//void virtual printPacket(ofstream& file) {}
};

class RawEthernetPacket : public EthernetPacket {
public:
	void getDataBlock(vector<uint8_t>& bytes) override {
		vector<uint8_t> data(bytes.begin() + 22, bytes.end() - 4);
		data_ = data;
	}

	void printPacket(ofstream& file) override {
		file << "CRC: " << setw(8) << (int)fcs_ << endl;
		file << "Destination Address: " << setw(4);
		for (int j = 0; j < 6; j += 2)
			file << (int)((destination_[j] << 8) | destination_[j + 1]);
		file << endl;
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
};

class EnhancedEthernetPacket : public EthernetPacket {
public:
	void getDataBlock(vector<uint8_t>& bytes) override {
		// change..
		vector<uint8_t> data(bytes.begin() + 22, bytes.end() - 4);
		data_ = data;
	}

	void printPacket(ofstream& file) override {
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
};

// Global function
EthernetPacket* getEthernetPacketInstance(uint16_t ethertype) {
	if (ethertype == RawEthernetCode) return new RawEthernetPacket();
	else if (ethertype == EnhancedEthernetCode) return new EnhancedEthernetPacket();
	// else error
}

class Scanner
{
public:
	static void readPacketsFromFile(const string& filename, vector<EthernetPacket>& packets_)
	{
		ifstream file(filename);
		if (!file.is_open())
		{
			cout << "Error: Failed to open file " << filename << endl;
			return;
		}

		string line;
		while (getline(file, line))
		{
			if (line.length() < minPacketSize || line.length() > maxPacketSize) // packet size out of range
			{
				cout << "Packet size error: Packet size must be between " << minPacketSize << " and " << maxPacketSize << " bytes" << endl;
				continue;
			}

			// Scan line
			vector<uint8_t> bytes;
			for (int i = 0; i < line.length(); i += 2)
			{
				uint8_t byte = static_cast<uint8_t>(stoul(line.substr(i, 2), nullptr, 16));
				bytes.push_back(byte);
			}

			// Extract the ethertype
			uint16_t ethertype = (bytes[20] << 8) | bytes[21];

			//EthernetPacket* ethernetPacket = EthernetPacket::getEthernetPacketInstance(ethertype);
			EthernetPacket* ethernetPacket = getEthernetPacketInstance(ethertype);
			ethernetPacket->parsePacket(bytes);

			// Add the packet to the list of packets
			packets_.push_back(*ethernetPacket);
		}
	}
};

class Printer {
public:
	static void writePacketsToFile(const string& filename, vector<EthernetPacket>& packets_)
	{
		ofstream file(filename);
		if (!file.is_open())
		{
			cout << "Error: Failed to open file " << filename << endl;
			return;
		}

		for (int i = 0; i < packets_.size(); ++i)
		{
			file << "Packet # " << i << ":" << endl;
			file << uppercase << hex << setfill('0') << setw(2);
			for (uint8_t byte : packets_[i].preamble_)
				file << (int)byte;
			for (uint8_t byte : packets_[i].destination_)
				file << (int)byte;
			for (uint8_t byte : packets_[i].source_)
				file << (int)byte;
			file << setw(4) << (int)packets_[i].ethertype_;
			for (uint8_t byte : packets_[i].data_)
				file << (int)byte;
			file << setw(8) << (int)packets_[i].fcs_ << endl;

			packets_[i].printPacket(file);
		}
	}
};

// Driver code
int main() {
	string input_file = "input_packets";
	string output_file = "output";
	vector<EthernetPacket> packets_;
	Scanner::readPacketsFromFile(input_file, packets_);
	Printer::writePacketsToFile(output_file, packets_);
}
