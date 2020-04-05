#include "Start.hpp"
void StartNotesLeftCounter(Il2CppObject* self)
{
    notesLeft = NotesCount;
    Il2CppObject* levelName = il2cpp_utils::GetFieldValue(self, "_scoreText");
    Il2CppObject* levelNameTransform;

    Il2CppObject* levelNameParent;
    if (il2cpp_utils::RunMethod(&levelNameTransform, levelName, "get_transform"))
    {
        log(DEBUG, "oopsies sorry sister! 5");
    }
    if (il2cpp_utils::RunMethod(&levelNameParent, levelNameTransform, "GetParent"))
    {
        log(DEBUG, "oopsies sorry sister! 6");
    }
    
    Notes.fontSize = 13.0F;
    if (!_360)
    {
        Notes.sizeDelta = { -640, -20 };
    }
    else
    {
        Notes.sizeDelta = { 0, -20 };
    }
    
    Notes.parentTransform = levelNameParent;
    Notes.create();
    Notes.set("\nNotes Left: " + std::to_string(notesLeft) + " (" + GetPercentFromNotesLeftAndNotes(notesLeft, NotesCount) + "%)");
}
void StartMissCounter(Il2CppObject* self)
{
    //text
    missed = 0;
    Il2CppObject* levelName = il2cpp_utils::GetFieldValue(self, "_scoreText");
    Il2CppObject* levelNameTransform;
    Il2CppObject* levelNameParent;
    if (il2cpp_utils::RunMethod(&levelNameTransform, levelName, "get_transform"))
    {
        log(DEBUG, "oopsies sorry sister! 3");
    }
    if (il2cpp_utils::RunMethod(&levelNameParent, levelNameTransform, "GetParent"))
    {
        log(DEBUG, "oopsies sorry sister! 4");
    }
    MISSED.fontSize = 12.0F;
    MISSED.parentTransform = levelNameParent;
    
    if(!_360)
    {
    MISSED.text = "\n \n \n<color=#FF6347>Missed</color> Notes: 0";
    }
    else
    {
        MISSED.text = std::string("\n \n<color=#FF6347>Missed</color> Notes: 0") + std::string(" All <color=#00FF00>Hit</color> Notes: 0");
        MISSED.sizeDelta = {5, -6};
    }
    MISSED.create();
    
}
void StartAverageSpeedCounter(Il2CppObject* self)
{
    Il2CppObject* levelName = il2cpp_utils::GetFieldValue(self, "_scoreText");
    Il2CppObject* levelNameTransform;
    Il2CppObject* levelNameParent;
    if (il2cpp_utils::RunMethod(&levelNameTransform, levelName, "get_transform"))
    {
        log(DEBUG, "oopsies sorry sister! 5");
    }
    if (il2cpp_utils::RunMethod(&levelNameParent, levelNameTransform, "GetParent"))
    {
        log(DEBUG, "oopsies sorry sister! 6");
    }
    LeftSaberSwingSpeed.text = "";
    LeftSaberSwingSpeed.fontSize = 12.0F;
    if (!_360)
    {
        LeftSaberSwingSpeed.sizeDelta = {0, -40};
    }
    else
    {
        LeftSaberSwingSpeed.sizeDelta = { 0, -30 };
    }
    
    LeftSaberSwingSpeed.parentTransform = levelNameParent;
    LeftSaberSwingSpeed.create();
}
void StartHitCounter(Il2CppObject* self)
{

    hit = 0;
    if(!_360)
    { 
    Il2CppObject* levelName = il2cpp_utils::GetFieldValue(self, "_scoreText");
    Il2CppObject* levelNameTransform;
    Il2CppObject* levelNameParent;
    if (il2cpp_utils::RunMethod(&levelNameTransform, levelName, "get_transform"))
    {
        log(DEBUG, "oopsies sorry sister! 5");
    }
    if (il2cpp_utils::RunMethod(&levelNameParent, levelNameTransform, "GetParent"))
    {
        log(DEBUG, "oopsies sorry sister! 6");
    }
    HIT.text = "\n\n\n\nAll <color=#00FF00>Hit</color> Notes: 0";
    HIT.fontSize = 12.0F;
    HIT.parentTransform = levelNameParent;
    HIT.sizeDelta = {0, 14.5f};
    HIT.create();
    }
    }
    void StartImage(Il2CppObject * self)
    {
        il2cpp_utils::RunMethod(il2cpp_utils::GetClassFromName("UnityEngine", "Object"), "Destroy", Image.gameObj);
        Il2CppObject* levelName;
        il2cpp_utils::RunMethod(&levelName, self, "get_playButton");
        Il2CppObject* levelNameTransform;
        Il2CppObject* levelNameParent;
        if (il2cpp_utils::RunMethod(&levelNameTransform, levelName, "get_transform"))
        {
            log(DEBUG, "oopsies sorry sister! 5");
        }
        if (il2cpp_utils::RunMethod(&levelNameParent, levelNameTransform, "GetParent"))
        {
            log(DEBUG, "oopsies sorry sister! 6");
        }
        Image.url = "https://cdn.discordapp.com/attachments/652678217177497611/695384224512999504/Untitled.png";
        Image.parentTransform = levelNameParent;
        Image.create();
    }