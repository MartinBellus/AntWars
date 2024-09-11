#program name
PROGRAM=../srv
#build file
OBJDIR = build

#c++ compiler
CC = g++

CFLAGS = -std=c++23 -O3 -g3 -Wall -c -Wno-reorder
CSOURCES := $(wildcard **/src/*.cpp) $(wildcard *.cpp)
COBJECTS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(subst /src/,/,$(CSOURCES)))
reverse =  $(subst /,/src/,$(patsubst $(OBJDIR)/%.o,%.cpp,$1))

#linker
LINKER = g++
LFLAGS =
INCLUDE = $(addprefix -I, $(wildcard **/include))


# colors
Color_Off='\033[0m'
Black='\033[1;30m'
Red='\033[1;31m'
Green='\033[1;32m'
Yellow='\033[1;33m'
Blue='\033[1;34m'
Purple='\033[1;35m'
Cyan='\033[1;36m'
White='\033[1;37m'


all: $(PROGRAM)

$(PROGRAM): $(COBJECTS) $(NOBJECTS)
	@$(LINKER) $(COBJECTS) $(NOBJECTS) -o $@ $(LFLAGS)
	@echo -e $(Yellow)"Linking complete!"$(Color_Off)


.SECONDEXPANSION:

$(COBJECTS): $$(call reverse,$$@)
	@mkdir -p $(dir $@)
	@echo -e $(Blue)"C++ compiling "$(Purple)$<$(Color_Off)

	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	@echo -e $(Blue)"C++ compiled "$(Purple)$<$(Blue)" successfully!"$(Color_Off)

clean:
	@rm -rf $(PROGRAM) $(OBJDIR)
	@echo -e $(Cyan)"Cleaning Complete!"$(Color_Off)

.PHONY: all clean
