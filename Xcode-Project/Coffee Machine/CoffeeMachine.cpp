//
//  CoffeeMachine.cpp
//  Coffee Machine
//
//  Created by Angad Pal Singh on 24/06/20.
//  Copyright © 2020 abc. All rights reserved.
//

#include "CoffeeMachine.h"
#include "json/json.h"

void CoffeeMachine::refillItem(string item_name, int quantity) {
    
}

void CoffeeMachine::prepareDrinks(vector<Beverage> beverages) {
    
}

CoffeeMachine::CoffeeMachine(int outlets, const Ingredients& total_items_available, int capacity) {
    _outlets = outlets;
    _item_capacity = capacity;
    _availableItems = total_items_available;
}

std::shared_ptr<ICoffeeMachine> createCoffeeMachineFromJson(const string jsonFile) {
    JSONCPP_STRING err;
    Json::Value root, availableItemsJson;
    Json::CharReaderBuilder builder;
    std::ifstream ifs(jsonFile);
    
    unordered_map<string, int> availableItems;
    int outlets;
    
    if (!parseFromStream(builder, ifs, &root, &err)) {
      std::cout << "error" << std::endl;
      return nullptr;
    }

    outlets = root["machine"]["outlets"]["count_n"].asInt();
    availableItemsJson = root["machine"]["total_items_quantity"];
    for(auto it = availableItemsJson.begin(); it != availableItemsJson.end(); ++it)
        availableItems[it.key().asString()] = it->asInt();
    
    return make_shared<CoffeeMachine>(outlets, availableItems);
}
