#include <iostream>
#include <string>
#include <sstream>



std::vector<std::string> stringToTokens(std::string& inputString){
	std::vector<std::string> tokens;
	std::istringstream str(inputString);
	std::string token;
	while(str >> token){
		tokens.push_back(token);
	}

	return tokens;
} 


int main() {
	bool continue_shell = true;
	while(continue_shell){
		// Basic UserInput 
		std::string user_input;
		std::cin >> user_input;
		
		//Shell Conditionals
		if (user_input == "exit"){
			continue_shell = false;
		}
	}
	return 0;
}
