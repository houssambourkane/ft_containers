# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hbourkan <hbourkan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/21 12:22:05 by hbourkan          #+#    #+#              #
#    Updated: 2022/12/10 18:41:13 by hbourkan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAP_BIN = map_bin

VECTOR_BIN = vector_bin

STACK_BIN = stack_bin

FLAGS = -Wall -Werror -Wextra

VERSION = -std=c++98

COMPILER = c++

TESTER_PATH = ft_containers_testers

# Color
BLACK = \e[30m
RED = \e[31;52m
RED_BOLD = \e[1;31m
GREEN = \e[32m
GREEN_BOLD = \e[1;32m
YELLOW = \e[33m
YELLOW_UNDERLINE = \e[4;33m
BLUE = \e[34m
BLUE_BOLD = \e[1;34m
MAGENTI = \e[35m
MAGENTI_BOLD = \e[1;35m
CYAN = \e[36m
WHITE = \e[37m
RESET= \e[0m

# CPP files:
MAP_FILE = $(TESTER_PATH)/map_test.cpp
VECTOR_FILE = $(TESTER_PATH)/vector_test.cpp
STACK_FILE = $(TESTER_PATH)/stack_test.cpp

all: $(MAP_BIN) $(VECTOR_BIN) $(STACK_BIN)
	
# MAP COMPILATION
$(MAP_BIN):
	@printf "✅ $(GREEN_BOLD)Compiling map Container Tester$(RESET)\n"
	@ $(COMPILER) $(FLAGS) $(VERSION) $(MAP_FILE) -o $(MAP_BIN)
	@printf "✅ $(BLUE_BOLD) map Compiled$(RESET)\n"

# VECTOR COMPILATION
$(VECTOR_BIN):
	@printf "✅ $(GREEN_BOLD)Compiling vector Container Tester$(RESET)\n"
	@ $(COMPILER) $(FLAGS) $(VERSION) $(VECTOR_FILE) -o $(VECTOR_BIN)
	@printf "✅ $(BLUE_BOLD) vector Compiled$(RESET)\n"

# STACK COMPILATION
$(STACK_BIN):
	@printf "✅ $(GREEN_BOLD)Compiling stack Container Tester$(RESET)\n"
	@ $(COMPILER) $(FLAGS) $(VERSION) $(STACK_FILE) -o $(STACK_BIN)
	@printf "✅ $(BLUE_BOLD) stack Compiled$(RESET)\n"
	
map:
	@./$(MAP_BIN)

vector:
	@./$(VECTOR_BIN)

stack:
	@./$(STACK_BIN)
	
clean:

fclean: clean
	@rm -f $(MAP_BIN) $(VECTOR_BIN) $(STACK_BIN)
	@printf "🗑 $(RED_BOLD) binaries successfully deleted\n"

re: fclean all