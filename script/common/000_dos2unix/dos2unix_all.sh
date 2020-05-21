
all_typ="m
        "
all_dir="sw
        "

for cur_typ in $all_typ
do
  for cur_dir in $all_dir
  do
    find "../$cur_dir" -name "*.$cur_typ"    \
      | xargs dos2unix >& /dev/null
  done
done
