#!/usr/bin/env bash

#Clear old docs
rm -rf html/ xml/ latex/ LoRaMesh.pdf README.md

doxygen #Generate HTML, LaTex and XML documentation
cd latex/
make #Create PDF from LaTex
mv refman.pdf ../LoRaMesh.pdf
cd ..
cd xml
moxygen --output README.md . #Create markdown from XML
mv README.md ../
