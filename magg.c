
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

typedef enum {
    STR2INT_SUCCESS,
    STR2INT_OVERFLOW,
    STR2INT_UNDERFLOW,
    STR2INT_INCONVERTIBLE
} str2int_errno;
unsigned long long getTotalSystemMemory()
{
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    return pages * page_size;
};

str2int_errno str2int(int *out, char *s, int base) {
    char *end;
    if (s[0] == '\0' || isspace(s[0]))
        return STR2INT_INCONVERTIBLE;
    errno = 0;
    long l = strtol(s, &end, base);
    /* Both checks are needed because INT_MAX == LONG_MAX is possible. */
    if (l > INT_MAX || (errno == ERANGE && l == LONG_MAX))
        return STR2INT_OVERFLOW;
    if (l < INT_MIN || (errno == ERANGE && l == LONG_MIN))
        return STR2INT_UNDERFLOW;
    if (*end != '\0')
        return STR2INT_INCONVERTIBLE;
    *out = l;
    return STR2INT_SUCCESS;
};

// void str2int_testsuite() {
//     int i;
//     /* Lazy to calculate this size properly. */
//     char s[256];

//     /* Simple case. */
//     assert(str2int(&i, "11", 10) == STR2INT_SUCCESS);
//     assert(i == 11);

//     /* Negative number . */
//     assert(str2int(&i, "-11", 10) == STR2INT_SUCCESS);
//     assert(i == -11);

//     /* Different base. */
//     assert(str2int(&i, "11", 16) == STR2INT_SUCCESS);
//     assert(i == 17);

//     /* 0 */
//     assert(str2int(&i, "0", 10) == STR2INT_SUCCESS);
//     assert(i == 0);

//     /* INT_MAX. */
//     sprintf(s, "%d", INT_MAX);
//     assert(str2int(&i, s, 10) == STR2INT_SUCCESS);
//     assert(i == INT_MAX);

//     /* INT_MIN. */
//     sprintf(s, "%d", INT_MIN);
//     assert(str2int(&i, s, 10) == STR2INT_SUCCESS);
//     assert(i == INT_MIN);

//     /* Leading and trailing space. */
//     assert(str2int(&i, " 1", 10) == STR2INT_INCONVERTIBLE);
//     assert(str2int(&i, "1 ", 10) == STR2INT_INCONVERTIBLE);

//     /* Trash characters. */
//     assert(str2int(&i, "a10", 10) == STR2INT_INCONVERTIBLE);
//     assert(str2int(&i, "10a", 10) == STR2INT_INCONVERTIBLE);

//     /* int overflow.
//      *
//      * `if` needed to avoid undefined behaviour
//      * on `INT_MAX + 1` if INT_MAX == LONG_MAX.
//      */
//     if (INT_MAX < LONG_MAX) {
//         sprintf(s, "%ld", (long int)INT_MAX + 1L);
//         assert(str2int(&i, s, 10) == STR2INT_OVERFLOW);
//     }

//     /* int underflow */
//     if (LONG_MIN < INT_MIN) {
//         sprintf(s, "%ld", (long int)INT_MIN - 1L);
//         assert(str2int(&i, s, 10) == STR2INT_UNDERFLOW);
//     }

//     /* long overflow */
//     sprintf(s, "%ld0", LONG_MAX);
//     assert(str2int(&i, s, 10) == STR2INT_OVERFLOW);

//     /* long underflow */
//     sprintf(s, "%ld0", LONG_MIN);
//     assert(str2int(&i, s, 10) == STR2INT_UNDERFLOW);
// };

int max(int a, int b) {
    if(a >= b) {
        return a;
    } else {
        return b;
    }
};

int min(int a, int b) {
    if(a <= b) {
        return a;
    } else {
        return b;
    }
};

char* concat(char *s1, char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
};

int main(int argc, char* argv[]) {
    char delimiter = ',';

    char filename[512];
    strcpy(filename, "");

    char output_filename[512];
    strcpy(output_filename, "");

    int skip=0;
    bool ignore_comments = true;
    bool verbose = false;
    bool explicit_output = false;
    char* logstr;
    size_t size;
    FILE* log = open_memstream(&logstr, &size);
    int opt;
    while((opt = getopt(argc, argv, ":vcs:d:f:i:o:")) != -1) {
        switch(opt) {
            case 'v':
                verbose = true;
                break;
            case 'c':
                ignore_comments = false;
                break;
            case 's':
                str2int(&skip, optarg, 10);
                fprintf(log, "Skip lines            : %d\n", skip);
                break;
            case 'd':
                if(strcmp(optarg, "tab") == 0 ||
                   strcmp(optarg, "\\t") == 0 ||
                   strcmp(optarg, "t")   == 0 ||
                   strcmp(optarg, "\t")  == 0) {
                    fprintf(log, "Delimiter selected    : TAB\n");
                    delimiter = '\t';
                } else if (strcmp(optarg, ",") == 0) {
                    fprintf(log, "Delimiter selected    : ,\n");
                } else if (strcmp(optarg, " ") == 0) {
                    fprintf(log, "Delimiter selected    : <space>\n");
                    delimiter = ' ';
                } else {
                    fprintf(stderr, "Delimiter must be comma or tab or space.\n");
                    return 0;
                }
                break;
            case 'i':
            case 'f':
                // fprintf(log, "Input filename        : %s\n", optarg);
                strcpy(filename, optarg);
                break;
            case 'o':
                fprintf(log, "Output filename       : %s\n", optarg);
                explicit_output = true;
                strcpy(output_filename, optarg);
                break;
            case ':':
                fprintf(stderr, "Option %c needs a value\n", optopt);
                return 0;
            case '?':
                fprintf(stderr, "Unknown option %c\n", optopt);
                return 0;
        }
    }

    for(; optind < argc; optind++){
        fprintf(log, "Supplied argument     : %s\n", argv[optind]);
        if(strcmp(filename, "") != 0) {
            fprintf(log, "Filename already supplied. Ignoring argument %s\n", argv[optind]);
        } else {
            strcpy(filename, argv[optind]);
        }
    }
    fprintf(log, "Using input file      : %s\n", filename);

    unsigned long long l = getTotalSystemMemory();
    fprintf(log, "Memory available  (MB): %.1f\n", l*1.0/1000000);
    int max_memory_mb = (int)(0.8*l*1.0/1000000);
    fprintf(log, "Memory max chosen (MB): %d\n", max_memory_mb);
    if(access(filename, R_OK) == -1 ) {
        fprintf(stderr, "Don't have access to input file or file doesn't exist.\n");
        return 0;
    }

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        fprintf(stderr, "Couldn't retrieve current working directory.");
        return 0;
    }

    FILE* file = fopen(concat(concat(cwd,"/"), filename), "r");
    int maxsize = 150; //can afford to make this pretty big. will warn if too big for later.
    char str[maxsize];
    int count = 0;
    int rowmin = -1;
    int rowmax = -1;
    int colmin = -1;
    int colmax = -1;
    while (fgets(str, maxsize, file) != NULL) {
        count = count + 1;
        if(count <= skip) {
            continue;
        }

        if(strcmp(str, "\n") == 0) {
            continue;
        }

        if(strlen(str)>0) {
            if(str[0] == '#' || str[0] == '%') {
                continue;
            }
        }

        char dd[2];
        dd[1] = '\0';
        dd[0] = delimiter;
        char *pch = strtok (str, dd);
        int row = -1;
        int col = -1;
        float val = 0;
        int field_count = 0;
        while (pch != NULL && field_count < 3)
        {
            field_count++;
            switch(field_count) {
                case 1:
                    str2int(&row, pch, 10);
                break;
                case 2:
                    str2int(&col, pch, 10);
                break;
                case 3:
                    // str2float(&val, pch);
                    val = strtof(pch, NULL);
            }
            pch = strtok (NULL, dd);
        }
        if(field_count < 3) {
            fprintf(stderr, "Line %d doesn't have enough fields.\n", count);
            return 0;
        }
        // if(row == NULL || col == NULL || val == NULL) {
        //     fprintf(stderr, "Parsing failed on line %d.\n", count);
        //     return 0;
        // }
        if(rowmin == -1) {
            rowmin = row;
        } else {
            rowmin = min(rowmin, row);
        }
        if(rowmax == -1) {
            rowmax = row;
        } else {
            rowmax = max(rowmax, row);
        }
        if(colmin == -1) {
            colmin = col;
        } else {
            colmin = min(colmin, col);
        }
        if(colmax == -1) {
            colmax = col;
        } else {
            colmax = max(colmax, col);
        }
    }
    fclose(file);

    if(rowmin < 0 || colmin < 0) {
        fprintf(stderr, "Row or column indices seem incorrect.\n");
        return 0;
    }

    file = fopen(concat(concat(cwd,"/"),filename), "r");
    count = 0;
    int rows = (rowmax - rowmin) + 1;
    int cols = (colmax - colmin) + 1;
    int size_mb = (int)(sizeof(float)*rows*cols/1000000);
    if(size_mb > max_memory_mb) {
        fprintf(stderr, "Expected float array size %d x %d too large (%d MB).\n", rows, cols, size_mb);
        return 0;
    } else {
        fprintf(log, "Float array size      : %dx%d\n", rows, cols);
        fprintf(log, "Float array memory(MB): %d\n", size_mb);
    }

    fclose(log);
    if(!explicit_output) {
        verbose = false;
    }
    if(verbose) {
        printf(logstr);
    }

    float **arr;
    arr = (float **)malloc(sizeof(float *)*rows);
    for(int i=0; i<rows; i++) {
        arr[i] = (float *)malloc(sizeof(float)*cols);
    }
    for(int i=0; i<rows; i++) {
        for(int j=0; j<cols; j++) {
            arr[i][j] = 0;
        }
    }

    while (fgets(str, maxsize, file) != NULL) {
        count = count + 1;
        if(count <= skip) {
            continue;
        }

        if(strcmp(str, "\n") == 0) {
            continue;
        }

        if(strlen(str)>0) {
            if(str[0] == '#' || str[0] == '%') {
                continue;
            }
        }

        char dd[2];
        dd[1] = '\0';
        dd[0] = delimiter;
        char *pch = strtok (str, dd);
        int row = -1;
        int col = -1;
        float val = 0;
        int field_count = 0;
        while (pch != NULL && field_count < 3)
        {
            field_count++;
            switch(field_count) {
                case 1:
                    str2int(&row, pch, 10);
                break;
                case 2:
                    str2int(&col, pch, 10);
                break;
                case 3:
                    val = strtof(pch, NULL);
            }
            pch = strtok (NULL, dd);
        }
        if(field_count < 3) {
            fprintf(stderr, "Line %d doesn't have enough fields.\n", count);
            return 0;
        }
        arr[row-rowmin][col-colmin] = val;
    }
    fclose(file);

    if(explicit_output) {
        FILE* outfile = fopen(concat(concat(cwd,"/"),output_filename), "w");
        for(int i=0; i<rows; i++) {
            for(int j=0; j<cols-1; j++) {
                if(arr[i][j] == 0) {
                    fprintf(outfile, "0%c", delimiter);
                } else {
                    fprintf(outfile, "%.6g%c", arr[i][j], delimiter);
                }
            }
            if(arr[i][cols-1] == 0) {
                fprintf(outfile, "0\n");
            } else {
                fprintf(outfile, "%.6g\n", arr[i][cols-1]);
            }
        }
        fclose(outfile);
    } else {
        for(int i=0; i<rows; i++) {
            for(int j=0; j<cols-1; j++) {
                if(arr[i][j] == 0) {
                    printf("0%c", delimiter);
                } else {
                    printf("%.6g%c", arr[i][j], delimiter);
                }
            }
            if(arr[i][cols-1] == 0) {
                printf("0\n");
            } else {
                printf("%.6g\n", arr[i][cols-1]);
            }
        }
    }

    return 0;
}





