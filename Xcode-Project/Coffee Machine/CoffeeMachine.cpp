//
//  CoffeeMachine.cpp
//  Coffee Machine
//
//  Created by Angad Pal Singh on 24/06/20.
//  Copyright © 2020 abc. All rights reserved.
//
#include <fstream>
#include <iostream>

#include "CoffeeMachine.h"
#include "json/json.h"

void CoffeeMachine::refillItem(string item_name, int quantity) {
    if (_availableItems.find(item_name) != _availableItems.end()) {
        // Ingredient is already present in some quantity.
        _availableItems[item_name] = min(_availableItems[item_name] + quantity, _item_capacity); // Don't exceed maximum capacity.
        cout << "Item " << item_name << " refilled successfully. Total quantity is: " << _availableItems[item_name] << endl;
    }
    else {
        // Ingredient is added for the first time.
        _availableItems[item_name] = min(quantity, _item_capacity); // Don't exceed maximum capacity.
        cout << quantity << " units of Item " << item_name << " added successfully" << endl;
    }
}

void CoffeeMachine::prepareDrinks(const vector<Beverage>& beverages) {
    for (const auto& beverage: beverages) {
        bool ingredientsAvailable = true;

        // Check all ingredients first.
        for (auto it = beverage.second.begin(); it != beverage.second.end(); ++it) {
            if (_availableItems.find(it->first) == _availableItems.end()) {
                // Current ingredient is not present at all.
                cout << beverage.first << " cannot be prepared because " << it->first << " is not available." << endl;
                ingredientsAvailable = false;
                break;
            }
            else if (_availableItems[it->first] < it->second) {
                // Current ingredient is not present in sufficient quantity.
                cout << beverage.first << " cannot be prepared because " << it->first << " is not sufficient." << endl;
                ingredientsAvailable = false;
                break;
            }
        }
        
        if (ingredientsAvailable) {
            // Consume the required ingredients and prepare beverage.
            for (auto it = beverage.second.begin(); it != beverage.second.end(); ++it) {
                _availableItems[it->first] -= it->second; // Consume this ingredient.
            }

            cout << beverage.first << " is prepared." << endl;
        }
    }
}

CoffeeMachine::CoffeeMachine(int outlets, const IngredientList& total_items_available, int capacity) {
    _outlets = outlets;
    _item_capacity = capacity;
    _availableItems = total_items_available;
}

std::shared_ptr<ICoffeeMachine> createCoffeeMachineFromJson(const string jsonFile) {
    static constexpr auto MACHINE_KEY = "machine";
    static constexpr auto OUTLETS_KEY = "outlets";
    static constexpr auto COUNT_KEY = "count_n";
    static constexpr auto ITEMS_KEY = "total_items_quantity";

    JSONCPP_STRING err;
    Json::Value root, availableItemsJson;
    Json::CharReaderBuilder builder;
    std::ifstream ifs(jsonFile);
    
    IngredientList availableItems;
    int outlets;
    
    if (!ifs.is_open() || !parseFromStream(builder, ifs, &root, &err)) {
      cout << "Error while trying to parse JSON file" << endl;
      return nullptr;
    }

    try {
        outlets = root[MACHINE_KEY][OUTLETS_KEY][COUNT_KEY].asInt();
        availableItemsJson = root[MACHINE_KEY][ITEMS_KEY];
        for(auto it = availableItemsJson.begin(); it != availableItemsJson.end(); ++it)
            availableItems[it.key().asString()] = it->asInt();
    } catch (...) {
        cout << "Invalid Schema JSON" << endl;
        return nullptr;
    }
    
    
    return make_shared<CoffeeMachine>(outlets, availableItems);
}

void prepareDrinksFromInput(const std::shared_ptr<ICoffeeMachine>& coffeeMachine, const string jsonFile) {
    static constexpr auto MACHINE_KEY = "machine";
    static constexpr auto BEVERAGES_KEY = "beverages";

    JSONCPP_STRING err;
    Json::Value root, availableItemsJson;
    Json::CharReaderBuilder builder;
    std::ifstream ifs(jsonFile);
    
    vector<Beverage> beverages;
    
    if (!ifs.is_open() || !parseFromStream(builder, ifs, &root, &err)) {
      cout << "Error while trying to parse JSON file" << endl;
      return;
    }

    try {
        auto beverageObject = root[MACHINE_KEY][BEVERAGES_KEY];
        
        for(auto it = beverageObject.begin(); it != beverageObject.end(); ++it) {
            IngredientList availableItems;
            auto availableItemsJson = *it;
            for(auto it = availableItemsJson.begin(); it != availableItemsJson.end(); ++it)
                availableItems[it.key().asString()] = it->asInt();
            
            beverages.emplace_back(it.key().asString(), availableItems);
        }
        
        coffeeMachine->prepareDrinks(beverages);
    } catch (...) {
        cout << "Invalid Schema JSON" << endl;
    }
}
