
/*
File: main.cpp
Author: FaOrdi-Ordikhani
Date Created: 2024-08-19
Last Updated: 2024-08-19    2024-08-27

Description: SNMP handler compatible with both Windows and Linux platforms
    This program uses the ProcessExecutor class to interact with SNMP-enabled devices.
    It includes functionality to perform SNMP GET and SET operations using the Net-SNMP tools.
    The program connects to a specified SNMP host, retrieves various metrics, and updates
    configuration values as needed.

Version:
    1.0  2.1

Notes:
    - Ensure that SNMP tools such as 'snmpget' and 'snmpset' are installed on the system.
    - The program supports both Windows and Unix-like systems (e.g., Linux).
    - Update the 'host' variable with the IP address or hostname of the SNMP device.
    - Ensure that the Net-SNMP tools are in the system's PATH.
    - For Windows systems, make sure to have Net-SNMP installed with the correct setup.
    - For Unix-like systems, Net-SNMP can be installed via package managers (e.g., 'apt-get' on Debian-based systems).

Required Libraries and Tools:
    - **SNMP Daemon**: `snmpd` (provides SNMP agent functionality)
    - **Net-SNMP Development Libraries**: `libsnmp-dev` (development files for Net-SNMP)
    - **Net-SNMP Tools**: `snmp` (command-line utilities for SNMP)
    - **SNMP MIBs Downloader**: `snmp-mibs-downloader` (downloads and installs SNMP MIBs)

Usage:
    Compile the program using a C++ compiler (e.g., g++).
    Run the executable and observe the output of SNMP GET operations and the result of SNMP SET operations.

Example:
    g++ -o myprogram main.cpp processexecutor.cpp
    ./myprogram
*/


#include "processexecutor.h"
#include <iostream>
#include <stdexcept>
#include <array>
#include <cstdio>
#include <memory>
#include <sstream>
#include <iomanip>  // for std::setprecision

#ifdef _WIN32
#include <windows.h>
#define popen _popen
#define pclose _pclose
#else
#include <unistd.h>
#endif

// Constructor
ProcessExecutor::ProcessExecutor(const std::string& host) : host(host) {}


#include <sstream>
#include <iomanip>  // for std::setprecision

float ProcessExecutor::run_snmpget(const std::string& oid) {
    std::string command = "snmpget -v 2c -c public " + host + " " + oid;
    std::array<char, 128> buffer;
    std::string result;

    // Execute the popen command to get the snmpget result
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    // Read the output and store it in the result string
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    // Extract the numeric value from the result string
    std::string::size_type pos = result.find_last_of(" ");
    if (pos == std::string::npos) {
        throw std::runtime_error("Failed to parse SNMP result");
    }

    std::string numberStr = result.substr(pos + 1);

 //   try {
        // Convert the string to a float
// std::cout << numberStr  << std::endl;
float resultFloat;
int valusize =numberStr.size();
if (valusize >= 2) {

            std::string resultStr = numberStr.substr(0, 2) + "." + numberStr.substr(2, valusize-2);
   // std::cout << " resultStr " <<  resultStr <<std::endl;
             resultFloat = std::stof(resultStr);

   // std::cout << "Result as float: " << resultFloat << std::endl;
} else {
    resultFloat = std::stof(numberStr);
   // std::cout << resultFloat << std::endl;
}

//float value = std::stof(numberStr);

        // Return the float value
       return resultFloat;
//    } catch (const std::invalid_argument& e) {
//        throw std::runtime_error("Invalid numeric value in SNMP result");
//    } catch (const std::out_of_range& e) {
//        throw std::runtime_error("Numeric value out of range in SNMP result");
//    }
}



// Helper method to execute SNMP GET
std::string ProcessExecutor::run_snmpget_control(const std::string& oid) {
    std::string command = "snmpget -v 2c -c public " + host + " " + oid;
    std::array<char, 128> buffer;
    std::string result;

    // Conditional compilation based on the operating system
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    //if
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

// Helper method to execute SNMP SET
bool ProcessExecutor::executeSNMPSet(const std::string& oid, const std::string& value, const std::string& community, const std::string& host) {
    std::string command = "snmpset -v 2c -c " + community + " " + host + " " + oid + " s \"" + value + "\"";
    std::array<char, 128> buffer;
    std::string result;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    //if
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result.find("STRING:") != std::string::npos;
    //return result.find("INTEGER:") != std::string::npos || result.find("STRING:") != std::string::npos;
}

// SNMP GET Methods

std::string ProcessExecutor::gettwtTemperature() {
    std::string oid = "1.3.6.1.2.1.25.1.6.0"; // OID twtTemperature   test oid: "1.3.6.1.2.1.25.1.6.0"
    float temperature = run_snmpget(oid);  // Get the float value from run_snmpget

    // Convert the float to a string with two decimal places
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << temperature;

    return ss.str();  // Return the formatted string
}




//std::string ProcessExecutor::getcoolingFanVoltage() {
//    std::string oid = "1.3.6.1.2.1.25.1.6.0"; // OID coolingFanVoltage
//    return run_snmpget(oid);
//}



// SNMP SET Method
bool ProcessExecutor::setControlStateString(const std::string &value) {
    std::string community = "private";
    std::string oid = "1.3.6.1.2.1.25.1.6.0"; // OID controlStateString
    return executeSNMPSet(oid, value, community, host);
}
