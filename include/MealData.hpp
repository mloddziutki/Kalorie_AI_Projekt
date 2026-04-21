#ifndef MEALDATA_HPP
#define MEALDATA_HPP

#include <string>

class MealData {
private:
    int kcal;
    int protein;
    int fat;
    int carbs;
    std::string raw_description;

public:
    MealData() : kcal(0), protein(0), fat(0), carbs(0) {}

    void setKcal(int k) { kcal = k; }
    int getKcal() const { return kcal; }

    void setProtein(int p) { protein = p; }
    int getProtein() const { return protein; }

    void setFat(int f) { fat = f; }
    int getFat() const { return fat; }

    void setCarbs(int c) { carbs = c; }
    int getCarbs() const { return carbs; }

    void setRawDescription(const std::string& desc) { raw_description = desc; }
    std::string getRawDescription() const { return raw_description; }
};

#endif