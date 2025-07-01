#pragma once

#include "scene/gui/panel_container.h"

class KudouDock : public PanelContainer {
    GDCLASS(KudouDock, PanelContainer);

public:
    KudouDock();

protected:
    void _notification(int p_what);
};
