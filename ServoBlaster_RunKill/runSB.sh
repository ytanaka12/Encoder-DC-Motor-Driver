#!/bin/sh
sudo servod --p1pins=7,16,18 --min=10us --max=5000us --step-size=2us \
--cycle-time=5000us

