void setupPrometheus() {
  prometheus.begin(server);
  prometheus.configureMetric(getInfo);
  prometheus.configureMetric(getStatus);
  prometheus.configureMetric(getUptime);
  prometheus.configureMetric(getSignalStrength);
  prometheus.configureMetric(getFreeHeap);
  prometheus.configureMetric(getColdBrightness);
  prometheus.configureMetric(getWarmBrightness);
  prometheus.configureMetric(getColorTemperature);
}

Metric getInfo() {
  String metric = String("window_led_up{") +
    "chipId=\"" + ESP.getChipId() + "\"," +
    "ip=\"" + WiFi.localIP().toString() + "\"," +
    "gw=\"" + WiFi.gatewayIP().toString() + "\"," +
    "nm=\"" + WiFi.subnetMask().toString() + "\"," +
    "flashChipId=\"" + ESP.getFlashChipId() + "\"," +
    "flashChipBytes=\"" + ESP.getFlashChipSize() + "\"," +
    "flashChipRealBytes=\"" + ESP.getFlashChipRealSize() + "\"}";
  return {metric, 1};
}

Metric getStatus() {
  String metric = String("window_led_on{chipId=\"") + ESP.getChipId() + "\"}";
  return {metric, ledState.isOn() ? 1 : 0};
}

Metric getSignalStrength() {
  String metric = String("window_led_signal_strength{chipId=\"") + ESP.getChipId() + "\"}";
  return {metric, WiFi.RSSI()};
}

Metric getFreeHeap() {
  String metric = String("window_led_free_heap_bytes{chipId=\"") + ESP.getChipId() + "\"}";
  return {metric, ESP.getFreeHeap()};
}

Metric getUptime() {
  String metric = String("window_led_uptime_ms{chipId=\"") + ESP.getChipId() + "\"}";
  return {metric, millis()};
}

Metric getWarmBrightness() {
  String metric = String("window_led_brightness{chipId=\"") + ESP.getChipId() + "\",cct=\"warm\"}";
  return {metric, ledState.getWarmBrightness()};
}

Metric getColdBrightness() {
  String metric = String("window_led_brightness{chipId=\"") + ESP.getChipId() + "\",cct=\"cold\"}";
  return {metric, ledState.getColdBrightness()};
}

Metric getColorTemperature() {
  String metric = String("window_led_color_temperature{chipId=\"") + ESP.getChipId() + "\"}";
  return {metric, ledState.getColorTemperature()};
}
