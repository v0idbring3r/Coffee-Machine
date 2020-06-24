//
//  ICoffeeMachine.h
//  Coffee Machine
//
//  Created by Angad Pal Singh on 24/06/20.
//  Copyright © 2020 abc. All rights reserved.
//
#include <string>
#include <unordered_map>
#include <iostream>

#include "json/json.h"

using namespace std;

class ICoffeeMachine {
public:
    // Refill/Add a particular ingredient in the machine.
    virtual void refillItem(string item_name, int quantity) = 0;

    // Prepare a set of beverages as requested in the input.
    virtual void prepareDrinks(Json::Value beverages) = 0;

protected:
    unordered_map<string, int> _availableItems; // Available quantities for each of the ingredients present in the machine.
    int _outlets; // Total number of outlets in the machine.
    int _item_capacity; // Maximum quantity for each type of item which the machine can hold.
};
