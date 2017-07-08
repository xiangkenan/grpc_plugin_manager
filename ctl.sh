#! /bin/bash


RootDir=$PWD
export LD_LIBRARY_PATH="$RootDir/lib/glog/lib:$LD_LIBRARY_PATH"

path=$(cd `dirname $0`/; pwd)

ps -aux | grep $path/bin/manager | grep -v grep | awk '{print $2}' | xargs kill
nohup $path/bin/manager -d $path/conf/ -f conf.xml -l $path/log/ 2>&1 &
