//
//  main.cpp
//  Coffee Machine
//
//  Created by Angad Pal Singh on 24/06/20.
//  Copyright © 2020 abc. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "ICoffeeMachine.h"

void helperFunction(std::string testFile, bool refillAndTest = false, bool largeCapacity = false) {
    auto coffeeMachine = createCoffeeMachineFromJson(testFile, largeCapacity);
    
    if (coffeeMachine)
        coffeeMachine->getItemsStatus();
    
    prepareDrinksFromInput(coffeeMachine, testFile);
    
    if (refillAndTest && coffeeMachine) {
        vector<pair<string, int>> refillItems = {
            {"hot_milk",5000},
            {"hot_water",500},
            {"sugar_syrup",500},
            {"ginger_syrup",100},
            {"tea_leaves_syrup",100},
            {"green_mixture",300}
        };
        
        for(const auto& item: refillItems)
            coffeeMachine->refillItem(item.first, item.second);
        
        coffeeMachine->getItemsStatus();
        
        prepareDrinksFromInput(coffeeMachine, testFile);
    }
    
    if (coffeeMachine)
        coffeeMachine->getItemsStatus();
    
}

int main(int argc, const char * argv[]) {
    
    // Sample Input
    //    black_tea is prepared.
    //    green_tea cannot be prepared because green_mixture is not available.
    //    hot_coffee is prepared.
    //    hot_tea cannot be prepared because sugar_syrup is not sufficient.
    helperFunction("test.json");
    
    // We'll prepare beverages and then prepare again after refilling the machine.
    // Before Refill:
    //    black_tea cannot be prepared because hot_water is not sufficient.
    //    hot_tea is prepared.
    //    hot_coffee cannot be prepared because tea_leaves_syrup is not sufficient.
    //    green_tea cannot be prepared because sugar_syrup is not sufficient.
    //
    // After refill:
    //    black_tea is prepared.
    //    green_tea is prepared.
    //    hot_coffee is prepared.
    //    hot_tea cannot be prepared because hot_water is not sufficient.
    helperFunction("test-with-refill.json", true /*refillAndPrepareAgain*/);
    
    // All beverages should be prepared.
    //    black_tea is prepared.
    //    green_tea is prepared.
    //    hot_coffee is prepared.
    //    hot_tea is prepared.
    helperFunction("test-all-ingredients-present.json");
    
    // Invalid number of outlets.
    helperFunction("test-0-count.json");
    
    // Invalid JSON schema.
    helperFunction("test-invalid-schema.json");
    
    // Invalid file name.
    helperFunction("test-nonexistent.json");
    
    // Large Capacity test: Capacity is increased to 10000.
    //  Sample Output:
    //    black_tea is prepared.
    //    black_tea1 is prepared.
    //    green_tea4 is prepared.
    //    black_tea3 is prepared.
    //    black_tea4 is prepared.
    //    hot_tea3 is prepared.
    //    green_tea is prepared.
    //    green_tea2 is prepared.
    //    green_tea3 is prepared.
    //    black_tea2 is prepared.
    //    hot_coffee1 is prepared.
    //    hot_coffee is prepared.
    //    hot_coffee2 is prepared.
    //    hot_coffee3 is prepared.
    //    hot_coffee4 is prepared.
    //    hot_tea is prepared.
    //    hot_tea1 is prepared.
    //    hot_tea2 cannot be prepared because hot_water is not sufficient.
    //    green_tea1 cannot be prepared because hot_water is not sufficient.
    //    hot_tea4 cannot be prepared because hot_water is not sufficient.
    helperFunction("test-large-input.json", false, true);
    
    return 0;
}
