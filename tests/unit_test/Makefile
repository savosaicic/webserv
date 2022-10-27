NAME := unit_tester
CXX := c++
CPPFLAGS := -Iincludes -I../../includes
CXXFLAGS := -std=c++11 -Wall -Wextra -Werror
LDFLAGS := -Ltests/unit_test/lib -lgtest
SRCS := main.cpp \
		TCPListenerTest.cpp		
SRCS += $(SOURCES)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d) 
OBJDIR := obj
DEPDIR := dep
SUFFIXES += .d

vpath %.cpp ../../srcs srcs
vpath %.o $(OBJDIR)
vpath %.d $(DEPDIR)

all: $(NAME)

$(NAME): $(DEPS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(addprefix $(OBJDIR)/, $(OBJS))

%.o: %.cpp $(DEPS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $(OBJDIR)/$@

%.d: %.cpp
	$(CXX) $(CPPFLAGS) -MM -MF $(DEPDIR)/$@ $<

ifneq "$(MAKECMDGOALS)" "clean"
 -include $(addprefix $(DEPDIR)/, $(DEPS))
endif

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(DEPS): | $(DEPDIR)

$(DEPDIR):
	mkdir -p $(DEPDIR)

clean:
	rm -rf $(OBJDIR) $(DEPDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all debug test clean fclean re