// infoware - C++ System information Library
//
// Written in 2016-2020 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>


#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


struct vendor_t {
	std::uint64_t pci_id;
	std::size_t name_index;
	std::vector<std::size_t> devices;
};

struct device_t {
	std::uint64_t pci_id;
	std::size_t name_index;
};


int main(int argc, const char** argv) {
	if(argc < 2) {
		std::cerr << "In file missing\n";
		return 1;
	}

	std::ifstream in(argv[1]);
	if(!in) {
		std::cerr << "Couldn't open input file\n";
		return 2;
	}


	std::vector<vendor_t> vendor_id_indices;
	std::vector<device_t> device_id_indices;
	std::vector<std::string> vendor_device_names;


	for(std::string line; std::getline(in, line);) {
		if(line.empty())
			continue;
		if(line[0] == 'C')  // Got to device classes. which we don't want
			break;

		const auto tabcount = line.find_first_not_of('\t');
		if(!std::isxdigit(line[tabcount]) || tabcount >= 3)
			continue;

		char* current_name{};
		auto current_number = std::strtoull(line.c_str() + tabcount, &current_name, 16);
		while(std::isspace(*current_name))
			++current_name;

		if(tabcount == 0)  // Vendor
			vendor_id_indices.push_back({current_number, vendor_device_names.size(), {}});
		else if(tabcount == 1) {  // Device
			vendor_id_indices.back().devices.push_back(device_id_indices.size());
			device_id_indices.push_back({current_number, vendor_device_names.size()});
		}

		vendor_device_names.emplace_back(current_name);
	}


	std::sort(std::end(vendor_id_indices), std::end(vendor_id_indices), [](auto&& a, auto&& b) { return a.pci_id < b.pci_id; });


	auto& out = std::cout;
	out << std::hex << std::showbase;

	out << "#define INFOWARE_GENERATED_PCI_INDICES";
	std::size_t idx = 0;
	for(auto&& index : vendor_id_indices) {
		out << " \\\n\t{" << index.pci_id << ", " << idx << "},";
		++idx;
	}

	out << "\n\n\n"
	       "#define INFOWARE_GENERATED_PCI_VENDORS";
	for(auto&& vendor : vendor_id_indices) {
		out << " \\\n\t{" << vendor.pci_id << ", R\"(" << vendor_device_names[vendor.name_index] << ")\", {";
		for(auto i : vendor.devices)
			out << i << ", ";
		out << "}},";
	}

	out << "\n\n\n"
	       "#define INFOWARE_GENERATED_PCI_DEVICES";
	for(auto&& device : device_id_indices)
		out << " \\\n\t{" << device.pci_id << ", R\"(" << vendor_device_names[device.name_index] << ")\"},";

	out << "\n\n\n"
	       "namespace {}\n";  // Suppress bogus "warning: backslash-newline at end of file" on GCC
}
