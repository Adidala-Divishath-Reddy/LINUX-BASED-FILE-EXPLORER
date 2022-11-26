# LINUX-BASED-FILE-EXPLORER
Implemented mini file explorer with Normal mode and Command mode in Linux using C++. 

Normal mode works like GUI. Command mode works like command prompt in Windows.

  ### Normal mode:

  Normal mode is the default mode of your application. It will have the following functionalities -

#### 1. Display a list of directories and files in the current folder
* Every file in the directory should be displayed on a new line with the following attributes for each file -
  
  i. File Name

  ii. File Size
  
  iii. Ownership (user and group) and Permissions
  
  iv. Last modified
* The file explorer will show entries “.” and “..” for current and parent directory respectively.
* The file explorer will handle scrolling in the case of vertical overflow using keys k & l.
* User will be able to navigate up and down in the file list using the corresponding up and down arrow keys.
####  2. Open directories and files when enter key is pressed -
* Directory - 
          
            Clears the screen
            navigates into the directory 
            shows the directory contents as specified in point 1.
* File - Opens the file in vi editor
####  3. Traversal
* Go back - `Left` arrow key will take the user to the previously visited directory
* Go forward - `Right` arrow key will take the user to the next directory
* Up one level - `Backspace` key will take the user up one level
* Home - `h` key will take the user to the home folder (the folder where the application was started)

### Command Mode:
The application will enter the Command button whenever “:” (colon) key is pressed. 
In the command mode, the user will be able to enter different commands. 
All commands appear in the status bar at the bottom.
####  1. Copy - ‘copy <source_file(s)> <destination_directory>’; Move - ‘move <source_file(s)> <destination_directory>’;  Rename - ‘rename <old_filename> <new_filename>’
* Example - 

      copy foo.txt bar.txt baz.mp4 ~/foobar    
      move foo.txt bar.txt baz.mp4 ~/foobar
      rename foo.txt bar.txt
* Assumed that the destination directory exists and have write permissions.
* Copying/Moving directories are also implemented
* The file ownership and permissions should remains intact
####  2. Create File - ‘create_file <file_name> <destination_path>’;  Creates Directory - ‘create_dir <dir_name> <destination_path>’
* Eg -

      create_file foo.txt ~/foobar
      create_file foo.txt .
      create_dir foo ~/foobar
#### 3. Delete File - ‘delete_file <file_path>’;  Delete Directory - ‘delete_dir <dir_path>’
* On deleting directory, recursively deletes all content present inside it.
#### 4. Goto - ‘goto <location>’
Example -   `goto <directory_path>`
#### 5. Search - ‘search <file_name>’ or ‘search <directory_name>’
* Searchs for a given file or folder under the current directory recursively.
* Output will be True or False depending on whether the file or folder exists
#### 6. On pressing ESC key, the application will go back to Normal Mode
#### 7. On pressing q key, the application will close


### Execution Steps:

      g++ main.cpp
      ./a.out
