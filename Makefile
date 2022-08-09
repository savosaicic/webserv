# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sasaicic <sasaicic@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/05 16:48:12 by sasaicic          #+#    #+#              #
#    Updated: 2022/08/09 07:49:02 by sasaicic         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := webserv
CXX := c++
CPPFLAGS := -Iincludes
CXXFLAGS := -std=c++98 -Wall -Wextra -Werror
SRCS := SocketServer.cpp TCPServer.cpp HTTPServer.cpp TCPConnection.cpp \
		webserv.cpp debug.cpp utils.cpp
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

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $(OBJDIR)/$@

ifneq ($(filter-out clean,$(MAKECMDGOALS)),)
-include $(addprefix $(DEPDIR)/, $(DEPS))
endif

%.d: %.cpp
	$(CXX) $(CPPFLAGS) -MM -MF $(DEPDIR)/$@ $<

debug: CXXFLAGS += -g -DDEBUG=1
debug: $(NAME)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(DEPS): | $(DEPDIR)

$(DEPDIR):
	mkdir -p $(DEPDIR)

test: $(NAME)
	./run_tests.sh

clean:
	rm -rf $(OBJDIR) $(DEPDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all debug test clean fclean re
