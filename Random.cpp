#include <cstddef>    // for std::size_t
#include <functional> // for std::function, std::bind, std::ref
#include <iostream>   // for std::cout, std::endl
#include <string>     // for std::stoi
#include <random>     // for std::random_device,
                      //     std::uniform_real_distribution

void operator*(std::size_t number, std::function<void (void)> func) {
    for(std::size_t i = 0; i < number; ++i)
        func();
}

inline void print_usage() {
    std::cout << "Usage: ./Random [number]" << std::endl;
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        print_usage();
        return 1;
    }

    int num;
    try {
        num = std::stoi(argv[1]);
    } catch(...) {
        print_usage();
        return 1;
    }

    if(num < 0) {
        print_usage();
        return 1;
    }

    std::random_device generator;
    auto distribution = std::uniform_real_distribution<float>{0, 1};

    auto rnd = std::bind(std::ref(distribution),
                         std::ref(generator)
                         ); 

    num * [&rnd](){ std::cout << rnd() << std::endl; };

    return 0;
}
