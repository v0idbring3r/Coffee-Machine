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

void CoffeeMachine::getItemsStatus() {
    cout<<endl;
    for(const auto& item: _availableItems) {
        // If item is less than 20% of maximum, it is running low.
        if (item.second <= 0.2*_itemCapacity)
            cout << item.first << " is running low." << endl;
    }
    cout<<endl;
}

void CoffeeMachine::refillItem(string item_name, int quantity) {
    if (_availableItems.find(item_name) != _availableItems.end()) {
        // Ingredient is already present in some quantity.
        _availableItems[item_name] = min(_availableItems[item_name] + quantity, _itemCapacity); // Don't exceed maximum capacity.
        cout << "Item " << item_name << " refilled successfully. Total quantity is: " << _availableItems[item_name] << endl;
    }
    else {
        // Ingredient is added for the first time.
        _availableItems[item_name] = min(quantity, _itemCapacity); // Don't exceed maximum capacity.
        cout << quantity << " units of Item " << item_name << " added successfully" << endl;
    }
}

void CoffeeMachine::prepareDrinks(const vector<Beverage>& beverages) {
    vector<future<void>> futures;
    std::mutex mut;

    // We use threadpool of size N(outlets) which is used to distribute the drinks preparing tasks.
    // This is the helper function which will be passed to each thread in the pool.
    auto prepareDrinkLambda = [&mut, this](int id, const Beverage& beverage) {
        mut.lock();
        
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
        
        mut.unlock();
    };

    for (const auto& beverage: beverages) {
        futures.push_back(_threadPool.push(prepareDrinkLambda, beverage));
    }
    
    for (int i = 0; i < futures.size(); ++i)
        futures[i].get(); // Wait for all threads to finish.
}

CoffeeMachine::CoffeeMachine(int outlets, const IngredientList& total_items_available, int capacity) {
    _outlets = outlets;
    _itemCapacity = capacity;
    _availableItems = total_items_available;
    
    _threadPool.resize(outlets); // If we don't assume infinte number of CPUs, we will limit the number of threads to hardware capacity. IN this case, we're limiting the number of threads to number of outlets.
}

std::shared_ptr<ICoffeeMachine> createCoffeeMachineFromJson(const string jsonFile, bool largeCapacity) {
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
    
    // Parse JSON file into object.
    if (!ifs.is_open() || !parseFromStream(builder, ifs, &root, &err)) {
      cout << "Error while trying to parse JSON file" << endl;
      return nullptr;
    }

    try {
        if (root.empty() || root[MACHINE_KEY].empty() || root[MACHINE_KEY][OUTLETS_KEY].empty() || root[MACHINE_KEY][OUTLETS_KEY][COUNT_KEY].empty()
            || root[MACHINE_KEY][ITEMS_KEY].empty()) {
            cout<< "Invalid Schema JSON" <<endl;
            return nullptr;
        }
        outlets = root[MACHINE_KEY][OUTLETS_KEY][COUNT_KEY].asInt();
        availableItemsJson = root[MACHINE_KEY][ITEMS_KEY];
        
        if (outlets <= 0) {
            cout<< "Invalid outlets value\n";
            return nullptr;
        }
    
        // Convert items json object to hashmap.
        for(auto it = availableItemsJson.begin(); it != availableItemsJson.end(); ++it)
            availableItems[it.key().asString()] = it->asInt();
    } catch (...) {
        cout << "Invalid Schema JSON" << endl;
        return nullptr;
    }
    
    
    return make_shared<CoffeeMachine>(outlets, availableItems, largeCapacity ? 10000 : 1000);
}

void prepareDrinksFromInput(const std::shared_ptr<ICoffeeMachine>& coffeeMachine, const string jsonFile) {
    if (coffeeMachine) {
        static constexpr auto MACHINE_KEY = "machine";
        static constexpr auto BEVERAGES_KEY = "beverages";

        JSONCPP_STRING err;
        Json::Value root, availableItemsJson;
        Json::CharReaderBuilder builder;
        std::ifstream ifs(jsonFile);
        
        vector<Beverage> beverages;

        // Parse JSON file into object.
        if (!ifs.is_open() || !parseFromStream(builder, ifs, &root, &err)) {
          cout << "Error while trying to parse JSON file" << endl;
          return;
        }

        try {
            auto beverageObject = root[MACHINE_KEY][BEVERAGES_KEY];
            
            // Convert Beverages json object to vector of hashmaps.
            for(auto it = beverageObject.begin(); it != beverageObject.end(); ++it) {
                IngredientList availableItems;
                auto availableItemsJson = *it;
                
                // Convert items json object to hashmap.
                for(auto it = availableItemsJson.begin(); it != availableItemsJson.end(); ++it)
                    availableItems[it.key().asString()] = it->asInt();
                
                beverages.emplace_back(it.key().asString(), availableItems);
            }
            
            coffeeMachine->prepareDrinks(beverages);
        } catch (...) {
            cout << "Invalid Schema JSON" << endl;
        }
    }
    else
        cout << "Empty coffee machine" << endl;
}
