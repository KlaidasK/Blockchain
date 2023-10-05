#include <iostream>
#include <fstream>
#include <istream>
#include <iomanip>
#include <chrono>
#include <bitset>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <random>
#include <numeric>
#include <execution>

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::endl;

class Timer { //Laiko matavimas
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

std::string StrG() { //String generatorius

    std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, charset.length() - 1);

    std::string line;
    int sz = 500;
    for (int i = 0; i < sz; ++i) {
        line += charset[distribution(gen)];
    }

    return line;
}

void stringGen() { //String'ų generatorius
    string ch = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string line = "";

    line = StrG();
    string temp = line;
    std::ofstream fr("1symbol.txt");
    int counter = 0;
    bool stop = false;
    int j = 0;

    for (int j = 0; j < 4; j++) {
        cout << "Jo" << endl;
        counter = 0;
        for (int i = 0; i < temp.size(); i++) {
            for (int ii = 0; ii < ch.size(); ii++)
            {
                if (line[i] != ch[ii]) {
                    temp[i] = ch[ii];
                    fr << temp << endl;
                    counter++;
                    if (counter == 25000) { stop = true; }
                }

            }
            if (stop == true) break;
            temp.clear();
            temp = line;
        }
        stop = false;
        line = StrG();
        temp = line;

    }
}

std::string customHash(const std::string& input) {

    if (input.empty()) { //Patikrina ar failas ne tuščias
        return "Nera teksto";
    }


    unsigned char hash[32];

    int sum = 0; //Sumuojama string'o visų char ascii reikšmės
    int k = 1;

    for (int i = 0; i < 32; i++) {
        for (int ii = 0; ii < input.length(); ii++) {
            sum = sum + static_cast<int>(input[ii]) ^ k;
            sum % 31;
            hash[i] ^= sum * 23;
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

void CommandLine(int argc, char* argv[]) { //Hashavimas per komandinę eilutę
    int k{ 0 };
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

void Konst() { //Konstitucijos failo hashavimas
    std::ifstream open_f("konstitucija.txt");
    string line;
    int k = 0; // Eiluciu skaicius
    int max = 64;
    std::stringstream ss;
    if (!open_f.fail()) {
        while (std::getline(open_f, line) && k < max) {
            ss << line;
            k++;
        }
    }
    open_f.close();

    Timer t;
    std::string hashResult = customHash(ss.str());
    std::cout << "256-bit Hexadecimal Hash: " << hashResult << std::endl;

    cout << t.elapsed() << " s" << endl;

}

void BigFile() { //Didelių failų (100000) hashavimas
    string input;
    cout << "Iveskite failo pavidinimą: " << endl;
    cin >> input;
    std::ifstream open_f(input);
    std::ofstream fr("Rezultatai.txt");
    string line;
    int k = 0; // Eiluciu skaicius
    std::vector<string> ss;
    std::vector<string>hash;
    if (!open_f.fail()) {
        while (std::getline(open_f, line)) {
            ss.push_back(line);
            k++;
        }
    }
    open_f.close();
    cout << k << endl;


    Timer t;
    for (int i = 0; i < k; i++) {
        std::string hashResult = customHash(ss[i]);
        hash.push_back(hashResult);
        fr << hashResult << std::endl;
    }

    cout << t.elapsed() << " s" << endl;

    std::list<string> dup;
    int counter = 0;

    std::sort(hash.begin(), hash.end());

    const string  empty{ "" };
    const string* prev_p = &empty;

    for (const string& s : hash) { //Pasikartojančių hash'ų skaičiavimas
        if (*prev_p == s) dup.push_back(s);
        prev_p = &s;
    }

    for (auto& w : dup) { cout << w << endl; counter++; }
    cout << "Pasikartojanciu hash'u: " << counter << endl;
    cout << endl;


}

string TextToBinaryString(string words) { //String'o į bitų vertimas
    string binaryString = "";
    for (char& _char : words) {
        binaryString += std::bitset<8>(_char).to_string();
    }
    return binaryString;
}


void Palyginimas() {
    std::ifstream fd("1symbolrez.txt"); //Nuskaitymas
    //std::ofstream fr("skirtumai.txt");
    std::vector<string> ss;
    string line;
    if (!fd.fail()) { //Nusktaitymas hex'ų
        while (std::getline(fd, line)) {
            ss.push_back(line);
        }
    }
    fd.close();


    std::vector<string>bin;
    for (int i = 0; i < 100000; i++) { //Nuskaitymas bitų
        bin.push_back(TextToBinaryString(ss[i]));
    }

    double skirt = 0; //String'ų skirtumas
    double binskirt = 0; //Bitų skirtumas
    std::vector<double>binskr;
    std::vector<double>skr;
    int LygStr = 0; //Palyginimasis stringas

    for (int i = 0; i < 100000; i++) {
        if (i == 25000) LygStr = 25000;
        if (i == 50000) LygStr = 50000;
        if (i == 75000) LygStr = 75000;
        if (i == 99999) break;

        for (int j = 0; j < ss[0].size(); j++) { //Patikrina po string'o kiekvieną simbolį, jei nesutinka skirtumas didėja

            if (ss[LygStr][j] != ss[i + 1][j])  skirt++;
        }

        for (int k = 0; k < bin[0].size(); k++) { //Patikrina po bito kiekvieną simbolį, jei nesutinka skirtumas didėja
            if (bin[LygStr][k] != bin[i + 1][k])  binskirt++;
        }

        skr.push_back(double(100 * (skirt / ss[0].size())));
        binskr.push_back(double(100 * (skirt / bin[0].size())));
        //fr << "Skirtumas tarp 1 ir " << i + 1 << " hash'u " << skr[i] << "%" << endl;

        skirt = 0;
        binskirt = 0;



    }


    auto const count = static_cast<double>(skr.size());
    double vid = std::reduce(skr.begin(), skr.end()) / count;
    //Vidutinis skirtumas tarp stringų
    cout << "Vidutinis skirtumas tarp hash'u " << vid << "%" << endl;
    // Maziausias skirtumas tarp stringų
    cout << "Maziausias skirtumas " << *min_element(skr.begin(), skr.end()) << "%" << endl;
    // Didžiausias skirtumas tarp stringų
    cout << "Didziausias skirtumas " << *max_element(skr.begin(), skr.end()) << "%" << endl;

    auto const count2 = static_cast<double>(binskr.size());
    double binvid = std::reduce(binskr.begin(), binskr.end()) / count2;
    //Vidutinis skirtumas tarp bitu
    cout << "Vidutinis skirtumas tarp hash'u bitu lygmenyje " << binvid << "%" << endl;
    // Maziausias skirtumas tarp bitu
    cout << "Maziausias skirtumas bitu lygmenyje " << *min_element(binskr.begin(), binskr.end()) << "%" << endl;
    // Didziausias skirtumas tarp bitu
    cout << "Didziausias skirtumas bitu lygmenyje " << *max_element(binskr.begin(), binskr.end()) << "%" << endl;

}





int main(int argc, char* argv[])
{
    //stringGen(); //String generatorius
    char pas = '0';
    cout << "1 - ranka ivedimas" << endl << "2 - Pagal command line argumentus" << endl << "3 - Konstitucijos testavimas" << endl << "4 - Dideliu failu testavimas" << endl << "5 - Palyginti didelius failus" << endl;
    do {
        cin >> pas;
    } while (pas != '1' && pas != '2' && pas != '3' && pas != '4' && pas != '5');

    if (pas == '1') { //Hashuoti įvestą string'ą
        string input;
        cout << "Iveskite eilute: ";
        cin >> input;
        cout << customHash(input) << endl;

    }
    if (pas == '2') { CommandLine(argc, argv); } //Hashuoti failus pagal komandinę eilutę
    if (pas == '3') { Konst(); } // Hahsuoti konstitucijos tekstinį failą
    if (pas == '4') { BigFile(); } //Hashuoti didelius (100000) failus
    if (pas == '5') { Palyginimas(); } //Palyginti didelius (100000) failus


    return 0;
}

