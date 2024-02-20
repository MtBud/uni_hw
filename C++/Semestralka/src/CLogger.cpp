#include <string>
#include <iostream>
#include <fstream>
#include "CLogger.h"
#include "CConfig.h"

void CLogger::log ( const std::string& message){
    CConfig conf;
    if( conf.data["log"]["console"] )
        std::cout << message << std::endl;

    if( conf.data["log"]["file"] ){
        std::ofstream ofs( std::string( conf.data["root"] ) + std::string( conf.data["log"]["path"] ), std::ios::app);
        ofs << message << std::endl;
        ofs.close();
    }
}
