#include <cmath>
#include "../beatsaber-hook/shared/utils/utils.h"
#include "../beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "../beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "../beatsaber-hook/shared/customui/customui.hpp"
    CustomUI::TextObject MISSED;
    CustomUI::TextObject HIT;
    CustomUI::TextObject Notes;
    int hit = 0;
    int missed = 0;
    int NotesPassed;
    int NotesCount;
    bool _360 = false;
    //int notesLeft;
float SafeDivideScore(int total, int count)
    {
        float result = round(((double)(total)) / (count));
        return result;
    }
void UpdateNotesLeft()
    {
        NotesCount -= 1;
        il2cpp_utils::RunMethod(Notes.textMesh , "set_text", il2cpp_utils::createcsstr("\n Notes Left: " + std::to_string(NotesCount)));
    }
MAKE_HOOK_OFFSETLESS(missStart, void, Il2CppObject* self)
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
    missStart(self);
}
MAKE_HOOK_OFFSETLESS(hitStart, void, Il2CppObject* self)
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
    hitStart(self);
}
MAKE_HOOK_OFFSETLESS(HandleComboBreakingEventHappened, void, Il2CppObject* self)  {
    UpdateNotesLeft();
    missed++;   
    il2cpp_utils::RunMethod(MISSED.textMesh , "set_text", il2cpp_utils::createcsstr("\n \n \n<color=#FF6347>Missed</color> Notes: " + std::to_string(missed)));
    HandleComboBreakingEventHappened(self);
}

MAKE_HOOK_OFFSETLESS(NotesStart, void, Il2CppObject* self)
{
    
    Il2CppObject* levelName = il2cpp_utils::GetFieldValue(self, "_energyBar");
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
    if(_360)
    {
        Notes.text = std::string("<align=center>\n \n \n Notes Left: ") + std::to_string(NotesCount) + std::string("<align>");
    }
    else
    {
        Notes.text = std::string("\n Notes Left: ") + std::to_string(NotesCount);
    }
    
    
    Notes.fontSize = 15.0F;
    Notes.parentTransform = levelNameParent;
    Notes.create();
    NotesStart(self);
}

//total hit notes
MAKE_HOOK_OFFSETLESS(HandleNoteWasCutEvent, void, Il2CppObject* self, Il2CppObject* noteController, Il2CppObject* noteCutInfo)
{
    bool allIsOK;
    if (!il2cpp_utils::RunMethod(&allIsOK, noteCutInfo, "get_allIsOK")) {
        log(CRITICAL, "Failed to get_allIsOK");
    }
    if(allIsOK)
    {
        UpdateNotesLeft();
        hit++;
        il2cpp_utils::RunMethod(HIT.textMesh, "set_text", il2cpp_utils::createcsstr("\n \n \n \n All <color=#00FF00>Hit</color> Notes: " + std::to_string(hit)));
    }
    
    HandleNoteWasCutEvent(self, noteController, noteCutInfo);
}
    
MAKE_HOOK_OFFSETLESS(GetNotes, void, Il2CppObject* self, Il2CppObject* difficultyBeatmap, Il2CppObject* overrideEnvironmentSettings, Il2CppObject* overrideColorScheme, Il2CppObject* gameplayModifiers, Il2CppObject* playerSpecificSettings, Il2CppObject* practiceSettings, Il2CppString* backButtonText, bool useTestNoteCutSoundEffects) {
    Il2CppObject* beatmapData;
    il2cpp_utils::RunMethod(&beatmapData, difficultyBeatmap, "get_beatmapData");
    il2cpp_utils::RunMethod(&NotesCount, beatmapData, "get_notesCount");
    GetNotes(self, difficultyBeatmap, overrideEnvironmentSettings, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, backButtonText, useTestNoteCutSoundEffects);
}

/*MAKE_HOOK_OFFSETLESS(Get360, void, Il2CppObject* self)
{
    Il2CppObject *sceneSetupDataObj;
	Il2CppObject *difficultyBeatmapObj;
	Il2CppObject *beatmapDataObj;
	int spawnRotationEventsCount;

	if (!il2cpp_utils::GetFieldValue(&sceneSetupDataObj, self, "_sceneSetupData"))
	{
		log(DEBUG, "Failed to get sceneSetupDataObj");
	}
	if (!il2cpp_utils::RunMethod(&difficultyBeatmapObj, sceneSetupDataObj, "get_difficultyBeatmap"))
	{
		log(DEBUG, "Failed to get difficultyBeatmapObj");
	}
	if (!il2cpp_utils::RunMethod(&beatmapDataObj, difficultyBeatmapObj, "get_beatmapData"))
	{
		log(DEBUG, "Failed to get beatmapDataObj");
	}
	if (!il2cpp_utils::RunMethod(&spawnRotationEventsCount, beatmapDataObj, "get_spawnRotationEventsCount"))
	{
		log(DEBUG, "Failed to get spawnRotationEventsCount");
	}
    if (spawnRotationEventsCount > 0)
	{
		_360 = true;
	}
	else
	{
        _360 = false;z
    Get360(self);
}*/



extern "C" void load() {
    log(INFO, "Hello from il2cpp_init!");
    log(INFO, "Installing hooks...");
    INSTALL_HOOK_OFFSETLESS(hitStart, il2cpp_utils::FindMethodUnsafe("", "ScoreUIController", "Start", 0));   
    INSTALL_HOOK_OFFSETLESS(missStart, il2cpp_utils::FindMethodUnsafe("", "ScoreUIController", "Start", 0));    
    INSTALL_HOOK_OFFSETLESS(HandleComboBreakingEventHappened, il2cpp_utils::FindMethodUnsafe("", "ComboUIController", "HandleComboBreakingEventHappened", 0));
    INSTALL_HOOK_OFFSETLESS(NotesStart, il2cpp_utils::FindMethodUnsafe("", "GameEnergyUIPanel", "Start", 0));   
    INSTALL_HOOK_OFFSETLESS(HandleNoteWasCutEvent, il2cpp_utils::FindMethodUnsafe("", "ScoreController", "HandleNoteWasCutEvent", 2));  
    INSTALL_HOOK_OFFSETLESS(GetNotes, il2cpp_utils::FindMethodUnsafe("", "StandardLevelScenesTransitionSetupDataSO", "Init", 8)); 
    //INSTALL_HOOK_OFFSETLESS(Get360, il2cpp_utils::FindMethodUnsafe("", "GameCoreSceneSetup", "InstallBindings", 0)); 
    log(INFO, "Installed all hooks!");
}
