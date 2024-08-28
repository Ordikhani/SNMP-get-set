#include "processexecutor.h"

#include <iostream>

using namespace std;

std::string host = "127.0.0.1";
int main() {


    ProcessExecutor executor(host);

    try {
        // methods of  SNMP GET
        std::cout << "Temperature: " << executor.gettwtTemperature() << std::endl;

        // methodes of SNMP SET
        std::string newValue = "STANDBY";
        bool success = executor.setControlStateString(newValue);
        if (success) {
            std::cout << "Control state updated successfully." << std::endl;
        } else {
            std::cout << "Failed to update control state." << std::endl;
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
