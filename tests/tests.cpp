/**
 * @file Tests.hpp
 * @brief Moduł testów jednostkowych do sprawdzania czy serce programu bije prawidłowo.
 */

#ifndef TESTS_HPP
#define TESTS_HPP

#include <cassert>
#include <iostream>
#include "MealData.hpp"

/**
 * @class UnitTests
 * @brief Klasa zawierająca testy sprawdzające poprawność działania obiektów MealData.
 * @details Testy jednostkowe (Unit Tests) to małe programy sprawdzające większy program. 
 * Wykorzystujemy tu makro assert(), które przerywa program, jeśli wykryje błąd.
 */
class UnitTests {
public:
    /**
     * @brief Główna funkcja uruchamiająca wszystkie testy.
     * * Prowadzący zapyta: "Po co to wywołujemy w main?"
     * * Twoja odpowiedź: "Aby upewnić się, że podstawowa logika programu działa poprawnie przed uruchomieniem okna."
     */
    static void runAll() {
        // 1. Tworzymy obiekt testowy
        MealData testData;

        // 2. Wpisujemy dane za pomocą "setterów" (bo pola są prywatne!)
        testData.setKcal(500);
        testData.setProtein(20);
        testData.setRawDescription("Kurczak");

        // 3. SPRAWDZANIE (Asserty)
        // Jeśli testData.getKcal() NIE zwróci 500, program się wyłączy z błędem.
        // Jeśli zwróci 500 - idziemy dalej. To nasza siatka bezpieczeństwa.
        assert(testData.getKcal() == 500);
        assert(testData.getProtein() == 20);
        assert(testData.getRawDescription() == "Kurczak");

        // 4. Testowanie wartości domyślnych (czy na starcie jest 0?)
        MealData emptyData;
        assert(emptyData.getFat() == 0);
        assert(emptyData.getCarbs() == 0);

        // Jeśli dojdziemy tutaj, to znaczy, że wszystko powyżej działa idealnie.
        std::cout << "[INFO] Testy jednostkowe przeszly pomyslnie!" << std::endl;
    }
};

#endif