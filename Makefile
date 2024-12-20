all :
	c++ main.cpp  srcs/Server.cpp -o irc srcs/Client.cpp

fclean :
	rm irc

re : all

.PHONY : all re fclean