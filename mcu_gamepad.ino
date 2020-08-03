#include "config.h"

#define BOUNCE_WITH_PROMPT_DETECTION
#include <Bounce2.h>
#include <Metro.h>

class MyButton {
  public:
    MyButton(unsigned char pin, unsigned char buttonId) : pin(pin), buttonId(buttonId) {}

    void setup() {
      this->button = Bounce();
      this->button.attach(this->pin, INPUT_PULLUP);
      this->button.interval(DEBOUNCE_INTERVAL);
    }

    bool process(bool force) {
      this->button.update();
      if (this->button.changed() || force) {
        Joystick.button(this->buttonId, !this->button.read());
        return true;
      } else {
        return false;
      }
    }

  protected:
    unsigned char pin;
    unsigned char buttonId;
    Bounce button;
};

class MyDirButton {
  protected:
    MyDirButton(unsigned char negPin, unsigned char posPin, unsigned char negButtonId, unsigned char posButtonId)
      : negPin(negPin), posPin(posPin), negButtonId(negButtonId), posButtonId(posButtonId), dir(0) {}

  public:
    virtual void setup() {
      this->negButton = Bounce();
      this->negButton.attach(this->negPin, INPUT_PULLUP);
      this->negButton.interval(DEBOUNCE_INTERVAL);
      this->posButton = Bounce();
      this->posButton.attach(this->posPin, INPUT_PULLUP);
      this->posButton.interval(DEBOUNCE_INTERVAL);
    }

    virtual bool process(bool force) {
      this->negButton.update();
      this->posButton.update();

      int newDir = dir;

      if (force || this->negButton.changed()) {
        Joystick.button(this->negButtonId, !this->negButton.read());

        if (!this->negButton.read()) {
          newDir = -1;
        } else {
          if (!this->posButton.read()) {
            newDir = 1;
          } else {
            newDir = 0;
          }
        }
      }

      if (force || this->posButton.changed()) {
        Joystick.button(this->posButtonId, !this->posButton.read());
        if (!this->posButton.read()) {
          newDir = 1;
        } else {
          if (!this->negButton.read()) {
            newDir = -1;
          } else {
            newDir = 0;
          }
        }
      }

      if (force || newDir != dir) {
        dir = newDir;
        // On the X/Y axis, left/down is 0, center is 512 and right/up is 1023
        int value = ((1 + dir) * 1023) / 2;
        this->setDir(value);
      }

      return this->negButton.changed() || this->posButton.changed() || force;
    }

  protected:
    virtual void setDir(unsigned int value) = 0;

    unsigned char negPin;
    unsigned char posPin;
    unsigned char negButtonId;
    unsigned char posButtonId;
    Bounce negButton;
    Bounce posButton;
    int dir;
};

class MyDirButtonX : public MyDirButton {
  public:
    MyDirButtonX(unsigned char pinNeg, unsigned char pinPos) : MyDirButton(pinNeg, pinPos, BUTTON_ID_L, BUTTON_ID_R) {}

  protected:
    virtual void setDir(unsigned int value) {
      Joystick.X(value);
    }
};

class MyDirButtonY : public MyDirButton {
  public:
    MyDirButtonY(unsigned char pinNeg, unsigned char pinPos) : MyDirButton(pinNeg, pinPos, BUTTON_ID_U, BUTTON_ID_D) {}

  protected:
    virtual void setDir(unsigned int value) {
      Joystick.Y(value);
    }
};

MyButton buttons[] = {
  MyButton(PIN_A, BUTTON_ID_A),
  MyButton(PIN_B, BUTTON_ID_B),
  MyButton(PIN_X, BUTTON_ID_X),
  MyButton(PIN_Y, BUTTON_ID_Y)
};
int numButtons = sizeof(buttons) / sizeof(MyButton);

// need pointer types here because MyDirButton is polymorphic
MyDirButton *dirButtonX = new MyDirButtonX(PIN_L, PIN_R);
MyDirButton *dirButtonY = new MyDirButtonY(PIN_D, PIN_U);

Metro refreshTimer = Metro(250);
bool refresh = true;

void setupButtons() {
  for (int i = 0; i < numButtons; ++i) {
    buttons[i].setup();
  }
  dirButtonX->setup();
  dirButtonY->setup();
}

bool processButtons(bool force) {
  bool hasChanged = false;
  for (int i = 0; i < numButtons; ++i) {
    hasChanged = hasChanged || buttons[i].process(force);
  }
  hasChanged = hasChanged || dirButtonX->process(force);
  hasChanged = hasChanged || dirButtonY->process(force);
  return hasChanged;
}

void setup() {
  // init the gamepad states
  setupButtons();
  processButtons(true);

  // Send gamepad events in a batch such that simoultaneous button presses/releases
  // are sent together
  Joystick.useManualSend(true);
}

void loop() {
  refresh = refresh || refreshTimer.check();
  if (processButtons(refresh)) {
    Joystick.send_now();
  }
  refresh = false;
}
