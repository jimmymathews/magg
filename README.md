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
See `man magg` after installation.
