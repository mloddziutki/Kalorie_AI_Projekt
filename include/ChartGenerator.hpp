#ifndef CHARTGENERATOR_HPP
#define CHARTGENERATOR_HPP

#include "MealData.hpp"
#include <fstream>
#include <QProcess> 
#include <QString>
#include <vector>
#include <numeric>

class ChartGenerator {
public:
    static void generateAndRun(const std::vector<MealData>& meals) {
        int totalKcal = 0, totalProtein = 0, totalFat = 0, totalCarbs = 0;

        for (const auto& meal : meals) {
            totalKcal += meal.getKcal();
            totalProtein += meal.getProtein();
            totalFat += meal.getFat();
            totalCarbs += meal.getCarbs();
        }
        
        std::ofstream script("generated_chart.py");
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

#ifdef _WIN32
        QString pythonCmd = "python";
#else
        QString pythonCmd = "python3";
#endif
        QProcess::startDetached(pythonCmd, QStringList() << "generated_chart.py");
    }
};

#endif