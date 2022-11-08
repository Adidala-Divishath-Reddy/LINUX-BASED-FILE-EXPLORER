//#include <conio>
#include <iostream>
#include <vector>
#include <dirent.h>
#include <filesystem>
#include <string>
#include <unistd.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <termios.h>
#include <fstream>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define gotoxy(x, y) printf("\033[%d;%dH", (x), (y))

//**************GLOBAL VARIABLES*******************

int x = 1, y = 81, countPositionOfFile = 1;
int windowSize = 3, start = 0, end, noOfEntries;
stack<string> backward, goForward;
char cwd[2048], pgm_launched_dir[2048];
struct passwd *homeDir = getpwuid(getuid());

//*********Function which converts given path to absolute path 

string modify_path(string path)
{
	if (path[0] == '~')
	{
		path = string(pgm_launched_dir) + "/" + path.substr(2, path.length() - 2);
		//creating file using absolute path from where the program launched 

	}
	else if (path[0] == '/')
	{
		path = path;	//creating file using absolute path from root of the system

	}
	else
	{
		if (path[1] == '/')
			path = string(cwd) + path.substr(1, path.length() - 1);
		else
			path = string(cwd) + "/" + path;	//creating file using relative path from current working directory

	}

	return path;
}

//*****returns all entry names inside given path***********

vector<string> displayFiles(char *path)
{
	vector<string> v;
	struct dirent * entry;
	DIR *dir = opendir(path);
	if (dir == NULL)
	{
		return v;
	}

	while ((entry = readdir(dir)) != NULL)
	{
		v.push_back(entry->d_name);

	}

	closedir(dir);
	return v;
}

//*****prints details of all entries inside given path***********

void printFiles(char *cwd)
{
	cout << "\033[H\033[2J\033[3J";

	vector<string> files;
	files.clear();
	files = displayFiles(cwd);
	//cout<<cwd<<endl;
	int i = 0, j = 0;
	//cout<<files[0]<<endl;
	noOfEntries = files.size();
	for (auto file: files)
	{
		//cout << file << endl ;
		if (j < start)
		{
			j++;
			continue;

		}

		struct stat s;
		char f[2048];
		strcpy(f, file.c_str());
		string str = string(cwd);
		string f1 = string(f);
		str = str + "/" + f1;
		strcpy(f, str.c_str());
		if (stat(f, &s) == 0)
		{
			if (S_ISDIR(s.st_mode))
				cout << "d";
			else
				cout << "-";
			if ((s.st_mode &S_IRUSR) && (s.st_mode &S_IREAD))
				cout << "r";
			else
				cout << "-";
			if ((s.st_mode &S_IWUSR) && (s.st_mode &S_IWRITE))
				cout << "w";
			else
				cout << "-";
			if ((s.st_mode &S_IXUSR) && (s.st_mode &S_IEXEC))
				cout << "x";
			else
				cout << "-";
			if ((s.st_mode &S_IRGRP) && (s.st_mode &S_IREAD))
				cout << "r";
			else
				cout << "-";
			if ((s.st_mode &S_IWGRP) && (s.st_mode &S_IWRITE))
				cout << "w";
			else cout << "-";
			if ((s.st_mode &S_IXGRP) && (s.st_mode &S_IEXEC))
				cout << "x";
			else
				cout << "-";
			if ((s.st_mode &S_IROTH) && (s.st_mode &S_IREAD))
				cout << "r";
			else
				cout << "-";
			if ((s.st_mode &S_IWOTH) && (s.st_mode &S_IWRITE))
				cout << "w";
			else
				cout << "-";
			if ((s.st_mode &S_IXOTH) && (s.st_mode &S_IEXEC))
				cout << "x";
			else
				cout << "-";
			struct passwd *pw = getpwuid(s.st_uid);
			struct group *gr = getgrgid(s.st_gid);

			cout << "\t" << pw->pw_name << "\t" << gr->gr_name << "\t" << s.st_size << "\t";

			auto dt = *(gmtime(&s.st_mtime));
			cout << to_string(dt.tm_mday) << "-" << to_string(dt.tm_mon) << "-" << to_string(dt.tm_year + 1900) << "  " << to_string(dt.tm_hour) << ":" << to_string(dt.tm_min) << ":" << to_string(dt.tm_sec) << "";

			cout << "\t" << file << endl;

			i++;
			if (i == windowSize)
				break;
		}
	}

	cout << endl;
}

//*****copies given source file to given destination

void copy_file(string src, string dest)
{
	int file_desc_source, file_desc_destination, nextt, errorr;
	unsigned char buffer[4096];
	char src_path[4096], dst_path[4096];
	//cout<<src<<endl;
	//cout<<dest<<endl;
	strcpy(src_path, (src).c_str());

	strcpy(dst_path, (dest).c_str());
	//cout<<"reached"<<endl;
	file_desc_source = open(src_path, O_RDONLY);
	file_desc_destination = open(dst_path, O_CREAT | O_WRONLY);

	while (1)
	{
		errorr = read(file_desc_source, buffer, 4096);
		if (errorr == -1)
		{
			gotoxy(21, 0);
			printf("\033[K");
			printf("Error reading file.\n");
			exit(1);
		}

		nextt = errorr;

		if (nextt == 0) break;

		errorr = write(file_desc_destination, buffer, nextt);
		if (errorr == -1)
		{
			gotoxy(21, 0);
			printf("\033[K");
			printf("Error writing to file.\n");
			exit(1);
		}
	}

	struct stat st;

	stat(src_path, &st);

	chmod(dst_path, st.st_mode);
	close(file_desc_source);
	close(file_desc_destination);

}

//copies directory. 
void copy_directory(string src, string dest)
{
	struct dirent * entry;
	DIR * dir;
	struct stat s;
	dir = opendir(src.c_str());
	if (dir)
	{
		chdir(src.c_str());
		while ((entry = readdir(dir)))
		{
			lstat(entry->d_name, &s);
			if (S_ISDIR(s.st_mode))
			{
				if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
				{
					continue;
				}

				mkdir((dest + "/" + string(entry->d_name)).c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
				//If we encounter a directory inside directory, we will create new directory and call recursively
				copy_directory((src + "/" + string(entry->d_name)), (dest + "/" + string(entry->d_name)));

			}
			else
			{
				copy_file((src + "/" + string(entry->d_name)), (dest + "/" + string(entry->d_name)));	//copies the file
			}
		}

		chdir("..");
		closedir(dir);

	}
}

//search for a file from "/home" and looks at all possible directories and files
void search_reqd(string r_file, char *temp_cwd)
{
	vector<string> entries;
	entries.clear();
	entries = displayFiles(temp_cwd);

	int i = 0, j = 0;

	noOfEntries = entries.size();
	for (auto entry: entries)
	{
		
		struct stat s;
		char f[2048];
		strcpy(f, entry.c_str());
		string str = string(temp_cwd);
		string f1 = string(f);
		str = str + "/" + f1;
		strcpy(f, str.c_str());
		if (stat(f, &s) == 0)
		{
			if ((entry != ".") && (entry != ".."))
			{
				if (s.st_mode &S_IFDIR)	//if encountered a directory
				{
					char te[2048];
					string convert = string(temp_cwd);
					convert = convert + "/" + entry;
					strcpy(te, convert.c_str());
					search_reqd(r_file, te);	//checks all entries inside this directory by calling recursively
				}
				else
				{
					if (r_file == entry)
					{
						gotoxy(21, 0);
						printf("\033[K");
						cout << "  Found file.Press key 'd' to enter commands again" << endl;
						cin.get();
						gotoxy(21, 0);
						printf("\033[K");
					}
				}
			}
		}
	}
}

//Deletes directory. If we have directory inside directory, deleting all files inside directory and deletes directory
void delete_directory(string directory_to_be_deleted)
{
	struct dirent * entry;
	DIR * dir;
	struct stat s;
	dir = opendir(directory_to_be_deleted.c_str());
	if (dir)
	{
		chdir(directory_to_be_deleted.c_str());
		while ((entry = readdir(dir)))
		{
			lstat(entry->d_name, &s);
			if (S_ISDIR(s.st_mode))
			{
				if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
				{
					continue;
				}

				delete_directory(directory_to_be_deleted + "/" + entry->d_name);
				rmdir(entry->d_name);
			}
			else
			{
				unlink(entry->d_name);
			}
		}

		chdir("..");
		closedir(dir);

	}
}

//This function handles different cases in command mode.
void execute_cmd(string str)
{
	int i;
	for (i = 0; i < str.length(); i++)
	{
		if (str[i] == ' ')
		{
			break;
		}
	}

	string comnd = str.substr(0, i);
	string path;
	if (comnd == "create_file")
	{
		int j;
		for (j = i + 1; j < str.length(); j++)
		{
			if (str[j] == ' ')
			{
				break;
			}
		}

		//converting any kind of path to absolute path
		if (str[j + 1] == '~')
		{
			path = string(pgm_launched_dir) + "/" + str.substr(j + 3, str.length() - j - 2);	
			//creating file using absolute path from where the program launched 

		}
		else if (str[j + 1] == '/')
		{
			path = str.substr(j + 1, str.length() - j - 1);	
			//creating file using absolute path from root of the system

		}
		else
		{
			if (str[j + 2] == '/')
				path = string(cwd) + str.substr(j + 2, str.length() - j - 1);
			else
				path = string(cwd) + "/" + str.substr(j + 1, str.length() - j);	
				//creating file using relative path from current working directory

		}

		ifstream test(path);
		if (!test)
		{
			gotoxy(21, 0);
			printf("\033[K");
			cout << endl << path << " is Invalid path" << endl;

		}
		else
		{
			char f[2048];
			str = path + "/" + str.substr(i + 1, j - i - 1);
			strcpy(f, str.c_str());
			fstream file_desc;

			int i, lastSlashPos = 0;
			//string str = string(cwd);
			for (int i = 0; i < str.length(); i++)
			{
				if (str[i] == '/')
				{
					lastSlashPos = i;
				}
			}

			file_desc.open(f, ios::out);

			if (!file_desc)
			{
				gotoxy(21, 0);
				printf("\033[K");
				cout << " file not created in " << cwd << " named " << f << endl;
			}
			else
			{
				gotoxy(21, 0);
				printf("\033[K");
				cout << " file created successfully" << endl;

			}
		}
	}
	else if (comnd == "create_dir")
	{
		int j;
		for (j = i + 1; j < str.length(); j++)
		{
			if (str[j] == ' ')
			{
				break;
			}
		}

		if (str[j + 1] == '~')
		{
			path = string(pgm_launched_dir) + "/" + str.substr(j + 3, str.length() - j - 2);	
			//creating file using absolute path from where the program launched 

		}
		else if (str[j + 1] == '/')
		{
			path = str.substr(j + 1, str.length() - j - 1);	
			//creating file using absolute path from root of the system

		}
		else
		{
			if (str[j + 2] == '/')
				path = string(cwd) + str.substr(j + 2, str.length() - j - 1);
			else
				path = string(cwd) + "/" + str.substr(j + 1, str.length() - j);	
				//creating file using relative path from current working directory

		}

		ifstream test(path);
		if (!test)
		{
			gotoxy(21, 0);
			printf("\033[K");
			cout << "Invalid path" << endl;

		}
		else
		{
			char f[2048];
			str = path + "/" + str.substr(i + 1, j - i - 1);
			strcpy(f, str.c_str());

			if (mkdir(f, 0777) == 0)
			{
				gotoxy(21, 0);
				printf("\033[K");
				cout << " directory created successfully in " << cwd << " named " << str << endl;
			}
		}
	}
	else if (comnd == "delete_file")
	{
		unlink(modify_path(str.substr(i + 1, str.length() - i - 1)).c_str());
		gotoxy(21, 0);
		printf("\033[K");
		cout << "deleted file" << endl;

	}
	else if (comnd == "search")
	{
		search_reqd(str.substr(i + 1, str.length() - i - 1), "/home");

	}
	else if (comnd == "goto")
	{
		//changing current working directory to this path
		char c;
		strcpy(cwd, (modify_path(str.substr(i + 1, str.length() - i - 1))).c_str());	//

		ifstream test(cwd);
		if (!test)
		{
			gotoxy(21, 0);
			printf("\033[K");
			cout << "Invalid path" << endl;

		}
		else
		{
			printFiles(cwd);
			gotoxy(20, 0);
			printf("\033[K");
			cout << "*************COMMAND MODE*************" << endl;
			gotoxy(21, 0);

			cout << "Click key 'd' to close this and enter commands again" << endl;
			cin.get();
			gotoxy(21, 0);
			printf("\033[K");
		}
	}
	else if (comnd == "copy")
	{
		vector<string> allFiles;
		int j, word_break = i;
		for (j = i + 1; j < str.length(); j++)
		{
			if (str[j] == ' ')
			{
				//cout<<"  "<<str.substr(word_break+1, j-word_break-1)<<endl;
				allFiles.push_back(str.substr(word_break + 1, j - word_break - 1));
				word_break = j;
			}
		}

		string path = str.substr(word_break + 1, str.length() - word_break - 1);
		//cout<<"Immediate path "<<path<<" "<<endl;
		if (path[0] == '~')
		{
			path = string(pgm_launched_dir) + "/" + path.substr(2, path.length() - 2);	
			//creating file using absolute path from where the program launched 

		}
		else if (path[0] == '/')
		{
			path = path;
			

		}
		else
		{
			if (path.length() >= 2 && path[1] == '/')
				path = string(cwd) + path.substr(1, str.length() - 1);
			else
				path = string(cwd) + "/" + path;	
				//creating file using relative path from current working directory

		}

		//cout<<endl<<path<<endl;
		ifstream test(path);
		if (!test)
		{
			gotoxy(21, 0);
			printf("\033[K");
			cout << endl << "Invalid path" << endl;
			cout << "Click key 'd' to close this and enter commands again" << endl;
			cin.get();
		}
		else
		{
			for (int i = 0; i < allFiles.size(); i++)
			{
				string final_dest = (path + "/" + allFiles[i]);
				//cout<<final_dest<<endl;
				struct stat s;
				char temp_allFiles[4096];
				strcpy(temp_allFiles, (modify_path(allFiles[i])).c_str());
				if (stat(temp_allFiles, &s) == 0)
				{
					if (s.st_mode &S_IFDIR)
					{
						char temp_final_src[4096];
						strcpy(temp_final_src, (modify_path(allFiles[i])).c_str());
						mkdir(final_dest.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
						copy_directory(temp_final_src, final_dest);
					}
					else
					{
						copy_file((modify_path(allFiles[i])), final_dest);
					}
				}

				gotoxy(21, 0);
				printf("\033[K");
				cout << "copied successfully";
			}
		}
	}
	else if (comnd == "move")
	{
		int j;
		string before_moved, path = "";
		for (j = i + 1; j < str.length(); j++)
		{
			if (str[j] == ' ')
			{
				before_moved = str.substr(i + 1, j - i - 1);
				break;
			}
		}

		string after_moved = str.substr(j + 1, str.length() - j - 1);
		struct stat s;
		before_moved = modify_path(before_moved);
		after_moved = modify_path(after_moved);
		int k, last_slash = 0;
		for (k = 0; k < before_moved.length(); k++)
		{
			if (before_moved[k] == '/')
			{
				last_slash = k;
			}
		}

		char temp_move[4096];
		strcpy(temp_move, before_moved.c_str());
		if (stat(temp_move, &s) == 0)
		{
			if (s.st_mode &S_IFDIR)
			{
				mkdir((after_moved + "/" +
					before_moved.substr(last_slash + 1, before_moved.length() - last_slash - 1)).c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
				copy_directory(before_moved,
					after_moved + "/" +
					before_moved.substr(last_slash + 1, before_moved.length() - last_slash - 1));

				delete_directory(before_moved);
				rmdir(before_moved.c_str());
			}
			else
			{
				copy_file(before_moved, after_moved + "/" +
					before_moved.substr(last_slash + 1, before_moved.length() - last_slash - 1));

				unlink(before_moved.c_str());
			}

			gotoxy(21, 0);
			printf("\033[K");
			cout << "   moved successfully" << endl;
		}
	}
	else if (comnd == "rename")
	{
		int j;
		string before_renamed, path = "";
		for (j = i + 1; j < str.length(); j++)
		{
			if (str[j] == ' ')
			{
				before_renamed = str.substr(i + 1, j - i - 1);
				break;
			}
		}

		if (before_renamed[0] == '~')
		{
			path = string(pgm_launched_dir) + "/";	//creating file using absolute path from where the program launched 

		}
		else if (before_renamed[0] == '/')
		{
			path = path;	//creating file using absolute path from root of the system

		}
		else
		{
			if (before_renamed[1] == '/')
				path = string(cwd) + "/";
			else
				path = string(cwd) + "/";	//creating file using relative path from current working directory

		}

		int k, last_slash = 0;
		for (k = 0; k < before_renamed.length(); k++)
		{
			if (before_renamed[k] == '/')
			{
				last_slash = k;
			}
		}

		string x = "";
		if (last_slash != 0)
			x = before_renamed.substr(0, last_slash + 1);
		string after_renamed = str.substr(j + 1, str.length() - j - 1);
		rename((modify_path(before_renamed)).c_str(), (path + x + after_renamed).c_str());
		gotoxy(21, 0);
		printf("\033[K");
		cout << "renamed successfully";

	}
	else if (comnd == "delete_dir")
	{
		delete_directory(modify_path(str.substr(i + 1, str.length() - i - 1)));
		rmdir((modify_path(str.substr(i + 1, str.length() - i - 1))).c_str());
		gotoxy(21, 0);
		printf("\033[K");
		cout << "deleted successfully" << endl;

	}
	else {}
}

void command_mode()
{
	gotoxy(20, 0);
	printf("\033[K");
	cout << "*************COMMAND MODE*************" << endl;
	char c;
	string str = "";
	while (1)
	{
		c = cin.get();
		if (c == 27)	//escape key
		{
			return;
		}
		else if (c == 10)	//enter key
		{
			execute_cmd(str);
			str = "";
		}
		else if (c == 127)	//backspace key
		{
			str = str.substr(0, str.length() - 1);
			gotoxy(21, 0);
			printf("\033[K");
			cout << str;
		}
		else if (c == 113)
		{
			//EXIT_SUCCESS;
			exit(0);
		}
		else
		{
			gotoxy(21, 0);
			printf("\033[K");
			str = str + c;
			cout << str;
		}
	}
}

char EnterRawMode(char *cwd)
{
	printFiles(cwd);
	char buf = 0;
	struct termios old = { 0 };
	if (tcgetattr(0, &old) < 0)
		perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0)
		perror("tcsetattr ICANON");
	else
	{
		char c, abc, cmd_mode;

		while (1)
		{
			gotoxy(x, y);
			c = cin.get();
			switch (c)
			{
				case 27:
					cin.get();

					abc = cin.get();
					if (abc == 'A')
					{
						if (x > 1)
							x = x - 1;
					}
					else if (abc == 'B')
					{
						if (x < windowSize)
							x = x + 1;
					}
					else if (abc == 'C')
					{
						//backward.push(cwd.substr(i+1,(sizeof(cwd)/sizeof(cwd[0]))-i));

						if (!goForward.empty())
						{
							backward.push(cwd);

							string stri = goForward.top();
							strcpy(cwd, stri.c_str());

							goForward.pop();
							start = 0;
							printFiles(cwd);

						}
					}
					else if (abc == 'D')
					{
						//backward.push(cwd.substr(i+1,(sizeof(cwd)/sizeof(cwd[0]))-i));

						if (!backward.empty())
						{
							goForward.push(cwd);
							string stri = backward.top();
							strcpy(cwd, stri.c_str());

							backward.pop();
							start = 0;
							printFiles(cwd);

						}
					}
					else
					{
						x = x;
					}

					break;
				case 58:
					{
						//string Current_cwd = string(cwd);
						command_mode();
						strcpy(cwd, string(pgm_launched_dir).c_str());
						//strcpy(cwd, (string(homeDir->pw_dir)).c_str());
						start = 0;
						printFiles(cwd);

					}

					break;
				case 107:
					if (start + windowSize < noOfEntries)
					{
						start = start + 1;

						printFiles(cwd);
					}

					break;
				case 108:
					if (start > 0)
					{
						start = start - 1;

						printFiles(cwd);
					}

					break;
				case 10:
					{
						int i;
						//backward.push(cwd);
						int j = 1;
						string path, pointedFile;
						char temp[2048];
						vector<string> files = displayFiles(cwd);
						for (auto file: files)
						{
							//cout << file << endl ;
							if (j < start + x)
							{
								j++;
								continue;

							}

							//pointedFile = file;
							path = string(cwd) + "/" + file;
							break;
						}

						strcpy(temp, path.c_str());
						struct stat s;
						if (stat(temp, &s) == 0)
						{
							if (s.st_mode &S_IFDIR)
							{
								backward.push(cwd);
								while (!goForward.empty())
									goForward.pop();

								strcpy(cwd, path.c_str());

								int temp = start;
								start = 0;
								printFiles(cwd);
								start = temp;
							}
							else if (s.st_mode &S_IFREG)
							{
								int pid = fork();
								if (pid == 0)
								{
									char file_name[2048];
									//file_name = path.substr(i+1, path.length()-i);
									strcpy(file_name, path.c_str());
									execlp("gedit", "gedit", file_name, NULL);

								}
							}
						}

						//strcpy(temp,path.c_str()); 
						//struct dirent * entry;
						//DIR *dir = temp;
						//entry = readdir(dir);
						//if(entry->d_type == DT_REG)
						//{
						//}

						//if(entry->d_type == DT_DIR){
						//}
					}

					break;
				case 127:
					if (strcmp(cwd, homeDir->pw_dir) != 0)
					{
						int i, lastSlashPos = 0;
						string stri = string(cwd);
						for (int i = 0; i < stri.length(); i++)
						{
							if (stri[i] == '/')
							{
								lastSlashPos = i;
							}
						}

						goForward.push(cwd);
						if (!backward.empty())
							backward.pop();
						stri = stri.substr(0, lastSlashPos);

						strcpy(cwd, stri.c_str());
						int temp = start;
						start = 0;
						printFiles(cwd);
						start = temp;

					}

					break;
				case 104:
					strcpy(cwd, string(pgm_launched_dir).c_str());
					start = 0;
					printFiles(cwd);
					break;
			}
		}

		old.c_lflag |= ICANON;
		old.c_lflag |= ECHO;
		if (tcsetattr(0, TCSADRAIN, &old) < 0)
			perror("tcsetattr ~ICANON");
		return (buf);
	}
}

void showEntriesInDirectory(char *cwd)
{
	//gotoxy(x,y);
	EnterRawMode(cwd);

}

int main()
{
	for (int i = 0; i < 2048; i++)
		cwd[i] = '0';
	getcwd(cwd, 2048);

	strcpy(pgm_launched_dir, string(cwd).c_str());
	//cout<<pgm_launched_dir<<endl;
	//cout<<homeDir->pw_dir<<endl;
	showEntriesInDirectory(cwd);
	//cout << "\033[2J\033[1;1H"; 	
	return EXIT_SUCCESS;
}
