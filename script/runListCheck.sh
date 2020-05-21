#!/bin/bash
#-------------------------------------------------------------------------------
  #
  #  Filename       : runListCheck.sh
  #  Author         : Huang Leilei
  #  Created        : 2020-05-21
  #  Description    : a script-based check list
  #
#-------------------------------------------------------------------------------

#*** SUB FUNCTION **************************************************************
  # run
  run() {
    timeBgn=$(date +%s)
    echo "running $1..."
    chmod a+x ./common/???_$1/$1_all.sh
    ./common/???_$1/$1_all.sh
    timeEnd=$(date +%s)
    printf "  elapsed %d s\n" $((timeEnd - timeBgn))
  }


#*** MAIN BODY *****************************************************************
  # item
  run "dos2unix"
  run "delete_blank_end"
  run "check_format_of_file_header"
