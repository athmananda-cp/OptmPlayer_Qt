#!/bin/sh

if [ $# != 2 ]
then
    echo "Invalid arguments send to the script"
    exit 0
fi

INSTALL_PATH=$PWD


echo "Running the application ..."
cd $INSTALL_PATH
chmod +x $2
./$2

