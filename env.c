/*
    Elijah Ager Luckett
    CS 3393 - Unix
    HW2 Env file
    Feb 18 2022
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

extern char** environ;

//PROTOTYPES
//print environ
void printenv(char **env);

// len of string array
int arrlen(char **arr);

// get index of val or next vacant spot
int getindex(char **arr, char *val, int length);

// extract env arr from argv
char **parseenv(int start, int argc, char **argv);

// merge new env array to environ
char **mergeenv(char **newenv);

// free environ allocation
void freeenv(char **env);

// run command from arg v
int execcommand(int start, char **argv);

// check if keys are the same
int sameKey(char *str1, char *str2);

int main(int argc, char *argv[]) {
    //basic print env case
    if (argc == 1){
        printenv(environ);
        return 0;
    }
    
    int index=1;    // skipping "./env" argument
    char **newenv;  // new env var
    
    if (!strcmp(argv[1], "-i")){ // if second arg is "-i"
        index = 2; // SKIP "-i" -> index = 2
        
        // if there is no other key-val or command, exit
        if (argc == 2){
            return 0;
        }
        
        // extract keyval pairs from argv and put in alocated string array
        newenv = parseenv(index,argc,argv); //array and ptrs allocated
        
        // -i indicates completly new env so just assign
        environ = newenv;
        // free newenv later
        
    }
    else{ // no -i ... index is still 1
        // extract keyval pairs from argv and put in alocated string array
        newenv = parseenv(index,argc,argv); //array and ptrs allocated

        environ = mergeenv(newenv); // array allocated but not ch ptrs
        //Free environ and newenv later.
    }
    
    int freemerge=index; // use later to free environ
    // find index after last key-val
    while(argv[index] && strchr(argv[index], '=')){
        index++;
    }
    // see if there are commmands after the key value index
    if (index < argc){
        int check = execcommand(index,argv); // if so execvp those commands
        if (check == 1){ // if it fails
            perror("fork failed");
            freeenv(environ);
            return 1;
        }
        if (check ==2){
            perror("execvp failed");
            freeenv(environ);
            return 1;
        }
    }
    else{ // if there are none then print out the new environ
        printenv(environ);
    }
    //exit(1);
    if (freemerge==1){ // if no -i, merge put env array on heap
        free(environ);
    }
    freeenv(newenv); // finally free the environment and return
    return 0;
}

/*
 takes null terminated string array and prints each string out
 
 return type: void
*/
void printenv(char **env){
    // loop and print
    for (int i = 0; env[i] != NULL; i++) {
        printf("%s\n", env[i]);
    }
}

/*
 takes null terminated string array
 returns array len
 */
int arrlen(char **arr){
    // loop and count
    int count = 0;
    while(arr[count]){
        count++;
    }
    return count;
}

/*
 checks if they chars before the key are the same
 returns 1 if they are
 returns 0 if they are not
 */
int sameKey(char* str1, char* str2) {
    //loop and compare
    for (;*str1 != '=';str1++,str2++){
        if (*str1 != *str2) {
            return 0; //return 0 if discrepency
        }
    }
    // if all are same then return 1
    return 1;
}

/*
 takes array of strings and a string
 if the string is in array it returns that first index
 else it returns the first vacant spot
 */
int getindex(char **arr, char *val, int length) {
    int index = 0;
    while(arr[index]){
        if (arr[index] == val) { //repeat indicator
            return length;
        }
        else if (!strcmp(arr[index], "")) { //vacant index
            return index;
        }
        else if (sameKey(val, arr[index])) { //same key index
            return index;
        }
        index++;
    }
    return index;//not found indicator
    
}

/*
 allocates the correct ammount of space for the new env
 takes in the index where key-val pairs start in argv, argv and argc
 returns new array of strings containing the original argv keyval pairs
 */
char **parseenv(int start, int argc, char **argv){
    //variables
    int newlength = 0, i = start, lenargv=arrlen(argv);
    //loop and find number of new keyval in argv
    while (i<argc && strchr(argv[i], '=')){
        int index=getindex(argv,argv[i], lenargv);
        //if == then its new
        if(lenargv == index){
            newlength++;
        }
        i++;
    }
    // malloc new sized array for env var
    char **newenv = (char **) malloc((newlength+1) * sizeof(char*));
    if (newenv==NULL){
        free(newenv);
        perror("not enough space to malloc");
        exit(1);
    }
    
    // malloc each char pointer
    int p = start;
    for (int i = 0; i < newlength; i++) {
        newenv[i] = (char *) malloc(strlen(argv[p]));
        if (newenv[i]==NULL){
            free(newenv[i]);
            perror("not enough space to malloc");
            exit(1);
        }
        p++;
    }
    int truelength=arrlen(newenv);
    newenv[newlength]=NULL;
    for (int i=start; i < argc; i++) {
        if (strchr(argv[i], '=') != NULL) {//add each pair
            int index=getindex(newenv,argv[i], truelength);
            strcpy(newenv[index], argv[i]);//copy to malloced space
        }
    }
    return newenv;
}

/*
 used in the case no -i is provided and merging is needed
 takes argv and new environ array
 */
char **mergeenv(char **newenv){
    // count the number of items in newenv not in environ
    int numnew = 0;
    for (char** ptr = newenv; *ptr != NULL; ptr++) {
        int lenOldEnv = arrlen(environ);
        int index = getindex(environ,*ptr,lenOldEnv);
        //if index returns length of env (new one found)
        if(lenOldEnv == index){
            numnew++;
        }
    }
    //allocate new string array for size of unique+old+1
    int oldenvlen = arrlen(environ);
    char **merged = (char **) malloc((numnew+oldenvlen+1) * sizeof(char*));
    //final one is null
    merged[numnew+oldenvlen] = NULL;
    //add ones from environ
    for (int i = 0; environ[i] != NULL; i++) {
        merged[i] = environ[i];
    }
    //add unique ones
    int mergesize=arrlen(merged);
    for (int i = 0; newenv[i] != NULL; i++) {
        int index = getindex(merged, newenv[i], mergesize);
        merged[index]=newenv[i];
    }
    return merged;
}

/*
 used to free any lingering environs
 takes in the size of the a
 */
void freeenv(char **env){
    for (int i = 0; env[i] != NULL; i++){
        free(env[i]);
    }
    free(env);
}

/*
 Used to run the commands that come at the end of the env statement
 */
int execcommand(int start, char **argv){
    pid_t child = fork(); //child execs parent continues
    if (child < 0) {//if child process is negative fork failed
        perror("fork failed\n");
        return 1;
    }
    else if (child == 0) { // if it is 0 then one should try to exec
        if(execvp(argv[start], argv+start)<0){
            return 2; //if execvp fails
        }
    }
    else { // parent should wait before returning
        wait(NULL);
    }
    return 0;
}
