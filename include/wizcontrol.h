#include <iostream>
#include <map>
#include <vector>

#include "bulb.h"

#include <chrono>
#include "TemperateTimeSchedule.h"

class WizControl
{
public:
    WizControl();
    ~WizControl();
//    static WizControl& getInstance();
    bool isCmdSupported(const std::string& cmd);
    bool validateArgsUsage(const std::vector<std::string>& args);
    std::string performWizRequest(const std::string& cmd);

    void changeTempInTime(int startTemp, int endTemp, TemperatureTimeFrame timeFrame, std::tm* localTime);
    bool findBulb(const std::string ipAdress);
    void findAllBulbs();
    void setActiveBulb(std::string ip);
    void turnOffAll();
    void turnOnAll();

private:
    std::string getSceneList();
    bool checkArgOptions(const std::vector<std::string>& args, const std::string& cmd,
        const std::string& option, std::string& param);
    bool checkArgOptions(const std::vector<std::string>& args, const std::string& cmd,
        const std::string& option, int& param);

    std::vector<std::string> extractIPAddresses(const std::string& input);



    enum WIZCMD {
        discover, on, off, status, reboot,
        getdeviceinfo, getwificonfig, getuserconfig, getmodelconfig, getsystemconfig,
      	setbrightness, setrgbcolor, setspeed, setcolortemp, setscene, allbulbs, offall, onall,
    };
    Bulb m_bulb;
    std::map<std::string, WIZCMD> g_cmdMap;
    std::vector<std::string> allIPs;
};
