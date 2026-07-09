#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/param.h>
#include <unistd.h>
#include <stdio.h>


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
		if (execvp(argv[0],argv) == -1){
			std::cerr << "ERROR: command not found: " << argv[0] << std::endl;
		}
		_exit(1);

	}else{
		//std::cout << "Parent Process PID: " << pid << std::endl;
		int status;
		waitpid(pid,&status,0);
		// pid of child, 0 means wait for provided pid and status holds how child died
		// ERROR MESSAGE FOR UNKNOWN command!
		if(WIFSIGNALED(status)){
			// program didn't run to completion via crash/error
			std::cout << "KILLED BY SIGNAL" << std::endl;
		}
	}

}


std::vector<char*> make_argv(std::vector<std::string> &tokens){
	//make the char* vector for execvp from vector<strings>
	std::vector<char*> args;
	//args.push_back(const_cast<char*>(tokens[0].c_str()));
	//args.push_back(const_cast<char*>(tokens[1].c_str()));
	for(const std::string& t : tokens){
		args.push_back(const_cast<char*>(t.c_str()));
	}
	args.push_back(NULL);
	return args;
}

void print_current_dir(){
	//implementation for pwd
	char buffer[MAXPATHLEN];
	if(getcwd(buffer,sizeof(buffer)) != nullptr){
		std::cout << buffer << std::endl;
	}else{
		perror("getcwd");
	}
	
}


int main() {
	bool continue_shell = true;
	while(continue_shell){
		// Basic UserInput 
		std::string user_input;
		std::cout << "$HELL % ";
		std::getline(std::cin,user_input);


		std::vector<std::string> tokens = stringToTokens(user_input);
		if (tokens.size() > 0){
			//Shell Conditionals
			if (tokens[0] == "exit"){
				continue_shell = false;
			}else if(tokens[0] == "pwd"){
				print_current_dir();
			}else if (tokens[0] == "cd"){
				if(tokens.size() == 1){
					// cd  -> if no other params we go to home dir
					// got to $HOME
					if (const char* home_dir = getenv("HOME")){
						if(chdir(home_dir) == -1){
							perror("chdir : ");
						}
					}
				}else if(tokens.size() > 1){
					// checking if second param is a valid directory
 					if(chdir(tokens[1].c_str()) == -1){
						perror("chdir : ");
					}
				}
			}else{
				//user inputted actual commands 	
				/* print out tokenized vector values
				for(const std::string& t : tokens){
					std::cout << "[" << t << "] ";
				}*/

				std::vector<std::string> io_vec;
				for(int i = 0; i < tokens.size();i++){
						if(
								tokens[i] == "<" || tokens[i] == ">" || tokens[i] == ">>"
						){
								// first instance of the pipes and we push them 
								// and the next arg into the io_vec
						io_vec.push_back(tokens[i]);
						if(i + 1 < tokens.size()){
								io_vec.push_back(tokens[i+1]);
						}
						}

				}
				// every arg in io_vec gets removed from tokens before 
				// passed to exec_vp
				// TODO: edge case if there is echo hello > hello
				// The std::remove will remove all instance the first argument "hello"
				// and echo will get no argument. future bug fix
				for(const std::string& t : io_vec){
						tokens.erase(std::remove(tokens.begin(),tokens.end(),t),tokens.end());

				}
				std::vector<char*> argv_tokens = make_argv(tokens);
				exec_command_via_child(argv_tokens.data());
			}
		}else{
			//user pressed enter withing typing anything in
			continue;
		}
	}
	return 0;
}
