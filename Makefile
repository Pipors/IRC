all :
	c++ main.cpp srcs/Client.cpp srcs/Server.cpp -o irc

fclean :
	rm irc

re : all

.PHONY : all re fclean