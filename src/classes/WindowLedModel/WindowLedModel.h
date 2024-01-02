#pragma once

class WindowLedState {
  public:
    WindowLedState(int warmKelvin, int numWarmPins, int* warmPins, int coldKelvin, int numColdPins, int* coldPins);
    void begin(int pwmRange);
    int getColorTemperature();
    int getOverallBrightness();
    int getWarmBrightness();
    int getColdBrightness();
    void setDirect(int newWarmBrightness, int newColdBrightness);
    void setIndirect(int newOverallBrightness, double newColorTemperature);
    bool isOn();
    void setOn(bool on);
    void toggle();
    void write();
  private:
    void determineNewColorTemperature();
    bool _on = false;
    int _pwmRange;
    int _warmKelvin;
    int _coldKelvin;
    int _numWarmPins;
    int _numColdPins;
    int* _warmPins;
    int* _coldPins;
    int _currentWarm;
    int _currentCold;
    int _colorTemperature;
};

