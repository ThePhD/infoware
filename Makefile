# infoware - C++ System information Library
#
# Written in 2017 by nabijaczleweli <nabijaczleweli@gmail.com> and ThePhD <phdofthehouse@gmail.com>
#
# To the extent possible under law, the author(s) have dedicated all copyright and related
# and neighboring rights to this software to the public domain worldwide. This software is
# distributed without any warranty.
#
# You should have received a copy of the CC0 Public Domain Dedication along with this software.
# If not, see <http://creativecommons.org/publicdomain/zero/1.0/>


include configMakefile


SOURCES := $(sort $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp))
OBJECTS := $(patsubst src/%.cpp,$(BUILD)/obj/%$(OBJ),$(SOURCES))
EXAMPLES := $(wildcard examples/*.cpp)


.PHONY : clean all stlib examples

all : stlib examples

clean :
	rm -rf $(BUILD)

examples : $(patsubst examples/%.cpp,$(BUILD)/examples/%$(EXE),$(EXAMPLES))
stlib : $(BUILD)/libinfoware$(ARCH)


$(BUILD)/libinfoware$(ARCH) : $(OBJECTS)
	$(AR) crs $@ $^


$(BUILD)/obj/%$(OBJ) : src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) $(CXXAR_LIB) $(PIC) -Iinclude -c -o$@ $^

$(BUILD)/examples/%$(EXE) : examples/%.cpp stlib
	@mkdir -p $(dir $@)
	$(CXX) $(CXXAR) -Iinclude -o$@ $< -L$(BUILD) -linfoware $(LDAR)
