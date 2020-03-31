#include "Start.hpp"
void StartNotesLeftCounter(Il2CppObject* self)
{
    notesLeft = NotesCount;
    Il2CppObject* levelName = il2cpp_utils::GetFieldValue(self, "_energyBar");
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
    Notes.text = std::string("<align=center>\n Notes Left: ") + std::to_string(NotesCount) + std::string("</align>");
    Notes.fontSize = 15.0F;
    Notes.parentTransform = levelNameParent;
    Notes.create();
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
        MISSED.text = std::string("\n \n \n<color=#FF6347>Missed</color> Notes: 0") + std::string("All <color=#00FF00>Hit</color> Notes: 0");
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
    LeftSaberSwingSpeed.text = "\n\n\n\n\n<color=#FF0000>Left</color> Saber Speed: " + LeftSpeed;
    LeftSaberSwingSpeed.fontSize = 12.0F;
    LeftSaberSwingSpeed.parentTransform = levelNameParent;
    LeftSaberSwingSpeed.create();
    RightSaberSwingSpeed.text = "\n\n\n\n\n\n<color=#0000FF>Right</color> Saber Speed: " + RightSpeed;
    RightSaberSwingSpeed.fontSize = 12.0F;
    RightSaberSwingSpeed.parentTransform = levelNameParent;
    RightSaberSwingSpeed.create();
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
        log(DEBUG,"oopsies sorry sister! 5");
    }
    if (il2cpp_utils::RunMethod(&levelNameParent, levelNameTransform, "GetParent"))
    {
        log(DEBUG,"oopsies sorry sister! 6");
    }
    HIT.text = "\n \n \n \n All <color=#00FF00>Hit</color> Notes: 0";
    HIT.fontSize = 12.0F;
    HIT.parentTransform = levelNameParent;
    HIT.create();
    }
}