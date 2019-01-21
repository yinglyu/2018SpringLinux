pre_rec = "`netstat --statistics --tcp`" : '.* \([0-9]*\) segments received.*$' 
pre_snd = "`netstat --statistics --tcp`" : '.* \([0-9]*\) segments send out.*$'
sleep 10 
for i in `seq 1 5`
	do 
	cur_rec = expr "`netstat --statistics --tcp`" : '.* \([0-9]*\) segments received.*$' 
	cur_snd = expr "`netstat --statistics --tcp`" : '.* \([0-9]*\) segments send out.*$'
	gap_rec = `expr cur_rec - pre_rec`
	gap_snd = `expr cur_snd - pre_snd`
	pre_rec = cur_rec
	pre_snd = cur_snd
	echo $gap_rec	$gap_snd
	sleep 10 
done
