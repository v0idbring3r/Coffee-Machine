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
using IngredientList = unordered_map<string, int>;
using Beverage = pair<string, IngredientList>;

class ICoffeeMachine {
public:
    // Refill/Add a particular ingredient in the machine.
    virtual void refillItem(string item_name, int quantity) = 0;

    // Prepare a set of beverages as requested in the input.
    virtual void prepareDrinks(vector<Beverage> beverages) = 0;
};

std::shared_ptr<ICoffeeMachine> createCoffeeMachineFromJson(const string jsonFile);
