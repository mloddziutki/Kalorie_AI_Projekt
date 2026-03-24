# Licznik Kalorii AI

Projekt zaliczeniowy z przedmiotu Języki Programowania Obiektowego 2025/2026.

## Cel projektu
Aplikacja służy do szacowania wartości energetycznej posiłków na podstawie opisu tekstowego przy użyciu lokalnego modelu językowego.

## Spełnione wymagania:
- **Model Językowy:** Integracja z Ollama (Llama3) przez REST API (punkt 1, 4).
- **Offline:** Program działa bez dostępu do internetu (punkt 3).
- **GUI:** Interfejs graficzny stworzony w frameworku Qt6 (punkt 5).
- **Wielowątkowość:** Zapytania do AI wykonywane asynchronicznie (wymóg na 4.0).
- **Automatyzacja:** Generowanie kodu Python i uruchamianie procesu generowania wykresu (wymóg na 5.0).
- **Techniczne:** C++17, Doxygen, CMake, testy jednostkowe (punkt 1, 8).

## Instrukcja uruchomienia:
1. Zainstaluj **Ollama** i pobierz model: `ollama run llama3`.
2. Zainstaluj **Python 3** oraz bibliotekę: `pip install matplotlib`.
3. Skompiluj projekt za pomocą CMake.
4. Uruchom `CalorieAI.exe`.

## Dokumentacja
Dokumentacja wygenerowana systemem Doxygen znajduje się w folderze `/docs`.