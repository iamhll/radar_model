
all_typ="m
        "
all_dir="sw
        "

for cur_typ in $all_typ
do
  for cur_dir in $all_dir
  do
    for cur_fil in $(find "../$cur_dir" -name "*.$cur_typ")
    do
      sed -i "s/\s\+$//" $cur_fil
    done
    # i dont know why but "xargs sed -i" failed
  done
done
