#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

char* makeParameter(char *arg){
	int lengthArg = strlen(arg);
	char *parameter = (char*)malloc((lengthArg - 2)*sizeof(char));
	
	for (int i=0; i<lengthArg-2; i++){
		parameter[i] = arg[i+2];
	}
	
	return parameter;
}


void makeDir(char *parameter){
	if (mkdir(parameter, 0777) == -1){
		printf("Dir %s wasn't created\n", parameter);
		return;
	}
}


void readDir(char *parameter){
	struct dirent **nameList;
	int n;
	
	if ((n = scandir(parameter, &nameList, 0, alphasort)) == -1){
		printf("It's impossible to show the filling of dir %s\n", parameter);
		return;
	}
	while (n--){
		printf("%s\n", nameList[n]->d_name);
		free(nameList[n]);
	}
	free(nameList);
}


void removeDir(char *parameter){
	if (rmdir(parameter) == -1){
		printf("Dir %s couldn't be removed\n", parameter);
		return;
	}
}


void createFile(char *parameter){
	if (creat(parameter, S_IRWXU) == -1){
		printf("File %s wasn't created\n", parameter);
		return;
	}
}


void readFile(char *parameter){
	FILE *file;
	char buff;
	
	if ((file = fopen(parameter, "rb")) == NULL){
		printf("File %s couln't be opened\n", parameter);
		return;
	}
	while(!feof(file)){
		fread(&buff, 1, 1, file);
		printf("%c", buff);
	}
	fclose(file);
}


void removeFile(char *parameter){
	if (remove(parameter) == -1){
		printf("File %s couldn't be removed\n", parameter);
		return;
	}
}


void createSoftLink(char *oldName, char *name){
	errno = 0;
	
	if (symlink(oldName, name) == -1){
		printf("Symbolic link %s for the file %s coulnd't be created\n", name, oldName);
		fprintf(stderr, "%s\n", strerror(errno));
		return;
	}
}


void readSoftLink(char *parameter){
	errno = 0;
	char buff[4096];
	if (readlink(parameter, buff, 4096) == -1){
		printf("Symbolic link %s couldn't be read\n", parameter);
		fprintf(stderr, "%s\n", strerror(errno));
		return;
	}
	printf("%s\n", buff);
}


void readSoftLinkFile(char *parameter){
	readFile(parameter);
}


void removeSoftLink(char *parameter){
	if (unlink(parameter) == -1){
		printf("Symbolic link %s couldn't be removed\n", parameter);
		return;
	}
}


void createHardLink(char *oldName, char *name){
	if (link(oldName, name) == -1){
		printf("Hard link %s for the file %s coulnd't be created\n", name, oldName);
		return;
	}
}


void removeHardLink(char *parameter){
	removeFile(parameter);
}


void showModeAmount(char *parameter){
	struct stat fileStat;
	
	if (stat(parameter, &fileStat) == -1){
		printf("Data of file %s couldn't be got\n", parameter);
		return;
	}
	
	printf("Mode of the file %s is %o and amount of hard links is %ld \n", parameter, fileStat.st_mode&0777, fileStat.st_nlink);
	printf("Sticky bit is %d\n", fileStat.st_mode&S_ISVTX);
}


void changeMode(char *parameter, char *mode){
	if (chmod(parameter, strtoll(mode, NULL, 8)) == -1){
		printf("%s's mode couldn't be changed\n", parameter);
		return;
	}	
}


int main(int argc, char **argv){
	if (argc < 2){
		printf("Invalid input");
		return -1;
	}
	
	char *parameter = makeParameter(argv[0]);
	char *command = argv[1];
	
	if (strcmp(command, "make_dir") == 0){
		char* newDir = argv[2];
		makeDir(newDir);
	}
	else if (strcmp(command, "read_dir") == 0){
		readDir(parameter);
	}
	else if (strcmp(command, "remove_dir") == 0){
		char *deletingDir = argv[2];
		removeDir(deletingDir);
	}
	else if (strcmp(command, "create_file") == 0){
		char *fileName = argv[2];
		createFile(fileName);
	}
	else if (strcmp(command, "read_file") == 0){
		char *fileName = argv[2];
		readFile(fileName);
	}
	else if (strcmp(command, "remove_file") == 0){
		char *fileName = argv[2];
		removeFile(fileName);
	}
	else if (strcmp(command, "create_soft_link") == 0){
		char *name = argv[2];
		createSoftLink(parameter, name);	
	}
	else if (strcmp(command, "read_soft_link") == 0){
		char *name = argv[2];
		readSoftLink(name);
	}
	else if (strcmp(command, "read_soft_link_file") == 0){
		char *name = argv[2];
		readSoftLinkFile(name);
	}
	else if (strcmp(command, "remove_soft_link") == 0){
		char *name = argv[2];
		removeSoftLink(name);
	}
	else if (strcmp(command, "create_hard_link") == 0){
		char *name = argv[2];
		createHardLink(parameter, name);
	}
	else if (strcmp(command, "remove_hard_link") == 0){
		char *name = argv[2];
		removeHardLink(name);
	}
	else if (strcmp(command, "show_mode_and_amount") == 0){
		showModeAmount(parameter);
	}
	else if (strcmp(command, "change_mode") == 0){
		char *mode = argv[2];
		changeMode(parameter, mode);		
	}
	else {
		printf("Invalid command\n");
	}
	
	return 0;
}
