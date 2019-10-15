/* ------
Reference:
binary search: https://blog.techbridge.cc/2016/09/24/binary-search-introduction/
linear interpolation: http://boson4.phys.tku.edu.tw/numerical_methods/nm_units/interpolation_n_extrapolation_intro_n_polynomial.htm
------ */

#include<iostream>
#include <fstream>
#include "SerialPort.h"

#define N 76
#define MOTOR_N 8
using namespace std;

int binSearch(double tar, double *data, double len)  // Return the index of which the element is smaller than tar
{
    int L=0, R=len-1, M=(L+R)/2;

    if (tar < data[0])
        return -1;                  // too small
    else if (tar > data[R])
        return -2;                  // too large

    while ((R-L+1) > 2) {           // at least two element in the range
        M = (L+R) / 2;
        if (data[M] <= tar)
            L = M;
        else
            R = M - 1;
    }
    for (int i=R; i>=L; i--)        // binSeach -> linear
        if (data[i] <= tar)
            return i;
}

double interpolation(double a)
{
    static double x[N] = {-9, -8.9, -8.7, -8.44, -8.23, -7.98, -7.59, -7.28, -7.02, -6.74, -6.41, -6.21, -5.97, 
                            -5.57, -5.21, -4.98, -4.8, -4.39, -4.15, -3.9, -3.62, -3.29, -2.94, -2.74, -2.47, -2.26, 
                            -2, -1.74, -1.53, -1.36, -1.15, -0.96, -0.78, -0.61, -0.42, -0.25, -0.12, -0.06, 0.13, 
                            0.25, 0.41, 0.58, 0.77, 0.92, 1.15, 1.4, 1.58, 1.86, 2.08, 2.43, 2.75, 3.07, 3.37, 3.7, 
                            3.98, 4.32, 4.62, 4.89, 5.32, 5.56, 5.94, 6.23, 6.66, 7.04, 7.39, 7.6, 7.7, 8.33, 
                            8.69, 8.83, 9.25, 9.65, 10.07, 10.59, 10.8, 11.23};
    static double y[N] = {1100, 1110, 1120, 1130, 1140, 1150, 1160, 1170, 1180, 1190, 1200, 1210, 1220, 1230, 
                            1240, 1250, 1260, 1270, 1280, 1290, 1300, 1310, 1320, 1330, 1340, 1350, 1360, 1370, 
                            1380, 1390, 1400, 1410, 1420, 1430, 1440, 1450, 1460, 1470, 1530, 1540, 1550, 1560, 
                            1570, 1580, 1590, 1600, 1610, 1620, 1630, 1640, 1650, 1660, 1670, 1680, 1690, 1700, 
                            1710, 1720, 1730, 1740, 1750, 1760, 1770, 1780, 1790, 1800, 1810, 1820, 1830, 1840, 
                            1850, 1860, 1870, 1880, 1890, 1900};

    int index = binSearch(a, x, N);
        
    if (index == -1)
        return (y[1] - y[0]) / (x[1] - x[0]) * (a - x[0]) + y[0];
    if (index == -2)
        return (y[N-1] - y[N-2]) / (x[N-1] - x[N-2]) * (a - x[N-2]) + y[N-2];
    return (y[index+1] - y[index]) / (x[index+1] - x[index]) * (a - x[index]) + y[index];
}

int main(int argc, char **argv)
{
    double tar;
    int result[MOTOR_N]={1500};
    char *port_loc, defult_port[] = "/dev/ttyACM0";

    int data_from_arduino=0;

    /* while (cin>>tar) {
        cout<<interpolation(tar)<<endl<<endl;
    } */

    /* // Send to Arduino with Serial Port
    ofstream arduino;
	arduino.open("/dev/ttyACM0");

	//write to it
    while (cin>>tar) {
        result = (int)interpolation(tar);
        arduino<<char(100)<<char(result/100)<<char(result%100);
    }
    
	arduino.close();
 */

    if (argv[1] == NULL)
        port_loc = defult_port;
    else
        port_loc = argv[1];
    SerialPort arduino(port_loc);
    
    if (arduino.open(115200))
        cout<<"Very good"<<endl;
    else
        cout<<"Very bad"<<endl;

    while (cin>>tar) {
        
        // Input data
        arduino.write(char(100));
        for(int i =0;i<8;i++){

            result[i] = (int)interpolation(tar);
            if(result[i]>1800)result[i]=1800;
            else if(result[i]<1200)result[i]=1200;
            else if(result[i]==1488)result[i]=1500;
            cout<<result[i]<<endl;
            arduino.write(char(result[i]/100));
            arduino.write(char(result[i]%100));
        }
        arduino.write(char(101));        

        // Read data to check
        data_from_arduino = arduino.read();
        cout<<"data_from_arduino: "<<(char)data_from_arduino<<' '<<(int)data_from_arduino<<endl;
        if (data_from_arduino)
            cout<<"Success"<<endl;
        else
            cout<<"Fail"<<endl;

        if (!arduino.available()) {
            cout<<"Arduino disconnected."<<endl;
            //exit(1);
        }
    }
    arduino.close();
    
    return 0;
}
