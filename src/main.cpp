#include <iostream> 
#include "Starbucks.hpp"

int main()
{
    // clear report file
    std::ofstream file("../report/report.txt", std::ios_base::ate);//open file
    file.clear();
    file.close();
    
    // N = 5  and Manhattan
    Starbucks star_bucks1("map_file.txt", "Data_starbucks.txt", "report.txt", 5, "Manhattan"); 
    star_bucks1.Find_points();
    star_bucks1.Calculation_maximum_profit();
    star_bucks1.Report();

    // N = 5  and euclidean
    Starbucks star_bucks2("map_file.txt", "Data_starbucks.txt", "report.txt", 5, "Euclidean"); 
    star_bucks2.Find_points();
    star_bucks2.Calculation_maximum_profit();
    star_bucks2.Report();

    // N = 6  and Manhattan
    Starbucks star_bucks3("map_file.txt", "Data_starbucks.txt", "report.txt", 6, "Manhattan"); 
    star_bucks3.Find_points();
    star_bucks3.Calculation_maximum_profit();
    star_bucks3.Report();

    // N = 6  and euclidean
    Starbucks star_bucks4("map_file.txt", "Data_starbucks.txt", "report.txt", 6, "Euclidean"); 
    star_bucks4.Find_points();
    star_bucks4.Calculation_maximum_profit();
    star_bucks4.Report();

    

    return 0;
}