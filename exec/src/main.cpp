#include "Application.h"
#include <Engine/Header/GameManager.h>


int main()
{
    Application app;
    app.StartUp(false, AttractorType::None);
    app.ClearUp();

    return 0;
}