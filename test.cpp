#include <iostream>
#include <random>
#include <chrono>

int main() {
    // Use the current time as a seed for the random number generator
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);
    
    // Generate a random number either 0 or 1
    for(int i=0;i<20;i++){
    std::uniform_int_distribution<int> dist(0, 1);
    int random_number = dist(rng);

    std::cout << "Random number (0 or 1): " << random_number << std::endl;
    }

    return 0;
}
