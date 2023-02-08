// should be the main Class
#include "Ethernet.h"
#include <stdio.h>
#include <array>
#include <vector>
#include <cstdint>
#include <fstream>
#include <iostream>

using namespace std;

class Ethernet
{
public:
    Ethernet(const array<uint8_t, 8> &preamble, const array<uint8_t, 6> &destination, const array<uint8_t, 6> &source, uint16_t ethertype,
             const vector<uint8_t> &data, uint32_t fcs)
        : preamble_{preamble}, destination_{destination}, source_{source}, ethertype_{ethertype}, data_{data}, fcs_{fcs} {}

    void readPacketsFromFile(const string &filename)
    {
        ifstream file(filename);
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                if (line.length() < 60 || line.length() > 1514) // packet size out of range
                {
                    cout << "Packet size error: Packet size must be between 60 and 1514 bytes" << endl;
                    continue;
                }

                vector<uint8_t> bytes;
                for (int i = 0; i < line.length(); i += 2)
                {
                    uint8_t byte = static_cast<uint8_t>(stoul(line.substr(i, 2), nullptr, 16));
                    bytes.push_back(byte);
                }

                // Extract the preamble
                array<uint8_t, 8> preamble;
                copy(bytes.begin(), bytes.begin() + 8, preamble.begin());

                // Extract the destination
                array<uint8_t, 6> destination;
                copy(bytes.begin() + 8, bytes.begin() + 14, destination.begin());

                // Extract the source
                array<uint8_t, 6> source;
                copy(bytes.begin() + 14, bytes.begin() + 20, source.begin());

                // Extract the ethertype
                uint16_t ethertype = (bytes[20] << 8) | bytes[21];

                // Extract the data
                vector<uint8_t> data(bytes.begin() + 22, bytes.end() - 4);

                // Extract the FCS
                uint32_t fcs = (bytes[bytes.size() - 4] << 24) | (bytes[bytes.size() - 3] << 16) | (bytes[bytes.size() - 2] << 8) | bytes[bytes.size() - 1];

                // Add the packet to the list of packets
                packets_.push_back(Ethernet(preamble, destination, source, ethertype, data, fcs));
            }
        }
        else
        {
            cout << "Error: Failed to open file " << filename << endl;
        }
    }

    void writePacketsToFile(const string &filename)
    {
        ofstream file(filename);
        if (file.is_open())
        {
            for (int i = 0; i < packets_.size(); ++i)
            {
                file << "Packet # " << i << ":" << endl;
                file << hex << setfill('0') << setw(2);
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

                file << "CRC: " << hex << setfill('0') << setw(8) << (int)packets_[i].fcs_ << endl;
                file << "Concatenation Indicator: 0" << endl;
                file << "Destination Address: " << hex << setfill('0') << setw(4);
                for (int j = 0; j < 6; j += 2)
                    file << (int)((packets_[i].destination_[j] << 8) | packets_[i].destination_[j + 1]);
                file << endl;
                file << "Message Type: " << hex << setfill('0') << setw(2) << ((packets_[i].ethertype_ & 0xFF00) >> 8) << endl;
                file << "Payload Size: " << hex << setfill('0') << setw(4) << packets_[i].data_.size() << endl;
                file << "Protocol Version: 1" << endl;
                file << "RTC ID: " << hex << setfill('0') << setw(4) << (int)packets_[i].fcs_ << endl;
                file << "Sequence ID: " << hex << setfill('0') << setw(4) << (int)packets_[i].fcs_ << endl;
                file << "Source Address: " << hex << setfill('0') << setw(4);
                for (int j = 0; j < 6; j += 2)
                    file << (int)((packets_[i].source_[j] << 8) | packets_[i].source_[j + 1]);
                file << endl;
                file << "Type: " << hex << setfill('0') << setw(4) << (int)packets_[i].ethertype_ << endl
                     << endl;
            }
        }
        else
        {
            cout << "Error: Failed to open file " << filename << endl;
        }
    }

private:
    vector<Ethernet> packets_;
    array<uint8_t, 8> preamble_;
    array<uint8_t, 6> destination_;
    array<uint8_t, 6> source_;
    uint16_t ethertype_;
    vector<uint8_t> data_;
    uint32_t fcs_;
};
