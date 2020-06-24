//
//  main.cpp
//  Coffee Machine
//
//  Created by Angad Pal Singh on 24/06/20.
//  Copyright © 2020 abc. All rights reserved.
//

#include <iostream>
#include "json/json.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    const std::string rawJson = R"({"Age": 20, "Name": "colin"})";
    const auto rawJsonLength = static_cast<int>(rawJson.length());
    JSONCPP_STRING err;
    Json::Value root;
    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root,
                       &err)) {
      std::cout << "error" << std::endl;
      return EXIT_FAILURE;
    }
    
    
    return 0;
}
