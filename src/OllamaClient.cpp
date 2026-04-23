/**
 * @file OllamaClient.cpp
 * @brief Implementacja klasy OllamaClient - mózgu komunikacji z AI.
 */

#include "OllamaClient.hpp"
#include "httplib.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <regex>

// Skrót, żeby nie pisać za każdym razem nlohmann::json
using json = nlohmann::json;

/**
 * @brief Konstruktor klienta AI.
 * @param modelName Nazwa modelu, który ma zostać użyty (np. "llama3").
 * * Tutaj ustawiamy adres serwera. Ollama domyślnie działa na "localhost:11434".
 */
OllamaClient::OllamaClient(std::string modelName) 
    : url("http://localhost:11434"), model(modelName) {
}

/**
 * @brief Funkcja wysyłająca pytanie do sztucznej inteligencji.
 * @param mealName Tekst wpisany przez użytkownika (np. "Pizza margherita").
 * @return MealData Zwraca obiekt wypełniony liczbami (kcal, białko itp.).
 */
MealData OllamaClient::askForData(std::string mealName) {
    // 1. Tworzymy klienta HTTP, który "zapuka" do serwera Ollama
    httplib::Client cli(url);
    MealData data; // Pusty pojemnik na dane
    
    //  Przygotowujemy "paczka danych" (JSON) do wysłania
    // Musimy powiedzieć AI: jakiego modelu użyć, o co pytamy i jak ma odpowiedzieć.
    json body = {
        {"model", model},
        {"prompt", mealName},
        // To jest bardzo ważne - instruujemy AI, żeby nie pisała wierszy ani opowiadań, 
        // tylko zwróciła surowe liczby w formacie, który rozumiemy.
        {"system", "Jesteś dietetykiem. Odpowiadaj WYŁĄCZNIE w formacie JSON: "
                   "{\"kcal\": 0, \"protein\": 0, \"fat\": 0, \"carbs\": 0}. "
                   "Nie dopisuj tekstu."},
        {"stream", false} // false oznacza: czekaj na całą odpowiedź, nie wysyłaj po literce.
    };

    try {
        //  WYSYŁANIE ZAPYTANIA
        // Metoda POST wysyła nasz JSON do "drzwi" o nazwie /api/generate
        if (auto res = cli.Post("/api/generate", body.dump(), "application/json")) {
            
            // CZYTANIE ODPOWIEDZI
            // Zamieniamy surowy tekst z serwera na obiekt JSON
            auto j_res = json::parse(res->body);
            std::string responseText = j_res["response"];
            
            // 
            // Czasem AI mimo próśb dopisze coś od siebie, np. "Oto wynik: { ... }".
            // Ten kod szuka tekstu, który jest wewnątrz klamerek { }, żeby wyciąć tylko czyste dane.
            std::smatch match;
            std::regex json_regex(R"(\{.*\})"); // Szukaj wzoru: klamerka - cokolwiek w środku - klamerka
            
            if (std::regex_search(responseText, match, json_regex)) {
                // Zamieniamy wycięty fragment tekstu na finalny obiekt JSON
                auto j = json::parse(match.str());
                
                // 5. PRZEPISYWANIE DANYCH DO OBIEKTU C++
                // Używamy setterów, które zdefiniowalismy w MealData.hpp
               
                data.setKcal(j.value("kcal", 0));
                data.setProtein(j.value("protein", 0));
                data.setFat(j.value("fat", 0));
                data.setCarbs(j.value("carbs", 0));
            }
        }
    } catch (const std::exception& e) {
        // Jeśli serwer Ollama jest wyłączony lub JSON jest błędny, wyświetlamy błąd w konsoli
        std::cerr << "Blad komunikacji z AI: " << e.what() << std::endl;
    }
    
    // Zwracamy uzupełniony (lub pusty w razie błędu) obiekt z danymi
    return data;
}