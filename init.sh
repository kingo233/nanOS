#!/bin/bash
if [[ ! -d obj  ]];then
    mkdir obj
fi

if [[ ! -d bin   ]];then
    mkdir bin
fi
make nanOS.img
