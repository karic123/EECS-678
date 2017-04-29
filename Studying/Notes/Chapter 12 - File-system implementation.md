# Chapter 

Rachel's summary notes

* [Official book slides - Chapter 12](http://codex.cs.yale.edu/avi/os-book/OS9/slide-dir/PPT-dir/ch12.ppt)


---

# Slide notes

These are the topics covered in the slides, but finding those topics
covered elseware on the internet, such as Wikipedia.

---

# Vocabulary

* File-system struture
  * blocks
  * file systems
  * I/O control
  * basic file system
  * file-organization module
  * logical file system
  * file-control block FCB
  * inode
  * UNIX file system UFS
  * extended file system

* File-system implementation
  * overview
    * boot control block
    * volume control block
      * superblock
      * master file table
    * mount table
    * system-wide open-file table
    * per-process open-file table
    * file descriptor
    * file handle
  * partitions and mounting
    * raw disk
    * boot loader
    * dual-booted
    * root partition
  * virtual file systems VFS
    * vnode
    * inode object
    * file object
    * superblock object
    * dentry object

* Directory implementation
  * linear list
  * hash table

* Allocation methods
  * contiguous allocation
    * dynamic storage-allocation
    * external fragmentation
    * compacts
    * off-line
    * down time
    * on-line
    * extent
  * linked allocation
    * clusters
    * file-allocation table FAT
  * indexed allocation
    * index block
    * linked scheme
    * multilevel scheme
    * combined scheme
      * direct blocks
      * indirect blocks
      * single indirect block
      * double indirect block
      * triple indirect block
  * performance

* Free-space management

* Efficiency and performance

* Recovery

* NFS

* WAFL File System

* Summary
