#include "../beatsaber-hook/shared/utils/utils.h"
#include "../beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "../beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "../beatsaber-hook/shared/customui/customui.hpp"
    CustomUI::TextObject MISSED;
    CustomUI::TextObject HIT;
    CustomUI::TextObject ACC;
    int hit = 0;
    int missed = 0;
    int notes;
    int NotesLeft;
int GetNotesCountDifficultyFromBeatmap(Il2CppObject* beatmap) {
    int notesCount;
    il2cpp_utils::RunMethod(&notesCount, beatmap, "get_notesCount");
    log(DEBUG, "Got notesCount: %d", notesCount);
    return notesCount;
}
void accStart(Il2CppObject* self)
{
    Il2CppObject* levelName = il2cpp_utils::GetFieldValue(self, "_scoreText");
    Il2CppObject* levelNameTransform;
    Il2CppObject* levelNameParent;
    if(il2cpp_utils::RunMethod(&levelNameTransform, levelName, "get_transform"))
    {
        log(DEBUG,"oopsies sorry sister! 1");
    }
    if(il2cpp_utils::RunMethod(&levelNameParent, levelNameTransform, "GetParent"))
    {
        log(DEBUG,"oopsies sorry sister! 2");
    }
    ACC.text = "\n \n \n \n \n "+ std::to_string(notes);
    ACC.fontSize = 12.0F;
    ACC.parentTransform = levelNameParent;
    ACC.create();
}
void missStart(Il2CppObject* self)
{
    //text
    missed = 0;
    Il2CppObject* levelName = il2cpp_utils::GetFieldValue(self, "_scoreText");
    Il2CppObject* levelNameTransform;
    Il2CppObject* levelNameParent;
    if(il2cpp_utils::RunMethod(&levelNameTransform, levelName, "get_transform"))
    {
        log(DEBUG,"oopsies sorry sister! 3");
    }
    if(il2cpp_utils::RunMethod(&levelNameParent, levelNameTransform, "GetParent"))
    {
        log(DEBUG,"oopsies sorry sister! 4");
    }
     MISSED.text = "\n \n \n<color=#FF6347>Missed</color> Notes: ";
    MISSED.fontSize = 12.0F;        
    MISSED.parentTransform = levelNameParent;
    MISSED.create();
}
 void hitStart(Il2CppObject* self)
{
    hit = 0;
    Il2CppObject* levelName = il2cpp_utils::GetFieldValue(self, "_scoreText");
    Il2CppObject* levelNameTransform;
    Il2CppObject* levelNameParent;
    if(il2cpp_utils::RunMethod(&levelNameTransform, levelName, "get_transform"))
    {
        log(DEBUG,"oopsies sorry sister! 5");
    }
    if(il2cpp_utils::RunMethod(&levelNameParent, levelNameTransform, "GetParent"))
    {
        log(DEBUG,"oopsies sorry sister! 6");
    }
    HIT.text = "\n \n \n \n All <color=#00FF00>Hit</color> Notes: ";
    HIT.fontSize = 12.0F;
    HIT.parentTransform = levelNameParent;
    HIT.create();
}

    //Song Score UI
MAKE_HOOK_OFFSETLESS(MissedStart, void, Il2CppObject* self) {
    missStart(self);
    hitStart(self);
    accStart(self);
}
MAKE_HOOK_OFFSETLESS(SongStart, void, Il2CppObject* self, Il2CppObject* difficultyBeatmap, Il2CppObject* overrideEnvironmentSettings, Il2CppObject* overrideColorScheme, Il2CppObject* gameplayModifiers, Il2CppObject* playerSpecificSettings, Il2CppObject* practiceSettings, Il2CppString* backButtonText, bool useTestNoteCutSoundEffects) {

    notes =  GetNotesCountDifficultyFromBeatmap(il2cpp_utils::GetFieldValue(difficultyBeatmap, "beatmapData"));
    //il2cpp_utils::RunMethod(&notes, il2cpp_utils::GetFieldValue(difficultyBeatmap, "beatmapData"), "get_notesCount");
    SongStart(self, difficultyBeatmap, overrideEnvironmentSettings, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, backButtonText, useTestNoteCutSoundEffects);
}


//total missed notes
MAKE_HOOK_OFFSETLESS(SendNoteWasMissedEvent, void, Il2CppObject* self)  {
    NotesLeft--;
    missed++;   
    MISSED.text = "\n \n \n<color=#FF6347>Missed</color> Notes: " + std::to_string(missed) ;
    SendNoteWasMissedEvent(self);
}


//total hit notes
MAKE_HOOK_OFFSETLESS(HandleNoteWasCut, void, Il2CppObject* self,Il2CppObject* noteController, Il2CppObject* noteCutInfo)
{
    NotesLeft--;
    HandleNoteWasCut(self, noteController, noteCutInfo);
    hit++;
    HIT.text = "\n \n \n \n All <color=#00FF00>Hit</color> Notes: " + std::to_string(hit);
}
    
extern "C" void load() {
    log(INFO, "Hello from il2cpp_init!");
    log(INFO, "Installing hooks...");
    INSTALL_HOOK_OFFSETLESS(MissedStart, il2cpp_utils::FindMethodUnsafe("", "ScoreUIController", "Start", 0));
    INSTALL_HOOK_OFFSETLESS(SendNoteWasMissedEvent, il2cpp_utils::FindMethodUnsafe("", "NoteController", "SendNoteWasMissedEvent", 0));
    INSTALL_HOOK_OFFSETLESS(HandleNoteWasCut, il2cpp_utils::FindMethodUnsafe("", "BeatmapObjectSpawnController", "HandleNoteWasCut", 2));
     INSTALL_HOOK_OFFSETLESS(SongStart, il2cpp_utils::FindMethodUnsafe("", "StandardLevelScenesTransitionSetupDataSO", "Init", 8));


    log(INFO, "Installed all hooks!");
}
