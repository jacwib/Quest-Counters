#include "../beatsaber-hook/shared/utils/utils.h"
#include "../beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "../beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "../beatsaber-hook/shared/customui/customui.hpp"
    CustomUI::TextObject MISSED;
    CustomUI::TextObject HIT;
    int hit = 0;
    int missed = 0;

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
MAKE_HOOK_OFFSETLESS(Start, void, Il2CppObject* self) {
    missStart(self);
    hitStart(self);
    Start(self);
}

//total missed notes
MAKE_HOOK_OFFSETLESS(HandleComboBreakingEventHappened, void, Il2CppObject* self)  {
    missed++;   
    MISSED.text = "\n \n \n<color=#FF6347>Missed</color> Notes: " + std::to_string(missed) ;
    HandleComboBreakingEventHappened(self);
}


//total hit notes
MAKE_HOOK_OFFSETLESS(HandleNoteWasCutEvent, void, Il2CppObject* self, Il2CppObject* noteSpawnController, Il2CppObject* noteController, Il2CppObject* noteCutInfo)
{
    hit++;
    HIT.text = "\n \n \n \n All <color=#00FF00>Hit</color> Notes: " + std::to_string(hit);
     HandleNoteWasCutEvent(self, noteSpawnController, noteController, noteCutInfo);
}
    
extern "C" void load() {
    log(INFO, "Hello from il2cpp_init!");
    log(INFO, "Installing hooks...");
    INSTALL_HOOK_OFFSETLESS(Start, il2cpp_utils::FindMethodUnsafe("", "ScoreUIController", "Start", 0));
    INSTALL_HOOK_OFFSETLESS(HandleComboBreakingEventHappened, il2cpp_utils::FindMethodUnsafe("", "ComboUIController", "HandleComboBreakingEventHappened", 0));
    INSTALL_HOOK_OFFSETLESS(HandleNoteWasCutEvent, il2cpp_utils::FindMethodUnsafe("", "ScoreController", "HandleNoteWasCutEvent", 3));   
    log(INFO, "Installed all hooks!");
}
