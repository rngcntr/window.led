#pragma once
#include "LinkedList.h"

struct Metric {
    String NameWithLabels;
    double Value;
};
typedef std::function<Metric(void)> TMetricGetter;

class Prometheus
{
  private:
    ESP8266WebServer* _server;
    LinkedList<TMetricGetter> _metrics;
    void renderMetrics();
  public:
    Prometheus();
    void begin(ESP8266WebServer& server);
    void configureMetric(TMetricGetter metric);
};

