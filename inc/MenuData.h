#ifndef MENUDATA_H_
#define MENUDATA_H_

#include "../System/System.h"


/*************** Data for Day Strings ***************/
typedef struct DayTable_s {
    DayOfWeek day;
    char*     dayString;
} DayTable_s;

extern const DayTable_s DayTable[] = {
    {Monday,       "Monday"}, {Tuesday,   "Tuesday"}, 
    {Wednesday, "Wednesday"}, {Thursday, "Thursday"}, 
    {Friday,       "Friday"}, {Saturday, "Saturday"}, 
    {Sunday,       "Sunday"}, 
};
/***************************************************/


/************** Data for Month Strings *************/
typedef struct MonthTable_s {
    MonthOfYear month;
    char*       monthString;
};

extern const MonthTable_s MonthTable[] = {
    {January,     "January"}, {February, "February"}, 
    {March,         "March"}, {April,       "April"}, 
    {May,             "May"}, {June,         "June"}, 
    {July,           "July"}, {August,     "August"}, 
    {September, "September"}, {October,   "October"}, 
    {November,   "November"}, {December, "December"}, 
};
/***************************************************/



#endif