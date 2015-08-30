if [ "$1" = "install" ] 
then
mkdir /usr/include/libzorbaneural
cp neuralnet.cpp /usr/include/libzorbaneural/neuralnet.cpp
cp neuralnet.h /usr/include/libzorbaneural/neuralnet.h
cp neuralnet_global.h /usr/include/libzorbaneural/neuralnet_global.h
chmod 644 /usr/include/libzorbaneural/neuralnet.cpp
chmod 644 /usr/include/libzorbaneural/neuralnet.h
chmod 644 /usr/include/libzorbaneural/neuralnet_global.h
fi

if [ "$1" = "uninstall" ] 
then
rm /usr/include/libzorbaneural/neuralnet.cpp
rm /usr/include/libzorbaneural/neuralnet.h
rm /usr/include/libzorbaneural/neuralnet_global.h
rmdir /usr/include/libzorbaneural
fi