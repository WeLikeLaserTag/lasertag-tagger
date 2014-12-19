#include <Gun.h>

#include <twi.h>
#include <Wire.h>
#include <Health.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int TYPE_HIT = 1;
int lastGunState = 0;


Gun gun;
Health health;

bool printReloadingState() {

    display.setCursor(0, 20);

    // is the gun reloading?
    if (gun.isReloading()) {
        display.print("Reloading");
        return true;
    }

    return false;
}

void printAmmo() {
    // print ammo to lcd

    int bullets = gun.getBulletsInMag();

    if (printReloadingState()) {
        return;
    }

    display.setTextSize(5);
    display.setCursor(10, 20);

    if (bullets < 10) {
        display.print(" ");
    }

    display.print(bullets, DEC);

    display.setTextSize(2);
    display.print("/");
    display.print(gun.getMaxAmmoInMag(), DEC);
}

/**
* Prints the current hitpoints of the player.
* Warning: The maximum HP can't be over 999
*/
void printHealth() {

    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print("HP:");

    int hp = health.getHealth();

    if (hp < 10) {
        display.print("  ");
        display.print(hp);
        return;
    }

    if (hp < 100) {
        display.print(" ");
        display.print(hp);
        return;
    }

    display.print(hp);
}

void setupLcd() {

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextColor(WHITE);

    display.display();

    //    lcd.print("Mag:");
//    lcd.setCursor(13, 0);
//    lcd.print("HP:");
}

void receiveEvent(int howMany) {
    int type = Wire.read();
    if (type == TYPE_HIT) {
        int playerId = Wire.read();
        int teamId = Wire.read();
        int damage = Wire.read();
        int critical = Wire.read();
        health.takeDamage(playerId, teamId, damage, critical);
        return;
    }
}

void setup() {

    Serial.begin(9600);
    Wire.begin(1);
    Wire.onReceive(receiveEvent);

    setupLcd();
}

void loop() {
    gun.tick();

    display.clearDisplay();

    printAmmo();
    printHealth();

    display.display();

    lastGunState = gun.getCurrentState();
}

