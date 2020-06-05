#!/bin/bash
#-------------------------------------------------------------------------------
  #
  #  Filename       : runListUpdate.sh
  #  Author         : Huang Leilei
  #  Created        : 2020-05-20
  #  Description    : a script-based update list
  #
#-------------------------------------------------------------------------------

#*** SUB FUNCTION **************************************************************


#*** MAIN BODY *****************************************************************
  # removing empty directories
  cd ../
    echo "removing empty directories..."
    dirEmpty=$(find . -type d -empty)
    while [ "$dirEmpty" != "" ]
    do
      for dirCur in $dirEmpty
      do
        echo "  $dirCur"
      done
      echo "  ---"
      rm -rf $dirEmpty
      dirEmpty=$(find . -type d -empty)
    done
  cd - > /dev/null
  echo ""

  # log time
  date
  echo ""
