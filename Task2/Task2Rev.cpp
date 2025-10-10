#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>       
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

namespace rj = rapidjson;
using namespace std;

class MotionFrame {
public:
    float time;
    int motion_frame;

    MotionFrame(const rj::Value& frame){
        time = frame["time"].GetFloat();
        motion_frame = frame["motion_frame"].GetInt();
    }
};

class MotionUnit {
public:
    int id;
    string name;
    int total_frame;
    int loop;
    float speed;
    vector<int> frames; 

    MotionUnit(const rj::Value& unit_json){
        id = unit_json["id"].GetInt();
        loop = unit_json["loop"].GetInt();
        speed = unit_json["speed"].GetFloat();

        string path_unit = "../../XL/motion_unit/" + to_string(id) + ".json";
        ifstream ifsu(path_unit);
        if(!ifsu.is_open()){
            cout << "open unit error" << endl;
            return;
        }

        rj::IStreamWrapper iswu(ifsu);
        rj::Document docu;
        docu.ParseStream(iswu);
        if(docu.HasParseError()){
            cout << "parse error" << endl;
            return;
        }

        name = docu["name"].GetString();
        total_frame = docu["total_frame"].GetInt();

        const rj::Value& motion_frames = docu["motion_frame"];
        
        for(const auto& arr : motion_frames.GetArray()){
            for(const auto& f : arr.GetArray()){
                frames.push_back(f.GetInt());
            }
        }
    }

    void print() const {
        for(int l = 0; l < loop; l++){ 
            for(const auto& f : frames){
                cout << "Unit ID: " << id 
                     << " | motion_frame : " << f << endl;
            }
        }
    }
};


class MotionMovie {
public:
    int id;
    string name;
    int total_unit;
    vector<MotionUnit> units;

    MotionMovie(const string& path_json){
        if(!filesystem::exists(path_json)){
            cout << "Movie JSON file not found: " << path_json << endl;
            return;
        }

        ifstream ifs(path_json);
        if(!ifs.is_open()){
            cout << "error opening " << path_json << endl;
            return;
        }

        rj::IStreamWrapper isw(ifs);
        rj::Document doc;
        doc.ParseStream(isw);
        if(doc.HasParseError()){
            cout << "json parse error" << endl;
            return;
        }

        id = doc["id"].GetInt();
        name = doc["name"].GetString();
        total_unit = doc["total_unit"].GetInt();

        const rj::Value& motion_units = doc["motion_unit"];
        for(const auto& u : motion_units.GetArray()){
            units.emplace_back(u);
        }
    }

    void print() const {
        for(const auto& u : units){
            u.print();
        }
    }
};

int main(){
    int M;
    cout << "ID :";
    cin >> M;

    string path_json = "../../XL/motion_movie/" + to_string(M)+ ".json";

    cout << "Current working directory: " << filesystem::current_path() << endl;
    cout << "Trying to open movie JSON: " << path_json << endl;

    MotionMovie movie(path_json);
    movie.print();

    return 0;
}


