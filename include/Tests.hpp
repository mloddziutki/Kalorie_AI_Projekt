/**
 * @file Tests.hpp
 * @brief Proste testy jednostkowe logiki programu.
 */

#ifndef TESTS_HPP
#define TESTS_HPP

#include <cassert>
#include <iostream>
#include "MealData.hpp"

/**
 * @class UnitTests
 * @brief Klasa sprawdzająca poprawność działania magazynu danych MealData.
 */
class UnitTests {
public:
    /**
     * @brief Uruchamia wszystkie testy po kolei.
     */
    static void runAll() {
        testMealDataInitialization();
        std::cout << "[INFO] Wszystkie testy jednostkowe zakonczone sukcesem!" << std::endl;
    }

private:
    /**
     * @brief Testuje, czy metody set i get działają poprawnie oraz czy dane są chronione.
     */
    static void testMealDataInitialization() {
        MealData m;

      
        m.setKcal(500); 
        m.setProtein(20);

        
        assert(m.getKcal() == 500);   // Sprawdza, czy liczba kalorii się zgadza
        assert(m.getProtein() == 20); // Sprawdza, czy białko się zgadza
        
        // Sprawdzamy domyślną inicjalizację
        assert(m.getFat() == 0); 
    }
};

#endif