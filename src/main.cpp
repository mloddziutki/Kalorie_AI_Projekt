#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QProgressBar>
#include <future> 
#include <vector>
#include <fstream>   
#include <iostream>
#include <cstdio> // NOWE: Nagłówek potrzebny do funkcji std::remove

#include "OllamaClient.hpp"
#include "ChartGenerator.hpp"
#include "MealData.hpp"
#include "nlohmann/json.hpp" 

using json = nlohmann::json;

class CalorieApp : public QWidget {
public:
    CalorieApp() {
        setWindowTitle("Dziennik Kalorii AI");
        resize(650, 550); // Lekko poszerzono okno, żeby zmieścić 3 przyciski
        setStyleSheet("background-color: #f5f5f5;");

        auto* layout = new QVBoxLayout(this);

        QLabel* label = new QLabel("<b>Co dzisiaj zjadles?</b>");
        label->setAlignment(Qt::AlignCenter);

        input = new QLineEdit();
        input->setPlaceholderText("Wpisz posilek...");
        input->setStyleSheet("padding: 8px; background: white; border: 1px solid #ccc;");

        btnAnalyze = new QPushButton("Dodaj posilek");
        btnAnalyze->setStyleSheet("padding: 10px; background-color: #27ae60; color: white; font-weight: bold;");

        progressBar = new QProgressBar();
        progressBar->setRange(0, 0); 
        progressBar->setVisible(false); 

        table = new QTableWidget(0, 5);
        table->setHorizontalHeaderLabels({"Posilek", "Kcal", "B", "T", "W"});
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        table->setStyleSheet("background: white;");

        auto* bottomLayout = new QHBoxLayout();

        // NOWE: Przycisk czyszczenia
        btnClear = new QPushButton("Zacznij nowy dzien (Reset)");
        btnClear->setStyleSheet("padding: 10px; background-color: #c0392b; color: white; font-weight: bold;");

        btnSave = new QPushButton("Zapisz do pliku");
        btnSave->setStyleSheet("padding: 10px; background-color: #8e44ad; color: white; font-weight: bold;");

        btnShowChart = new QPushButton("Generuj wykres dnia");
        btnShowChart->setStyleSheet("padding: 10px; background-color: #2980b9; color: white; font-weight: bold;");

        // Dodanie 3 przycisków na dół
        bottomLayout->addWidget(btnClear);
        bottomLayout->addWidget(btnSave);
        bottomLayout->addWidget(btnShowChart);

        layout->addWidget(label);
        layout->addWidget(input);
        layout->addWidget(btnAnalyze);
        layout->addWidget(progressBar); 
        layout->addWidget(table);
        layout->addLayout(bottomLayout);

        // Podpięcie sygnałów
        connect(btnAnalyze, &QPushButton::clicked, this, &CalorieApp::onAnalyze);
        connect(btnShowChart, &QPushButton::clicked, this, &CalorieApp::onShowChart);
        connect(btnSave, &QPushButton::clicked, this, &CalorieApp::onSave);
        connect(btnClear, &QPushButton::clicked, this, &CalorieApp::onClear); // Podpięcie nowego przycisku

        loadDayLog();
    }

private slots:
    void onAnalyze() {
        std::string text = input->text().toStdString();
        if(text.empty()) return;

        btnAnalyze->setEnabled(false);
        btnAnalyze->setText("AI analizuje posilek... Prosze czekac.");
        input->setDisabled(true);
        progressBar->setVisible(true); 

        fut = std::async(std::launch::async, [this, text]() {
            try {
                OllamaClient client("llama3");
                MealData data = client.askForData(text);
                data.setRawDescription(text);

                QMetaObject::invokeMethod(this, [this, data]() {
                    if (data.getKcal() > 0) {
                        dayLog.push_back(data);
                        updateTable(data);
                        input->clear();
                    } else {
                        QMessageBox::warning(this, "Blad", "AI nie rozpoznalo posilku.");
                    }
                    
                    btnAnalyze->setEnabled(true);
                    btnAnalyze->setText("Dodaj posilek");
                    input->setDisabled(false);
                    progressBar->setVisible(false); 
                });
            } catch (...) {
                QMetaObject::invokeMethod(this, [this]() {
                    QMessageBox::critical(this, "Blad", "Blad polaczenia z Ollama.");
                    btnAnalyze->setEnabled(true);
                    btnAnalyze->setText("Dodaj posilek");
                    input->setDisabled(false);
                    progressBar->setVisible(false);
                });
            }
        });
    }

    void onShowChart() {
        if (dayLog.empty()) {
            QMessageBox::information(this, "Info", "Brak danych do wykresu.");
            return;
        }
        ChartGenerator::generateAndRun(dayLog);
    }

    void onSave() {
        if (dayLog.empty()) {
            QMessageBox::information(this, "Info", "Brak danych do zapisu.");
            return;
        }

        btnSave->setEnabled(false);
        btnSave->setText("Zapisywanie...");
        progressBar->setVisible(true);

        std::vector<MealData> logCopy = dayLog;

        saveFut = std::async(std::launch::async, [this, logCopy]() {
            bool success = false;
            
            try {
                json j_array = json::array();
                
                for (const auto& meal : logCopy) {
                    json j_meal = {
                        {"description", meal.getRawDescription()},
                        {"kcal", meal.getKcal()},
                        {"protein", meal.getProtein()},
                        {"fat", meal.getFat()},
                        {"carbs", meal.getCarbs()}
                    };
                    j_array.push_back(j_meal);
                }

                std::ofstream file("dziennik.json");
                if (file.is_open()) {
                    file << j_array.dump(4);
                    file.close();
                    success = true;
                }
            } catch (const std::exception& e) {
                std::cerr << "Blad zapisu do pliku: " << e.what() << std::endl;
            }

            QMetaObject::invokeMethod(this, [this, success]() {
                if (success) {
                    QMessageBox::information(this, "Sukces", "Zapisano dane do pliku dziennik.json");
                } else {
                    QMessageBox::critical(this, "Blad", "Nie udalo sie zapisac pliku.");
                }
                btnSave->setEnabled(true);
                btnSave->setText("Zapisz do pliku");
                progressBar->setVisible(false);
            });
        });
    }

    // NOWE: Funkcja czyszcząca dane
    void onClear() {
        if (dayLog.empty()) {
            QMessageBox::information(this, "Info", "Dziennik jest juz pusty.");
            return;
        }

        // Pytamy użytkownika o potwierdzenie
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Potwierdzenie", 
                                      "Czy na pewno chcesz usunac cala dzisiejsza historie? Tej operacji nie mozna cofnac.",
                                      QMessageBox::Yes | QMessageBox::No);
                                      
        if (reply == QMessageBox::Yes) {
            // 1. Czyszczenie wektora
            dayLog.clear();
            
            // 2. Czyszczenie interfejsu (usuwa wszystkie wiersze, zostawia nagłówki)
            table->setRowCount(0);
            
            // 3. Usunięcie pliku z dysku
            if (std::remove("dziennik.json") == 0) {
                QMessageBox::information(this, "Sukces", "Rozpoczęto nowy dzien!");
            } else {
                // Plik mógł nie istnieć (np. ktoś dodał posiłki, ale jeszcze nie kliknął Zapisz)
                QMessageBox::information(this, "Sukces", "Rozpoczęto nowy dzien (brak pliku do usuniecia).");
            }
        }
    }

private:
    void updateTable(const MealData& data) {
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(data.getRawDescription())));
        table->setItem(row, 1, new QTableWidgetItem(QString::number(data.getKcal())));
        table->setItem(row, 2, new QTableWidgetItem(QString::number(data.getProtein()) + "g"));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(data.getFat()) + "g"));
        table->setItem(row, 4, new QTableWidgetItem(QString::number(data.getCarbs()) + "g"));
    }

    void loadDayLog() {
        try {
            std::ifstream file("dziennik.json");
            if (!file.is_open()) {
                return; 
            }

            json j_array;
            file >> j_array;

            for (const auto& j_meal : j_array) {
                MealData data;
                
                data.setRawDescription(j_meal.value("description", "Brak opisu"));
                data.setKcal(j_meal.value("kcal", 0));
                data.setProtein(j_meal.value("protein", 0));
                data.setFat(j_meal.value("fat", 0));
                data.setCarbs(j_meal.value("carbs", 0));

                dayLog.push_back(data);
                updateTable(data);
            }
        } catch (const std::exception& e) {
            std::cerr << "Blad wczytywania pliku: " << e.what() << std::endl;
        }
    }

    std::future<void> fut;
    std::future<void> saveFut; 
    std::vector<MealData> dayLog;
    QLineEdit* input;
    QPushButton* btnAnalyze;
    QPushButton* btnShowChart;
    QPushButton* btnSave;      
    QPushButton* btnClear; // NOWE: Wskaźnik na przycisk czyszczenia    
    QTableWidget* table;
    QProgressBar* progressBar; 
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    CalorieApp w;
    w.show();
    return a.exec();
}