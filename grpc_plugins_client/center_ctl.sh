#! /bin/bash

RootDir=$PWD

path=$(cd `dirname $0`/; pwd)

if [[ $1 == "stop" ]]; then
  ps -ef|grep "$path/plugin_client" | grep -v grep | awk '{print $2}' | xargs kill -9

elif [[ $1 == "restart" ]]; then
  ps -ef|grep "$path/plugin_client" | grep -v grep | awk '{print $2}' | xargs kill -9
  sleep 2

  /usr/local/bin/spawn-fcgi -a 127.0.0.1 -p 9501 -f $path/plugin_client -F 1

elif [[ $1 == "start" ]]; then
  /usr/local/bin/spawn-fcgi -a 127.0.0.1 -p 9501 -f $path/plugin_client -F 1

else
  echo "please input start|restart|stop?"
fi
