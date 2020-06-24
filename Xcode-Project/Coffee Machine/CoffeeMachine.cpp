//
//  CoffeeMachine.cpp
//  Coffee Machine
//
//  Created by Angad Pal Singh on 24/06/20.
//  Copyright © 2020 abc. All rights reserved.
//

#include "CoffeeMachine.h"

void CoffeeMachine::refillItem(string item_name, int quantity) {
    
}

void CoffeeMachine::prepareDrinks(Json::Value beverages) {
    
}

CoffeeMachine::CoffeeMachine(int outlets, Json::Value total_items_available, int capacity = 10000) {
    _outlets = outlets;
    _item_capacity = capacity;
    
    for(auto it = total_items_available.begin(); it != total_items_available.end(); ++it)
        _availableItems[it.key().asString()] = it->asInt();
}
