/**
 * @file MealData.hpp
 * @brief Definicja klasy MealData przechowującej szczegółowe dane o posiłku.
 */

#ifndef MEALDATA_HPP
#define MEALDATA_HPP

#include <string>

/**
 * @class MealData
 * @brief Klasa modelowa reprezentująca pojedynczy posiłek.
 * @details Przechowuje wartości liczbowe (makroskładniki) oraz opis tekstowy.
 * Wykorzystuje mechanizm enkapsulacji (prywatne pola i publiczne akcesory).
 */
class MealData {
private:
    // --- POLA PRYWATNE (Ukryte dane, nikt z zewnątrz ich nie zmieni bezpośrednio) ---
    int kcal;                   ///< Liczba kalorii [kcal]
    int protein;                ///< Zawartość białka [g]
    int fat;                    ///< Zawartość tłuszczu [g]
    int carbs;                  ///< Zawartość węglowodanów [g]
    std::string raw_description; ///< Surowy tekst wpisany przez użytkownika

public:
    /**
     * @brief Konstruktor domyślny.
     * Inicjalizuje wszystkie wartości liczbowe na zero, aby uniknąć "śmieciowych" danych w pamięci.
     */
    MealData() : kcal(0), protein(0), fat(0), carbs(0) {}

    // --- METODY DOSTĘPOWE (Gettery i Settery) ---
    // Pozwalają na bezpieczny odczyt i zapis danych do prywatnych pól klasy.

    /** @brief Ustawia kalorie. @param k wartość kcal. */
    void setKcal(int k) { kcal = k; }
    /** @brief Pobiera kalorie. @return liczba kcal. */
    int getKcal() const { return kcal; }

    /** @brief Ustawia białko. @param p gramy białka. */
    void setProtein(int p) { protein = p; }
    /** @brief Pobiera białko. @return gramy białka. */
    int getProtein() const { return protein; }

    /** @brief Ustawia tłuszcz. @param f gramy tłuszczu. */
    void setFat(int f) { fat = f; }
    /** @brief Pobiera tłuszcz. @return gramy tłuszczu. */
    int getFat() const { return fat; }

    /** @brief Ustawia węglowodany. @param c gramy węgli. */
    void setCarbs(int c) { carbs = c; }
    /** @brief Pobiera węglowodany. @return gramy węgli. */
    int getCarbs() const { return carbs; }

    /** @brief Zapisuje tekstowy opis posiłku. @param desc tekst (np. "bułka z szynką"). */
    void setRawDescription(const std::string& desc) { raw_description = desc; }
    /** @brief Pobiera tekstowy opis posiłku. @return string z opisem. */
    std::string getRawDescription() const { return raw_description; }
};

#endif