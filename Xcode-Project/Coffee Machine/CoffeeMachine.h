//
//  CoffeeMachine.hpp
//  Coffee Machine
//
//  Created by Angad Pal Singh on 24/06/20.
//  Copyright © 2020 abc. All rights reserved.
//

/**
 Assumptions made from problem statement and sample input:
 - All the ingredients which are available are listed in total_items_quantity. The rest won't be available until we refill them.
 - We can add/refill new ingredients any time and the machine will be offline during that period.
 - Number of different types of ingredients in machine is not limited.
 - The capacity for each different ingredient type is fixed and same for all ingredients (I've set it up as 10000 for now). We can support different capacities by including those values in the available_items property.
 - Low quantity of items will be detected if the item is filled < 20% of its capacity.
 */

#include "ICoffeeMachine.h"

class CoffeeMachine: public ICoffeeMachine {
public:
    CoffeeMachine(int outlets, Json::Value total_items_available, int capacity);
    
    void refillItem(string item_name, int quantity);

    void prepareDrinks(Json::Value beverages);
};