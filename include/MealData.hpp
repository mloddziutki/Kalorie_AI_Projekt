/**
 * @file MealData.hpp
 * @brief Definicja struktury danych posiłku.
 */

#ifndef MEALDATA_HPP
#define MEALDATA_HPP

#include <string>

/**
 * @class MealData
 * @brief Przechowuje informacje o kaloryczności i makroskładnikach.
 */
struct MealData {
    int kcal = 0;      /**< Liczba kalorii */
    int protein = 0;   /**< Gramy białka */
    int fat = 0;       /**< Gramy tłuszczu */
    int carbs = 0;     /**< Gramy węglowodanów */
    std::string raw_description; /**< Oryginalny opis posiłku */
};

#endif