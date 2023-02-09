#include "Ethernet.h"

void Scanner::readPacketsFromFile(const string& filename, vector<EthernetPacket*>& packets_)
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
		EthernetPacket* ethernetPacket = EthernetPacket::getEthernetPacketInstance(ethertype);
		ethernetPacket->parsePacket(bytes);

		// Add the packet to the list of packets
		packets_.push_back(ethernetPacket);
	}
}
