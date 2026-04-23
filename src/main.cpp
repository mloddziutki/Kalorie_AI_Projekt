/**
 * @file main.cpp
 * @brief Główny plik aplikacji "Dziennik Kalorii AI".
 * * Plik zawiera definicję klasy CalorieApp, która odpowiada za wyświetlanie okna,
 * obsługę przycisków, zapisywanie danych do pliku JSON oraz asynchroniczną komunikację z AI.
 */

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
#include <cstdio> 

#include "OllamaClient.hpp"
#include "ChartGenerator.hpp"
#include "MealData.hpp"
#include "nlohmann/json.hpp" 

using json = nlohmann::json;

/**
 * @class CalorieApp
 * @brief Główne okno aplikacji interfejsu użytkownika 
 * * Klasa dziedziczy po QWidget i zarządza wszystkimi elementami widocznymi na ekranie
 * oraz listą posiłków zapisaną w pamięci (dayLog).
 */
class CalorieApp : public QWidget {
public:
    /**
     * @brief Konstruktor klasy CalorieApp.
     * * Tutaj budujemy wygląd okna: przyciski, tabelę, kolory i układ .
     * Wywołujemy też wczytywanie danych z dysku na starcie.
     */
    CalorieApp() {
        setWindowTitle("Dziennik Kalorii AI");
        resize(650, 550); 
        setStyleSheet("background-color: #f5f5f5;"); 

        // Główny układ pionowy - układa elementy jeden pod drugim
        auto* layout = new QVBoxLayout(this);

        // Nagłówek tekstowy
        QLabel* label = new QLabel("<b>Co dzisiaj zjadłeś?</b>");
        label->setAlignment(Qt::AlignCenter);

        // Pole tekstowe do wpisywania posiłku
        input = new QLineEdit();
        input->setPlaceholderText("Wpisz posiłek (np. 3 jajka i kromka chleba)...");
        input->setStyleSheet("padding: 8px; background: white; border: 1px solid #ccc;");

        // Przycisk analizy - zielony
        btnAnalyze = new QPushButton("Dodaj posiłek");
        btnAnalyze->setStyleSheet("padding: 10px; background-color: #27ae60; color: white; font-weight: bold;");

        // Pasek ładowania (kręciołek) - widoczny tylko gdy AI pracuje
        progressBar = new QProgressBar();
        progressBar->setRange(0, 0); // Tryb "nieokreślony" - pasek lata w lewo i prawo
        progressBar->setVisible(false); 

        // Tabela z wynikami
        table = new QTableWidget(0, 5);
        table->setHorizontalHeaderLabels({"Posiłek", "Kcal", "B", "T", "W"});
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Kolumny zajmują całą szerokość
        table->setStyleSheet("background: white;");

        // Układ poziomy na przyciski dolne
        auto* bottomLayout = new QHBoxLayout();

        // Przycisk Reset - czerwony
        btnClear = new QPushButton("Zacznij nowy dzień (Reset)");
        btnClear->setStyleSheet("padding: 10px; background-color: #c0392b; color: white; font-weight: bold;");

        // Przycisk Zapisu - fioletowy
        btnSave = new QPushButton("Zapisz do pliku");
        btnSave->setStyleSheet("padding: 10px; background-color: #8e44ad; color: white; font-weight: bold;");

        // Przycisk Wykresu - niebieski
        btnShowChart = new QPushButton("Generuj wykres dnia");
        btnShowChart->setStyleSheet("padding: 10px; background-color: #2980b9; color: white; font-weight: bold;");

        // Dodawanie przycisków do dolnego rzędu
        bottomLayout->addWidget(btnClear);
        bottomLayout->addWidget(btnSave);
        bottomLayout->addWidget(btnShowChart);

        // Składanie wszystkiego w jedną całość (pionowo)
        layout->addWidget(label);
        layout->addWidget(input);
        layout->addWidget(btnAnalyze);
        layout->addWidget(progressBar); 
        layout->addWidget(table);
        layout->addLayout(bottomLayout);

        // POŁĄCZENIA SYGNAŁÓW 
        // Gdy klikniesz ten guzik, wykonaj tę funkcję
        connect(btnAnalyze, &QPushButton::clicked, this, &CalorieApp::onAnalyze);
        connect(btnShowChart, &QPushButton::clicked, this, &CalorieApp::onShowChart);
        connect(btnSave, &QPushButton::clicked, this, &CalorieApp::onSave);
        connect(btnClear, &QPushButton::clicked, this, &CalorieApp::onClear);

        // Wczytujemy historię z pliku dziennik.json (jeśli istnieje)
        loadDayLog();
    }

private slots:
    /**
     * @brief Obsługuje kliknięcie "Dodaj posiłek".
     * * Uruchamia wątek w tle (std::async), który łączy się z serwerem Ollama.
     * Dzięki temu okno programu nie zacina się podczas czekania na odpowiedź AI.
     */
    void onAnalyze() {
        std::string text = input->text().toStdString();
        if(text.empty()) return;

        // Blokujemy interfejs na czas pracy AI
        btnAnalyze->setEnabled(false);
        btnAnalyze->setText("AI analizuje posiłek... Proszę czekać.");
        input->setDisabled(true);
        progressBar->setVisible(true); 

        // WIELOWĄTKOWOŚĆ 
        fut = std::async(std::launch::async, [this, text]() {
            try {
                OllamaClient client("llama3");
                MealData data = client.askForData(text); // Pytamy AI o liczby
                data.setRawDescription(text);

                // Powrót do wątku głównego, aby zaktualizować okno 
                QMetaObject::invokeMethod(this, [this, data]() {
                    if (data.getKcal() > 0) {
                        dayLog.push_back(data); // Dodajemy do naszej listy
                        updateTable(data);     // Dodajemy wiersz do tabeli
                        input->clear();
                    } else {
                        QMessageBox::warning(this, "Błąd", "AI nie rozpoznało posiłku.");
                    }
                    
                    // Odblokowanie interfejsu
                    btnAnalyze->setEnabled(true);
                    btnAnalyze->setText("Dodaj posiłek");
                    input->setDisabled(false);
                    progressBar->setVisible(false); 
                });
            } catch (...) {
                QMetaObject::invokeMethod(this, [this]() {
                    QMessageBox::critical(this, "Błąd", "Błąd połączenia z Ollama.");
                    btnAnalyze->setEnabled(true);
                    btnAnalyze->setText("Dodaj posiłek");
                    input->setDisabled(false);
                    progressBar->setVisible(false);
                });
            }
        });
    }

    /**
     * @brief Wywołuje generator wykresów w Pythonie.
     */
    void onShowChart() {
        if (dayLog.empty()) {
            QMessageBox::information(this, "Info", "Brak danych do wykresu.");
            return;
        }
        ChartGenerator::generateAndRun(dayLog); // Wywołuje metodę statyczną z ChartGenerator.hpp
    }

    /**
     * @brief Zapisuje aktualną listę posiłków do pliku dziennik.json.
     * * Używa biblioteki nlohmann/json do stworzenia pliku tekstowego z danymi.
     */
    void onSave() {
        if (dayLog.empty()) {
            QMessageBox::information(this, "Info", "Brak danych do zapisu.");
            return;
        }

        btnSave->setEnabled(false);
        btnSave->setText("Zapisywanie...");
        progressBar->setVisible(true);

        std::vector<MealData> logCopy = dayLog; // Kopiujemy dane do bezpiecznego wątku

        saveFut = std::async(std::launch::async, [this, logCopy]() {
            bool success = false;
            
            try {
                json j_array = json::array(); // Tworzymy pustą tablicę JSON
                
                // Przepisujemy każdy posiłek na format JSON
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
                    file << j_array.dump(4); // Zapis 
                    file.close();
                    success = true;
                }
            } catch (const std::exception& e) {
                std::cerr << "Błąd zapisu do pliku: " << e.what() << std::endl;
            }

            QMetaObject::invokeMethod(this, [this, success]() {
                if (success) {
                    QMessageBox::information(this, "Sukces", "Zapisano dane do pliku dziennik.json");
                } else {
                    QMessageBox::critical(this, "Błąd", "Nie udało się zapisać pliku.");
                }
                btnSave->setEnabled(true);
                btnSave->setText("Zapisz do pliku");
                progressBar->setVisible(false);
            });
        });
    }

    /**
     * @brief Czyści historię i usuwa plik zapisu.
     */
    void onClear() {
        if (dayLog.empty()) {
            QMessageBox::information(this, "Info", "Dziennik jest już pusty.");
            return;
        }

        // Okno z pytaniem Tak/Nie
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Potwierdzenie", 
                                      "Czy na pewno chcesz usunąć całą dzisiejszą historię? Tej operacji nie można cofnąć.",
                                      QMessageBox::Yes | QMessageBox::No);
                                      
        if (reply == QMessageBox::Yes) {
            dayLog.clear(); // Czyścimy listę w pamięci
            table->setRowCount(0); // Czyścimy widok tabeli
            
            if (std::remove("dziennik.json") == 0) { // Usuwamy fizyczny plik
                QMessageBox::information(this, "Sukces", "Rozpoczęto nowy dzień!");
            } else {
                QMessageBox::information(this, "Sukces", "Rozpoczęto nowy dzień (brak pliku do usunięcia).");
            }
        }
    }

private:
    /**
     * @brief Dodaje nowy wiersz do tabeli w interfejsie.
     * @param data Obiekt MealData, który ma zostać dopisany.
     */
    void updateTable(const MealData& data) {
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(data.getRawDescription())));
        table->setItem(row, 1, new QTableWidgetItem(QString::number(data.getKcal())));
        table->setItem(row, 2, new QTableWidgetItem(QString::number(data.getProtein()) + "g"));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(data.getFat()) + "g"));
        table->setItem(row, 4, new QTableWidgetItem(QString::number(data.getCarbs()) + "g"));
    }

    /**
     * @brief Wczytuje dane z pliku dziennik.json przy starcie programu.
     */
    void loadDayLog() {
        try {
            std::ifstream file("dziennik.json");
            if (!file.is_open()) return; 

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
        } catch (...) {
            std::cerr << "Błąd wczytywania pliku." << std::endl;
        }
    }

    std::future<void> fut;      ///< Obiekt przyszłości dla wątku analizy AI
    std::future<void> saveFut;  ///< Obiekt przyszłości dla wątku zapisu pliku
    std::vector<MealData> dayLog; ///< Lista wszystkich posiłków dodanych dzisiaj
    QLineEdit* input;           ///< Pole tekstowe
    QPushButton* btnAnalyze;    ///< Przycisk analizy
    QPushButton* btnShowChart;  ///< Przycisk wykresu
    QPushButton* btnSave;       ///< Przycisk zapisu
    QPushButton* btnClear;      ///< Przycisk resetu
    QTableWidget* table;        ///< Tabela z danymi
    QProgressBar* progressBar;  ///< Pasek postępu
};

/**
 * @brief Funkcja główna programu.
 */
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    CalorieApp w;
    w.show();
    return a.exec();
}