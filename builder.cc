/*
 * Headstart for Ostermann's shell project
 *
 * Shawn Ostermann -- Sept 11, 2022
 */
 
#include <string>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
using namespace std;

// types and definitions live in "C land" and must be flagged
extern "C"
{
#include "parser.tab.h"
#include "bash.h"
    extern "C" void yyset_debug(int d);
}

void debug_print_line (struct command *pcmd);
string var_expansion(string var);
void echo(struct command *pcmd);
void load_path(string temp_path);
string whence(char *progname);
void get_ps1();
void add_var(char *cmd, const char *var);
int count_dirs(const char *dirs);
string strip_var(string var);
string strip_quotes(string var);
int file_redir(struct command *pcmd);
void clean_arguments(struct command *pcmd);

// global debugging flag
int debug = 0;
string exit_string = "exit";
//global environment path vector
vector<string> environment;

int main(
    int argc,
    char *argv[])
{   
    
    load_path(getenv("PATH"));
    if (debug)
        yydebug = 1; /* turn on ugly YACC debugging */

    /* parse the input file */
    yyparse();

    exit(0);
}

void debug_print_line (struct command *pcmd) {
    struct command *curr = pcmd;
    int pipe_count = 1;
    //NEED THIS BECAUSE FOR SOME REASON THE IF CONDITIONS FOR "PIPE" WONT WORK
    //BUT IT WORKS WITH JUST THE VARIABLE THAT IT EQUAL TO "PIPE"?????????????????????????
    //TOOK ME 40 MINUTES TO FIGURE OUT WTF
    string pipe_string = "PIPE";
    if (curr->error_check != 1) {
        printf("========== line %d ==========\n", lines);
        while (curr != nullptr) {
            printf("Command name: '%s'\n", curr->command);
            printf("    argv[0]: '%s'\n", curr->command);
            for (int j = 0; j < curr->argc; j++) {
                printf("    argv[%d]: '%s'\n", j + 1, curr->argv[j]);
            }
            if (curr->infile == NULL) {printf("  stdin:  UNDIRECTED\n");}
            else {
                if (curr->infile == pipe_string) {
                    printf("  stdin:  %s%d\n", curr->infile, pipe_count - 1);
                }
                else {printf("  stdin:  '%s'\n", curr->infile);}
            }
            if (curr->outfile == NULL) {printf("  stdout: UNDIRECTED\n");}
            else {
                if (curr->outfile == pipe_string) {
                    printf("  stdout: %s%d\n", curr->outfile, pipe_count);
                    pipe_count++;
                }
                else {
                    if (curr->output_append != 1) {
                        printf("  stdout: '%s'\n", curr->outfile);
                    }
                    else {
                        printf("  stdout: '%s' (append)\n", curr->outfile);
                    }
                }
            }
            if (curr->errfile == NULL) {printf("  stderr: UNDIRECTED\n");}
            else {
                if (curr->error_append != 1) {
                        printf("  stderr: '%s'\n", curr->errfile);
                }
                else {
                        printf("  stderr: '%s' (append)\n", curr->errfile);
                    }
                }
            curr = curr->next;
        }
        printf("\n");
    }

}

void doline(
    struct command *pcmd)
{   
    int pid;
    string command;

    int stdout_dup = dup(STDOUT_FILENO);
    int stdin_dup = dup(STDIN_FILENO);
    int stderr_dup = dup(STDERR_FILENO);


    if (pcmd->argc > 100) {
        cout << "Argument list too long" << endl;
        return;
    }
    //if error has occured just return
    if (pcmd->error_check == 1) {return;}
    while (pcmd != NULL) {
        command = pcmd->command;
        int file_redir_flag = file_redir(pcmd);
        //if redir flag -1 then just break
        if (file_redir_flag == -1) {break;}
        //if given exit string exit without error
        if (pcmd->command == exit_string) {_exit(0);}
        //if given env variable add it
        else if (pcmd->env_var == 1) {
            add_var(pcmd->command, pcmd->argv[1]);
            break;
        }
        //if echo
        else if (command == "echo" || command == "/bin/echo") {
            echo(pcmd);
            break;
        }
        //if absolute path given
        else if (command.substr(0, 2) == "./") {
            clean_arguments(pcmd);
            const char *path = command.substr(2).c_str();
            pid = fork();
            if (pid == 0) {
                //if access is okay execute
                if (access(path, X_OK) == 0) {execv(path, pcmd->argv);}
                //if not raise error
                else {perror(pcmd->command);}
            }
        }
        //if change dir
        else if (command == "cd") {
            //if too many args
            if (pcmd->argc >= 3) {
                cout << "'cd' requires exactly one argument" << endl;
                break;
            }
            else if (pcmd->argc == 1) {
                //if can't get env raise error
                if (chdir(getenv("HOME")) == -1) {perror(pcmd->argv[1]);}
                break;
            }
            else {
                //if can't get env raise error
                if (chdir(pcmd->argv[1]) == -1) {perror(pcmd->argv[1]);}
                break;
            }
        }
        //none of our options above
        else {
            pid = fork();
            if (pid == 0) {
                clean_arguments(pcmd);
                string str_path = whence(pcmd->command);
                auto path = str_path.c_str();
                execv(path, pcmd->argv);
                perror(pcmd->command);
            }
        }
        
        pcmd = pcmd->next;
    }
    wait(NULL);
    get_ps1();
    fflush(stdout);
    dup2(stdout_dup, STDOUT_FILENO);
    dup2(stdin_dup, STDIN_FILENO);
    dup2(stderr_dup, STDERR_FILENO);
}

void echo(struct command *pcmd) {
    string exp_var;
    for (int i = 1; i < pcmd->argc; i++) {
        string temp(pcmd->argv[i]);
        //expand argument if neede
        string argument = var_expansion(temp);
        printf("%s", argument.c_str());
        if (i != pcmd->argc - 1) {
            printf(" ");
        }
    }
    cout << endl;
}

string whence(char *progname) {
    string temp_progname = progname;
    char pathbuf[PATH_MAX + 1];
    if (temp_progname.find("/") == string::npos) {
        for (size_t i = 0; i < environment.size(); ++i) {
            snprintf(pathbuf, sizeof(pathbuf), "%s/%s", environment[i].c_str(), progname);
            if (access(pathbuf, X_OK) == 0) {
                return pathbuf;
                break;
            }
            if (i == environment.size() - 1) {
                printf("%s: command not found\n", progname);
            }
        }
    }
    else if (access(progname, X_OK) == 0 && temp_progname.find("/") != string::npos) {return progname;}
    else {perror(progname);}
    //shouldn't reach here but if we do exit with error
    exit(-1);
}

void add_var(char *cmd, const char *var) {
    string temp_command = cmd;
    string temp_var = var;
    temp_var = var_expansion(temp_var);
    var = temp_var.c_str();
    if (temp_command.substr(0, 4) == "PATH")
    {
        if (count_dirs(var) > 100)
        {
            cout << "Path directory list too long" << endl;
            return;
        }
    }
    //cant find a way to use string here, need char * for putenv
    //string string_command = cmd;
    //string_command = string_command + "=";
    strcat(cmd, "=");
    if (var != NULL) {strcat(cmd, var);}
    putenv(cmd);
    //if we have path load it into environment
    if (temp_command.substr(0, 4) == "PATH") {load_path(temp_command);}
}

void get_ps1() {
    if (getenv("PS1") != NULL) {printf("%s", getenv("PS1"));}
    //else do nothing 
}

string var_expansion(string str_var) {
    string expanded_var = "";
    string temp;
    int index;
    string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string lower = "abcdefghijklmnopqrstuvwxyz";
    string digits = "0123456789";
    string specials = "_{}$";
    //only uppercase, lowercase, digits, and _{}$ are valid characters in env variables
    string valid = upper + lower + digits + specials;
    if (str_var.at(0) == '\'') {return str_var.substr(1, str_var.length() - 2);}
    else if (str_var.find('$') == string::npos) {expanded_var = strip_quotes(str_var);}
    else {
        str_var = strip_quotes(str_var);
        temp = str_var;
        for (size_t i = 0; i < str_var.length(); i++) {
            //if env variable find first not valid character
            if (str_var[i] == '$') {
                index = str_var.find_first_not_of(valid, i);
                if (index == -1) {
                    index = str_var.find_first_of("$", i + 1);
                    if (index == -1) {
                        //if all valid set index to end
                        index = str_var.length();
                    }
                }
                temp = str_var.substr(i, index - i);
                //strip variable just to get the inside
                temp = strip_var(temp);
                if (getenv(temp.c_str()) == NULL) {
                    expanded_var += "";
                    i = index - 1;
                }
                else {
                    expanded_var += string(getenv(temp.c_str()));
                    i = index - 1;
                }
            }
            //if not $ append it
            else {expanded_var += str_var[i];}
        }
    }
    return expanded_var;
}

int count_dirs(const char *dirs) {
    //dirs separated by colons so count them
    int count = 0;
    for (size_t i = 0; i < strlen(dirs); i++)
    {
        if (dirs[i] == ':')
        {
            count++;
        }
    }
    return count;
}

void load_path(string temp_path) {
    //clear current env
    environment.clear();
    int prefix_index = 0;
    //get path
    string PATH = getenv("PATH");
    string temp_string = "";
    for (size_t i = 0; i < PATH.length(); i++) {
        //if end of dir, add to environment increase index and reset temp_string
        if (PATH[i] == ':') {
            environment.push_back(temp_string);
            prefix_index++;
            temp_string = "";
        }
        //if not : push back char to holder string
        else {temp_string += PATH[i];}
    }
    environment.push_back(temp_string);
}

string strip_var(string var) {
    string stripped_var = var;
    //get rid of the ${}
    if (var.substr(0, 2) == "${") {
        stripped_var = var.substr(2, var.length() - 3);
    }

    else {
        stripped_var = var.substr(1, var.length() - 1);
    }
    return stripped_var;
}

string strip_quotes(string var) {
    string stripped_var = var;
    //get rid of quotes
    if (var.substr(0, 1) == "\'" || var.substr(0, 1) == "\"") {
        stripped_var = var.substr(1, var.length() - 2);
    }
    return stripped_var;
}

int file_redir(struct command * pcmd) {
    //if there is an infile
    if (pcmd->infile != NULL) {
        string temp = pcmd->infile;
        temp = var_expansion(temp);
        auto filename = temp.c_str();
        int fd = open(filename, O_RDONLY);
        if (fd == -1) {
            perror(filename);
            return -1;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    //if there is an outfile
    if (pcmd->outfile != NULL)
    {
        int fd;
        string temp(pcmd->outfile);
        temp = var_expansion(temp);
        const char *filename = temp.c_str();
        if (pcmd->output_append) {
            fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
        }
        else {
            fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
        }
        if (fd == -1) {
            perror(filename);
            return -1;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    //if there is an errfile
    if (pcmd->errfile != NULL) {
        int fd;
        string temp(pcmd->errfile);
        temp = var_expansion(temp);
        const char *filename = temp.c_str();
        if (pcmd->error_append) {
            fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
        }
        else {
            fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
        }
        if (fd == -1) {
            perror(filename);
            return -1;
        }
        dup2(fd, STDERR_FILENO);
        close(fd);
    }
    return 1; 
}

void clean_arguments(struct command * pcmd) {
    //expand all variables that can be expanded/need to be expanded
    for (int i = 0; i < pcmd->argc; i++) {
        string temp = pcmd->argv[i];
        temp = var_expansion(temp);
        //cant just set equal because const char * and char *
        pcmd->argv[i] = strdup(temp.c_str());
    }
}