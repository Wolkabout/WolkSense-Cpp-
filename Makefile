# Copyright 2016 WolkAbout Technology s.r.o.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

TARGET	= wolksense-cpp-example

CLANG_FORMAT ?= clang-format-3.8

Q ?= @

INC_DIR = include
SRC_DIR = src
LIB_DIR = lib
OBJ_DIR = obj

SRC 	= $(wildcard $(SRC_DIR)/*.cpp)
OBJ 	= $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC:.cpp=.o))

CXX = ${PREFIX}g++
CC  = ${PREFIX}gcc
LD  = ${PREFIX}ld
AS  = ${PREFIX}gcc -x assembler-with-cpp
CP  = ${PREFIX}objcopy
OD  = ${PREFIX}objdump
SZ  = ${PREFIX}size

CFLAGS	= -Wall -std=c++11 -O0 -g
CFLAGS += -I./$(INC_DIR)
CFLAGS += -I./dependencies/build/$(INC_DIR)

LFLAGS  = -L./out/$(LIB_DIR)

LFLAGS += -Wl,-rpath,../$(LIB_DIR) -lrestclient-cpp
LFLAGS += -Wl,-rpath,../$(LIB_DIR) -lssl
LFLAGS += -Wl,-rpath,../$(LIB_DIR) -lcrypto
LFLAGS += -Wl,-rpath,../$(LIB_DIR) -lmqttpp
LFLAGS += -Wl,-rpath,../$(LIB_DIR) -lpaho-mqtt3as
LFLAGS += -lpthread

all : dependencies mkobjdir formatsource $(TARGET)

dependencies :
	@(cd ./dependencies; sh ./download.sh && sh ./make_install.sh; cd ..)

$(TARGET) : $(OBJ)
	@echo "  [LD]      $@"
	$(Q)$(CXX) -o out/bin/$@ $^ $(LFLAGS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@echo "  [CXX]     $<"
	$(Q)$(CXX) $(CFLAGS) -c  $< -o $@

help :
	@echo "  [SRC]:      $(SRC)"
	@echo
	@echo "  [OBJ]:     $(OBJ)"

clean :
	@echo "  [RM]    $(OBJ)"
	@$(RM) $(OBJ)
	@echo
	@echo "  [RM]     $(TARGET) "
	@$(RM) out/bin/$(TARGET)

formatsource :
	@$(CLANG_FORMAT) -i -sort-includes -style=file include/*.h | true
	@$(CLANG_FORMAT) -i -sort-includes -style=file src/*.cpp   | true

mkobjdir :
	@mkdir -p obj

.PHONY : all dependencies help clean formatsource mkobjdir
