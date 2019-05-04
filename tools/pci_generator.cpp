// infoware - C++ System information Library
//
// Written in 2016-2019 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>


#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


struct vendor_t {
	int64_t pci_id;
	std::size_t name_index;
	std::vector<std::size_t> devices;
};

struct device_t {
	int64_t pci_id;
	std::size_t name_index;
};


void write(std::string name, std::ostream& out, const std::vector<vendor_t>& vendor_indices, const std::vector<device_t>& device_indices,
           const std::vector<std::string>& names) {
	out << std::hex << std::showbase;

	out << "#define INFOWARE_GENERATED_PCI_INDICES";
	std::size_t idx = 0;
	for(auto&& index : vendor_indices) {
		out << " \\\n\t{" << index.pci_id << ", " << idx << "},";
		++idx;
	}

	out << "\n\n\n"
	       "#define INFOWARE_GENERATED_PCI_VENDORS";
	for(auto&& vendor : vendor_indices) {
		out << " \\\n\t{" << vendor.pci_id << ", {R\"(" << names[vendor.name_index] << ")\", " << names[vendor.name_index].size() << "}, {";
		for(auto i : vendor.devices)
			out << i << ", ";
		out << "}},";
	}

	out << "\n\n\n"
	       "#define INFOWARE_GENERATED_PCI_DEVICES";
	for(auto&& device : device_indices)
		out << " \\\n\t{" << device.pci_id << ", {R\"(" << names[device.name_index] << ")\", " << names[device.name_index].size() << "}},";

	out << "\n\n\n"
	       "namespace {}";  // Suppress bogus "warning: backslash-newline at end of file" on GCC
}

bool read_pci_ids_hex(std::istream& input, int64_t& streamid) {
	if(!input) {
		return false;
	}
	input >> std::hex >> streamid;
	if(!input) {
		return false;
	}
	for(; input && input.peek() == ' ';) {
		input.get();
		if(std::isalnum(input.peek())) {
			// More numbers
			int64_t nexthex = 0;
			input >> std::hex >> nexthex;
			if(input) {
				streamid <<= 16;
				streamid |= nexthex;
			}
		} else {
			break;
		}
	}
	return true;
}

bool read_pci_ids_name(std::istream& input, std::string& streamname, std::string& poststreamname) {
	if(!(input >> streamname))
		return false;

	if(!std::getline(input, poststreamname))
		return true;

	streamname.append(poststreamname);
	for(auto it = streamname.find('"'); it != std::string::npos; it = streamname.find('"', it + 2))
		streamname.replace(it, 1, R"(\")", 2);

	return true;
}

bool is_interesting(int64_t pci_vendor_id) {
#if 0
	static const int64_t interesting[]{
		1002,
		1022,
	};
	return std::find(std::cbegin(interesting), std::cend(interesting), pci_vendor_id) != std::cend(interesting);
#else
	(void)pci_vendor_id;
	return true;
#endif
}

int main(int argc, const char** argv) {
	if(argc < 2) {
		std::cerr << "Input file missing\n";
		return 1;
	}

	std::ifstream input(argv[1]);
	if(!input) {
		std::cerr << "Couldn't open input file\n";
		return 2;
	}

	std::vector<vendor_t> vendor_id_indices{};
	std::vector<device_t> device_id_indices{};
	std::vector<std::string> vendor_device_names{};
	device_id_indices.reserve(2000);
	vendor_id_indices.reserve(3500);
	vendor_device_names.reserve(2000 + 3500);

	std::string streamname;
	std::string poststreamname;
	int64_t streamid;
	for(bool freeze = false; input;) {
		int ic = input.peek();
		if(!input)
			break;

		char c        = static_cast<char>(ic);
		bool readable = std::isalnum(c) || c == '\t';
		if(!readable) {
			input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		int tabcount = 0;
		for(; input && c == '\t'; ++tabcount) {
			input.get();
			c = input.peek();
		}
		if(freeze && tabcount > 0) {
			input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		freeze = false;
		switch(tabcount) {
			case 0:  // vendor id
				if(read_pci_ids_hex(input, streamid) && read_pci_ids_name(input, streamname, poststreamname)) {
					if(!is_interesting(streamid)) {
						freeze = true;
						break;
					}
					vendor_id_indices.push_back({streamid, vendor_device_names.size(), {}});
					vendor_device_names.push_back(streamname);
				}
				break;

			case 1:  // device id
				if(read_pci_ids_hex(input, streamid) && read_pci_ids_name(input, streamname, poststreamname)) {
					vendor_t& v = vendor_id_indices.back();
					v.devices.push_back(device_id_indices.size());
					device_id_indices.push_back({streamid, vendor_device_names.size()});
					vendor_device_names.push_back(streamname);
				}
				break;

			case 2:  // subsystem id and stuff, which we don't want
				input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				continue;

			default:
				std::cerr << "uwu\n";
				return 2;
		}
	}

	std::sort(vendor_id_indices.begin(), vendor_id_indices.end(), [](auto&& a, auto&& b) { return a.pci_id < b.pci_id; });

	write("identify_device", std::cout, vendor_id_indices, device_id_indices, vendor_device_names);
}
