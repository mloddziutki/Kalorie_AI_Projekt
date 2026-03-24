#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <future> 
#include <iostream>

#include "OllamaClient.hpp"
#include "ChartGenerator.hpp"
#include "MealData.hpp"

class CalorieApp : public QWidget {
public:
    CalorieApp() {
        setWindowTitle("Licznik Kalorii AI");
        resize(400, 250);
        setStyleSheet("background-color: #f0f0f0;");

        auto* layout = new QVBoxLayout(this);

        QLabel* label = new QLabel("<b>Co zjadles?</b>");
        label->setAlignment(Qt::AlignCenter);

        input = new QLineEdit();
        input->setPlaceholderText("Np. nalesniki z serem...");
        input->setStyleSheet("padding: 8px; background: white;");

        btn = new QPushButton("Analizuj i pokaz wykres");
        btn->setStyleSheet("padding: 10px; background-color: #27ae60; color: white; font-weight: bold;");

        resultLabel = new QLabel("Wynik pojawi sie na wykresie.");
        resultLabel->setAlignment(Qt::AlignCenter);
        resultLabel->setStyleSheet("color: #555;");

        layout->addWidget(label);
        layout->addWidget(input);
        layout->addWidget(btn);
        layout->addWidget(resultLabel);

        connect(btn, &QPushButton::clicked, this, &CalorieApp::onAnalyze);
    }

private slots:
    void onAnalyze() {
        std::string text = input->text().toStdString();
        if(text.empty()) return;

        btn->setEnabled(false);
        btn->setText("AI analizuje...");
        resultLabel->setText("Trwa laczenie z Llama3...");

        // Uruchamiamy AI w tle
        fut = std::async(std::launch::async, [this, text]() {
            try {
                OllamaClient client("llama3");
                MealData data = client.askForData(text);

                // Wracamy do watku GUI tylko po to, zeby odblokowac przycisk
                QMetaObject::invokeMethod(this, [this, data]() {
                    if (data.kcal > 0) {
                        resultLabel->setText("Wykres zostal otwarty w osobnym oknie.");
                        // ODPALAMY WYKRES - dzieki QProcess w srodku, okno C++ nie zamarznie!
                        ChartGenerator::generateAndRun(data);
                    } else {
                        resultLabel->setText("Blad danych AI.");
                    }
                    btn->setEnabled(true);
                    btn->setText("Analizuj i pokaz wykres");
                });

            } catch (...) {
                QMetaObject::invokeMethod(this, [this]() {
                    QMessageBox::critical(this, "Blad", "Upewnij sie, ze Ollama dziala!");
                    btn->setEnabled(true);
                });
            }
        });
    }

private:
    std::future<void> fut;
    QLineEdit* input;
    QPushButton* btn;
    QLabel* resultLabel;
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    CalorieApp w;
    w.show();
    return a.exec();
}