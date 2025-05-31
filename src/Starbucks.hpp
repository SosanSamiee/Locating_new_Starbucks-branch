#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <math.h>
#include <algorithm>
#include <cfloat>

struct Coordinates_plase
{
    int x;
    int y;
    std::string plase; //  Starbucks  Subway gas station
    int Coefficient;
    float optimal_distance;
};                       

class Starbucks
{
    private:
        int x,y; // Starbucks coordinates
        int N; // 
        int line; 
        int column;
        int num_place;
        int num_optimal_locations ;
        std::vector<Coordinates_plase> Lication_plases;
        std::vector<std::vector<int>> Map ;
        int x1; // Optimal coordinates
        int x2; // line2
        int y1; // column1
        int y2;
        std::string map_file;
        std::string Data_starbucks;
        std::string Reports;
        std::string Measurement_criteria; // Manhattan & euclidean
        int optimal_average[4];
        std::vector<Coordinates_plase>Possible_coordinates;

        void Optimal_range(int x, int y);
        void possible_middle_point();
        float calculateManhattanDistance(int x1, int y1, int x2, int y2) ;
        float euclideanDistance(int x1, int y1, int x2, int y2);
        void Initialization_map();
        void Optimal_points();
        void sort_optimal(std::vector<Coordinates_plase> distans_N);

    public:
        Starbucks(std::string map_file, std::string Data_starbucks, std::string Reports, int n, std::string Measurement_criteria);
        void Find_points();
        void Calculation_maximum_profit();
        void Report();
};

Starbucks::Starbucks(std::string map_file, std::string Data_starbucks, std::string Reports, int n, std::string Measurement_criteria)
{
    num_place = 0;
    this->map_file = map_file ;
    this->Data_starbucks = Data_starbucks ;
    this->Reports = Reports ;
    N = n;
    this->Measurement_criteria = Measurement_criteria;
    
// The number of optimal locations required
    std::fstream file;
    std::string Row; 
    file.open("../file/"+Data_starbucks , std::ios::in);
    std::getline(file, Row, '\n');
    num_optimal_locations = stoi(Row);
    file.close();
}

float Starbucks::calculateManhattanDistance(int x1, int y1, int x2, int y2) 
{
    return abs(x2 - x1) + abs(y2 - y1);
}

float Starbucks::euclideanDistance(int x1, int y1, int x2, int y2)
{
    int xx = abs(x1 - x2);
    int yy = abs(y1 -y2);
    return  std::sqrt(xx * xx + yy * yy) ;
}

//Initialization
void Starbucks::Initialization_map()
{
    std::fstream file;
    std::string Row; 
    int f = 0;
    file.open("../file/"+map_file , std::ios::in);
    std::string arr_x;
    std::string arr_y;
    std::string place;
    int fin;

    // Dimensions
    std::getline(file, Row, '\n');
    fin = Row.find(' ');

    line = std::stoi( Row.substr(0, fin) )*2+1;
    column = std::stoi( Row.substr(fin+1, Row.length()) )*2+1;
    // std::cout << line << " * " << column << "\n";

    Map.resize(line);
    for (size_t i = 0; i < line; i++)
    {
        Map[i].resize(column, 0);
    }
    
    Coordinates_plase valu ;
    // Taking important places
    while (!file.eof())
    {
        std::getline(file, Row, '\n');
        fin = Row.find(',');

        valu.x = std::stof(Row.substr(1, fin-1))*2 ; // x

        Row = Row.substr(fin+2 , Row.length());
        fin = Row.find(',');

        valu.y = std::stof(Row.substr(0, fin-1))*2 ; // y

        if ( Row.find("Starbucks") != std::string::npos ) // plase & Coefficient
        {
            valu.plase = "Starbucks";
            valu.Coefficient = 1 ;
            Map[valu.x][valu.y] = 1;
        }
        else if ( Row.find("Subway") != std::string::npos)
        {
            valu.plase = "Subway";
            valu.Coefficient = 2 ;
            Map[valu.x][valu.y] = 2;
        }
        else if (Row.find("gas_station") != std::string::npos )
        {
            valu.plase = "gas_station";
            valu.Coefficient = 3 ;
            Map[valu.x][valu.y] = 3;
        }

        Lication_plases.push_back(valu) ;
        num_place++;
    }
    if ( num_optimal_locations > num_place )
    {
        num_optimal_locations = num_place;
    }
}

void Starbucks::Optimal_range(int xx, int yy)
{
    float distans [num_place];    
    float Difference;
    for (size_t i = 0; i < num_place; i++)  
    {
        if( Measurement_criteria == "Manhattan" ) { // Manhattan & Euclidean
            distans[i] = calculateManhattanDistance( xx, yy, Lication_plases[i].x, Lication_plases[i].y );

        }
        else {
            distans[i] = euclideanDistance( xx, yy, Lication_plases[i].x, Lication_plases[i].y );
        }
    }
    

    std::sort(distans, distans+num_place);

    // distans[N-1] -> Radius
    int R = std::round(distans[N-1]);
    x1 = xx - R ;
    x2 = xx + R ;
    y1 = yy - R ;
    y2 = yy + R ;

    if ( x1 < 0) {
        x1 = 0;
    }
    if ( y1 < 0) {
        y1 = 0;
    }
    if ( x2 > line-1) {
        x2 = line-1;
    }
    if ( y2 > column-1 ) {
        y2 = column-1;
    }

    optimal_average[0] = x1;
    optimal_average[1] = x2;
    optimal_average[2] = y1;
    optimal_average[3] = y2;
}

void Starbucks::possible_middle_point()
{
    float Average_x=0;
    float Average_y=0;
    int counter = 0;

// Average
    for (size_t i = 0; i < num_place; i++)
    {
        if ( Lication_plases[i].plase == "Starbucks" )
        {
            Average_x += Lication_plases[i].x ;
            Average_y += Lication_plases[i].y ;
            counter++ ;
        }
        else if ( Lication_plases[i].plase == "Subway" )
        {
            Average_x += Lication_plases[i].x*2 ;
            Average_y += Lication_plases[i].y*2 ;
            counter += 2;
        }
        else if ( Lication_plases[i].plase == "gas_station" )
        {
            Average_x += Lication_plases[i].x*3 ;
            Average_y += Lication_plases[i].y*3 ;
            counter += 3;
        }
    }
    Average_x = std::round(Average_x/counter) ;
    Average_y = std::round(Average_y/counter) ;


// Delate outliers
    float distans [num_place];    
    float Difference;
    for (size_t i = 0; i < num_place; i++)  
    {
        if( Measurement_criteria == "Manhattan" ) { // Manhattan & Euclidean
            distans[i] = calculateManhattanDistance( Average_x, Average_y, Lication_plases[i].x, Lication_plases[i].y );

        }
        else {
            distans[i] = euclideanDistance( Average_x, Average_y, Lication_plases[i].x, Lication_plases[i].y );
        }
    }

    float min = FLT_MAX;
    float max = -1;
    for (size_t i = 0; i < num_place; i++)
    {
        if ( distans[i] >= 5 && min >= distans[i] )
        {
            min = distans[i]; 
        }
        if ( max < distans[i] )
        {
            max = distans[i];
        }
    }
    if ( max == min )
    {
        min = 0;
    }

    Difference = max - min ;

    int count_less = 0;
    for (size_t i = 0; i < num_place ; i++)
    {
        if ( distans[i] > Difference )
        {
            distans[i] = 0;
        }
        else
        {
            count_less++;
        }
    }
    if ( count_less < N )
    {
        Optimal_range(Average_x, Average_y);
        return;
    }
    

// optimal average
    for (size_t i = 0; i < num_place; i++)
    {
        if ( distans[i] != 0 )
        {
            if ( Lication_plases[i].plase == "Starbucks" )
            {
                Average_x += Lication_plases[i].x ;
                Average_y += Lication_plases[i].y ;
                counter++ ;
            }
            else if ( Lication_plases[i].plase == "Subway" )
            {
                Average_x += Lication_plases[i].x*2 ;
                Average_y += Lication_plases[i].y*2 ;
                counter += 2;
            }
            else if ( Lication_plases[i].plase == "gas_station" )
            {
                Average_x += Lication_plases[i].x*3 ;
                Average_y += Lication_plases[i].y*3 ;
                counter += 3;
            }
        }
    }
    Average_x = std::round(Average_x/counter) ;
    Average_y = std::round(Average_y/counter) ;


    Optimal_range(Average_x, Average_y);

}

void Starbucks::Optimal_points()
{

    std::vector<Coordinates_plase> distans_N;
    float distance_set = 0;
    int weight_set = 0;

    for (size_t i = x1; i <= x2; i++)
    {
        for (size_t j = y1; j <= y2; j++)
        {
            int r = i;
            int w = j;
            int f = 1;
            int x_1;
            int xx;
            int y_1;
            int yy;
            int d = 0;
            for (size_t k = 0; k < N && d == 0; f++)
            {
                d = 1;
                if(k < N && r-f >= 0)
                {
                    d = 0;
                    xx = r - f;
                    if(w + f < column)
                        yy = w + f;
                    else
                        yy = column-1;
                    if(w - f >= 0)
                        y_1 = w -f;
                    else
                        y_1 = 0; 

                    for(size_t g = y_1; g <= yy && k < N; g++)
                    {
                        if(Map[xx][g] != 0)
                        {
                            if( Measurement_criteria == "Manhattan" ) { // Manhattan & Euclidean
                                distance_set += calculateManhattanDistance(xx, g, r, w);
                            }
                            else {
                                distance_set += euclideanDistance(xx, g, r, w);
                            }
                            weight_set += Map[xx][g]; 
                            k++;
                        }
                        
                    }
                }
                if(k < N &&  w + f < column)
                {
                    if (r - f + 1 >= 0)
                    {
                        x_1 = r - f + 1;
                    }
                    else
                    {
                        x_1 = 0;
                    }
                    
                    d = 0;
                    yy = w + f;
                    if(r + f < line)
                        xx = r + f;
                    else
                        xx = line - 1;
                    for(size_t g = x_1; g <= xx && k < N; g++)
                    {
                        if(Map[g][yy] != 0)
                        {
                            if( Measurement_criteria == "Manhattan" ) { // Manhattan & Euclidean
                                distance_set += calculateManhattanDistance(g, yy, r, w);
                            }
                            else {
                                distance_set += euclideanDistance(g, yy, r, w);
                            }
                            weight_set += Map[g][yy];
                            k++;

                        }
                    }
                }
                if(k < N && r + f < line)
                {
                    if ( w + f - 1 < column )
                    {
                        yy = w + f - 1;
                    }
                    else
                    {
                        yy = column - 1 ;
                    }
                                        
                    d = 0;
                    xx = r + f;
                    if(w - f  >= 0)
                        y_1 = w - f;
                    else
                        y_1 = 0;
                    for (size_t g = y_1; g <= yy && k < N; g++)
                    {
                        if (Map[xx][g] != 0)
                        {
                            if( Measurement_criteria == "Manhattan" ) { // Manhattan & Euclidean
                                distance_set += calculateManhattanDistance(xx, g, r, w);
                            }
                            else {
                                distance_set += euclideanDistance(xx, g, r, w);
                            }
                            weight_set += Map[xx][g];
                            k++;
                        }
                    }
                }
                if(k < N && w - f >= 0)
                {
                    d = 0;
                    if(r - f + 1 >= 0)
                    {
                        x_1 = r - f + 1;
                    }
                    else
                    {
                        x_1 = 0;
                    }
                    if(r + f - 1 < line)
                    {
                        xx = r + f - 1;
                    }
                    else
                    {
                        xx = line - 1;
                    }
                    yy = w - f;
                    for (size_t g = x_1; g <= xx && k < N; g++)
                    {
                        if (Map[g][yy] != 0)
                        {
                            if( Measurement_criteria == "Manhattan" ) { // Manhattan & Euclidean
                                distance_set += calculateManhattanDistance(g, yy, r, w);
                            }
                            else {
                                distance_set += euclideanDistance(g, yy, r, w);
                            }
                            weight_set += Map[g][yy];
                            k++;
                        }
                    }
                }
            }

            Coordinates_plase t;
            t.x = r;
            t.y = w;
            
            if( distance_set ==  0 || weight_set == 0 || Map[r][w] != 0 )
            {
                t.optimal_distance = FLT_MAX;
            }
            else
            {
                t.optimal_distance = distance_set / weight_set;
            }
            
            distans_N.push_back(t);
            distance_set = 0;
            weight_set = 0; 
        }
        
    }
    sort_optimal(distans_N);
}

void Starbucks::sort_optimal(std::vector<Coordinates_plase> distans_N)
{
    float Maxim = FLT_MAX;
    int f = 0;
    Coordinates_plase c;
    for (size_t j = 0; j < num_optimal_locations; j++)
    {
        for (size_t i = 0; i < distans_N.size(); i++)
        {
            if(distans_N[i].optimal_distance < Maxim && distans_N[i].optimal_distance != 0)
            {
                Maxim = distans_N[i].optimal_distance;
                f = i;
            }
        } 
        c.x = distans_N[f].x;
        c.y = distans_N[f].y;
        c.optimal_distance = std::round(distans_N[f].optimal_distance);
        Possible_coordinates.push_back(c);

        distans_N[f].optimal_distance = FLT_MAX;
        Maxim = FLT_MAX;
    }

}

void Starbucks::Calculation_maximum_profit()
{
    
    // Greedy (per kilo)
    std::fstream file;
    std::string Row; 
    file.open("../file/"+Data_starbucks , std::ios::in);

    std::getline(file, Row, '\n');
    std::getline(file, Row, '\n');

    double per_kilo[num_optimal_locations];
    double Cost;
    int demand;
    
    for ( int i=0 ;!file.eof(); i++)
    {
        std::getline(file, Row, '\n');
        demand = std::stoi(Row);
        
        std::getline(file, Row, '\n');
        Cost = std::stoll(Row);

        per_kilo[i] = Cost/demand;
        std::getline(file, Row, '\n');

    }
    int Which = 0;
    double less =  per_kilo[0];
    for (size_t i = 1; i < num_optimal_locations; i++)
    {
        if( less > per_kilo[i])
        {
            less = per_kilo[i];
            Which = i;
        }
    }
    

    x = Possible_coordinates[Which].x ;
    y = Possible_coordinates[Which].y ;

    file.close();
}

void Starbucks::Find_points()
{
    Initialization_map();

    possible_middle_point();

    Optimal_points();
}

void Starbucks::Report()
{    
    std::ofstream file("../report/"+Reports, std::ios_base::app);//open file

    if (!file.is_open()) 
    {
        std::cerr << "ERROR: The report_file does not exist.\n";
        return;
    }

    file << "Measurement criteria : " << Measurement_criteria << "\n";
    file << "number of N : " << N << "\n";
    file << "optimal average : " << "\n" ;
    file << "x1: " << optimal_average[0] << "  x2: " << optimal_average[1] << "\n" ;
    file << "y1: " << optimal_average[2] << "  y2: " << optimal_average[3] << "\n" ;
    file << "5 optimal points : " << "\n";
    for (size_t i = 0; i < num_optimal_locations; i++)
    {
        file << Possible_coordinates[i].x << " " <<  Possible_coordinates[i].y << "\n";
    }
    
    file << "The best place to build a Starbucks : " << x << " " << y << "\n";
    file << "-----------------------------------------------------" << "\n" ;

    file.close();
}
