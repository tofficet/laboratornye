#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <tuple>

const double PI = 3.14159265358979323846;

double theoreticalAllInTriangle() {
    double p = (3.0 * std::sqrt(3.0)) / (4.0 * PI);
    return std::pow(p, 4);
}

double theoreticalOneInTriangleOneInEachSegment() {
    double p = (3.0 * std::sqrt(3.0)) / (4.0 * PI);
    double q = 1.0 - p;
    return (8.0 / 9.0) * p * std::pow(q, 3);
}

struct Point {
    double x, y;
    double r, theta;
};

Point generateRandomPointInCircle(std::mt19937& gen, std::uniform_real_distribution<>& disR, 
                                   std::uniform_real_distribution<>& disTheta) {
    double r = disR(gen);
    double theta = disTheta(gen);
    return {r * std::cos(theta), r * std::sin(theta), r, theta};
}

bool isInTriangle(double x, double y, double R) {
    double h = R / 2.0;
    double side = R * std::sqrt(3.0);
    double triangleHeight = (std::sqrt(3.0) / 2.0) * side;
    
    double x1 = 0, y1 = R;
    double x2 = -side/2.0, y2 = -triangleHeight + R;
    double x3 = side/2.0, y3 = -triangleHeight + R;
    
    auto sign = [](double x1, double y1, double x2, double y2, double x3, double y3) {
        return (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3);
    };
    
    double d1 = sign(x, y, x1, y1, x2, y2);
    double d2 = sign(x, y, x2, y2, x3, y3);
    double d3 = sign(x, y, x3, y3, x1, y1);
    
    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    
    return !(has_neg && has_pos);
}

int getSegmentIndex(double theta, double R) {
    double angle = theta;
    if (angle < 0) angle += 2 * PI;
    
    if (angle >= 0 && angle < 2 * PI / 3) return 0;
    if (angle >= 2 * PI / 3 && angle < 4 * PI / 3) return 1;
    return 2;
}

void printResults(const std::string& title, 
                  double theoreticalA, double theoreticalB,
                  const std::vector<int>& trialCounts,
                  const std::vector<double>& empiricalA,
                  const std::vector<double>& empiricalB) {
    std::cout << "\n" << title << std::endl;
    std::cout << "Теоретическая вероятность а) все четыре точки в треугольнике = " << std::fixed << std::setprecision(8) << theoreticalA << std::endl;
    std::cout << "Теоретическая вероятность б) одна в треугольнике, по одной в каждом сегменте = " << theoreticalB << std::endl;
    std::cout << "\nРезультаты моделирования:\n" << std::endl;
    
    for (size_t i = 0; i < trialCounts.size(); i++) {
        std::cout << "Испытаний: " << trialCounts[i] 
                  << " | а) эмпир. = " << std::setprecision(8) << empiricalA[i]
                  << " | б) эмпир. = " << empiricalB[i] << std::endl;
    }
}

int main() {
    std::vector<int> trialCounts = {1000, 10000, 100000};
    const double R = 1.0;
    
    const double THEOR_A = theoreticalAllInTriangle();
    const double THEOR_B = theoreticalOneInTriangleOneInEachSegment();
    
    std::vector<double> empiricalA;
    std::vector<double> empiricalB;
    
    for (int numTrials : trialCounts) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> disR(0.0, R);
        std::uniform_real_distribution<> disTheta(0.0, 2 * PI);
        
        int successA = 0;
        int successB = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            int inTriangle = 0;
            int segmentCount[3] = {0, 0, 0};
            
            for (int point = 0; point < 4; point++) {
                Point p = generateRandomPointInCircle(gen, disR, disTheta);
                
                if (isInTriangle(p.x, p.y, R)) {
                    inTriangle++;
                } else {
                    int seg = getSegmentIndex(p.theta, R);
                    segmentCount[seg]++;
                }
            }
            
            if (inTriangle == 4) {
                successA++;
            }
            
            if (inTriangle == 1 && segmentCount[0] == 1 && segmentCount[1] == 1 && segmentCount[2] == 1) {
                successB++;
            }
        }
        
        empiricalA.push_back(static_cast<double>(successA) / numTrials);
        empiricalB.push_back(static_cast<double>(successB) / numTrials);
    }
    
    printResults("ЗАДАЧА 62: В круг радиуса R вписан правильный треугольник. Брошены 4 точки", 
                 THEOR_A, THEOR_B, trialCounts, empiricalA, empiricalB);
    
    std::cout << "\nАналитические вычисления:\n";
    double p = (3.0 * std::sqrt(3.0)) / (4.0 * PI);
    double q = 1.0 - p;
    std::cout << "P(точка в треугольнике) = 3√3/(4π) = " << std::setprecision(8) << p << std::endl;
    std::cout << "P(точка в сегменте) = " << q/3.0 << std::endl;
    std::cout << "а) P = p^4 = " << theoreticalAllInTriangle() << std::endl;
    std::cout << "б) P = 4! * p * (q/3)^3 = " << theoreticalOneInTriangleOneInEachSegment() << std::endl;
    
    return 0;
}