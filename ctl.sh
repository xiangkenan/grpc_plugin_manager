#! /bin/bash


RootDir=$PWD
export LD_LIBRARY_PATH="$RootDir/lib/glog/lib:$LD_LIBRARY_PATH"

path=$(cd `dirname $0`/; pwd)

nohup $path/bin/server -d $path/conf/ -l $path/log/ 2>&1 &
