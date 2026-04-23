/**
 * @file ChartGenerator.hpp
 * @brief Klasa odpowiedzialna za generowanie wizualizacji danych (wykresów) przy użyciu Pythona.
 */

#ifndef CHARTGENERATOR_HPP
#define CHARTGENERATOR_HPP

#include "MealData.hpp"
#include <fstream>
#include <QProcess> 
#include <QString>
#include <vector>
#include <numeric>

/**
 * @class ChartGenerator
 * @brief Odpowiada za integrację z interpreterem Pythona i biblioteką Matplotlib.
 * @details Klasa sumuje dane z wielu posiłków, generuje skrypt w języku Python 
 * i uruchamia go jako niezależny proces systemowy.
 */
class ChartGenerator {
public:
    /**
     * @brief Główna metoda generująca wykres podsumowujący.
     * @param meals Wektor (lista) obiektów MealData zebranych w ciągu dnia.
     */
    static void generateAndRun(const std::vector<MealData>& meals) {
        // --- 1. SUMOWANIE DANYCH ---
        // Przygotowujemy zmienne na sumy z całego dnia
        int totalKcal = 0, totalProtein = 0, totalFat = 0, totalCarbs = 0;

        // Przechodzimy przez każdy posiłek w liście i dodajemy wartości do sumy
        for (const auto& meal : meals) {
            totalKcal += meal.getKcal();
            totalProtein += meal.getProtein();
            totalFat += meal.getFat();
            totalCarbs += meal.getCarbs();
        }
        
        // --- 2. TWORZENIE SKRYPTU PYTHONA ---
        // Otwieramy plik do zapisu (nadpisujemy stary jeśli istnieje)
        std::ofstream script("generated_chart.py");
        
        // Wpisujemy do pliku kod w języku Python, który narysuje wykres kołowy
        script << "import matplotlib.pyplot as plt\n"
               << "labels = ['Bialko (" << totalProtein << "g)', 'Tluszcz (" << totalFat << "g)', 'Wegle (" << totalCarbs << "g)']\n"
               << "values = [" << totalProtein << ", " << totalFat << ", " << totalCarbs << "]\n"
               << "colors = ['#ff9999','#66b3ff','#99ff99']\n"
               << "fig, ax = plt.subplots(figsize=(7, 7))\n"
               << "ax.pie(values, labels=labels, autopct='%1.1f%%', startangle=140, colors=colors)\n"
               << "plt.title('Podsumowanie dnia: " << totalKcal << " kcal')\n"
               << "info_text = f'Lacznie: Bialko {" << totalProtein << "}g | Tluszcz {" << totalFat << "}g | Wegle {" << totalCarbs << "}g'\n"
               << "plt.figtext(0.5, 0.05, info_text, ha='center', fontsize=12, bbox=dict(facecolor='orange', alpha=0.2))\n"
               << "plt.show()\n";
        script.close(); 

        // URUCHAMIANIE PROCESU 
        // Sprawdzamy system operacyjny, żeby wiedzieć jak zawołać Pythona
#ifdef _WIN32
        QString pythonCmd = "python";  // Na Windows zazwyczaj wystarczy "python"
#else
        QString pythonCmd = "python3"; // Na Linux/Mac zazwyczaj używamy "python3"
#endif

        // QProcess::startDetached odpala Pythona jako zupełnie osobny proces.
        
        QProcess::startDetached(pythonCmd, QStringList() << "generated_chart.py");
    }
};

#endif