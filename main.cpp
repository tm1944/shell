#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>



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



void exec_command_via_child(char *const argv[]){
	//fork -> exec -> waitpid
	pid_t pid;
	pid = fork(); // creating the child process
	if (pid == -1){
		std::cerr << "Fork Failed!"  << std::endl;
	}else if(pid == 0){
		//std::cout << "Child Process PID: " << pid << std::endl;
		// name of exec file, argv 
		execvp(argv[0],argv);
		_exit(1);

	}else{
		//std::cout << "Parent Process PID: " << pid << std::endl;
		int status;
		waitpid(pid,&status,0);
		// pid of child, 0 means wait for provided pid and status holds how child died
		// ERROR MESSAGE FOR UNKNOWN command!
		if(WIFEXITED(status)){
			int code = WEXITSTATUS(status);
			if (code != 0){
				// code not being 0 means that the code 
				// didn't exit/end via exit(0) or return 0;
				// code ran to completion but reported error
				std::cerr << "ERROR: command not found: " << argv[0] << std::endl;
			}
		}else if(WIFSIGNALED(status)){
			// program didn't run to completion via crash/error
			std::cout << "KILLED BY SIGNAL" << std::endl;
		}

	}

}


std::vector<char*> make_argv(std::vector<std::string> tokens){
	//make the char* vector for execvp from vector<strings>
	std::vector<char*> args;
	args.push_back(const_cast<char*>(tokens[0].c_str()));
	args.push_back(const_cast<char*>(tokens[1].c_str()));
	args.push_back(nullptr);
	return args;
}

int main() {
	bool continue_shell = true;
	while(continue_shell){
		// Basic UserInput 
		std::string user_input;
		std::cout << "$HELL % ";
		std::getline(std::cin,user_input);

		//Shell Conditionals
		if (user_input == "exit"){
			continue_shell = false;
		}else//user inputted actual commands
		{ 	
			std::vector<std::string> tokens = stringToTokens(user_input);
			/* print out tokenized vector values
			for(const std::string& t : tokens){
				std::cout << "[" << t << "] ";
			}*/
			exec_command_via_child(make_argv(tokens).data());
		}
	}
	return 0;
}
