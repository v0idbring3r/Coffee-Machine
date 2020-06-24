/**
ICoffeeMachine is the only header file which needs to be exposed. The rest of the code can be an abstraction for the end user.
We can use the createCoffeeMachineFromJson() Factory method to create the Coffee Machine.
 */
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;
using IngredientList = unordered_map</*Ingredient name*/ string, /*Quantity*/ int>;
using Beverage = pair</*Beverage Name*/ string, IngredientList>;

class ICoffeeMachine {
public:
    // Refill/Add a particular ingredient in the machine.
    virtual void refillItem(string item_name, int quantity) = 0;

    // Prepare a set of beverages as requested in the input.
    virtual void prepareDrinks(const vector<Beverage>& beverages) = 0;
};

// Factory method to create CoffeeMachine object from JSON input.
// We can create multiple factory methods to create CoffeeMachine objects in different ways.
std::shared_ptr<ICoffeeMachine> createCoffeeMachineFromJson(const string jsonFile);

// Method which parses an input file to get an order for beverage preparation and uses the CoffeeMachine object to do so.
void prepareDrinksFromInput(const std::shared_ptr<ICoffeeMachine>& coffeeMachine, const string jsonFile);
