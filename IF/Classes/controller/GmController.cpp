//
//  GmController.cpp
//  IF
//
//  Created by xxrdsg on 15-2-27.
//
//

#include "GmController.h"

static GmController* _gmInstance = NULL;

GmController* GmController::getInstance()
{
    if (_gmInstance == NULL) {
        _gmInstance = new GmController();
    }
    return _gmInstance;
}