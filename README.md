# Kartesio
Program for calculating best fit curves for a set of experimental points using regression algorithms and neural networks.

This repository contains the source code of Kartesio and ZorbaNeuralNetwork. <br>
If you are looking for ready to use binary packages, please take a look at https://github.com/zorbaproject/kartesio-build


<h4>
BUILDING:
</h4>
To build Kartesio you just need QT-SDK. If you use Windows you can download it from https://www.qt.io/download-open-source/.
If you use GNU/Linux Debian-Ubuntu, instead, you can just run this command:
<br><i>
sudo apt-get install qt-sdk qtscript5-dev
</i> <br>
To use Kartesio regression feature you'll need to install Maxima on your computer. If you use Windows, download one of these packages: http://sourceforge.net/projects/maxima/files/Maxima-Windows/ and at the first start of Kartesio tell it the position of the file maxima.bat
If you use GNU/Linux Debian-Ubuntu, use this command:
<br><i>
sudo apt-get install maxima
</i> <br>


<h4>
DEBIAN PACKAGING:
</h4>
To create a Debian package of Kartesio, just enter the source code directory (named for example "kartesio-1.0") and run the following commands:
<br><i>
dh_make -e TRINGALINVENT@libero.it -s --createorig
<br>
debuild -S -sa
<br>
debuild binary
</i> <br>
You'll find the binary package for your architecture in the up level directory ("cd ../").

Of course, you'll need to edit the files "debian/control" and "debian/changelog" with your email address and your name, so you can sign the packages with your PGP key.
If you made some changes to Kartesio source code, and you need to find out which libraries are needed to let the program work, you can just run the following command:
<br><i>
./list-libraries.sh
</i> <br>
To use this command, you'll need to install the program apt-file (with "sudo apt-get install apt-file").
