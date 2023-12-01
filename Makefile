NAME= ft_IRC
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)

%.o: %.cpp
	c++ -Wall -Wextra -Werror -std=c++98 -I./ -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	c++ -Wall -Wextra -Werror -std=c++98 $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
