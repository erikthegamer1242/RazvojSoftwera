echo "Deleting old lib zip: \n";
rm -rfv LoRaMesh.zip;
echo "Creating new lib zip: \n";
zip -rv "LoRaMesh.zip" "LoRaMesh.cpp" "LoRaMesh.h" "library.json";
