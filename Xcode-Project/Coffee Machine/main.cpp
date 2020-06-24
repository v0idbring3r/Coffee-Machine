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


int main(int argc, const char * argv[]) {
    auto c = createCoffeeMachineFromJson("test.json");
    prepareDrinksFromInput(c, "test.json");
    
    return 0;
}
