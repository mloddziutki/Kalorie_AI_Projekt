#include "OllamaClient.hpp"
#include "httplib.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <regex>

using json = nlohmann::json;


OllamaClient::OllamaClient(std::string modelName) 
    : url("http://localhost:11434"), model(modelName) {
}


MealData OllamaClient::askForData(std::string mealName) {
    httplib::Client cli(url);
    MealData data;
    
    json body = {
        {"model", model},
        {"prompt", mealName},
        {"system", "Jesteś dietetykiem. Odpowiadaj WYŁĄCZNIE w formacie JSON: "
                   "{\"kcal\": 0, \"protein\": 0, \"fat\": 0, \"carbs\": 0}. "
                   "Nie dopisuj tekstu."},
        {"stream", false}
    };

    try {
        if (auto res = cli.Post("/api/generate", body.dump(), "application/json")) {
            auto j_res = json::parse(res->body);
            std::string responseText = j_res["response"];
            
            
            std::smatch match;
            std::regex json_regex(R"(\{.*\})");
            if (std::regex_search(responseText, match, json_regex)) {
                auto j = json::parse(match.str());
                data.kcal = j.value("kcal", 0);
                data.protein = j.value("protein", 0);
                data.fat = j.value("fat", 0);
                data.carbs = j.value("carbs", 0);
            }
        }
    } catch (...) {
        std::cerr << "Blad komunikacji z AI" << std::endl;
    }
    return data;
}