# Licznik Kalorii AI

Projekt zaliczeniowy z przedmiotu Języki Programowania Obiektowego 2025/2026.
Autorzy: Kacper Olszowski, Jakub Król.

##  Kluczowe Funkcjonalności
**Integracja z AI (Ollama):** Program łączy się z lokalnym modelem `llama3` przez REST API (protokół HTTP), przesyłając zapytania w formacie JSON.
**Wielowątkowość (Multithreading):** Wykorzystanie `std::async` zapobiega blokowaniu interfejsu GUI podczas oczekiwania na odpowiedź od modelu AI.
**Automatyzacja i IPC:** Program dynamicznie generuje skrypty w języku Python i uruchamia je jako zewnętrzne procesy (`QProcess`), aby stworzyć wizualizację danych w bibliotece Matplotlib.
**Hermetyzacja:** Pełna implementacja obiektowa z prywatnymi polami i publicznymi metodami dostępowymi (Getters/Setters).
**Testy Jednostkowe:** Wbudowany moduł weryfikujący integralność struktur danych przy każdym starcie aplikacji.
**Persystencja danych:** Zapis i odczyt dziennika posiłków do pliku `dziennik.json`.


## Cel projektu
Aplikacja służy do szacowania wartości energetycznej posiłków na podstawie opisu tekstowego przy użyciu lokalnego modelu językowego.

## Spełnione wymagania:
- **Model Językowy:** Integracja z Ollama (Llama3) przez REST API .
- **Offline:** Program działa bez dostępu do internetu.
- **GUI:** Interfejs graficzny stworzony w frameworku Qt6 .
- **Wielowątkowość:** Zapytania do AI wykonywane asynchronicznie .
- **Automatyzacja:** Generowanie kodu Python i uruchamianie procesu generowania wykresu .
- **Techniczne:** C++17, Doxygen, CMake, testy jednostkowe .

## Wymagania systemowe

* **C++:** Kompilator wspierający standard C++17 (np. MSVC 2022, GCC).
* **Framework:** Qt 6.x.
* **AI Backend:** Zainstalowana i uruchomiona [Ollama](https://ollama.com/) z pobranym modelem `llama3`.
* **Wizualizacja:** Python 3.x z zainstalowanymi bibliotekami z pliku `requirements.txt`.

##  Instalacja i uruchomienie

1.  Upewnij się, że Ollama działa: `ollama run llama3`.
2.  Zainstaluj zależności Pythona:
    ```bash
    pip install -r requirements.txt
    ```
3.  Skompiluj projekt przy użyciu **CMake**.
4.  Uruchom aplikację i wpisz np. "Jajecznica z 3 jaj i awokado"


## Dokumentacja
Dokumentacja wygenerowana systemem Doxygen znajduje się w folderze `/docs`.