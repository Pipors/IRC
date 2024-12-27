#include <iostream>
#include <sstream>
#include <vector>
#include <string>

// Function to split a string into words and return as a vector


int main() {
    std::string input = "This is a sample string with words.";
    
    // Call the function and get the words
    std::vector<std::string> words = getWords(input);
    
    // Print the words
    for (const std::string& word : words) {
        std::cout << word << std::endl;
    }

    return 0;
}
