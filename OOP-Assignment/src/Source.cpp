#include "Ethernet.h"

// Driver code
int main() {
	string input_file = "input_packets";
	string output_file = "output";
	vector<EthernetPacket*> packets_;
	Scanner::readPacketsFromFile(input_file, packets_);
	Printer::writePacketsToFile(output_file, packets_);
}
