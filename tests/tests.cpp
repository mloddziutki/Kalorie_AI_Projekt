#include <cassert>
#include "MealData.hpp"
#include <iostream>

void runTests() {
    MealData testData;
    testData.kcal = 500;
    testData.protein = 20;
    
    // Sprawdzamy czy dane sie poprawnie zapisuja
    assert(testData.kcal == 500);
    assert(testData.protein == 20);
    
    std::cout << "Testy jednostkowe przeszly pomyslnie!" << std::endl;
}