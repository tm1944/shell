#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/param.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>


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


// makes io_vec which is a vector<vector<string>> 
// tokens ={"echo","hello",">","out.txt"}
// to:
// {{">","out.txt"}}
std::vector<std::vector<std::string>> make_io_vec(std::vector<std::string> tokens){
			std::vector<std::vector<std::string>> io_vec;

				int outer_index = 0;
				for(int i = 0; i < tokens.size();i++){
						if(
								tokens[i] == "<" || tokens[i] == ">" || tokens[i] == ">>"
						){
								// first instance of the pipes and we push them 
								// and the next arg into the io_vec
							io_vec.push_back({});
							io_vec[outer_index].push_back(tokens[i]);
							if(i + 1 < tokens.size()){
									io_vec[outer_index].push_back(tokens[i+1]);
								}
							outer_index++;
						}
				}
				return io_vec;
}



//removes all values from tokens vec that is the in the io_vec
std::vector<std::string> stripped_tokens(std::vector<std::string> default_vector){
	int i = 0;
	std::vector<std::string> stripped_tokens;
	while (i < default_vector.size()){
					if(
							default_vector[i] != "<" && default_vector[i] != ">" && default_vector[i] != ">>"
						)
					{
						stripped_tokens.push_back(default_vector[i]);
						i++;
					}else{
						i+=2; //skip the next one cause thats the file 
					}
	}
	return stripped_tokens;
}


void io_filedescriptors(std::vector<std::vector<std::string>> io_vec){
	//opens the fd required for the program 
	// what fd to open etc
	int fd;
	for (const std::vector<std::string>& t : io_vec){
		if (t[0] == ">"){
			fd = open(t[1].c_str(),O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if(fd != -1){
				dup2(fd,1);
				close(fd);
			}else{
				perror("io_filedescriptors: ");
				_exit(1);
			}
		}else if(t[0] == "<"){
			fd = open(t[1].c_str(),O_RDONLY); 
			if(fd != -1){
				dup2(fd,0);
				close(fd);
			}else{
				perror("io_filedescriptors: ");
				_exit(1);
			}
		}else if(t[0] == ">>"){
			fd = open(t[1].c_str(),O_WRONLY | O_CREAT | O_APPEND , 0644);
			if(fd != -1){
				dup2(fd,1);
				close(fd);
			}else{
				perror("io_filedescriptors: ");
				_exit(1);
			}
		}
	}
}



void exec_command_via_child(char *const argv[],std::vector<std::vector<std::string>> io_vec){
	//fork -> exec -> waitpid
	pid_t pid;
	pid = fork(); // creating the child process

	
	if (pid == -1){
		std::cerr << "Fork Failed!"  << std::endl;
	}else if(pid == 0){
		//CHILD PROCESS
		//std::cout << "Child Process PID: " << pid << std::endl;
		// name of exec file, argv 

		io_filedescriptors(io_vec);
		if (execvp(argv[0],argv) == -1){
			std::cerr << "ERROR: command not found: " << argv[0] << std::endl;
		}
		_exit(1);

	}else{
		// PARENT PROCESS
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
				//strip the tokens vector of IO redirection
				//convert the stripped_tokens vector to argv_tokens
				//pass to exec_vp to exec the command
				std::vector<std::vector<std::string>> io_vec = make_io_vec(tokens);
				tokens = stripped_tokens(tokens);
				std::vector<char*> argv_tokens = make_argv(tokens);
				exec_command_via_child(argv_tokens.data(),io_vec);
			}
		}else{
			//user pressed enter withing typing anything in
			continue;
		}
	}
	return 0;
}
