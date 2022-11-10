NAME := webserv
CXX := c++
CPPFLAGS := -Iincludes
CXXFLAGS := -std=c++98 -Wall -Wextra -Werror
SRCS := webserv.cpp \
		TCPListener.cpp \
		Config.cpp \
		VirtualServer.cpp \
		Location.cpp \
		IPAdress.cpp \
		TCPStream.cpp \
		HTTPRequest.cpp \
		TCPStreamManager.cpp \
		CGIRequest.cpp \
		HTTPServer.cpp \
		HTTPStream.cpp \
		utils.cpp \

OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d) 
OBJDIR := obj
DEPDIR := dep
SUFFIXES += .d
UNIT_TEST_DIR := tests/unit
UNIT_TESTS := $(wildcard $(UNIT_TEST_DIR)/*.cpp)
UNIT_TEST_BUILD_DIR := build/test/unit
ALL_UNIT_TESTS := $(UNIT_TEST_BUILD_DIR)/all_tests

vpath %.cpp srcs
vpath %.o $(OBJDIR)
vpath %.d $(DEPDIR)

all: $(NAME)

$(NAME): $(DEPS) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(addprefix $(OBJDIR)/, $(OBJS))

%.o: %.cpp $(DEPS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $(OBJDIR)/$@

%.d: %.cpp
	$(CXX) $(CPPFLAGS) -MM -MF $(DEPDIR)/$@ $<

ifneq "$(MAKECMDGOALS)" "clean"
 -include $(addprefix $(DEPDIR)/, $(DEPS))
endif

debug: CXXFLAGS += -g -DDEBUG=1 -DWS_DEBUG=1
debug: $(NAME)

end_to_end: $(NAME)
	@ HARNESS_OPTIONS=c WEBSERV_EXE_PATH=$(PWD)/webserv \
		PERLLIB=$(PERLIB):$(PWD)/tests/lib \
		PERLLIB5=$(PERLIB5):$(PWD)/tests/lib \
					./tests/integration/run_tests.pl ./tests/integration

unit_test: $(ALL_UNIT_TESTS)
	./$<

$(ALL_UNIT_TESTS): $(UNIT_TESTS) $(SRCS)
	bash support/test/unit/make.sh

$(UNIT_TEST_BUILD_DIR)%_tests: $(UNIT_TEST_DIR)/%.cpp
	bash support/test/unit/make.sh

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(DEPS): | $(DEPDIR)

$(DEPDIR):
	mkdir -p $(DEPDIR)

clean:
	rm -rf $(OBJDIR) $(DEPDIR) build

fclean: clean
	$(RM) -rf $(NAME)

re: fclean all

.PHONY: all debug test clean fclean re
