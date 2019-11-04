magg
====

**magg** (matrix aggregate) writes a given sparse matrix to standard output as a non-sparse CSV-formatted string. It should work on Linux/Unix.

Installation
------------
Use `./build_install.sh`

Simple usage
------------
```
magg sample_entries.csv
```

Output:
```
10.1,0,0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0,0,0
10.3,0,0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0,0,11.5
```

Another usage example
---------------------
```
magg -d, -v -i sample_entries.csv -o aggregated.csv
```

Output:
```
Delimiter selected    : ,
Output filename       : aggregated.csv
Using input file      : sample_entries.csv
Memory available  (MB): 4030.5
Memory max chosen (MB): 3224
Float array size      : 10x10
Float array memory(MB): 0
```

Contents of aggregated.csv:
```
10.1,0,0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0,0,0
10.3,0,0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0,0,0
0,0,0,0,0,0,0,0,0,11.5
```

More information
----------------
See `man magg` after installation:

```
man(1)                   magg man page                  man(1)

NAME
       magg - matrix aggregate

SYNOPSIS
       magg  [ -d delimiter ] [ [-f | -i] alt_input_filename ]
       [ -o output_filename ] [ -v ] [ -c ] [ -s  number_lines
       ] input_filename

DESCRIPTION
       magg writes a given numerical sparse matrix to standard
       output as a CSV-formatted string. It takes as input the
       sparse  matrix  entries in CSV file format, the entries
       being of the form row index, column index, value.   The
       output  has  one  line for each row index and delimited
       columns for each column index. The  values/entries  are
       those  explicitly specified, and zero if not explicitly
       specified. By default magg assumes that all  row  (col‐
       umn) indices between the minimum and maximum values en‐
       countered should be used in the resulting  matrix,  in‐
       serting  zero entries if necessary. On Linux/Unix, magg
       aborts if it would need to allocate more  than  80%  of
       the memory available according to sysconf .

OPTION
       -d     Delimiter  used  for parsing and output. Default
              is comma (,). Also supports tab by  using  '\t',
              the  character  't',  the string 'tab', or a tab
              literal between single quotes. Also  supports  a
              single space ' '.

       -f     input_filename  Optional explicit declaration of
              the input filename. Otherwise the primary  argu‐
              ment is taken to be the input filename.

       -o     output_filename  If not supplied, output goes to
              standard output STDOUT .

       -v     Verbose output provided to standard output. Only
              provided  if  -o  output_file is supplied (since
              otherwise the processed output goes to  standard
              output as well).

       -c     The  default  behavior  is  to  interpret  lines
              starting with the characters % or # as ignorable
              comment lines. If -c is set, magg will revert to
              naive parsing, even if % or  #  are  present  in
              some tokens.
       -s     number_lines Skips the first number_lines of the
              input file.

EXAMPLES
       magg entry_list.csv > matrix.csv
       magg -d' ' -v -f entry_list.txt -o matrix.csv

AUTHOR
       Jimmy Mathews (jimmy@jmath.read-books.org)

1.0                       01 Nov 2019                   man(1)
```
