
all_typ="m
        "
all_dir="sw
        "

echo "from check_format_of_file_header_all.sh:" > skip.log

for cur_typ in $all_typ
do
  for cur_dir in $all_dir
  do
    for cur_fil in $(find "../$cur_dir" -name "*.$cur_typ")
    do
      writeByMe=$(grep "Author.*Huang Leilei" $cur_fil)
      if [ "$writeByMe" == "" ]
      then
        echo "  $cur_fil is skipped since the author is not me or no header exists" >> skip.log
      else
        ./common/*/check_format_of_file_header.pl $cur_fil
      fi
    done
  done
done
