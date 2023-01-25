# Project-2-F22

#  Project 2: Developing a Linux Shell
**NOTE** What follows is the only the introduction to the problem statement.  
**The full Project 2 description is [here](https://github.com/CIS-3207-F22/Project-2-f22/blob/main/Project%202%20Developing%20a%20Linux%20Shell.pdf)**.   

An **Introduction to Developing a Shells** for this project is found [here](https://github.com/CIS-3207-F22/Project-2-f22/blob/main/3207%20Introduction%20to%20Developing%20a%20Linux%20Shell.pdf).  

In this project, you’ll build a simple Unix/Linux shell. The shell is the heart of the command-line interface, and thus is central to the Linux/Unix/C programming environment. Mastering use of the shell is necessary to become proficient in this world; knowing how the shell itself is built is the focus of this project.  
There are three specific objectives to this assignment:
- To further familiarize yourself with the Linux programming environment.
- To learn how processes are created, destroyed, and managed.
- To gain exposure to the necessary functionality in shells.
# Overview
In this assignment, you will implement a **command line interpreter (CLI)** or, as it is more commonly known, a **shell**. The shell should operate in this basic way: when you type in a command (in response to its prompt), the shell creates a child process that executes the command you entered and then prompts for more user input when it has finished.  

The shells you implement will be similar to, but simpler than, the one you run every day in Linux. If you don’t know what shell you are running, it’s probably bash. One thing you should do on your own time is learn more about your shell, by reading the man pages or other online materials (and the associated Shell Introduction file).  
# Program Specifications
### Basic Shell:

### myshell  

Your basic shell, called *myshell* is fundamentally an interactive loop: it repeatedly prints a prompt myshell>  (note the space after the greater-than sign), parses the input, executes the command specified on that line of input, and waits for the command to finish. This is repeated until the user types *exit*
. The name of your final executable should be myshell.  

The shell can be invoked with either no arguments (interactive) or a single argument (batch0; anything else is an error. Here is the no-argument way:  
prompt> ./myshell  
myshell>   
At this point, myshell is running, and ready to accept commands. Type away!  
The mode above is called /interactive/ mode, and allows the user to type commands directly. The shell also supports a /batch mode/, which instead reads input from a batch file and executes commands found in the file. Here is how you run the shell with a batch file named  `batch.txt`
:  

` prompt> ./myshell batch.txt`  

There is a difference between batch and interactive modes: in interactive mode, a prompt is printed (myshell> ). In batch mode, no prompt should be printed during execution of commands.  

You should structure your shell such that it creates a process for each new command (the exceptions are /built-in commands/, discussed below). Your basic shell should be able to parse a command and run the program corresponding to the command. For example, if the user types 'ls -la /tmp', your shell should run the program `/bin/ls`
 with the given arguments
 ` - la`
 and ` /tmp`
 (how does the shell know to run ` /bin/ls`
? It’s something called the shell *path*; more on this below).  

Your project is to develop/write a simple shell - myshell - that has the following properties:   
1.  The shell must support the following internal commands:  
a. cd <directory> - Change the current default directory to 
<directory>. If the <directory> argument is not present, report the current directory. If the directory does not exist an appropriate error should be reported. This command should also change the PWD environment variable.  
b. clr - Clear the screen.  
c. dir <directory> - List the contents of directory <directory>.  
d. environ - List all the environment strings.  
e. echo <comment> - Display  <comment> on the display followed by a new line (multiple spaces/tabs may be reduced to a single space).  
f. help - Display the user manual using the more filter.   
g. pause - Pause operation of the shell until ‘Enter’ is pressed.  
h. quit - Quit the shell.  
i. The shell environment should contain shell=<pathname>/myshell where <pathname>/myshell is the full path for the shell executable(not a hardwired path back to your directory, but the one from which it was executed).   

 2.   All other command line input is interpreted as program invocation, which should be done by the shell *fork*ing and *exec*ing the programs as its own child processes. The programs should be executed with an environment that contains the entry: parent=<pathname>/myshell where <pathname>/myshell is as described in 1.i. above.  
  
 3.   The shell must be able to take its command line input from a file. That is, if the shell is invoked with a command line argument: 
  myshell batchfile
then batchfile is assumed to contain a set of command lines for the shell to process. When the end-of-file is reached, the shell should exit. Obviously, if the shell is invoked without a command line argument, it solicits input from the user via a prompt on the display. 
  
 4. The shell must support I/O - redirection on either or both  *stdin* and/or *stdout*. That is, the command line
   programname arg1 arg2 < inputfile > outputfile
will execute the program /programname/ with arguments /arg1/ and /arg2/, the stdin FILE stream replaced by /inputfile/ and the stdout FILE stream replaced by /outputfile/. 
 stdout redirection should also be possible for the internal commands 
dir,  environ, echo, & help. 
With output redirection, if the redirection character is > then the outputfile is created if it does not exist and truncated if it does. If the redirection token is >> then outputfile is created if it does not exist and appended to if it does.  

 5. The shell must support background execution of programs. An ampersand (*&*) at the end of the command line indicates that the shell should return to the command line prompt immediately after launching that program.   
 
6. You are to include an implementation of command line pipes. (essentially an extension of redirection) so that commands can be strung together. An example is 
cat out.txt | wc –l  

7. The command line prompt must contain the pathname of the current directory.
/Note:/ You can assume that all command line arguments (including the redirection symbols, <, > & >> and the background execution symbol, &) will be delimited from other command line arguments by white space - one or more spaces and/or tabs (see the command line in 4. above).
