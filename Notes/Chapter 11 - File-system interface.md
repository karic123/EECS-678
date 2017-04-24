# Chapter 

Rachel's summary notes

* [Official book slides - Chapter 11](http://codex.cs.yale.edu/avi/os-book/OS9/slide-dir/PPT-dir/ch11.ppt)

---

# Notes

---

## Vocabulary

* File concept
  * file
  * text file, source file, executable file
  * File attributes
    * Name, identifier, type, location, size, protection, time & date & user identification
    * extended file attributes
  * file operations
    * creating a file
    * writing a file
      * write pointer
    * reading a file
      * read pointer
      * current-file-position pointer
    * repositioning within a file
      * seek
    * deleting a file
    * truncating a file
    * open-file table
    * open count
    * file pointer
    * file-open count
    * disk location of the file
    * access rights
    * shared lock
    * exclusive lock
    * mandatory or advisory file-locking mechanisms
  * file types
    * shell script
    * magic number
  * file structure
  * internal file structure
* Access methods
  * sequential access
  * direct access (relative access)
    * logical records
    * relative block number
    * allocation problem
  * other access methods
    * index
* Directory and disk structure
  * partition
  * volume
  * device directory
  * volume table of contents
  * storage structure
    * tmpfs - temporary file system
    * objfs - virtual file system
    * ctfs - virtual file system, contract information
    * lofs - loop back filesystem
    * procfs - virtual file system, information on all processes
    * ufs, zfs - general purpose
  * directory overview
    * search for a file
    * create a file
    * delete a file
    * list a directory
    * rename a file
    * traverse the file system
  * single-level directory
  * two-level directory
    * user file directory UFD
    * master file directory MFD
    * path name
    * search path
  * tree-structured directories
    * current directory
    * absolute path
    * relative path name
  * acyclic-graph directories
    * acyclic graph
    * link
    * resolve
    * hard links
  * general graph directory
    * garbage collection
* File-system mounting
  * mount point
* File sharing
* multiple users
  * owner/user
  * group
* remote file systems
  * distributed file system DFS
  * world wide web
  * anonymous access
  * client-server model
    * server
    * client
    * spoofed
  * distributed information systems
    * distributed naming services
    * domain name system DNS
    * network information service NIS
    * common Internet file system CIFS
    * active directory
    * lightweight directory-access protocol LDAP
  * failure modes
    * metadata
    * state information
    * stateless
  * consistency semantics
    * file session
    * UNIX semantics
    * session semantics
    * immutable-shared-files semantics
      * immutable shared files
* Protection
  * types of access
    * Read, write, execute, append, delete, list
    * access control ACL
    * owner, group, universe
  * other protection approaches
