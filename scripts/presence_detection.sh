#!/bin/bash
### BEGIN INIT INFO
# Provides: HMI
# Required-Start: $all
# Required-Stop: $all
# Default-Start: 2 3 4 5
# Default-Stop: 0 1 6
# Description: HMI Presence Detection Demo
# ExecStart=sudo /bin/bash /etc/init.d/presence_detection.sh start
# ExecStop=sudo /bin/bash /etc/init.d/presence_detection.sh stop
### END INIT INFO

. /lib/lsb/init-functions

NAME="presence_detection"
DAEMON_PATH=/home/pi/RadarSDK/example/app_presence_detection
DAEMON=build/app_presence_detection
DESC="HMI Presence Detection"
USER=pi

PIDFILE=$DAEMON_PATH/${NAME}.pid

function start(){
    printf "%-50s" "Starting $NAME..."
    start-stop-daemon --make-pidfile --pidfile $PIDFILE --start --name $NAME --exec $DAEMON --chdir $DAEMON_PATH --chuid $USER --test || return 1
    start-stop-daemon --make-pidfile --pidfile $PIDFILE --start --name $NAME --exec $DAEMON --chdir $DAEMON_PATH --chuid $USER || return 2
}

function stop(){
    start-stop-daemon --stop --retry INT/30/KILL/5 --pidfile $PIDFILE --name $NAME
    RETVAL="$?"
    [ "$RETVAL" = 2 ] && return 2
    return 0
}

function status(){
    start-stop-daemon --status --exec $DAEMON --chdir $DAEMON_PATH && exit 0 || exit $?
}

case "$1" in
start)
    start
;;
status)
    status
;;
stop)
    stop
;;
restart)
    $0 stop
    sleep 1
    $0 start
;;

*)
      echo "Usage: $0 {status|start|stop|restart}"
      exit 1
esac