#include "Start.hpp"
#include "../beatsaber-hook/shared/config/rapidjson-utils.hpp"

rapidjson::Document& config_doc = Configuration::config;

void SaveConfig() {
    log(INFO, "Saving Configuration...");
    config_doc.RemoveAllMembers();
    config_doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = config_doc.GetAllocator();
    config_doc.AddMember("Missed Notes Counter", Config.Missed, allocator);
    config_doc.AddMember("Hit Notes Counter", Config.Hits, allocator);
    config_doc.AddMember("Notes Left / Note Percentage", Config.NotesLeft, allocator);
    config_doc.AddMember("Average Accuracy", Config.AAcc, allocator);
    config_doc.AddMember("Average Score", Config.Ascore, allocator);
    config_doc.AddMember("Average Session Score", Config.ASScore, allocator);

    Configuration::Write();
    log(INFO, "Saved Configuration!");
}   

void LoadConfig() {
    log(INFO, "Loading Configuration...");
    Configuration::Load();
    bool foundEverything = true;
    if (config_doc.HasMember("Missed Notes Counter") && config_doc["Missed Notes Counter"].IsBool()) {
        Config.Missed = config_doc["Missed Notes Counter"].GetBool();
    }
    else {
        foundEverything = false;
    }
    if (config_doc.HasMember("Walls") && config_doc["Walls"].IsBool()) {
        Config.Hits = config_doc["Walls"].GetBool();
    }
    else {
        foundEverything = false;
    }
    if (config_doc.HasMember("Sabers") && config_doc["Sabers"].IsBool()) {
        Config.NotesLeft = config_doc["Sabers"].GetBool();
    }
    else {
        foundEverything = false;
    }
    if (config_doc.HasMember("Trails") && config_doc["Trails"].IsBool()) {
        Config.AAcc = config_doc["Trails"].GetBool();
    }
    else {
        foundEverything = false;
    }
    if (config_doc.HasMember("Notes") && config_doc["Notes"].IsBool()) {
        Config.Ascore = config_doc["Notes"].GetBool();
    }
    else {
        foundEverything = false;
    }
    if (config_doc.HasMember("QSabers") && config_doc["QSabers"].IsBool()) {
        Config.ASScore = config_doc["QSabers"].GetBool();
    }
    else {
        foundEverything = false;
    }
    if (foundEverything) {
        log(INFO, "Loaded Configuration!");
    }
}
void SetDefault()
{
    Config.Missed = true;
}