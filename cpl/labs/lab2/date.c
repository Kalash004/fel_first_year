#include <stdio.h>

int main() {
    int year;
    int month;
    int day;
    int ret = scanf("%i-%i-%i",&year, &month, &day);
    if (ret != 3) return 1;
    char format[] = "%ith %s %i\n";
    switch (month){
        case 1:
            printf(format, day, "January", year);
            break;
        case 2:
            printf(format, day, "February", year);
            break;
        case 3:
            printf(format, day, "March", year);
            break;
        case 4:
            printf(format, day, "April", year);
            break;
        case 5:
            printf(format, day, "May", year);
            break;
        case 6:
            printf(format, day, "June", year);
            break;
        case 7:
            printf(format, day, "July", year);
            break;
        case 8:
            printf(format, day, "August", year);
            break;
        case 9:
            printf(format, day, "September", year);
            break;
        case 10:
            printf(format, day, "October", year);
            break;
        case 11:
            printf(format, day, "November", year);
            break;
        case 12:
            printf(format, day, "December", year);
            break;
        default:
            return 1;
    }
    return 0;

}