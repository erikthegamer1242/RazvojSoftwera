#!/usr/bin/env bash

doxygen #Generate HTML, LaTex and XML documentation
cd latex/
make #Create PDF from LaTex
mv refman.pdf ../LoRaMesh.pdf
cd ..
cd xml
moxygen --output API.md . #Create markdown from XML
mv API.md ../
