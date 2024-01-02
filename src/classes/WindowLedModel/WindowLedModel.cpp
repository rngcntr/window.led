#include "Arduino.h"
#include "WindowLedModel.h"

WindowLedState::WindowLedState(int warmKelvin, int numWarmPins, int* warmPins, int coldKelvin, int numColdPins, int* coldPins) {
  _warmKelvin = warmKelvin;
  _coldKelvin = coldKelvin;
  _numWarmPins = numWarmPins;
  _numColdPins = numColdPins;
  _warmPins = warmPins;
  _coldPins = coldPins;
  _colorTemperature = (_warmKelvin + _coldKelvin) / 2;
  _currentWarm = 0;
  _currentCold = 0;
}

void WindowLedState::begin(int pwmRange) {
  for (int i = 0; i < _numWarmPins; ++i) {
    pinMode(_warmPins[i], OUTPUT);
  }
  for (int i = 0; i < _numColdPins; ++i) {
    pinMode(_coldPins[i], OUTPUT);
  }
  _pwmRange = pwmRange;
  analogWriteRange(pwmRange - 1);
  write();
}

void WindowLedState::determineNewColorTemperature() {
  if (getOverallBrightness() == 0) {
    return;
  } else {
    int temp = _warmKelvin * _currentWarm;
    temp += _coldKelvin * _currentCold;
    _colorTemperature = temp / getOverallBrightness();
  }
}

int WindowLedState::getColorTemperature() {
  return _colorTemperature;
}

int WindowLedState::getOverallBrightness() {
  return _currentWarm + _currentCold;
}

int WindowLedState::getWarmBrightness() {
  return _currentWarm;
}

int WindowLedState::getColdBrightness() {
  return _currentCold;
}

bool WindowLedState::isOn() {
  return _on;
}

void WindowLedState::setOn(bool on) {
  _on = on;
}

void WindowLedState::toggle() {
  _on = !_on;
}

void WindowLedState::setDirect(int newWarmBrightness, int newColdBrightness) {
  _currentWarm = newWarmBrightness;
  _currentCold = newColdBrightness;
  determineNewColorTemperature();
}

void WindowLedState::setIndirect(int newOverallBrightness, double newColorTemperature) {
    _colorTemperature = newColorTemperature;
    // temp = (a * warm_kelvin + b * cold_kelvin) / (a+b) where (a+b) = overallBrightness
    _currentWarm = -(newOverallBrightness * (newColorTemperature - _coldKelvin)) / (_coldKelvin - _warmKelvin);
    _currentCold = newOverallBrightness - _currentWarm;
    write();
}

void WindowLedState::write() {
  for (int i = 0; i < _numWarmPins; ++i) {
    analogWrite(_warmPins[i], _pwmRange - 1 - ((_on ? _currentWarm : 0) + i) / _numWarmPins);
  }
  for (int i = 0; i < _numColdPins; ++i) {
    analogWrite(_coldPins[i], _pwmRange - 1 - ((_on ? _currentCold : 0) + i) / _numColdPins);
  }
}
