#include <iostream>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include <time.h>

using namespace std;

int main()
{
    time_t timep; 
    time (&timep); 
    printf("%s",ctime(&timep)); 

    time_t seconds;
    seconds = time(&seconds);
    std::cout << "自 1970-01-01 起的second:" << seconds << std::endl;

    system("pause");
    return 0;

}
