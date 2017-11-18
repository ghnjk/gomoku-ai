/**
 * @File sgf2db.h
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-05 14:50:43
 */
#include "utils/stringutils.h"
#include "gomoku/hash_table.h"
#include <string>
using namespace std;
using namespace gomoku;
using namespace utils;
void showHelp(int argc, char ** argv)
{
    printf("[usage] %s sgfFile dbFile [spanCount]\n", argv[0]);
}
int main(int argc, char ** argv)
{
    if(argc != 3 && argc != 4)
    {
        printf("invalid argumens.\n");
        showHelp(argc, argv);
        return -1;
    }
    sgfFile = argv[1];
    dbFile = argv[2];
    int spanCount = 0;
    if(argc == 4)
    {
        spanCount = StringUtils::parse<int>(argv[3]);
    }
    return sgf2db()
    return 0;
}