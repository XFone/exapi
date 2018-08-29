#include <stdio.h>

#include "Base.h"
#include "ConConf.h"

/* TEST CASES */
void test_log();
void test_trace();

/** Read command line options
 */
int checkopt(int opt, char *parg)
{
    int result = 0;
    switch (opt) {
    case 'l':
        // card_type = (TDEV_TYPE)atoi(parg);
        break;

    default:
        result = -1;
        break;
    }

    return result; 
}

int main(int argc, char *argv[])
{
    int res;
    if ((res = parse_args(argc, argv, "l", checkopt)) < 0 ||
        optind != argc) {
        printf("Test Utils\n");
        print_usage(argv[0], "",
                    "  -l       create a udp port for transmitting\n"
        );
        return 0;
    }

    test_log();
    test_trace();

    return 0;
}
