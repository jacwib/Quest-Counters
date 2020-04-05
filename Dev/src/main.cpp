#include "Start.hpp"
#include "../beatsaber-hook/shared/utils/utils.h"
#include "../beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "../beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "../beatsaber-hook/shared/customui/customui.hpp"
#include <math.h>
#include <cmath>
#include <string>
#include <iostream>
bool runScores = false;
int num = 0;
int num2 = 0;
int sum = 0;
int sum2 = 0;
int sumAcc = 0;
float average = 0.0f;
float averageAcc = 0.0f;
float sessionAverage = 0.0f;
char averageFinal[40] = { 0 };
char averageAccFinal[40] = { 0 };
char averageSessionFinal[40] = { 0 };
int songs = 0;
int hit = 0;
std::string hitString = std::to_string(hit);
std::string missedString = std::to_string(missed);
int missed = 0;
int NotesPassed;
int NotesCount;
bool _360 = false;
int notesLeft;
bool advancedHud;
float RightSaberSpeed, LeftSaberSpeed;
std::string RightSpeed = std::to_string(RightSaberSpeed), LeftSpeed = std::to_string(LeftSaberSpeed);
CustomUI::TextObject averageText;
CustomUI::TextObject MISSED;
CustomUI::TextObject HIT;
CustomUI::TextObject Notes;
CustomUI::RawImageObject Image;
CustomUI::TextObject LeftSaberSwingSpeed;
CustomUI::TextObject RightSaberSwingSpeed;

std::string GetPercentFromNotesLeftAndNotes(int NotesLeft, int Notes)
    {
       int oof = (Notes - NotesLeft) * 100 / (float)Notes;
        return std::to_string(oof);
    }

std::string GetShortStringFromLongAssFloat(float victim)
{
    int cutVictem = (int)victim;
    std::string FinalTortureSequence = std::to_string(cutVictem);
    return FinalTortureSequence;
}

float SafeDivideScore(int total, int count)
    {
        float result = round(((double)(total)) / (count));
        return result;
    }
void UpdateNotesLeft()
    {
        notesLeft -= 1;
        Notes.set("\nNotes Left: " + std::to_string(notesLeft) + " (" + GetPercentFromNotesLeftAndNotes(notesLeft, NotesCount) + "%)");
    }

MAKE_HOOK_OFFSETLESS(StartScoreCounters, void, Il2CppObject* self)
{
    StartAverageSpeedCounter(self);
    StartMissCounter(self);
    StartHitCounter(self);
    StartScoreCounters(self);
    
    StartNotesLeftCounter(self);
}   
MAKE_HOOK_OFFSETLESS(StartLevelDetailButtons, void, Il2CppObject* self)
{
    StartImage(self);
    StartLevelDetailButtons(self);
}
MAKE_HOOK_OFFSETLESS(HandleComboBreakingEventHappened, void, Il2CppObject* self, Il2CppObject* noteController) {
    Il2CppObject* NoteData;
    int NoteType;
    il2cpp_utils::RunMethod(&NoteData, noteController, "get_noteData");
    il2cpp_utils::RunMethod(&NoteType, NoteData, "get_noteType");
    if (NoteType == 0 || NoteType == 1)
    {
        UpdateNotesLeft();
        missed++;
        std::string combine2 = "\n \n \n<color=#FF6347>Missed</color> Notes: " + std::to_string(missed);
        std::string combine3 = "\n \n<color=#FF6347>Missed</color> Notes: " + std::to_string(missed) + " All <color=#00FF00>Hit</color> Notes: " + std::to_string(hit);
        if (_360)
        {
            il2cpp_utils::RunMethod(MISSED.textMesh, "set_text", il2cpp_utils::createcsstr(combine3));
        }
        else
        {

            il2cpp_utils::RunMethod(MISSED.textMesh, "set_text", il2cpp_utils::createcsstr(combine2));
        }

    }        
    HandleComboBreakingEventHappened(self, noteController);
}   
    //total hit notes                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
    MAKE_HOOK_OFFSETLESS(HandleNoteWasCutEvent, void, Il2CppObject * self, Il2CppObject * noteController, Il2CppObject * noteCutInfo)
    {
        std::string combine2 = "\n \n \n<color=#FF6347>Missed</color> Notes: " + std::to_string(missed);
                     
        std::string combine3 = "\n \n<color=#FF6347>Missed</color> Notes: " + std::to_string(missed) + " All <color=#00FF00>Hit</color> Notes: " + std::to_string(hit);
        std::string combined5 = "\n \n<color=#FF6347>Missed</color> Notes: " + std::to_string(missed) + " All <color=#00FF00>Hit</color> Notes: " + std::to_string(hit);
        if (!_360)
        {
            bool allIsOK;
            if (!il2cpp_utils::RunMethod(&allIsOK, noteCutInfo, "get_allIsOK"))
            {
                log(CRITICAL, "Failed to get_allIsOK");
            }
            if (allIsOK)
            {
                UpdateNotesLeft();
                hit++;
                std::string combined1 = "\n \n \n \nAll <color=#00FF00>Hit</color> Notes: " + std::to_string(hit);

                il2cpp_utils::RunMethod(HIT.textMesh, "set_text", il2cpp_utils::createcsstr(combined1));
            }
            else
            {
                UpdateNotesLeft();
                missed++;

                il2cpp_utils::RunMethod(MISSED.textMesh, "set_text", il2cpp_utils::createcsstr(combine2));
            }
        }
        else
        {
            bool allIsOK;
            if (!il2cpp_utils::RunMethod(&allIsOK, noteCutInfo, "get_allIsOK"))
            {
                log(CRITICAL, "Failed to get_allIsOK");
            }
            if (allIsOK)
            {
                UpdateNotesLeft();
                hit++;
                il2cpp_utils::RunMethod(MISSED.textMesh, "set_text", il2cpp_utils::createcsstr(combined5));
            }
            else
            {
                UpdateNotesLeft();
                missed++;

                il2cpp_utils::RunMethod(MISSED.textMesh, "set_text", il2cpp_utils::createcsstr(combine3));
            }
        }
        HandleNoteWasCutEvent(self, noteController, noteCutInfo);
        
}
    
MAKE_HOOK_OFFSETLESS(GetNotes, void, Il2CppObject* self, Il2CppObject* difficultyBeatmap, Il2CppObject* overrideEnvironmentSettings, Il2CppObject* overrideColorScheme, Il2CppObject* gameplayModifiers, Il2CppObject* playerSpecificSettings, Il2CppObject* practiceSettings, Il2CppString* backButtonText, bool useTestNoteCutSoundEffects) {
    Il2CppObject* beatmapData;
    il2cpp_utils::RunMethod(&beatmapData, difficultyBeatmap, "get_beatmapData");
    il2cpp_utils::RunMethod(&NotesCount, beatmapData, "get_notesCount");
    GetNotes(self, difficultyBeatmap, overrideEnvironmentSettings, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, backButtonText, useTestNoteCutSoundEffects);
}
MAKE_HOOK_OFFSETLESS(Get_SaberSpeed, void, Il2CppObject* self) {
    Get_SaberSpeed(self);
    Il2CppObject* LeftSaber;
    Il2CppObject* RightSaber;

    il2cpp_utils::RunMethod(&LeftSaberSpeed, self, "get_bladeSpeed");
    il2cpp_utils::RunMethod(&RightSaberSpeed, self, "get_bladeSpeed");
    RightSpeed = std::to_string(RightSaberSpeed), LeftSpeed = std::to_string(LeftSaberSpeed);
    int speed = (int)LeftSaberSpeed;
    if (!_360)
    {
        LeftSaberSwingSpeed.set("\n\n\n<color=#FF0000>Saber</color>  Speed: " + std::to_string(speed));
    }
    else
    {
        LeftSaberSwingSpeed.set("\n\n<color=#FF0000>Saber</color>  Speed: " + std::to_string(speed));
    }
    
    
}

// Thanks Henwill 
MAKE_HOOK_OFFSETLESS(RawScore, void, Il2CppObject* noteCutInfo, int* beforeCutRawScore, int* afterCutRawScore, int* cutDistanceRawScore) {
    RawScore(noteCutInfo, beforeCutRawScore, afterCutRawScore, cutDistanceRawScore);
    std::string combined6 = std::string("Average Score: ") + averageFinal + "\nAverage Accuracy: " + averageAccFinal + "\nAverage Session Score: " + averageSessionFinal;
    std::string combined7 = std::string("Average Score: ") + averageFinal + "\nAverage Accuracy: " + averageAccFinal + "\nAverage Session Score: " + averageSessionFinal + "\n<color=#FFA500><size=75%>Thanks Henwill For these</size></color>";
    if(runScores) {
        int beforeCut = *beforeCutRawScore;
        int afterCut = *afterCutRawScore;
        int cutDist = *cutDistanceRawScore;

        int totalScore = beforeCut+afterCut+cutDist;

        sumAcc += cutDist;
        sum += totalScore;
        sum2 += totalScore;
        num++;
        num2++;

        averageAcc = float(sumAcc) / float(num);
        average = float(sum) / float(num);
        sessionAverage = float(sum2) / float(num2);

        log(DEBUG, "%f %f %f", averageAcc, average, sessionAverage);

        sprintf(averageFinal, "%.2f", average);
        sprintf(averageAccFinal, "%.2f", averageAcc);
        sprintf(averageSessionFinal, "%.2f", sessionAverage);
        std::string Percent = GetPercentFromNotesLeftAndNotes(notesLeft, NotesCount);
        if (Percent > "75")
        {
            il2cpp_utils::RunMethod(averageText.textMesh, "set_text", il2cpp_utils::createcsstr(combined7));
        }
        else
        {
            il2cpp_utils::RunMethod(averageText.textMesh, "set_text", il2cpp_utils::createcsstr(combined6));
        }
        runScores = false;
    }
}

MAKE_HOOK_OFFSETLESS(FinishScore, void, Il2CppObject* self, Il2CppObject* swingRatingCounter) {
    runScores = true;

    FinishScore(self, swingRatingCounter);
}

MAKE_HOOK_OFFSETLESS(SongStart, void, Il2CppObject* self, Il2CppObject* difficultyBeatmap, Il2CppObject* overrideEnvironmentSettings, Il2CppObject* overrideColorScheme, Il2CppObject* gameplayModifiers, Il2CppObject* playerSpecificSettings, Il2CppObject* practiceSettings, Il2CppString* backButtonText, bool useTestNoteCutSoundEffects) {
    il2cpp_utils::RunMethod(&advancedHud, playerSpecificSettings, "get_advancedHud");
    
    SongStart(self, difficultyBeatmap, overrideEnvironmentSettings, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, backButtonText, useTestNoteCutSoundEffects);
}

MAKE_HOOK_OFFSETLESS(Start, void, Il2CppObject* self) {
    songs++;
    
    if(advancedHud) {
        Il2CppObject* score = il2cpp_utils::GetFieldValue(self, "_scoreText");
        Il2CppObject* scoreTransform;
        Il2CppObject* scoreParent;

        il2cpp_utils::RunMethod(&scoreTransform, score, "get_transform");
        il2cpp_utils::RunMethod(&scoreParent, scoreTransform, "GetParent");

        averageText.text = "Average Score : 0\nAverage Accuracy : 0\nAverage Session Score : " + std::string(averageSessionFinal);
        averageText.fontSize = 12.0F;        
        averageText.parentTransform = scoreParent;
        if (_360)
        {
            averageText.sizeDelta = { -315, 120 };
        }
        else
        {
            averageText.sizeDelta = {-615, 220};
        }
        
        averageText.create();
    }

    if(songs > 1) {
        num = 0;
        sumAcc = 0;
        sum = 0;
    }
    
    Start(self);
}

MAKE_HOOK_OFFSETLESS(Get360, void, Il2CppObject* self, Il2CppObject* difficultyBeatmap, Il2CppObject* overrideEnvironmentSettings, Il2CppObject* overrideColorScheme, Il2CppObject* gameplayModifiers, Il2CppObject* playerSpecificSettings, Il2CppObject* practiceSettings, Il2CppString* backButtonText, bool useTestNoteCutSoundEffects) {
    Il2CppObject* beatmapDataObj;
    int spawnRotationEventsCount;
    il2cpp_utils::RunMethod(&beatmapDataObj, difficultyBeatmap, "get_beatmapData");
    il2cpp_utils::RunMethod(&spawnRotationEventsCount, beatmapDataObj, "get_spawnRotationEventsCount");
    if (spawnRotationEventsCount > 0) {
        log(DEBUG, "360 = true");
        _360 = true;
    }
    else {
        log(DEBUG, "360 = false");
        _360 = false;
    }
    Get360(self, difficultyBeatmap, overrideEnvironmentSettings, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, backButtonText, useTestNoteCutSoundEffects);
}

extern "C" void load() {
    log(INFO, "Hello from il2cpp_init!");
    log(INFO, "Installing hooks...");
    INSTALL_HOOK_OFFSETLESS(StartScoreCounters, il2cpp_utils::FindMethodUnsafe("", "ScoreUIController", "Start", 0));
    INSTALL_HOOK_OFFSETLESS(HandleComboBreakingEventHappened, il2cpp_utils::FindMethodUnsafe("", "ScoreController", "HandleNoteWasMissedEvent", 1));
    //INSTALL_HOOK_OFFSETLESS(StartLevelDetailButtons, il2cpp_utils::FindMethodUnsafe("", "StandardLevelDetailView", "RefreshContent", 0));
    INSTALL_HOOK_OFFSETLESS(HandleNoteWasCutEvent, il2cpp_utils::FindMethodUnsafe("", "ScoreController", "HandleNoteWasCutEvent", 2));  
    INSTALL_HOOK_OFFSETLESS(GetNotes, il2cpp_utils::FindMethodUnsafe("", "StandardLevelScenesTransitionSetupDataSO", "Init", 8)); 
    INSTALL_HOOK_OFFSETLESS(Get_SaberSpeed, il2cpp_utils::FindMethodUnsafe("", "Saber", "ManualUpdate", 0));   
    INSTALL_HOOK_OFFSETLESS(RawScore, il2cpp_utils::FindMethodUnsafe("", "ScoreModel", "RawScoreWithoutMultiplier", 4));
    INSTALL_HOOK_OFFSETLESS(FinishScore, il2cpp_utils::FindMethodUnsafe("", "CutScoreBuffer", "HandleSwingRatingCounterDidFinishEvent", 1));
    INSTALL_HOOK_OFFSETLESS(SongStart, il2cpp_utils::FindMethodUnsafe("", "StandardLevelScenesTransitionSetupDataSO", "Init", 8));
    INSTALL_HOOK_OFFSETLESS(Start, il2cpp_utils::FindMethodUnsafe("", "ScoreUIController", "Start", 0));
    INSTALL_HOOK_OFFSETLESS(Get360, il2cpp_utils::FindMethodUnsafe("", "StandardLevelScenesTransitionSetupDataSO", "Init", 8)); 
    log(INFO, "Installed all hooks!");
//    SetDefault();
}