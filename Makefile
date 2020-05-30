SHELL := /bin/bash

read:
	cat README.md

check:
	[[ ${PWD} = "${HOME}/Arduino/my_skeches" ]] && echo all good
