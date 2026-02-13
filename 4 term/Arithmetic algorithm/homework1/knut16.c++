#include <iostream>
#include <vector>

using namespace std;

using digit_t=uint32_t;
using double_digit_t=uint64_t;

pair<vector<digit_t>, digit_t> short_division(
    const vector<digit_t>& u,
    digit_t v,
    double_digit_t b
){
    vector<digit_t> w(u.size(), 0);
    double_digit_t r=0;
    for (size_t i=0; i<u.size();++i){
        double_digit_t current =r*b+u[i];
        w[i]=current/v;
        r=current%v;


    }
    return {w,static_cast<digit_t>(r)};
}



int main() {
    
vector<digit_t> dividend = {1,2};
    digit_t divisor = 3;
    double_digit_t base = 10;
    
    cout << "Делимое: " << dividend[0] << " " << dividend[1]<< endl;
    cout << "Делитель: " << divisor << endl;
    
    pair<vector<digit_t>, digit_t> result = short_division(dividend, divisor, base);
    vector<digit_t> quotient = result.first;
    digit_t remainder = result.second;
    
    cout << "Частное: " << quotient[0] << " " << quotient[1] << endl;
    cout << "Остаток: " << remainder << endl;
    
    return 0;
}