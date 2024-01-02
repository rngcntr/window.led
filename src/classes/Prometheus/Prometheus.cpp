#include "ESP8266WebServer.h"
#include "Prometheus.h"
#include "LinkedList.h"

Prometheus::Prometheus() {
}

void Prometheus::begin(ESP8266WebServer& server) {
    this->_server = &server;
    this->_server->on("/metrics", std::bind(&Prometheus::renderMetrics, this));
}

void Prometheus::configureMetric(TMetricGetter metric) {
    this->_metrics.add(metric);
}

void Prometheus::renderMetrics() {
    String response = "";
    for (int i = 0; i < this->_metrics.size(); i++) {
        Metric observation = this->_metrics.get(i)();
        response += observation.NameWithLabels;
        response += " ";
        response += String(observation.Value);
        response += "\n";
    }
    this->_server->send(200, "text/plain", response);
}

