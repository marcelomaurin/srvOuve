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
	cp ./src/srvmonitor2 ./srvmonitor2/usr/bin/srvmonitor2
	chmod 777 ./srvmonitor2/usr/bin/srvmonitor2
	#cp ./src/srvmonitor2.png ./srvMonitor2/usr/share/icons/hicolor/srvMonitor2.png
	#cp ./srvMonitor2.desktop_arm ./srvMonitor2/usr/share/applications/srvMonitor2.desktop
	#ln -s /usr/bin/srvMonitor2 ./srvMonitor2/usr/share/applications/srvMonitor2
	echo "Empacotando"
	dpkg-deb --build srvmonitor2
	echo "Movendo para pasta repositorio"
	FILE=`echo "srvmonitor2-$VERSAO""_amd64.deb"`
	echo $FILE
	mv srvmonitor2.deb $FILE
	cp $FILE ./bin/
	exit 1;
fi

if [ $ARQUITETURA = 'i686' ];
then
	echo "i686 Script"
	echo "Preparando binarios"
	cp ./src/srvMonitor2 ./srvmonitor2/usr/bin/srvmonitor2
	chmod 777 ./srvmonitor2/usr/bin/srvmonitor2
	#cp ./src/srvMonitor2.png ./srvMonitor2/usr/share/icons/hicolor/srvMonitor2.png
	#cp ./srvMonitor2.desktop_arm ./srvMonitor2/usr/share/applications/srvMonitor2.desktop
	#ln -s /usr/bin/srvMonitor2 ./srvMonitor2/usr/share/applications/srvMonitor2
	echo "Empacotando"
	dpkg-deb --build srvmonitor2
	echo "Movendo para pasta repositorio"
        FILE=`echo "srvmonitor2-$VERSAO""_i686.deb"`
        echo $FILE
	mv srvmonitor2.deb $FILE
	cp ./$FILE ./bin/
	exit 1;
fi

if [ $ARQUITETURA = 'i386' ];
then
	echo "i386 Script"
	echo "Preparando binarios"
	cp ./src/srvmonitor2 ./srvmonitor2/usr/bin/srvmonitor2
	chmod 777 ./srvmonitor2/usr/bin/srvmonitor2
	#cp ./src/srvMonitor2.png ./srvMonitor2/usr/share/icons/hicolor/srvMonitor2.png
	#cp ./srvMonitor2.desktop_arm ./srvMonitor2/usr/share/applications/srvMonitor2.desktop
	#ln -s /usr/bin/srvMonitor2 ./srvMonitor2/usr/share/applications/srvMonitor2
	echo "Empacotando"
	dpkg-deb --build srvmonitor2
	echo "Movendo para pasta repositorio"
        FILE=`echo "srvmonitor2-$VERSAO""_i386.deb"`
        echo $FILE
	mv srvmonitor2.deb $FILE
	cp ./$FILE ./bin/
	exit 1;
fi

if [ $ARQUITETURA =  'armv7l' ]; then
	echo "ARM Script"
	echo "Preparando binarios"
	cp ./src/srvmonitor2 ./srvmonitor2/usr/bin/srvmonitor2
	chmod 777 ./srvmonitor2/usr/bin/srvmonitor2
	#ln -s /usr/bin/srvMonitor2 ./srvMonitor2/usr/bin/srvMonitor2
	#cp ./src/srvMonitor2.png ./srvMonitor2/usr/share/icons/hicolor/srvMonitor2.png
	#cp ./srvMonitor2.desktop_arm ./srvMonitor2/usr/share/applications/srvMonitor2.desktop
	echo "Empacotando"
	dpkg-deb --build srvmonitor2
	echo "Movendo para pasta repositorio"
        FILE=`echo "srvmonitor2-$VERSAO""_arm.deb"`
        echo $FILE
	mv srvmonitor2.deb $FILE
	cp ./$FILE ./bin/
	exit 1;
fi
