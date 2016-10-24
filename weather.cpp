#include "weather.h"
#include "connecttoserverweather.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <map>

using std::string;

Weather::Weather()
{

}

void Weather::connectWeather(const string &city)
{
    ConnectToServerWeather connectServer;
    string weather = connectServer.getWeather(city);
    if(!weather.empty())
    {
        mTextToJason(weather);
        mJsonParser(weather);
        connectServer.saveIconFile(mIconName);
    }
    else
        mComment = "Error: connect";
}

void Weather::mTextToJason(string &text) const
{
    size_t start = text.find("{");
    size_t end = text.rfind('}');

    if((start != std::string::npos) && (end != std::string::npos))
    {
        text.erase(end+1);
        text.erase(text.begin(), text.begin() + start);
    }
    else
        text.clear();
}

void Weather::mJsonParser(const string &json)
{
    if(!json.empty())
    {
        QJsonDocument doc;
        doc = doc.fromJson(json.c_str());

        if(!doc.isNull())
        {
            QJsonObject obj = doc.object();
            mComment = obj["message"].toString().toStdString();
            if(mComment.empty())
            {
                mCity = obj["name"].toString().toStdString() + ", ";
                mCity += obj["sys"].toObject()["country"].toString().toStdString();
                mComment = obj["weather"].toArray()[0].toObject()["description"].toString().toStdString();
                mIconName = obj["weather"].toArray()[0].toObject()["icon"].toString().toStdString();
                mTemp = std::to_string((int) obj["main"].toObject()["temp"].toDouble());
                mTemp += " \xC2\xB0\C";
            }
        }
    }
}

string Weather::getTemp()
{
    return mTemp;
}

string Weather::getComment()
{
    return mComment;
}

string Weather::getCity()
{
    return mCity;
}
