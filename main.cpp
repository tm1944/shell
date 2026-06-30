#include <iostream>
#include <string>

int main() {
	bool continue_shell = true;
	while(continue_shell){
		std::string user_input;
		std::cin >> user_input;
		if (user_input == "exit"){
			continue_shell = false;
		}
	}
	return 0;
}
