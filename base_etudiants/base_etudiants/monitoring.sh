#!/bin/bash
USAGE='script.sh [-d|--dir directory] [-o|--opt options] [filename]'

function parse_args (){
    for param ; do
        if [ $param = '-h' ] || [ $param = '--help' ] || [ $param = 'help' ] ; then
            echo "$USAGE" 1>&2
            exit
        fi
    done
    while [ $# -gt 0 ]; do
        case "$1" in
            -d|--dir)
                if [ $# -lt 2 ] ; then
                    echo '-d|--dir requires an argument' 1>&2
                    exit $BAD_USAGE
                fi
                DIR="$2"
                shift ; shift
                ;;
            -c|--opt)
                if [ $# -lt 2 ] ; then
                    echo '-o|--opt requires an argument' 1>&2
                    exit $BAD_USAGE
                fi
                OPTIONS="$2"
                shift ; shift
                ;;
            -*)
                echo "Option inconnue: $1"
                exit $BAD_USAGE
                ;;
            *)
                FILE_NAME="$1"
                shift
                ;;
        esac
    done
}

function get_ppids(){
	#fonction qui renvois les PIDs des processus principaux des database lancées
	PPIDS=()
	while IFS= read -r line;do
		PPIDS+=("$line")
	done < <(ps ef -opid,cmd -C tinydb | awk '$2 !~ /^[|\\]/ { print $1 }')
	PPIDS=("${PPIDS[@]:1}")
}

function get_allpids(){
	#fonction qui trouve les PIDs de tous les processus tinydb
	PIDS=()
	while IFS= read -r line;do
		PIDS+=("$line")
	done < <(ps | grep tinydb | awk '{print $1}')
}

function get_cpids(){
	#fonction qui trouve les PIDs des processus fils d'un certain processus
	CPIDS=()
	while IFS= read -r line;do
		PIDS+=("$line")
	done < <(pgrep -P $1)
}

case "$1" in
	"run")
		#Lance un processus tinydb
		parse_args"$@"
		;;
	"status")
		#Affiche les PIDs des processus principaux en cours d'exécution
		get_ppids
		if [ ${#PPIDS[@]} == 0 ]
		then
			echo "No databases running"
		else
			echo "${#PPIDS[@]} instances running"
			for p in ${PPIDS[@]};do
				echo "	> PID $p"
			done
		fi
		;;
	"sync")
		#Envoie un signal vers les processus tinydb principaux pour qu'ils
		# Terminent les demandent et se ferment
		get_ppids
		if [ ${#PPIDS[@]} == 0 ]
		then
			echo "No databases running"
		else
			echo "Found ${#PPIDS[@]} instances of tinydb"
			for p in ${PPIDS[@]};do
				echo "	Sync process $p..."
				kill -s USR1 $p
			done
		fi
		;;
	"shutdown")
		#Shutdown le processus tinydb choisi ou tous les processus tinydb lancé
		if [ $# -gt 1 ]
		then
		#Si l'utilisateur donne un PID à shutdown alors on teste si
		#Il se trouve dans la liste des processus principaux.
			get_ppids
			if [ " ${PPIDS[*]} " =~ "$2" ];then
				get_cpids "$2"
				for p in ${CPIDS[@]};do
					kill $p
				done
				kill $2
			else
				echo "$2 is not a parent process of tinydb"
				exit $BAD_USAGE
		else
		#Si l'utilisateur ne donne pas de PID alors on shutdown tous les 
		#processus tinydb (même les fils)
			get_allpids
			for p in ${PIDS[@]};do
				kill $p
			done
		;;
	*)
		echo ""
esac
		

