#include <iostream>
#include <fstream>//bisa membuka file JSON seperti motion_movie/53.json
#include <string>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>//buat doc.ParseStream(isw);.

namespace rj = rapidjson;
using namespace std;


int main(){
    int M;
    cout << "ID :" ;
    cin >> M;
    string path_json = "XL/motion_movie/" + to_string(M)+ ".json";

    ifstream ifs(path_json);//cb baca file
    if (!ifs.is_open()){
        cout << "error opening " << path_json << endl;
        return 1;
    }

    rj :: IStreamWrapper isw (ifs);//bungkus file input biar bisa dibaca oleh RapidJSON.
    rj :: Document doc;//tempat nyimpan hasil parse JSON.

    doc.ParseStream(isw); //baca dan ubah isi file JSON jadi data struktur di dalam doc.
    if(doc.HasParseError()){
        cout <<"json parse error"<<endl;
        return 1;
    }

    const rj::Value& id = doc["id"];
    const rj::Value& name = doc["name"];
    const rj::Value& total_unit = doc["total_unit"];
    const rj::Value& motion_units = doc["motion_unit"];

    for (const auto& unit : motion_units.GetArray()){
        cout << "Unit ID: " << unit["id"].GetInt() << endl;
        cout << "Speed: " << unit["speed"].GetFloat() << endl;
        cout << "loop : " << unit["loop"].GetInt() << endl;

        int UnitId = unit["id"].GetInt();
        string PathUnit = "XL/motion_unit/" + to_string(UnitId) + ".json";
        ifstream ifsu(PathUnit);
        if(!ifsu.is_open()){
            cout << "open unit error" << endl;
            return 1;
        }

        rj::IStreamWrapper iswu (ifsu);
        rj::Document docu;

        docu.ParseStream(iswu);
        if(docu.HasParseError()){
            cout << "parse error";
            return 1;
        }

        int unit_id = docu["id"].GetInt();
        string name = docu["name"].GetString();
        int total_frame = docu["total_frame"].GetInt();

        const rj::Value& frames = docu["motion_frame"];
        for(const auto& frame : frames.GetArray()){
            float time = frame["time"].GetFloat();
            int motion_frame = frame["motion_frame"].GetInt();

            cout << "time : " << time << "| motion_frame : " << motion_frame << endl;
        }

    }

    return 0;
}
