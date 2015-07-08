#!/bin/bash

set -e

SCRIPT=$(readlink -f "$0")
EXEPATH=$(dirname "$SCRIPT")

e-reset

cd $EXEPATH/bin; ./flop.elf

retval=$?

exit $retval
