for ((i = 0; i < 1000; i++)); do
      ./a.out -r "$i" -t "$1"
done
