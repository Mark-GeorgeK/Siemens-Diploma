#include "Ethernet.h"

EthernetPacket *EthernetPacket::getEthernetPacketInstance(uint16_t ethertype)
{
	if (ethertype == RawEthernetCode)
		return new RawEthernetPacket();
	else if (ethertype == EnhancedEthernetCode)
		return new EnhancedEthernetPacket();
	// else error
	else
	{
		return 0;
	}
}

void EthernetPacket::parsePacket(vector<uint8_t> &bytes)
{
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
