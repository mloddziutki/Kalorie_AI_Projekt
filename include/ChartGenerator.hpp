#ifndef CHARTGENERATOR_HPP
#define CHARTGENERATOR_HPP

#include "MealData.hpp"
#include <fstream>
#include <QProcess> 
#include <QString>

class ChartGenerator {
public:
    static void generateAndRun(const MealData& data) {
        
        std::ofstream script("generated_chart.py");
        script << "import matplotlib.pyplot as plt\n"
               << "labels = ['Bialko (" << data.protein << "g)', 'Tluszcz (" << data.fat << "g)', 'Wegle (" << data.carbs << "g)']\n"
               << "values = [" << data.protein << ", " << data.fat << ", " << data.carbs << "]\n"
               << "colors = ['#ff9999','#66b3ff','#99ff99']\n"
               << "fig, ax = plt.subplots(figsize=(7, 7))\n"
               << "ax.pie(values, labels=labels, autopct='%1.1f%%', startangle=140, colors=colors)\n"
               << "plt.title('Podsumowanie posilku: " << data.kcal << " kcal')\n"
              
               << "info_text = f'Skladniki: Bialko { " << data.protein << " }g | Tluszcz { " << data.fat << " }g | Wegle { " << data.carbs << " }g'\n"
               << "plt.figtext(0.5, 0.05, info_text, ha='center', fontsize=12, bbox=dict(facecolor='orange', alpha=0.2))\n"
               << "plt.show()\n";
        script.close();

       
        QProcess::startDetached("python", QStringList() << "generated_chart.py");
    }
};

#endif