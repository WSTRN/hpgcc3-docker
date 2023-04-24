#!/usr/bin/env bash
# grab current user name info

HOSTUID=$(id -u)
HOSTGID=$(id -g)
HOSTUSER=$(id -un)
HOSTGROUP=$(id -gn)

printHelp() {
    echo "HPGCC3 cross compiler assitant script"
    echo "all commands are run in current working dir\n"
    echo "Useages:"
	echo "./build.sh [arg]"
	echo "		[arg]: run any command in the container"
	echo "			   e.g.: make, make clean"
}


case $# in
    0)
        printHelp
        exit 0
        ;;

    *)
        SOPTS=$*
        ;;
esac
echo $SOPTS

CONTAINER_NAME=hpgcc3_$RANDOM
if ! [[ $SOPTS ]]; then
    docker run --name $CONTAINER_NAME -it -v $PWD:/work \
            -e HOST_UID=$HOSTUID -e HOST_GID=$HOSTGID \
            -e HOST_USER=$HOSTUSER -e HOST_GROUP=$HOSTGROUP \
            hpgcc "$@"
else
    docker run --name $CONTAINER_NAME -it -v $PWD:/work \
            -e HOST_UID=$HOSTUID -e HOST_GID=$HOSTGID \
            -e HOST_USER=$HOSTUSER -e HOST_GROUP=$HOSTGROUP \
            hpgcc $SOPTS
fi
RUN_EXIT_CODE=$?
docker rm $CONTAINER_NAME
exit $RUN_EXIT_CODE
