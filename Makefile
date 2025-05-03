# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/03 20:58:00 by hoskim            #+#    #+#              #
#    Updated: 2025/05/03 21:15:58 by hoskim           ###   ########seoul.kr   #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = clang
CFLAGS = -Wall -Wextra -Werror -Iinclude
LDFLAGS = -pthread

SCR_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SCR_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | %(OBJ_DIR)
		%(CC) %(CFLAGS) -c $< -o $@

$(OBJ_DIR):
		mkdir -p $(OBJ_DIR)

clean:
		rm -rf $(OBJ_DIR)

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
