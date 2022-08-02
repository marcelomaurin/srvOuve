#!/bin/bash
ARQUITETURA=$(uname -m)
VERSAO=1.0

case $(uname -m) in
	i386) 	ARQUITETURA="i386";;
	i686) 	ARQUITETURA="i386";;
	x86_64)	ARQUITETURA="amd64";;
	arm) 	ARQUITETURA="arm";;
esac

echo $ARQUITETURA

if [ $ARQUITETURA = 'amd64' ]; 
then
	echo "AMD64 Script"
	echo "Preparando binarios"
	cp ./src/srvouve ./srvouve/usr/bin/srvouve
	chmod 777 ./srvouve/usr/bin/srvouve
	#cp ./src/srvouve.png ./srvouve/usr/share/icons/hicolor/srvouve.png
	#cp ./srvouve.desktop_arm ./srvouve/usr/share/applications/srvouve.desktop
	#ln -s /usr/bin/srvouve ./srvouve/usr/share/applications/srvouve
	echo "Empacotando"
	dpkg-deb --build srvouve
	echo "Movendo para pasta repositorio"
	FILE=`echo "srvouve-$VERSAO""_amd64.deb"`
	echo $FILE
	mv srvouve.deb $FILE
	cp $FILE ./bin/
	exit 1;
fi

if [ $ARQUITETURA = 'i686' ];
then
	echo "i686 Script"
	echo "Preparando binarios"
	cp ./src/srvouve ./srvouve/usr/bin/srvouve
	chmod 777 ./srvouve/usr/bin/srvouve
	#cp ./src/srvouve.png ./srvouve/usr/share/icons/hicolor/srvouve.png
	#cp ./srvouve.desktop_arm ./srvouve/usr/share/applications/srvouve.desktop
	#ln -s /usr/bin/srvouve ./srvouve/usr/share/applications/srvouve
	echo "Empacotando"
	dpkg-deb --build srvouve
	echo "Movendo para pasta repositorio"
        FILE=`echo "srvouve-$VERSAO""_i686.deb"`
        echo $FILE
	mv srvouve.deb $FILE
	cp ./$FILE ./bin/
	exit 1;
fi

if [ $ARQUITETURA = 'i386' ];
then
	echo "i386 Script"
	echo "Preparando binarios"
	cp ./src/srvouve ./srvouve/usr/bin/srvouve
	chmod 777 ./srvouve/usr/bin/srvouve
	#cp ./src/srvouve.png ./srvouve/usr/share/icons/hicolor/srvouve.png
	#cp ./srvouve.desktop_arm ./srvouve/usr/share/applications/srvouve.desktop
	#ln -s /usr/bin/srvouve ./srvouve/usr/share/applications/srvouve
	echo "Empacotando"
	dpkg-deb --build srvouve
	echo "Movendo para pasta repositorio"
        FILE=`echo "srvouve-$VERSAO""_i386.deb"`
        echo $FILE
	mv srvouve.deb $FILE
	cp ./$FILE ./bin/
	exit 1;
fi

if [ $ARQUITETURA =  'armv7l' ]; then
	echo "ARM Script"
	echo "Preparando binarios"
	cp ./src/srvouve ./srvouve/usr/bin/srvouve
	chmod 777 ./srvouve/usr/bin/srvouve
	#ln -s /usr/bin/srvouve ./srvouve/usr/bin/srvouve
	#cp ./src/srvouve.png ./srvouve/usr/share/icons/hicolor/srvouve.png
	#cp ./srvouve.desktop_arm ./srvouve/usr/share/applications/srvouve.desktop
	echo "Empacotando"
	dpkg-deb --build srvouve
	echo "Movendo para pasta repositorio"
        FILE=`echo "srvouve-$VERSAO""_arm.deb"`
        echo $FILE
	mv srvouve.deb $FILE
	cp ./$FILE ./bin/
	exit 1;
fi
