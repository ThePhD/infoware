// infoware - C++ System information Library
//
// Written in 2016 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
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

void write(std::string name, std::ofstream& out, const std::vector<vendor_t>& vendor_indices, const std::vector<device_t>& device_indices,
           const std::vector<std::string>& names) {
	out << "\t\tstd::pair<std::string, std::string> " << name << "(int64_t vendor_pci_id, int64_t device_pci_id) noexcept {\n";
	out << "\t\t\tstatic const id_pair_t indices[] {\n";
	// indices
	std::size_t idx = 0;
	for(const auto& index : vendor_indices) {
		out << "\t\t\t\t{ 0x" << std::hex << index.pci_id << ", 0x" << idx << " }," << '\n';
		++idx;
	}
	out << "\t\t\t};\n";

	out << "\t\t\tstatic const pci_vendor_info_t vendors[] {\n";
	for(const auto& vendor : vendor_indices) {
		out << "\t\t\t\t{ 0x" << std::hex << vendor.pci_id << ", { R\"(" << names[vendor.name_index] << ")\", 0x" << names[vendor.name_index].size() << " }, { ";
		for(std::size_t i : vendor.devices) {
			out << "0x" << i << ", ";
		}
		out << " } }," << '\n';
	}
	out << "\t\t\t};\n";

	out << "\t\t\tstatic const pci_device_info_t devices[] {\n";
	for(const auto& device : device_indices) {
		out << "\t\t\t\t{ 0x" << std::hex << device.pci_id << ", { R\"(" << names[device.name_index] << ")\", 0x" << names[device.name_index].size() << " } },"
		    << '\n';
	}
	out << "\t\t\t};\n";

	out <<
	    R"(
			auto it = std::lower_bound(std::begin(indices), std::end(indices), vendor_pci_id, [](const id_pair_t& left, const int64_t& right) { return left.id < right; });
			if (it == std::end(indices) || it->id != vendor_pci_id) {
				return { "unknown", "unknown" };
			}
			const pci_vendor_info_t& vendor = vendors[it->index];
			auto deviceit = std::lower_bound(std::begin(vendor.device_indices), std::end(vendor.device_indices), device_pci_id, [](const std::size_t& left, const int64_t& right) { return devices[left].pci_id < right; });
			if (deviceit == std::end(vendor.device_indices) || devices[*deviceit].pci_id != device_pci_id) {
				return { vendor.name.arr, "unknown" };
			}
			const pci_device_info_t& device = devices[*deviceit];
			return { vendor.name.arr, device.name.arr };
		}

)";
}

bool read_pci_ids_hex(std::ifstream& input, int64_t& streamid) {
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

bool read_pci_ids_name(std::ifstream& input, std::string& streamname, std::string& poststreamname) {
	input >> streamname;
	if(!input) {
		return false;
	}
	std::getline(input, poststreamname);
	if(!input) {
		return true;
	}
	streamname.append(poststreamname);
	for(auto it = streamname.find('"'); it != std::string::npos; it = streamname.find('"', it + 2)) {
		streamname.replace(it, 1, R"(\")", 2);
	}
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

int main(int argc, char* argv[]) {
	// Use the pci ids file from
	// https://github.com/pciutils/pciids/blob/master/pci.ids
	std::string file = "pci.ids";
	if(argc > 1 && argv[1] != nullptr) {
		file = argv[1];
	}

	std::string outfile = "pci.generated.cpp";

	std::vector<vendor_t> vendor_id_indices{};
	std::vector<device_t> device_id_indices{};
	std::vector<std::string> vendor_device_names{};
	device_id_indices.reserve(15000);
	vendor_id_indices.reserve(2500);
	vendor_device_names.reserve(15000 + 2500);

	std::ifstream input(file);

	std::string streamname;
	std::string poststreamname;
	int64_t streamid;
	for(bool freeze = false; input;) {
		char c = '#';
		c      = input.peek();
		if(!input) {
			break;
		}
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
			default:
				input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				continue;
				break;
		}
	}

	std::sort(vendor_id_indices.begin(), vendor_id_indices.end(), [](auto& a, auto& b) { return a.pci_id < b.pci_id; });

	std::ofstream out(outfile);

	out << R"(// infoware - C++ System information Library
//
// Written in 2016 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Generated from )";
	out << file;
	out << R"(. Typically retrieved from https://github.com/pciutils/pciids/blob/master/pci.ids
		
#include <algorithm>
#include <infoware/detail/pci.hpp>
#include <utility>

)";

	out << R"(namespace iware {
	namespace detail {

		struct id_pair_t { int64_t id; std::size_t index; };
		template <typename T>
		struct cheap_view {
			T* arr; std::size_t arr_size; 
			T* begin() const { return arr; } 
			T* end() const { return arr + arr_size; }
		};
		struct pci_vendor_info_t { int64_t pci_id; cheap_view<const char> name; std::initializer_list<const int64_t> device_indices; };
		struct pci_device_info_t { int64_t pci_id; cheap_view<const char> name; };

)";

	write("identify_device_pci", out, vendor_id_indices, device_id_indices, vendor_device_names);

	out <<
	    R"(		std::string identify_vendor(int64_t pci_id) noexcept {
			return std::move(identify_device_pci(pci_id, 0).first);
		}
)";


	out <<
	    R"(	}
}
)";

	return 0;
}