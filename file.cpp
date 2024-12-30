
#include <iostream>
#include <vector>


//":" + cl[0].getNickName() + "!" + cl[0].getUserName() + "@" + cl[0].getIPadd() + " PRIVMSG " + sendTo + message + "\r\n";
//      |  |
//      |  |
//    __|  |__
//    \      /
//     \    /
//      \  /
//       \/
//:nick!user@clientip IP PRIVMSG 
//:nick!user@172.19.160.1.IP JOIN #anas * //PROTOCOL TO SEND JOIN A CHANNEL REQUEST





std::string printVectorWords(std::vector<std::string> vec, size_t pos)
{
	std::string message;
	while (pos != vec.size())
	{
		message = message + vec[pos] + " ";
		pos++;
	}
	return message;
}



int main()
{
	std::vector<std::string> vec;
	int it = 0;

	vec.push_back("15");
	vec.push_back("25");
	vec.push_back("35");
	vec.push_back("45");
	vec.push_back("55");
	vec.push_back("65");
	vec.push_back("75");
	vec.push_back("85");

	std::cout << printVectorWords(vec, 4) << " ";
}