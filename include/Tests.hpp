#ifndef TESTS_HPP
#define TESTS_HPP

#include <cassert>
#include <iostream>
#include "MealData.hpp"

/**
 * @brief Proste testy jednostkowe logiki programu.
 */
class UnitTests {
public:
    static void runAll() {
        testMealDataInitialization();
        std::cout << "Wszystkie testy zakonczone sukcesem!" << std::endl;
    }

private:
    static void testMealDataInitialization() {
        MealData m;
        m.kcal = 500;
        assert(m.kcal == 500); // Sprawdza czy dane się zapisują
        assert(m.protein == 0); // Sprawdza domyślną inicjalizację
    }
};

#endif