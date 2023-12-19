./testD1 &

pid = $!

sleep 1

./rdprocD -p $pid
./rdprocD -f $pid
./rdprocD -t $pid

kill $pid