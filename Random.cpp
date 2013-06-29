#include <iostream>
#include <random>
#include <sstream>

int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::cout << "Enter # of Rand Nums to Generate" << std::endl;
        return -1;
    }

    std::stringstream s;
    s << argv[1];
    int num;
    s >> num;

    std::normal_distribution<float> f(0, 100);
    std::random_device test;

    for(int i = 0; i < num; i++)
        std::cout << f(test) << std::endl;

    std::cout << std::endl;

    return 0;
}
