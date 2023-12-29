/***************************************************************************
 *  Project                WIZLIGHTCPP
 *
 * Copyright (C) 2022 , Sri Balaji S.
 *
 * This software is licensed as described in the file LICENSE, which
 * you should have received as part of this distribution.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the LICENSE file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * @file wizcontrol.cpp
 *
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <clocale>
#include <vector>
#include "wizcontrol.h"
#include "log.h"
#include "TemperateTimeSchedule.h"

#include <cstdlib>
#include <regex>
#include <fstream>
#include <nlohmann/json.hpp>
#include <optional>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>


bool isInTimeFrame(int currentHour, int currentMinute, int startHour, int startMinute, int endHour, int endMinute);

using namespace std;



void printUsage()
{
    std::cout << "\n"
                "Usage: wizcontrol {Options} [Commands] {Args}\n"
                "Options:\n"
                "  --help\t\t\tShow the usage menu of this app\n"
                "  --verbose\t\t\tEnable verbose logs\n"
                << std::endl;

    std::cout << "Commands:\n"
                "  discover\t\t\tDiscover the Wiz bulb on the network\n"
                "  off\t\t\t\tTurn Off Wiz bulb\n"
                "  on\t\t\t\tTurn On Wiz bulb\n"
                "  status\t\t\tGet the current status of Wiz bulb\n"
                "  reboot\t\t\tReboot Wiz bulb\n"
                "  getdeviceinfo\t\t\tGet the bulb information\n"
                "  getwificonfig\t\t\tGet the Wifi Configuration\n"
                "  getuserconfig\t\t\tGet the User Configuration\n"
                "  getmodelconfig\t\t\tGet the Model Configuration\n"
                "  getsystemconfig\t\tGet the System Configuration\n"
                "  setbrightness\t\t\tSets the brightness on the bulb in percent\n"
                "  setrgbcolor\t\t\tSets the R G B color on the bulb\n"
                "  setspeed\t\t\tSets the color changing speed in percent\n"
		        "  setcolortemp\t\t\tSets the color temperature in kelvins\n"
                "  setscene\t\t\tSets the scene mode on the bulb\n"
                << std::endl;
}

std::string WizControl::getSceneList()
{
    std::stringstream sceneList;
    sceneList << "Available Scene modes:\n"
                "  1\tOcean\n"
                "  2\tRomance\n"
                "  3\tSunset\n"
                "  4\tParty\n"
                "  5\tFireplace\n"
                "  6\tCozy\n"
                "  7\tForest\n"
                "  8\tPastel Colors\n"
                "  9\tWake up\n"
                "  10\tBedtime\n"
                "  11\tWarm White\n"
                "  12\tDaylight\n"
                "  13\tCool white\n"
                "  14\tNight light\n"
                "  15\tFocus\n"
                "  16\tRelax\n"
                "  17\tTrue colors\n"
                "  18\tTV time\n"
                "  19\tPlantgrowth\n"
                "  20\tSpring\n"
                "  21\tSummer\n"
                "  22\tFall\n"
                "  23\tDeepdive\n"
                "  24\tJungle\n"
                "  25\tMojito\n"
                "  26\tClub\n"
                "  27\tChristmas\n"
                "  28\tHalloween\n"
                "  29\tCandlelight\n"
                "  30\tGolden white\n"
                "  31\tPulse\n"
                "  32\tSteampunk\n"
                // "  1000\tRhythm\n"
                << std::endl;
    return sceneList.str();
}

bool WizControl::checkArgOptions(const std::vector<std::string>& args, const std::string& cmd,
        const std::string& option, int& param)
{
    std::string strParam;
    auto ret = checkArgOptions(args, cmd, option, strParam);
    try {
        param = std::stoi(strParam);
    }
    catch (exception& e) {
        LOG_D("Exception @ %s", e.what());
    }
    return ret;
}

bool WizControl::checkArgOptions(const std::vector<std::string>& args, const std::string& cmd,
    const std::string& option,  std::string& param)
{
    auto it = std::find(args.begin(), args.end(), option);
    if (it != args.end()) {
        if (std::next(it) != args.end()) {
            param = *(std::next(it));
            return true;
        }
        else {
            std::cerr << "Error: Missing required parameter for " << option << " option\n";
            return false;
        }
    }
    else {
        std::cerr << "Unknown Option \n\n"
            "Usage: wizlightcpp " << cmd << " {OPTIONS}\n"
            "Try 'wizlightcpp " << cmd << " --help' for help."
            << std::endl;
        return false;
    }
    return true;
}

bool WizControl::validateArgsUsage(const std::vector<std::string>& args)
{
    if (args.empty() ||  g_cmdMap.find(args.at(0)) == g_cmdMap.end()) {
        printUsage();
        return false;
    }

    bool showArgUsage = false;
    std::string ipAddr, ret, cmd = args.at(0);
    std::vector<std::string> argUsageList;
    auto eCmd = g_cmdMap.at(cmd);

    if (args.at(1) == "--help") {
        showArgUsage = true;
        if (eCmd != discover)
            argUsageList.emplace_back("--ip\t\t\tIP address of the bulb.\n");
        argUsageList.emplace_back("--help\t\t\tShow this message and exit.\n");
    }
    else {
        if (eCmd != discover && !checkArgOptions(args, cmd, "--ip", ipAddr))
            return false;
        m_bulb.setDeviceIP(ipAddr);
    }

    switch (eCmd)
    {
    case discover:
        if (showArgUsage)
            argUsageList.emplace_back("--bcast\t\t\tBroadcast IP Address [eg: 192.168.1.255].\n");
        else {
            std::string bCastIp;
            if (checkArgOptions(args, cmd, "--bcast", bCastIp))
                ret = m_bulb.discover(bCastIp);
        }
	break;
	case on:
	case off:
	case status:
	case reboot:
	case getdeviceinfo:
    case getwificonfig:
	case getuserconfig:
    case getmodelconfig:
	case getsystemconfig:
        if (!showArgUsage)
            performWizRequest(cmd);
    break;
	case setbrightness:
        if (showArgUsage)
            argUsageList.emplace_back("--dim\t\t\tBrightness level in percent [0 to 100].\n");
        else {
            int dimlevel;
            if (checkArgOptions(args, cmd, "--dim", dimlevel))
                ret = m_bulb.setBrightness(dimlevel);
        }
    break;
	case setrgbcolor:
        if (showArgUsage) {
            argUsageList.emplace_back("--r\t\t\tRed color range [0 to 255].\n");
            argUsageList.emplace_back("--g\t\t\tGreen color range [0 to 255].\n");
            argUsageList.emplace_back("--b\t\t\tBlue color range [0 to 255].\n");
        }
        else {
            int r, g, b;
            if (checkArgOptions(args, cmd, "--r", r) && checkArgOptions(args, cmd, "--g", g) && checkArgOptions(args, cmd, "--b", b))
                ret = m_bulb.setRGBColor(r, g, b);
        }
    break;
	case setspeed:
        if (showArgUsage)
            argUsageList.emplace_back("--speed\t\t\tColor changing speed in percent [0 to 100].\n");
        else {
            int speed;
            if (checkArgOptions(args, cmd, "--speed", speed))
                ret = m_bulb.setSpeed(speed);
        }

    break;
	case setcolortemp:
        if (showArgUsage)
            argUsageList.emplace_back("--temp\t\t\tColor temperature in kelvins [1000 to 8000].\n");
        else {
            int temp;
            if (checkArgOptions(args, cmd, "--temp", temp))
                ret = m_bulb.setColorTemp(temp);
        }
    break;
	case setscene:
        if (showArgUsage) {
            argUsageList.emplace_back("--scene\t\t\tScene mode [1 to 32].\n");
            argUsageList.emplace_back(getSceneList());
        }
        else {
            int scene;
            if (checkArgOptions(args, cmd, "--scene", scene))
                ret = m_bulb.setScene(scene);
        }
    break;

	default:
        printUsage();
	return false;
    }

    if (showArgUsage) {
        std::cout << "Usage: wizlightcpp "<< cmd << " {OPTIONS}\n\n"
                "Options:" << std::endl;
        for (auto& it : argUsageList)
            std::cout << it;
        return false;
    }
    else if (!ret.empty())
        cout << ret << endl;
    return true;
}

//WizControl& WizControl::getInstance() {
//    static WizControl instance;
//    return instance;
//}

WizControl::WizControl()
{
    g_cmdMap = {
        {"discover", discover},
        {"on", on},
        {"off", off},
        {"status", status},
        {"reboot", reboot},
        {"getdeviceinfo", getdeviceinfo},
        {"getwificonfig", getwificonfig},
        {"getuserconfig", getuserconfig},
        {"getmodelconfig", getmodelconfig},
        {"getsystemconfig", getsystemconfig},
        {"setbrightness", setbrightness},
        {"setrgbcolor", setrgbcolor},
        {"setspeed", setspeed},
        {"setcolortemp", setcolortemp},
        {"setscene", setscene},
        {"allbulbs", allbulbs},
        {"offall", offall},
        {"onall", onall}
    };
    findAllBulbs();
}

WizControl::~WizControl() {
}

bool WizControl::isCmdSupported(const std::string& cmd) {
    auto it = g_cmdMap.find(cmd);

    if (it != g_cmdMap.end())
        return true;
    else
        return false;
}

std::string WizControl::performWizRequest(const std::string& cmd)
{
    std::string ret;
    if (g_cmdMap.find(cmd) == g_cmdMap.end()) {
        printUsage();
        return ret;
    }
    auto eCmd = g_cmdMap.at(cmd);

    if(eCmd == allbulbs)
    {
        findAllBulbs();
    }
    if(eCmd == offall)
    {
        turnOffAll();
    }
    if(eCmd == onall)
    {
        turnOnAll();
    }

    else if (eCmd != discover && m_bulb.getDeviceIp().empty()) {
        std::string ip;
        while (1) {
            cout << "Enter the bulb IP address: ";
            getline(cin, ip);
            if (ip.empty())
                continue;
            else
                break;
        }
        m_bulb.setDeviceIP(ip);
    }

    switch (eCmd) {
        case discover:
        {
            std::string bcastIp;
            cout << "Enter the broadcast IP Address: ";
            cin >> bcastIp;
            ret = m_bulb.discover(bcastIp);
        }
        break;
        case on:
            ret = m_bulb.toggleLight(true);
        break;

        case off:
            ret = m_bulb.toggleLight(false);
        break;

        case status:
            ret = m_bulb.getStatus();
        break;

        case reboot:
            ret = m_bulb.reboot();
        break;

        case getdeviceinfo:
            ret = m_bulb.getDeviceInfo();
        break;

        case getuserconfig:
            ret = m_bulb.getUserConfig();
        break;

        case getmodelconfig:
            ret = m_bulb.getModelConfig();
        break;

        case getwificonfig:
            ret = m_bulb.getWifiConfig();
        break;

        case getsystemconfig:
            ret = m_bulb.getSystemConfig();
        break;

	case setbrightness:
            ushort level;
            cout << "Enter the brightness level [0 to 100]:";
            cin >> level;
            ret = m_bulb.setBrightness(level);
        break;

        case setrgbcolor:
            ushort r, g, b;
            cout << "Enter the Red color range [0 to 255]:";
            cin >> r;
            cout << "Enter the Green color range [0 to 255]:";
            cin >> g;
            cout << "Enter the Blue color range [0 to 255]:";
            cin >> b;
            ret = m_bulb.setRGBColor(r, g, b);
        break;

        case setspeed:
            ushort speed;
            cout << "Enter the color changing speed in percent [0 to 100]:";
            cin >> speed;
            ret = m_bulb.setSpeed(speed);
        break;

        case setcolortemp:
            ushort color;
            cout << "Enter the color temperature in kelvins [1000 to 8000]:";
            cin >> color;
            ret = m_bulb.setColorTemp(color);
        break;

        case setscene:
            ushort scene;
            cout << getSceneList();
            cout << "Enter the scene mode [1 to 32]:";
            cin >> scene;
            ret = m_bulb.setScene(scene);
        break;

        default:
        break;
    }
    if (!ret.empty())
        cout << ret << endl;

    return ret;
}

void WizControl::changeTempInTime(int startTemp, int endTemp, TemperatureTimeFrame timeFrame, std::tm* localTime) {

    auto localHour = localTime->tm_hour;
    auto localMin = localTime->tm_min;

    auto minutes = (timeFrame.endTime.tm_hour-timeFrame.startTime.tm_hour) * 60 + (timeFrame.endTime.tm_min-timeFrame.startTime.tm_min);
    auto delta = (endTemp - startTemp) * 1.0 / minutes;
    auto deltaT = (localHour-timeFrame.startTime.tm_hour) * 60 + (localMin - timeFrame.startTime.tm_min);

    auto currentTemerature = startTemp + delta * deltaT;
    std::cout << "Current temperature: " << currentTemerature << " deltaT: " << deltaT <<  " delta: " << delta << endl;
    m_bulb.setColorTemp(currentTemerature);

}

bool WizControl::findBulb(const std::string ipAdress) {
    auto ret = m_bulb.discover(ipAdress);
    if (!ret.empty())
    {
        if(ret.find("bulb_response") != std::string::npos)
        {
//            cout << ret << endl;
            return true;
        }
        return false;
    }
    return false;
}

std::vector<std::string> WizControl::extractIPAddresses(const std::string& input) {
    std::vector<std::string> addresses;
    std::regex regex("\\b(?:\\d{1,3}\\.){3}\\d{1,3}\\b");

    auto words_begin = std::sregex_iterator(input.begin(), input.end(), regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        addresses.push_back(i->str());
    }

    return addresses;
}

std::vector<std::string> WizControl::findAllBulbs()
{
    allIPs.clear();
    const char* command = "arp -a > devicesInNetwork.txt";

    int result = system(command);
    if (result == 0) {
        std::stringstream buffer;
        buffer << std::ifstream("devicesInNetwork.txt").rdbuf();
        std::vector<std::string> ipAddresses = extractIPAddresses(buffer.str());


//        std::cout << "Extracted IP addresses:" << std::endl;
        for (const auto& ipAddress : ipAddresses) {
//            std::cout << ipAddress << std::endl;
            if(findBulb(ipAddress))
            {
                allIPs.push_back(ipAddress);
            }
        }
    }
    return allIPs;
}

void WizControl::setActiveBulb(std::string ip) {

    m_bulb.setDeviceIP(ip);
}

void WizControl::turnOffAll()
{
    for(const auto& ip : allIPs)
    {
        m_bulb.setDeviceIP(ip);
        m_bulb.toggleLight(false);
    }
}

void WizControl::turnOnAll()
{
    for(const auto& ip : allIPs)
    {
        m_bulb.setDeviceIP(ip);
        m_bulb.toggleLight(true);
    }
}

std::vector<TemperatureTimeSchedule> loadTemperatureSchedule() {
    ifstream file("temperatureSchedule.json");
    nlohmann::json jsonData;
    file >> jsonData;

    // Accessing values from the JSON object

    std::vector<TemperatureTimeSchedule> schedules;
    for(const auto& bulb : jsonData["bulbes"]) {

        TemperatureTimeSchedule temperatureTimeSchedule;
        temperatureTimeSchedule.m_devIP = bulb["m_devIP"];
        temperatureTimeSchedule.name = bulb["name"];

        for (const auto &timeFrame: bulb["temperatureTimeFrames"]) {
            TemperatureTimeFrame temperatureTimeFrame;
            temperatureTimeFrame.startTemperature = timeFrame["Temperature"]["start"];
            temperatureTimeFrame.endTemperature = timeFrame["Temperature"]["end"];
            temperatureTimeFrame.startTime.tm_hour = timeFrame["startTime"]["hour"];
            temperatureTimeFrame.startTime.tm_min = timeFrame["startTime"]["min"];
            temperatureTimeFrame.endTime.tm_hour = timeFrame["endTime"]["hour"];
            temperatureTimeFrame.endTime.tm_min = timeFrame["endTime"]["min"];

            temperatureTimeSchedule.temperatureTimeFrames.push_back(temperatureTimeFrame);
        }
        schedules.push_back(temperatureTimeSchedule);
    }

    // Print the values
    for(const auto& temperatureTimeSchedule : schedules)
    {
        cout << "m_devIP: " << temperatureTimeSchedule.m_devIP << endl;
        cout << "name: " << temperatureTimeSchedule.name << endl;
        for (const auto& timeFrame : temperatureTimeSchedule.temperatureTimeFrames)
        {
            cout << "Temperature start: " << timeFrame.startTemperature << endl;
            cout << "Temperature end: " << timeFrame.endTemperature << endl;
            cout << "Start time: " << timeFrame.startTime.tm_hour << ":" << timeFrame.startTime.tm_min << endl;
            cout << "End time: " << timeFrame.endTime.tm_hour << ":" << timeFrame.endTime.tm_min << endl;
        }
    }


    return schedules;
}

std::optional<TemperatureTimeFrame> isInAnyTimeFrame(const std::tm* localTime, const TemperatureTimeSchedule &temperatureTimeSchedule)
{

    int currentHour = localTime->tm_hour;
    int currentMinute = localTime->tm_min;
    for(const auto& frame : temperatureTimeSchedule.temperatureTimeFrames)
    {
        if(isInTimeFrame(currentHour, currentMinute, frame.startTime.tm_hour, frame.startTime.tm_min, frame.endTime.tm_hour, frame.endTime.tm_min))
        {
            return frame;
        }
    }
    return {};
}


std::mutex mtx;
std::condition_variable cv;
std::queue<char> keyQueue;
std::atomic<bool> keepRunning(true);

void keyboardInputThread() {
    while (keepRunning) {
        char pressedKey = std::cin.get();

        {
            std::unique_lock<std::mutex> lock(mtx);
            keyQueue.push(pressedKey);
        }

        cv.notify_one();

        // Exit on 'q' key press
        if (pressedKey == 'q') {
            keepRunning = false;
            break;
        }
    }
}

void processingThread(WizControl& wiz) {
    while (keepRunning) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !keyQueue.empty(); });

        char key = keyQueue.front();
        keyQueue.pop();
        lock.unlock();

        // Process the key
        if (key == 'o') {
            std::cout << "Processing thread: 'o' key pressed." << std::endl;
            wiz.turnOnAll();
        }
        if (key == 'f') {
            std::cout << "Processing thread: 'f' key pressed." << std::endl;
            wiz.turnOffAll();
        }
        if (key == 'q') {
            keepRunning = false;
            break;
        }
    }
}

void schedulingThread(WizControl& wiz, const vector<TemperatureTimeSchedule> &temperatureTimeSchedules) {

    while (keepRunning)
    {
//        std::unique_lock<std::mutex> lock(mtx);
//        cv.wait(lock, [] { return !keyQueue.empty(); });
//
//        char key = keyQueue.front();
//        keyQueue.pop();
//        lock.unlock();
//
//        // Exit on 'q' key press
//        if (key == 'q') {
//            keepRunning = false;
//            break;
//        }

        auto currentTime = std::chrono::system_clock::now();
        std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
        std::tm* localTime = std::localtime(&currentTime_t);

        for(const auto& schedule : temperatureTimeSchedules)
        {
            if( auto frame = isInAnyTimeFrame(localTime, schedule))
            {
                TemperatureTimeFrame timeFrame = frame.value();

                std::cout << "current IP: "<< schedule.m_devIP << std::endl;
                wiz.setActiveBulb(schedule.m_devIP);
                wiz.changeTempInTime(timeFrame.startTemperature, timeFrame.endTemperature, timeFrame, localTime);
            }

            else{
                std::cout << "Bad time, no action" << endl;
            }
        }
        sleep(1);
    }
}


//int main(int argc, char *argv[])
//{
////    WizControl& wiz = WizControl::getInstance();
//
//    WizControl wiz;
//    auto temperatureTimeSchedules = loadTemperatureSchedule();
//
//    std::jthread inputThread(keyboardInputThread);
//    std::jthread processThread(processingThread, std::ref(wiz));
//    std::jthread scheduleThread(schedulingThread, std::ref(wiz), std::ref(temperatureTimeSchedules));
//
//
////    while (true)
////    {
////        auto currentTime = std::chrono::system_clock::now();
////        std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
////        std::tm* localTime = std::localtime(&currentTime_t);
////
////        for(const auto& schedule : temperatureTimeSchedules)
////        {
////            if( auto frame = isInAnyTimeFrame(localTime, schedule))
////            {
////                TemperatureTimeFrame timeFrame = frame.value();
////
////                std::cout << "current IP: "<< schedule.m_devIP << std::endl;
////                wiz.setActiveBulb(schedule.m_devIP);
////                wiz.changeTempInTime(timeFrame.startTemperature, timeFrame.endTemperature, timeFrame, localTime);
////            }
////
////            else{
////                std::cout << "Bad time, no action" << endl;
////            }
////        }
////        sleep(1);
////    }
//
//
////    if (argc == 1) {
////        printUsage();
////        return 0;
////    }
////
////    std::vector<std::string> args;
////    for (int i = 1; i < argc; i++) {
////        args.emplace_back(argv[i]);
////    }
////
////    auto hIt = std::find(args.begin(), args.end(), "--help");
////    if ((hIt != args.end()) && (hIt == args.begin())) {
////        printUsage();
////        return 0;
////    }
////
////    auto vIt = std::find(args.begin(), args.end(), "--verbose");
////    if (vIt != args.end()) {
////        L::setLogLevel(L::d);
////        LOG_D("Verbose log enabled");
////        args.erase(vIt);
////    }
////
////    if (!wiz.isCmdSupported(args.at(0))) {
////        printUsage();
////        return 0;
////    }
////
////    if (args.size() == 1)
////        wiz.performWizRequest(args.at(0));
////    else
////        wiz.validateArgsUsage(args);
//
//
//}



bool isInTimeFrame(int currentHour, int currentMinute, int startHour, int startMinute, int endHour, int endMinute) {
    return (currentHour > startHour || (currentHour == startHour && currentMinute >= startMinute)) &&
               (currentHour < endHour || (currentHour == endHour && currentMinute <= endMinute));
}
