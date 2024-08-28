#ifndef PROCESSEXECUTOR_H
#define PROCESSEXECUTOR_H

#include <string>

class ProcessExecutor {
public:
    // Constructor
    ProcessExecutor(const std::string& host);

    // SNMP Get Methods
    std::string gettwtTemperature();


    // SNMP Set Method
    bool setControlStateString(const std::string &value);

private:
    // Member variables
    std::string host;

    // Helper methods
    std::string run_snmpget_control(const std::string& oid);
  float  run_snmpget(const std::string& oid);

    bool executeSNMPSet(const std::string& oid, const std::string& value, const std::string& community, const std::string& host);
};

#endif // PROCESSEXECUTOR_H
