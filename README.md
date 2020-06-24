# Coffee-Machine

## Code Structure:
Main.cpp: Contains all the tests and used to run the program.
  
ICoffeeMachine.h: Contains the exposed Interface/Virtual class ICoffeeMachine and exposed factory method to create a Coffee Machine instance from a JSON file. This is the only file which needs to be exposed to the end user. The ICoffeeMachine base class can be derived by other classes if we wish to create different types of machines in the future and it allows the user to only worry about the interface and not the actual implementation.

Abstracted/Hidden code:

CoffeeMachine.h: Contains the declaration for our CoffeeMachine class.

CoffeeMachine.cpp: Contains the actual implementation.

## Steps to Run:
1. Open CoffeeMachine.xcodeproj
2. Product->Run. This will build and run the project.
