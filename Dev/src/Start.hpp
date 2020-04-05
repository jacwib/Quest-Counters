#include "../beatsaber-hook/shared/utils/utils.h"
#include "../beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "../beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "../beatsaber-hook/shared/customui/customui.hpp"
#include "../beatsaber-hook/shared/config/config-utils.hpp"

//extern rapidjson::Document& config_doc;

#include <math.h>
#include <cmath>
#include <string>
#include <iostream>

extern bool runScores;
extern int num;
extern int num2;
extern int sum ;
extern int sum2;
extern int sumAcc;
extern float average;
extern float averageAcc;
extern float sessionAverage;
extern char averageFinal[40];
extern char averageAccFinal[40];
extern char averageSessionFinal[40]	;
extern int songs;
extern int hit;
extern int missed;
extern std::string hitString;
extern std::string missedString;
extern int NotesPassed;
extern int NotesCount;
extern bool _360 ;
extern int notesLeft;
extern bool advancedHud;
extern float RightSaberSpeed, LeftSaberSpeed;

extern std::string RightSpeed, LeftSpeed;

extern CustomUI::TextObject averageText;
extern CustomUI::TextObject MISSED;
extern CustomUI::RawImageObject Image;
extern CustomUI::TextObject HIT;
extern CustomUI::TextObject Notes;
extern CustomUI::TextObject LeftSaberSwingSpeed;
extern CustomUI::TextObject RightSaberSwingSpeed;

//void LoadConfig();
//void SaveConfig();
//extern void SetDefault();

std::string GetPercentFromNotesLeftAndNotes(int NotesLeft, int Notes);
void StartNotesLeftCounter(Il2CppObject* self);
void StartMissCounter(Il2CppObject* self); 
void StartHitCounter(Il2CppObject* self);
void StartAverageSpeedCounter(Il2CppObject* self);
void StartImage(Il2CppObject* self);

/*static struct Config_t {
    bool Missed = true;
    bool Hits = true;
    bool NotesLeft = true;
    bool AAcc = true;
    bool Ascore = true;
    bool ASScore = true;
} Config;
*/