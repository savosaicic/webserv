# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abiju-du <abiju-du@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/05 16:48:12 by sasaicic          #+#    #+#              #
#    Updated: 2022/11/01 12:53:02 by sasaicic         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := webserv
CXX := c++
CPPFLAGS := -Iincludes
CXXFLAGS := -std=c++98 -Wall -Wextra -Werror
SRCS := webserv.cpp \
		TCPListener.cpp \
		Config.cpp\
		VirtualServer.cpp\
		Location.cpp
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d) 
OBJDIR := obj
DEPDIR := dep
SUFFIXES += .d

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

debug: CXXFLAGS += -g -DDEBUG=1
debug: $(NAME)

end_to_end: $(NAME)
	@ HARNESS_OPTIONS=c WEBSERV_EXE_PATH=$(PWD)/webserv \
		PERLLIB=$(PERLIB):$(PWD)/tests/lib \
		PERLLIB5=$(PERLIB5):$(PWD)/tests/lib \
					./tests/integration/run_tests.pl ./tests/integration

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
