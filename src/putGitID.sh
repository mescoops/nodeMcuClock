#!/bin/bash
echo "document.getElementById(\"clock_software_id\").textContent=\"`git rev-parse --short HEAD`\";" > data/revision.js

