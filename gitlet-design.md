# Gitlet Design Document

**Name**:Qiu Wenqiang

The main functionality that Gitlet supports is:
1. Saving the contents of entire directories of files. In Gitlet, this is called committing, 
   and the saved contents themselves are called commits.

2. Restoring a version of one or more files or entire commits. In Gitlet, this is called 
   checking out those files or that commit.

3. Viewing the history of your backups. In Gitlet, you view this history in something 
   called the log.

4. Maintaining related sequences of commits, called branches.

5. Merging changes made in one branch into another.


## Classes and Data Structures

### Repository
This is the main logic of Gitlet will live. This file will handle all the 
actual Gitlet commands by reading/writing from/to the correct file, 
setting up persistence and additional error checking.

It will also be responsible for setting up all persistence within Gitlet. 
This includes creating the `.gitlet` folder as well as folder and file where 
we will store `Commit` objects and other metadata.

#### Fields
1. `public static fs::path CWD = fs::current_path()` 
   The Current Working Directory. 

2. `public static fs::path GITLET_DIR = CWD / ".gitlet"`
   The hidden `.gitlet` directory. This is where all the state of `Repository` will be stored.

3. `public static fs::path HEAD = GITLET_DIR / "HEAD"` 
   The file contains persistence data of the front of current branch of Gitlet.

4. `public static fs::path INDEX = GITLET_DIR / "INDEX"`
   The file contains persistence data of the staging area.

5. `public static fs::path COMMIT_DIR = GITLET_DIR / "commits"`
   The folder holds the file of persistence data of Commit objects.

6. `public static fs::path BLOB_DIR = GITLET_DIR / "blobs"`
   The folder holds the file of persistence data of Blob objects.

7. `public static fs::path TREE = GITLET_DIR / "TREE"`
   The file containing the persistence data of commit tree.

8. `public static fs::path CONFIG = GITLET_DIR / "CONFIG"`
   Configure file where info of remote repos are saved

9. `private static Repository::Tree tree = Tree();`
   An instantiated object of class Tree to manage Commit objects.

10. `private static StagingArea Repository::stagingArea = StagingArea();`
   An instantiated object of class StagingArea to manage added info and removed info.

11. `private static RemoteRepo Repository::repo = RemoteRepo();`
   An instantiated object of class RemoteRepo containing info of the added remote repo.


### StagingArea
This is a place containing staged files(for addition or removal) after command `add` 
and command `rm`.

#### Fields
1. `private unordered_map<string, string> m_addition;` 
   For the files to be added.

2. `private unordered_map<string, string> m_removal;` 
   For the files to be removed.

### Commit
The saved contents of entire directories of files. `Commit` is a combination of 
log messages, other metadata (date, message...), a reference of the current tree, 
and references to parent commits.

#### Fields
1. `private Date m_timestamp`
   date when Commit object is constructed.
   
2. `private string m_message;` 
   annotation of the current commit.
3. `private string m_parent_ref;` 
   a parent reference

4. `private string m_second_parent_ref;` 
   second parent reference(for merges)

5. `private string m_commit_ref;`
   The reference of current Commit object, an SHA-1 value produced by using 
   the metadata(combination of only timestamp and log message), the mapping of names 
   to references, and the parent reference.

6. `private unordered_map<string, string> m_map;` 
   a map of file names to references to their class Blob objects.


### Blob
The `Blob` class is where contents of files are saved. A single file might 
correspond to multiple blobs: each being tracked in a different `commit`.

#### Fields
1. `private string m_filename;` 
   The file name in the working directory.

2. `private string m_content;` 
   The saved content of a file.

3. `private string m_blob_ref;`
   The Reference of a class Blob object and filename of the file in local repo.


### Tree
Directory structures mapping names to reference to blob and other trees(subdirectories).

#### Fields
1. `private string m_head;`
   The reference to the front of the active branch.

2. `private string m_active_branch;`
   The active branch name.

3. `private unordered_map<string, string> m_branches;`
   All branches and the references to their own front commit(branchName : headRef)


### Utils
This header file contains helpful utility methods to read/write objects or string contents from/to files, as well as reporting errors when they occur.

#### Fields
None

### SHA1
This class will create a unique string of SHA-1 value according to the input strings. 

### Date
This class will record info of time specified and provides specified format of string of time. 




## Algorithms

### class Repository
1. `public static void init()`
   Initialize all directory structure of .gitlet;
   Initialize all static variables in class Repository and complete their data persistence;
   Complete first commit with message "initial commit" and add it to the tree;

2. `public static void add(string fileName)`
   Add the file name and its reference to the staging area if it is not 
   tracked in the head of current branch.

3. `public static void commit(string message)`
   Create a new commit. The commit will get the same snapshot from the head of current branch which will become
   the parent of the commit. Add all staged files to the commit and delete all removed
   files from the commit if their exist. Add the commit to the current branch and the commit
   become the new head of current branch.
   
4. `public static void checkout(string doubleDash, string fileName)`
   checkout -- fileName
   Check the existence of the file in the head of current branch. 
   Read the file from the head commit and write the file to the working directory.

5. `public static void checkout(string commitRef, string doubleDash, string fileName)`
   checkout commitID -- fileName
   Check the existence of the target commit. Check the existence of the target file in that commit.
   Read the file from the commit and write it to the working directory.
   
6. `public static void checkout(string branchName)`
   checkout branchName
   Check the existence of the branch. Write all files in the head of the branch to the
   working directory, remove all files that are untracked by the head.
   
7. `public static void rm(string fileName)`
   check if the file is staged for addition or tracked in the current head commit;
   untrack the file from staging area;
   remove file in current directory if it has been tracked in the current head

8. `public static void log()`
   Starting at the current head commit, display information about each commit backwards along the commit tree until the initial commit, following the first parent commit links, ignoring any second parents found in merge commits.

9. `public static void globalLog()`
   Like log, except displays information about all commits ever made.

10. `public static void find(string commitMessage)`
   Prints out the reference ids of all commits that have the given commit message, one per line. 

11. `public static void status()`
   displays what branches currently exist, and marks the current branch with a *;
   displays all staged files;
   displays all removed files;
   displays files modified but not staged;
   displays files untracked;

12. `public static void branch(string branchName)`
   create a new branch if it does not exist.

13. `public static void rmBranch(string branchName)`
   remove branch specified if it exists or it isn't the current active branch.

14. `public static void reset(string commitRef)`
   read commit object according to the commit reference if it exists;
   set the commit as new head of current branch;
   check out the current branch

15. `private static string getTheSplitPoint(Commit currentHead, Commit givenHead)`
   get set of all parents of current head;
   iterate parents of given head backward, starting from given head;
   get the split point commit as long as the parent of given head occurs in the parents set above first time.

16. `public static void merge(string branchName)`
   error check;
   get the split point of current branch and given branch;

   any files that have been modified in the given branch since the split point, but not modified in the current branch since the split point should be changed to their versions in the given branch;

   any files that have been modified in the current branch but not in the given branch since the split point should stay as they are;

   any files that have been modified in both the current and given branch in the same way are left unchanged by the merge;

   any files that were not present at the split point and are present only in the current branch should remain as they are;

   any files that were not present at the split point and are present only in the given branch should be checked out and staged;

   any files present at the split point, unmodified in the current branch, and absent in the given branch should be removed (and untracked);

   any files present at the split point, unmodified in the given branch, and absent in the current branch should remain absent;

   any files modified in different ways in the current and given branches are in conflict;

   set current head as first parent, given head as second parent;

   add new merged commit to the current branch.

### Utils
1. `std::string sha1(std::initializer_list<std::string> lst)`
   Return a unique string value of SHA-1 according to the input strings.

2. `std::string readContent(std::string& file_name)`
   check if the file is a regular file
   read its content to a string
   return the string

3. `std::string readContent(fs::path& file_name)`
   same as above

4. `void writeContents(std::string& file_name, std::initializer_list<std::string> lst)`
   Write all contents in lst to the file, creating or overwriting it as needed.

5. `void writeContents(fs::path& file_name, std::initializer_list<std::string> lst)`
   same as above

6. `void create_file(const fs::path& p)`

7. `template <typename T> void readObject(const std::string& file_name, T& object)`
   Serialize an object and write it into target file. The function serialization is 
   supposed by the Boost Libraries.

8. `template <typename T> void writeObject(const std::string& file_name, const T& object)`
   Read data from target file and deserialize the data to the target object. The function 
   deserialization is supported by the Boost Libraries.

9. `std::list<std::string> plainFilenamesIn(const fs::path& dir_name)`
   Returns a list of the names of all plain files in the target directory, in lexicographic order.
   Returns null if DIR does not denote a directory.

10. `std::fstream join(const std::string& first, std::initializer_list<std::string> lst)`
   Return the concatenation of FIRST and OTHERS into a file stream.

11. `void message(const std::string& msg, std::initializer_list<std::string> lst)`
   Print a message composed from MSG and LST, followed by a newline.


## Persistence
### Directory Structure
````
CWD/
   - .gitlet/ -- top level folder for all persistence data
      - blobs/ -- folder for persistence data of Blob objects
         - blob1 -- a single Blob object stored to a file
         - blob2
         - ...
         - blobN
      - commits/ -- folder for persistence data of Commit objects
         - subdiretory1/ -- folder name is first six characters of SHA-1 ref of commit object
            - commit1 -- file name is the rest charaters of SHA-1 ref of the commit object
         - subdiretory2/
            - commit2
         ...
         - subdiretoryN/
            - commitN
      - HEAD -- the front of current branch of Gitlet
      - INDEX -- file containing persistence data of Staging Area
      - TREE -- file containing persistence data of commit tree
      - config - file recording name and location of remote repo
````

### IO
The `Repository` class will set up all persistence. It will: 
1. Create `.gitlet` folder if it doesn't exist.
2. Create `blobs` folder if it doesn't exist.
3. Create `commits` folder if it doesn't exist.
4. Create `HEAD` file if it doesn't exist.
5. Create `INDEX` file if it doesn't exist.

### Class Blob
The `Blob` class will handle the serialization of `Blob` objects.
1. `public void readFromOriginalFile();` Read the contents of the file in CWD using 
   `Utils.readContentAsString()` method.

2. `public void writeToOriginalFile();`  Write `this.content` to the file in CWD 
   named `.gitlet/blobs/this.fileID`

3. `public void readFromBlobFile();` Read from the file named `.gitlet/blobs/this.fileID` 
   and convert it into `Blob` object using `Utils.readObject();` method.

4. `public void writeToBlobFile();` Write an entire `Blob` object into the file 
   named `.gitlet/blobs/this.fileID` using `Utils.writeObject()` method.
   (Actually, blob file is not allowed to be modified, so the calling of the function 
   is just only one time.)

### Class Commit
The `Commit` class will handle the serialization of `Commit` objects.
1. `public void readFromFile();` Read from the file `join(COMMIT_DIR, fileID)`
   and convert the data into `Commit` object using `Utils.readObject()` method.

2. `public void writeToFile();` Write `this` to the file `join(COMMIT_DIR, fileID)`
   using `Utils.writeObject()` method.
   
The `StagingArea` class will handle the serialization of `StagingArea` objects.
1. `public void readFromFile();` Read from the file `.gitlet/INDEX` and convert 
   the data into `StagingArea` object using `Utils.readObject()` method.
   
2. `public void writeToFile();` Write `this` to the file `.gitlet/INDEX` using 
   `Utils.writeObject()` method.

### Class StagingArea`
1. `public void readFromFile()`
   Check out the existence of file `Repository.INDEX`, if not, return;
   Read object from file `Repository.INDEX` by `Utils.h::readObject()` method 
   and update member variables `addition` and `removal` of current object.

2. `public void writeToFile()`
   Write current object into file `Repository.INDEX` created if not exist by `Utils.h::writeObject()` method.
