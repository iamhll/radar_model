#!/usr/bin/perl
#-------------------------------------------------------------------------------
  #
  #  Filename       : check_format_of_file_header.pl
  #  Author         : Huang Leilei
  #  Created        : 2019-05-27
  #  Description    : check the format of file header
  #
#-------------------------------------------------------------------------------

#*** OPTION ********************************************************************
  #use 5.010;
  use utf8;
  use strict;


#*** MAIN BODY *****************************************************************
  # open file
  open F_I, "< $ARGV[0]";
  #printf $ARGV[0];

  # variables
  my $numLine = 0;
  my $lenHighLight = 0;
  my $symComment = "\/\/";

  # choose header according file type
  if ($ARGV[0] =~ /\/.*\.(.*)/) {
    if ($1 eq "c" || $1 eq "v" || $1 eq "vh") {
      $symComment = "\/\/";
    }
    if ($1 eq "m") {
      $symComment = "%";
    }
    if ($1 eq "mk" || $1 eq "pl" || $1 eq "prj" || $1 eq "sgdc" || $1 eq "sh" || $1 eq "tcl") {
      $symComment = "#";
    }
  }

  # main loop
  while (<F_I>) {
    # pre-process
    chomp;

    # update line number
    $numLine++;

    # skip #!
    if (/^#\!/) {
      next;
    }

    # stop at blank line
    if (/^$/) {
      last;
    }

    # match symComment
    elsif (/(.*)$symComment(.*)/) {
      # get strPre and strPst
      my $strPre = $1;
      my $strPst = $2;

      # if strPre is empty
      if ($strPre eq "") {
        # all characters in strPst must be "-"
        if ($strPst eq "" || $strPst =~ /[^-]/) {
          printf "  please check $ARGV[0]:$numLine: $_ to remove incorrect contents (A)\n";
          last;
        }

        # but the length of current line must be 80
        if (length($_) != 80) {
          printf "  please check $ARGV[0]:$numLine: $_ to correct the length\n";
          last;
        }
      } # if strPre is empty

      # if strPre is "  "
      elsif ($strPre eq "  ") {
        # strPst could be emtpy
        if ($strPst eq "") {
          next;
        }

        # strPst could be infos
        elsif (/  $symComment  Filename       : \S+/
            || /  $symComment  Author         : .+/
            || /  $symComment  Created        : .+/
            || /  $symComment  Modified       : [\d?]{4}-[\d?]{2}-[\d?]{2} by .+/
            || /  $symComment  Description    : .+/
            || /  $symComment                   .+/
        ) {
          next;
        }

        # strPst could be highlighted
        elsif (/  $symComment  \*\*\* (.+) \*\*\*/) {
          # but the length of highlight must be the same
          if ($lenHighLight == 0) {
            $lenHighLight = length($1);
            next;
          }
          else {
            if (length($1) != $lenHighLight) {
              printf "  please check $ARGV[0]:$numLine: $_ to correct the length of high lighted content\n";
              last;
            }
          }
        }

        # other content
        else {
          printf "  please check $ARGV[0]:$numLine: $_ to remove incorrect contents (B)\n";
          last;
        }
      } # if strPre is "  "

      # other intentation
      else {
        printf "  please check $ARGV[0]:$numLine: $_ to correct the intentation\n";
        last;
      } # other intentation
    } # match symComment

    # other content
    else {
      printf "  please check $ARGV[0]:$numLine: $_ to remove incorrect contents (C)\n";
      last;
    }
  }
