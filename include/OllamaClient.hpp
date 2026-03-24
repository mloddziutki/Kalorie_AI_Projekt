/**
 * @file OllamaClient.hpp
 * @brief Klasa obsługująca komunikację z API Ollama.
 */

#ifndef OLLAMACLIENT_HPP
#define OLLAMACLIENT_HPP

#include <string>
#include "MealData.hpp"

/**
 * @class OllamaClient
 * @brief Klient REST API do komunikacji z lokalnym modelem LLM.
 */
class OllamaClient {
private:
    std::string url;   /**< Adres URL serwera Ollama */
    std::string model; /**< Nazwa używanego modelu (np. llama3) */

public:
    /**
     * @brief Konstruktor klienta.
     * @param modelName Nazwa modelu do uruchomienia.
     */
    OllamaClient(std::string modelName);

    /**
     * @brief Wysyła zapytanie do AI i parsuje odpowiedź do struktury MealData.
     * @param mealName Nazwa posiłku do analizy.
     * @return Obiekt MealData z wypełnionymi danymi.
     */
    MealData askForData(std::string mealName);
};

#endif