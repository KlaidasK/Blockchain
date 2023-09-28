// v0.1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <istream>
#include <iomanip>
#include <chrono>
#include <bitset>
#include <string>
#include <sstream>
#include <vector>

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::endl;

class Timer {
private:
    using hrClock = std::chrono::high_resolution_clock;
    std::chrono::time_point<hrClock> start;
public:
    Timer() :start{ hrClock::now() } {}
    void reset() {
        start = hrClock::now();
    }
    double elapsed()const {
        std::chrono::duration<double> diffs = hrClock::now() - start;
        return diffs.count();
    }
};




std::string customHash(const std::string& input) {

    if (input.empty()) {
        return "Nera teksto";
    }

   
    unsigned char hash[32];

    int sum=0;
    int k = 0;
    
    for (int i = 0; i < 32; i++){

        for (int ii = 0; ii < input.length(); ii++) {
            sum = sum + static_cast<int>(input[ii]) * 23^k;
            
            hash[i] ^= sum;
        }
        k++;
        
    }

    // Konvertavimas hash į hex 64 ilgio
    std::stringstream ss;
    for (int i = 0; i < 32; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

void CommandLine(int argc, char* argv[]) {
     int k{0};
    std::vector<string> inputfile;
    for (int count{ 0 }; count < argc; ++count)
        {
            inputfile.push_back(argv[count]);
            k++;
        }


    for (int i = 1; i < k; i++) {
        std::ifstream open_f(inputfile[i]);
        string line;
        if (!open_f.fail()) {
            std::getline(open_f, line);
        }
        std::string hashResult = customHash(line);

        std::cout << "256-bit Hexadecimal Hash: " << hashResult << std::endl;
        open_f.close();
    }
    
}

void Konst() {
        std::ifstream open_f("konstitucija.txt");
        string line;
        int k=0; // Eiluciu skaicius
        int max = 64;
        std::stringstream ss;
        if (!open_f.fail()) {
            while (std::getline(open_f, line)&& k < max) {
                ss<<line;
                k++;
            }
        }
        open_f.close();

        Timer t;
        std::string hashResult = customHash(ss.str());
        std::cout << "256-bit Hexadecimal Hash: " << hashResult << std::endl;
       
        cout << t.elapsed() << " s" << endl;

}    





int main(int argc, char* argv[])
{

    char pas='0';
    cout << "1 - ranka ivedimas" << endl << "2 - Pagal command line argumentus" << endl << "3 - Konstitucijos testavimas"<<endl;
    do {
        cin >> pas;
    } while (pas !='1' && pas!='2' && pas != '3');

    if (pas == '1') {
        string input; 
        cout << "Iveskite eilute: ";
        cin >> input;
        cout<<customHash(input)<<endl;

    }
    if (pas == '2') { CommandLine(argc, argv); }
    if(pas=='3'){ Konst();}
   

    return 0;
}

