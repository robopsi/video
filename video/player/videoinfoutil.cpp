#include "videoinfoutil.h"
#include <stdio.h>
#include <regex>
#include <vector>

std::vector<std::string> stringSplit(std::string srcStr, std::string delimStr,bool repeatedCharIgnored)
{
    std::vector<std::string> resultStringVector;
    std::replace_if(srcStr.begin(), srcStr.end(), [&](const char& c){if(delimStr.find(c)!=std::string::npos){return true;}else{return false;}}/*pred*/, delimStr.at(0));
    size_t pos = srcStr.find(delimStr.at(0));
    std::string addedString="";
    while(pos != std::string::npos) {
        addedString=srcStr.substr(0,pos);
        if (!addedString.empty()||!repeatedCharIgnored) {
            resultStringVector.push_back(addedString);
        }
        srcStr.erase(srcStr.begin(), srcStr.begin()+pos+1);
        pos=srcStr.find(delimStr.at(0));
    }
    addedString = srcStr;
    if (!addedString.empty()||!repeatedCharIgnored) {
        resultStringVector.push_back(addedString);
    }
    return resultStringVector;
}

VideoInfoUtil::VideoInfoUtil()
{
}

bool VideoInfoUtil::isVideoSolutionSuitable(const QString &filePath)
{
    char cmdLine[512];
    char buff[1024];
    FILE *stream = NULL;
    // matching rule
    std::smatch matchResult;
    std::regex regex("([\\s\\S]*)Video:(.*?),(.*?),(.*?)([\\s\\S]*)");
    bool isSuitable = true;

    sprintf(cmdLine,"ffmpeg -i %s -hide_banner 2>&1",filePath.toLocal8Bit().constData());

    if((stream = popen(cmdLine,"r")) != NULL){
        while(fgets(buff,1024,stream)){
            //Stream #0:1: Video: rv20 (RV20 / 0x30325652), yuv420p, 352x288, 117 kb/s,
            //15 fps, 15 tbr, 1k tbn, 1k tbc
            std::string str(buff);
            if(std::regex_match(str,matchResult,regex)){
                std::vector<std::string> vector = stringSplit(str,",",true);
                if(vector.size() >= 3){
                    // get resolution width and height.
                    std::vector<std::string> sizeVector = stringSplit(vector.at(2),"x",true);
                    if(sizeVector.size() >= 2){
                        int width = std::stoi(sizeVector.at(0));
                        int height = 0;
                        if(sizeVector.at(1).find(" ") != std::string::npos){
                            height = std::stoi(stringSplit(sizeVector.at(1)," ",true).at(0));
                        }else{
                            height = std::stoi(sizeVector.at(1));
                        }
                        printf("video resulution: width: %d,height: %d \n",width,height);
                        if(width > 1920 || height > 1080){
                            isSuitable = false;
                        }
                        break;
                    }
                }
            }
        }
    }

    pclose(stream);
    return isSuitable?true:false;
}
