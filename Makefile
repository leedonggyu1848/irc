NAME		=	ircserv
CXX			=	c++ -fsanitize=address -g3
RM			=	rm -rf
CXXFLAGS	=	-std=c++98 -g -MMD # -Wall -Werror -Wextra
SRC_DIR		=	srcs/
CINCLUDE	=	-Isrcs/ -Iincludes/

SRCS		=	$(wildcard $(SRC_DIR)*.cpp) $(wildcard $(SRC_DIR)**/*.cpp)
OBJS		=	$(SRCS:.cpp=.o)
DEPS		=	$(SRCS:.cpp=.d)

all			:	$(NAME)

%.o			:	%.cpp
				$(CXX) $(CXXFLAGS) $(CINCLUDE) -c $< -o $@

$(NAME)		:	$(OBJS)
				$(CXX) $(CXXFLAGS) $^ -o $@

clean		:
				$(RM) $(OBJS)
				$(RM) $(DEPS)

fclean		:
				$(MAKE) clean
				$(RM) $(NAME)

re			:
				$(MAKE) fclean
				$(MAKE) all

.PHONY		:	all clean re fclean
