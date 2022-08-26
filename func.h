#ifndef FUNC_H
#define FUNC_H
#define _GNU_SOURCE
//for the struct stat type
#include <sys/stat.h>

//primary functions

/// parses the command line input into an array with options arguments, by utislizing the getopt() function
/// \param argc number of command line arguments
/// \param argv an array of strings containing the command line input and the name of executable file
/// \return a string array the parsed arguments, formatted for the do_dir() and do_entry() function
char **parseUserInput(int argc, char **argv);
/// Processes the entries of a directory and calls do_entry() for each directory entry in an appropriate way 
/// \param dir_name is the name of the directory to be processed
/// \param parms a string array with parsed arguments, formatted for the do_dir() and do_entry() function
void do_dir(const char *dir_name, char **parms);
/// Processes single directory entries and prints the paths and/or the entry details to stdout
/// \param entry_name the name of the directory entry to be processed
/// \param parms a string array with parsed arguments, formatted for the do_dir() and do_entry() function
void do_entry(char *entry_name,char ** parms);

//helper functions

/// Compares a directory entry's file type to a given one
/// \param fileMode entry information needed to determine the file type
/// \param typeToPrint file type used for comparison
/// \param printFlag a flag, that is to be unset if the comparison is false
/// \return returns 0 if the file types match and 1 otherwise
int findByType(mode_t fileMode ,char *typeToPrint, int printFlag);
/// prints a directory entry's absolute path to stdout
/// \param entry_name the name of the directory entry to be processed
void printPath(char *entryName);
/// prints a directory entry's detailed information to stdout
/// \param entry_name the name of the directory entry to be processed
void listEntryDetails(char *entryName);
/// prints the paths of all entries of a given directory to stdout
/// \param path path to the directory
void printAllDirEntries(const char *path);
/// prints a directory entry's permission information to stdout
/// \param entry_name the name of the directory entry to be processed
void printPermissions(char *entryName);
/// takes a time in seconds and prints it to stdout in a date and time format
/// \param time time in seconds
void printDateAndTime(unsigned int time);
/// printhelp() Prints the program usage
void printHelp();

#endif