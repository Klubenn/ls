# ft_ls
*MacOS implementation of ls shell program*

# Usage
```
./ft_ls [-1adfGglRrtu] [arg] ...
```
# Options
```
-1 : Force output to be one entry per line
-a : Include directory entries whose names begin with a dot '.'.
-d : Directories are listed as plain files.
-f : Output is not sorted.  This option turns on the -a option.
-G : Colored output.
-g : Display the group name in the long (-l) format output (the owner name is suppressed).
-l : List in long format. A total sum for all the file sizes is output on a line before the long listing.
-R : Recursively list subdirectories encountered.
-r : Reverse the order of the sort.
-t : Sort by time modified.
-u : Use time of last access, instead of last modification of the file for sorting (-t) or long printing (-l).
```
