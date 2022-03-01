export IDF_PATH=$(pwd)/dep/esp-idf
. ./dep/esp-idf/export.sh
mkdir build_esp32
cd build_esp32
cmake ../my_app/
