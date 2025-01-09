#include <iostream>
#include <vector>

// std::string getRangeAsString(std::vector<std::string> vec, size_t start, size_t end, std::string delimiter) 
// {
// 	if (start > vec.size() || end > vec.size() || start > end) 
// 		throw std::out_of_range("Invalid range specified.");
// 	// Extract range and concatenate strings
// 	std::string result = "";
// 	std::vector<std::string>::iterator it = vec.begin() + start;
// 	while (it != vec.begin() + end) 
// 	{
// 		if (!result.empty()) 
// 			result += delimiter; // Add a space between words
// 		result += *it;
// 		it++;
// 	}
// 	return result;
// }


std::string getRangeAsString(std::vector<std::string> vec, std::vector<std::string>::iterator *it, size_t end, std::string delimiter) 
{
	if (*it >= vec.end() || end > vec.size()) 
		throw std::out_of_range("Invalid range specified.");
	// Extract range and concatenate strings
	std::string result = "";
	while (*it != vec.begin() + end) 
	{
		if (!result.empty()) 
			result += delimiter; // Add a space between words
		result += *(*it);
		it++;
	}
	return result;
}

int main()
{
	std::vector<std::string> vec;

	// std::cout << "*it" << "\n";
	vec.push_back("LS");
	vec.push_back("PASS");
	vec.push_back("1337");
	vec.push_back("network");
	vec.push_back("world");

	std::vector<std::string>::iterator it = vec.begin();
	// std::cout << *(*it) << "\n";

	std::cout << getRangeAsString(vec, &it, vec.size(), " ") << "\n"; 
	// std::cout << getRangeAsString(vec, 1, 2, " ").size(); 
}