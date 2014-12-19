#include "Arduino.h"
#include "IRremote.h"

#include "Gun.h"
#include "gun_config.h"

Gun::Gun() {
    _currentAmmoInMag = AMMO_IN_MAG;
    _buttonState = 0;
    _reloadUntil = 0;
    _timeNextBulletInChamber = 0;
    _time = 0;
    _state = STATE_INITIAL;

    setState(STATE_READY_TO_FIRE);
    
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void Gun::setState(int state) {
    _lastState = _state;
    _state = state;
}

int Gun::getCurrentState() {
    return _state;
}


int Gun::getLastState() {
    return _lastState;
}

void Gun::tick() {
    checkReload();
    checkHitIndicator();
    // read the state of the pushbutton value:
    _buttonState = !digitalRead(BUTTON_PIN);

    // check if the pushbutton is pressed.
    // if it is, the _buttonState is HIGH:
    if (_buttonState == HIGH) {
        shoot();
    }
}

bool Gun::isReloading() {
    return getCurrentState() == STATE_RELOADING;
}

void Gun::checkReload() {
    if(getCurrentState() == STATE_RELOADING && millis() > _reloadUntil) {
        setState(STATE_READY_TO_FIRE);
        _currentAmmoInMag = AMMO_IN_MAG;
        playSound(2);
    }
}

bool Gun::shoot() {
    if(getCurrentState() != STATE_READY_TO_FIRE || checkBulletInChamber() != true) {
        return false;
    }

//    unsigned int data[3] = {1, 2, 3};
//    _irsend.sendRaw(data, 3, 58);

    // actual shot
    _irsend.sendSony(0x68B92, 20);
    hitIndicatorOn();

    // remove bullet from mag
    _currentAmmoInMag = _currentAmmoInMag - 1;

    // set timer for next bullet in chamber
    _timeNextBulletInChamber = millis() + TIME_BETWEEN_SHOTS_IN_MS;

    if(_currentAmmoInMag <= 0) {
        // do reload
        reload();
     } else {
        playSound(0);
    }

    return true;
}

/**
* Plays a sound
*
* Sound types:
* type 0: shot
* type 1: reloading start
* type 2: reloading stop
*/
void Gun::playSound(int type) {
    switch(type) {
        case 0:
            tone(4, 262, 50); // c4
            break;
        case 1:
        case 2:
            tone(4, 523, 50); // c5
            break;
    }
}

int Gun::getBulletsInMag() {
    return _currentAmmoInMag;
}

int Gun::getMaxAmmoInMag() {
    return AMMO_IN_MAG;
}

bool Gun::checkBulletInChamber() {
    if(millis() > _timeNextBulletInChamber) {
        return true;
    }
    return false;
}

void Gun::reload() {
    playSound(1);
    _reloadUntil = millis() + RELOAD_TIME_IN_MS;
    setState(STATE_RELOADING);
}

void Gun::hitIndicatorOn() {
    _time = millis() + 50;
}

void Gun::hitIndicatorOff() {
}

void Gun::checkHitIndicator() {
    if(millis() > _time && getCurrentState() == STATE_READY_TO_FIRE) {
        hitIndicatorOff();
    }
}
