#!/bin/bash
set -m
USAGE='./monitoring run [<database_file>] [-f <queries_file>]'

function parse_args (){
    for param ; do
        if [ $param = '-h' ] || [ $param = '--help' ] || [ $param = 'help' ] ; then
            echo "$USAGE" 1>&2
            exit
        fi
    done
    while [ $# -gt 0 ]; do
        case "$1" in
            -f)
                if [ $# -lt 2 ] ; then
                    echo '-f requires an argument' 1>&2
                    exit $BAD_USAGE
                fi
                QUERIES="$2"
                shift ; shift
                ;;
            -*)
                echo "Option inconnue: $1"
                exit $BAD_USAGE
                ;;
            *)
                if [ -f $1 ] ; then
                	touch "$1"    
                fi
                DATABASE="$1"          
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

case "$1" in
	"run")
		#Lance un processus tinydb
		DATABASE="data/students.bin"
		shift
		parse_args "$@"
		./tinydb $DATABASE < $QUERIES&
		echo "pid=$!"
		fg
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
				kill -10 $p
			done
			while [ ${#PPIDS[@]} -gt 0 ];do
				get_ppids
			done
			echo "done"
		fi
		;;
	"shutdown")
		#Shutdown le processus tinydb choisi ou tous les processus tinydb lancé
		if [ $# -gt 1 ]
		then
		#Si l'utilisateur donne un PID à shutdown alors on teste si
		#Il se trouve dans la liste des processus principaux.
			get_ppids
			if [[ " ${PPIDS[*]} " =~ "$2" ]];then
				kill -12 $2
			else
				echo "$2 is not a parent process of tinydb"
				exit $BAD_USAGE
			fi
		else
		#Si l'utilisateur ne donne pas de PID alors on shutdown tous les 
		#processus tinydb (même les fils)
			get_ppids
			for p in ${PPIDS[@]};do
				kill -12 $p
			done

		fi
		;;
	*)
		echo ""
esac
