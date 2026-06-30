#include <iostream>
#include <string>
#include <sstream>
#include <vector>




// Input a string return vector<string> using whitespace as delimeter
std::vector<std::string> stringToTokens(const std::string& inputString){
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
		std::getline(std::cin,user_input);

		//Shell Conditionals
		if (user_input == "exit"){
			continue_shell = false;
		}else//user inputted actual commands
		{ 	
			for(const std::string& t : stringToTokens(user_input)){
				std::cout << "[" << t << "] ";
			}
			std::cout << std::endl;
		}
	}
	return 0;
}
